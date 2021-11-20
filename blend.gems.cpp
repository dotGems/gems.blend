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
        const auto my_asset = atomic::get_asset( get_self(), asset_id );
        received_nfts.push_back( atomic::nft{ my_asset.collection_name, my_asset.template_id } );
    }

    // parse blend id
    const auto blend_id = sx::utils::parse_name(memo);
    check(blend_id.value, "blend::on_nft_transfer: invalid [blend_id] in the memo");

    // attempt to blend
    attempt_to_blend( from, blend_id, asset_ids, received_nfts);

    // stats
    update_status(0, 1);
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

name blend::detect_recipe( const set<name> recipe_ids, vector<atomic::nft> received_templates )
{
    blend::recipes_table _recipes( get_self(), get_self().value );
    sort( received_templates.begin(), received_templates.end());

    for ( const name recipe_id : recipe_ids ) {
        const auto recipe = _recipes.get( recipe_id.value, "blend::detect_recipe: [recipe_id] does not exists");
        if( recipe.templates.size() != received_templates.size()) continue;

        // sort recipe ingredients and compare to sorted received vector
        sort( recipe.templates.begin(), recipe.templates.end());
        if( received_templates == recipe.templates) return recipe_id;
    }
    check( false, "blend::detect_recipe: could not detect any valid recipes");
    return ""_n;
}

void blend::attempt_to_blend( const name owner, const name blend_id, const vector<uint64_t>& in_asset_ids, const vector<atomic::nft>& received_templates )
{
    blend::blends_table _blends( get_self(), get_self().value );
    blend::recipes_table _recipes( get_self(), get_self().value );

    const auto & blend = _blends.get( blend_id.value, "blend::attempt_to_blend: [blend_id] in the memo does not exists");
    const name recipe_id = detect_recipe( blend.in_recipe_ids, received_templates );

    const auto & recipe = _recipes.get( recipe_id.value, "blend::attempt_to_blend: [recipe_id] does not exists");
    check_time( blend.start_time, blend.end_time );

    // already picked our recipe, no need for extra checks
    vector<atomic::nft> recipe_templates = recipe.templates;
    for ( const uint64_t asset_id : in_asset_ids ) {
        atomic::burnasset( get_self(), asset_id );
    }

    // mint blended NFT asset to owner
    const uint64_t next_asset_id = atomic::get_next_asset_id();
    const name collection_name = blend.out_template.collection_name;
    const int32_t template_id = blend.out_template.template_id;
    const name schema = atomic::get_template( collection_name, template_id ).schema_name;
    atomic::mintasset( get_self(), collection_name, schema, template_id, get_self(), {}, {}, {} );
    atomic::transfer_nft( get_self(), owner, { next_asset_id }, "blended at .gems 💎" );

    // logging
    blend::blendlog_action blendlog( get_self(), { get_self(), "active"_n });
    blendlog.send( owner,
                   blend.blend_id,
                   recipe.recipe_id,
                   1,
                   in_asset_ids.size(),
                   in_asset_ids,
                   received_templates,
                   next_asset_id );
}

[[eosio::action]]
void blend::blendlog(  const name owner,
                       const name blend_id,
                       const name recipe_id,
                       const int total_mint,
                       const int total_burn,
                       const vector<uint64_t> in_asset_ids,
                       const vector<atomic::nft> in_templates,
                       const uint64_t out_asset_id )
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
        const auto my_template = atomic::get_template( item.collection_name, item.template_id );
        if ( burnable ) check( my_template.burnable, "blend::validate_templates: [template] must be `burnable`");
        check( my_template.transferable, "blend::validate_templates: [template] must be `transferable`");
    }
}

[[eosio::action]]
void blend::setrecipe( const name recipe_id, const vector<atomic::nft> templates )
{
    require_auth( get_self() );

    blend::recipes_table _recipes( get_self(), get_self().value );

    // validate
    check( recipe_id.length() <= 12, "blend::setrecipe: invalid [recipe_id] name must be 12 characters or less");
    check( recipe_id.length() >= 1, "blend::setrecipe: invalid [recipe_id] cannot be empty");
    check( name{recipe_id.value}.value, "blend::setrecipe: invalid [recipe_id]");
    check( sx::utils::parse_name(recipe_id.to_string()) == recipe_id, "blend::setrecipe: invalid [recipe_id] not matching");
    check( templates.size() >= 1, "blend::setrecipe: [templates] cannot be empty");
    validate_templates( templates, true );

    // recipe content
    auto insert = [&]( auto & row ) {
        row.recipe_id = recipe_id;
        row.templates = templates;
    };

    // create/modify blend
    auto itr = _recipes.find( recipe_id.value );
    if ( itr == _recipes.end() ) _recipes.emplace( get_self(), insert );
    else  _recipes.modify( itr, get_self(), insert );
}

[[eosio::action]]
void blend::setblend( const name blend_id, const set<name> in_recipe_ids, const atomic::nft out_template, const optional<time_point_sec> start_time, const optional<time_point_sec> end_time )
{
    require_auth( get_self() );

    blend::blends_table _blends( get_self(), get_self().value );
    blend::recipes_table _recipes( get_self(), get_self().value );

    // validate
    check( blend_id.length() <= 12, "blend::setblend: invalid [blend_id], name must be 12 characters or less");
    check( blend_id.length() >= 1, "blend::setblend: invalid [blend_id], cannot be empty");
    check( name{blend_id.value}.value, "blend::setblend: invalid [blend_id]");
    check( sx::utils::parse_name(blend_id.to_string()) == blend_id, "blend::setblend: invalid [blend_id] not matching");
    check( in_recipe_ids.size(), "blend::setblend: [in_recipe_ids] must contain at least 1 item");
    check( out_template.collection_name.value, "blend::setblend: [out_template.collection_name] is required");
    check( out_template.template_id, "blend::setblend: [out_template.template_id] is required");
    validate_templates( { out_template }, false );

    // validate recipes
    for ( const name recipe_id : in_recipe_ids ) {
        _recipes.get( recipe_id.value, "blend::setblend: [recipe_id] does not exists");
    }

    // recipe content
    auto insert = [&]( auto & row ) {
        row.blend_id = blend_id;
        row.in_recipe_ids = in_recipe_ids;
        row.out_template = out_template;
        row.start_time = start_time ? *start_time : static_cast<time_point_sec>( current_time_point() );
        row.end_time = *end_time;
    };

    // create/modify blend
    auto itr = _blends.find( blend_id.value );
    if ( itr == _blends.end() ) _blends.emplace( get_self(), insert );
    else  _blends.modify( itr, get_self(), insert );
}

[[eosio::action]]
void blend::delblend( const name blend_id )
{
    require_auth( get_self() );

    blend::blends_table _blends( get_self(), get_self().value );
    auto & blend = _blends.get( blend_id.value, "blend::delblend: [blend_id] does not exist" );
    _blends.erase( blend );
}

[[eosio::action]]
void blend::delrecipe( const name recipe_id )
{
    require_auth( get_self() );

    // erase recipe from existing blends
    blend::blends_table _blends( get_self(), get_self().value );
    for(const auto& blend: _blends){
        if( blend.in_recipe_ids.count(recipe_id)){
            _blends.modify( blend, get_self(), [&]( auto & row ) {
                row.in_recipe_ids.erase( recipe_id);
             });
        }
    }

    blend::recipes_table _recipes( get_self(), get_self().value );
    auto & recipe = _recipes.get( recipe_id.value, "blend::delrecipe: [recipe_id] does not exist" );
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

}
