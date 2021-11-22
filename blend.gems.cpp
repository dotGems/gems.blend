#include <sx.utils/utils.hpp>

#include "blend.gems.hpp"
#include "src/helpers.cpp"

namespace gems {

[[eosio::on_notify("atomicassets::transfer")]]
void blend::on_nft_transfer( const name from, const name to, const vector<uint64_t> asset_ids, const std::string memo )
{
    require_auth( from );

    // ignore transfers
    if ( is_account( "notify.gems"_n )) require_recipient( "notify.gems"_n );
    if ( from == get_self() || memo == get_self().to_string() ) return;

    // build vector of received templates
    vector<atomic::nft> received_nfts;
    for ( const uint64_t asset_id : asset_ids ) {
        received_nfts.push_back( atomic::get_nft( get_self(), asset_id ) );
    }

    // parse memo
    const auto [ collection_name, template_id ] = parse_memo( memo );

    // attempt to blend
    attempt_to_blend( from, collection_name, template_id, asset_ids, received_nfts);

    // stats
    update_status(1, 1); // mint
    update_status(2, asset_ids.size()); // burn
}

std::pair<name, int32_t> blend::parse_memo( const string memo )
{
    check(memo.length(), "blend::parse_memo: memo is required (ex: 'mycollection:12345'");
    const auto memo_parts = sx::utils::split(memo, ":");
    check( sx::utils::is_digit(memo_parts[1]), "blend::parse_memo: invalid [template_id] in the memo (ex: 'mycollection:12345'");
    const name collection_name = sx::utils::parse_name(memo_parts[0]);
    check( collection_name.value, "blend::parse_memo: invalid [collection_name] in the memo (ex: 'mycollection:12345'");
    const int32_t template_id = std::stol(memo_parts[1]);

    return { collection_name, template_id };
}

void blend::check_time( const time_point_sec start_time, const time_point_sec end_time )
{
    const int64_t remaining = (start_time - current_time_point()).to_seconds();
    const int64_t hours = remaining / 60 / 60;
    const int64_t minutes = (remaining - hours * 60 * 60) / 60;
    const int64_t seconds = remaining - hours * 60 * 60 - minutes * 60;
    check( remaining <= 0, "blend::check_time: not yet availabe, opening in " + to_string(hours) + "h " + to_string(minutes) + "m " + to_string(seconds) + "s");
    if ( end_time.sec_since_epoch() ) check( end_time > current_time_point(), "blend::check_time: has ended");
}

uint64_t blend::detect_recipe( const set<uint64_t> recipe_ids, vector<atomic::nft> received_templates )
{
    blend::recipes_table _recipes( get_self(), get_self().value );
    sort( received_templates.begin(), received_templates.end());

    for ( const uint64_t recipe_id : recipe_ids ) {
        auto recipe = _recipes.get( recipe_id, "blend::detect_recipe: [recipe_id] does not exists");
        if ( is_match( received_templates, recipe.templates )) return recipe_id;
    }
    check( false, "blend::detect_recipe: could not detect any valid recipes");
    return 0;
}

bool blend::is_match( const vector<atomic::nft>& sorted_templates, vector<atomic::nft>& templates )
{
    if( templates.size() != sorted_templates.size()) return false;

    // sort recipe ingredients and compare to sorted received vector
    sort( templates.begin(), templates.end());
    return sorted_templates == templates;
}

void blend::attempt_to_blend( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomic::nft>& received_templates )
{
    blend::blends_table _blends( get_self(), get_self().value );
    blend::recipes_table _recipes( get_self(), get_self().value );

    // get blend
    const auto & blend = _blends.get( template_id, "blend::attempt_to_blend: [template_id] in the memo does not exists");
    check( blend.id.collection_name == collection_name, "blend::attempt_to_blend: [collection_name] in the memo does not match blend");

    // get recipe
    const uint64_t recipe_id = detect_recipe( blend.recipe_ids, received_templates );
    const auto & recipe = _recipes.get( recipe_id, "blend::attempt_to_blend: [recipe_id] does not exists");
    check_time( blend.start_time, blend.end_time );

    // already picked our recipe, no need for extra checks
    vector<atomic::nft> recipe_templates = recipe.templates;
    for ( const uint64_t asset_id : in_asset_ids ) {
        atomic::burnasset( get_self(), asset_id );
    }

    // mint blended NFT asset to owner
    const uint64_t next_asset_id = atomic::get_next_asset_id();
    const name schema = atomic::get_template( collection_name, template_id ).schema_name;
    atomic::mintasset( get_self(), collection_name, schema, template_id, get_self(), {}, {}, {} );
    atomic::transfer_nft( get_self(), owner, { next_asset_id }, "blended at .gems 💎" );

    // logging
    blend::blendlog_action blendlog( get_self(), { get_self(), "active"_n });
    blendlog.send( owner,
                   in_asset_ids,
                   next_asset_id,
                   received_templates,
                   blend.id,
                   1,
                   in_asset_ids.size() );
}

[[eosio::action]]
void blend::blendlog( const name owner,
                      const vector<uint64_t> in_asset_ids,
                      const uint64_t out_asset_id,
                      const vector<atomic::nft> in_templates,
                      const atomic::nft out_template,
                      const int total_mint,
                      const int total_burn )
{
    require_auth( get_self() );
    require_recipient( owner );
    if ( is_account( "notify.gems"_n )) require_recipient( "notify.gems"_n );
}

[[eosio::action]]
void blend::reset( const name table, const optional<name> scope  )
{
    require_auth( get_self() );

    blend::blends_table _blends( get_self(), get_self().value );
    blend::recipes_table _recipes( get_self(), get_self().value );

    if ( table == "blends"_n ) clear_table( _blends );
    else if ( table == "recipes"_n ) clear_table( _recipes );
    else check( false, "invalid table name");
}

void blend::validate_templates( const vector<atomic::nft> templates, const bool burnable )
{
    for ( const atomic::nft item : templates ) {
        check( item.collection_name.value, "blend::validate_templates: [collection_name] is required");
        check( item.template_id, "blend::validate_templates: [template_id] is required");
        const auto my_template = atomic::get_template( item.collection_name, item.template_id );
        if ( burnable ) check( my_template.burnable, "blend::validate_templates: [template] must be `burnable`");
        check( my_template.transferable, "blend::validate_templates: [template] must be `transferable`");
    }
}

[[eosio::action]]
void blend::initrecipe( vector<atomic::nft> templates )
{
    require_auth( get_self() );

    // validate
    check( templates.size() >= 1, "blend::initrecipe: [templates] cannot be empty");
    validate_templates( templates, true );

    // pre-sort ingredients for detect_recipe efficiency
    sort( templates.begin(), templates.end() );

    blend::recipes_table _recipes( get_self(), get_self().value );

    // uncomment this loop to disallow duplicate recipes
    // for( auto& recipe: _recipes )) {
    //     check( !is_match( templates, recipe.templates ), "blend::initrecipe: recipe already exists" );
    // }

    // recipe content
    auto insert = [&]( auto & row ) {
        row.id = get_next_recipe_id();
        row.templates = templates;
    };
    _recipes.emplace( get_self(), insert );

    // stats
    update_status(0, 1); // recipe counter
}

uint64_t blend::get_next_recipe_id()
{
    blend::status_table _status( get_self(), get_self().value );
    if ( !_status.exists() ) return 0;
    return _status.get().counters[0];
}

[[eosio::action]]
void blend::setblend( const atomic::nft id, const set<uint64_t> recipe_ids, const string description, const optional<time_point_sec> start_time, const optional<time_point_sec> end_time )
{
    require_auth( get_self() );

    blend::blends_table _blends( get_self(), get_self().value );
    blend::recipes_table _recipes( get_self(), get_self().value );

    // validate
    check( recipe_ids.size(), "blend::setblend: [recipe_ids] must contain at least 1 item");
    validate_templates( { id }, false );

    // validate recipes
    for ( const uint64_t recipe_id : recipe_ids ) {
        _recipes.get( recipe_id, "blend::setblend: [recipe_id] does not exists");
    }

    // recipe content
    auto insert = [&]( auto & row ) {
        row.id = id;
        row.recipe_ids = recipe_ids;
        row.description = description;
        row.start_time = start_time ? *start_time : static_cast<time_point_sec>( current_time_point() );
        row.end_time = *end_time;
    };

    // create/modify blend
    auto itr = _blends.find( id.template_id );
    if ( itr == _blends.end() ) _blends.emplace( get_self(), insert );
    else  _blends.modify( itr, get_self(), insert );
}

[[eosio::action]]
void blend::delblend( const atomic::nft id )
{
    require_auth( get_self() );

    blend::blends_table _blends( get_self(), get_self().value );
    auto & blend = _blends.get( id.template_id, "blend::delblend: [id.template_id] does not exist" );
    check( blend.id.collection_name == id.collection_name, "blend::delblend: [id.collection_name] does not match blend");
    _blends.erase( blend );
}

[[eosio::action]]
void blend::delrecipe( const uint64_t recipe_id )
{
    require_auth( get_self() );

    // erase recipe from existing blends
    blend::blends_table _blends( get_self(), get_self().value );
    for (const auto& blend: _blends) {
        if ( blend.recipe_ids.count(recipe_id) ) {
            _blends.modify( blend, get_self(), [&]( auto & row ) {
                row.recipe_ids.erase( recipe_id );
            });
        }
    }

    blend::recipes_table _recipes( get_self(), get_self().value );
    auto & recipe = _recipes.get( recipe_id, "blend::delrecipe: [recipe_id] does not exist" );
    _recipes.erase( recipe );
}

void blend::update_status( const uint32_t index, const uint32_t count )
{
    status_table _status( get_self(), get_self().value );
    auto status = _status.get_or_default();

    if( status.counters.size() <= index ) status.counters.resize( index + 1);
    status.counters[index] += count;
    status.last_updated = current_time_point();
    _status.set( status, get_self() );
}


[[eosio::action]]
void blend::cleanup( )
{
    require_auth( get_self() );

    set<uint64_t> used_recipes;
    blend::blends_table _blends( get_self(), get_self().value );
    for (const auto& blend: _blends) {
        for (const auto& recipe_id: blend.recipe_ids) {
            used_recipes.insert(recipe_id);
        }
    }

    blend::recipes_table _recipes( get_self(), get_self().value );
    int erased = 0;
    for( auto it = _recipes.begin(); it != _recipes.end(); ){
        if( used_recipes.count( it->id ) == 0 ) {
            it = _recipes.erase( it );
            ++erased;
        }
        else {
            ++it;
        }
    }
    check( erased, "blend::cleanup: nothing to cleanup");
}

}
