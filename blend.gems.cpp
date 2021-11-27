#include <sx.utils/utils.hpp>

#include "blend.gems.hpp"
#include "src/helpers.cpp"
#include "plugins/plugins.cpp"

namespace gems {

[[eosio::on_notify("atomicassets::transfer")]]
void blend::on_nft_transfer( const name from, const name to, const vector<uint64_t> asset_ids, const std::string memo )
{
    require_auth( from );

    // ignore transfers
    if ( is_account( "notify.gems"_n )) require_recipient( "notify.gems"_n );
    if ( from == get_self() || memo == get_self().to_string() ) return;

    // parse memo
    const auto [ collection_name, template_id ] = parse_memo( memo );

    // attempt to blend
    attempt_to_blend( from, collection_name, template_id, asset_ids );

    // stats
    update_status(0, 1); // mint
    update_status(1, asset_ids.size()); // burn
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

uint64_t blend::detect_recipe( const name collection_name, const vector<uint64_t> asset_ids, const set<uint64_t> recipe_ids, vector<atomic::nft> received_templates )
{
    blend::recipes_table _recipes( get_self(), collection_name.value );

    // order templates
    sort( received_templates.begin(), received_templates.end());

    // detect recipe
    for ( const uint64_t recipe_id : recipe_ids ) {
        auto recipe = _recipes.get( recipe_id, "blend::detect_recipe: [recipe_id] does not exists");
        if ( is_match( received_templates, recipe.templates )) return recipe_id;
    }
    check( false, "blend::detect_recipe: could not detect any valid recipes");
    return 0;
}

bool blend::is_match( const vector<atomic::nft>& sorted_templates, vector<atomic::nft>& templates )
{
    if ( templates.size() != sorted_templates.size()) return false;

    // sort recipe ingredients and compare to sorted received vector
    sort( templates.begin(), templates.end());
    return sorted_templates == templates;
}

void blend::attempt_to_blend( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids )
{
    blend::blends_table _blends( get_self(), collection_name.value );
    blend::recipes_table _recipes( get_self(), collection_name.value );

    // get blend
    const auto & blend = _blends.get( template_id, "blend::attempt_to_blend: [template_id] in the memo does not exists");
    check( blend.id.collection_name == collection_name, "blend::attempt_to_blend: [collection_name] in the memo does not match blend");

    // build vector of received templates
    vector<atomic::nft> received_templates;
    for ( const uint64_t asset_id : in_asset_ids ) {
        received_templates.push_back( atomic::get_nft( get_self(), asset_id ) );
    }

    // get recipe
    const uint64_t recipe_id = detect_recipe( collection_name, in_asset_ids, blend.recipe_ids, received_templates );
    const auto & recipe = _recipes.get( recipe_id, "blend::attempt_to_blend: [recipe_id] does not exists");
    check_time( blend.start_time, blend.end_time );

    // already picked our recipe, no need for extra checks
    for ( const uint64_t asset_id : in_asset_ids ) {
        atomic::burnasset( get_self(), asset_id );
    }
    // generate immutate/mutable attributes
    const auto [ immutable_attributes, mutable_attributes ] = gems::blend::mint_attributes( owner, collection_name, template_id, in_asset_ids );

    // mint blended NFT asset to owner
    const uint64_t next_asset_id = atomic::get_next_asset_id();
    const name schema = atomic::get_template( collection_name, template_id ).schema_name;
    atomic::mintasset( get_self(), collection_name, schema, template_id, get_self(), immutable_attributes, mutable_attributes, {} );
    atomic::transfer_nft( get_self(), owner, { next_asset_id }, "blended at .gems 💎" );

    // logging
    blend::blendlog_action blendlog( get_self(), { get_self(), "active"_n });
    blendlog.send( owner,
                   collection_name,
                   in_asset_ids,
                   next_asset_id,
                   received_templates,
                   blend.id,
                   1,
                   in_asset_ids.size() );
}

[[eosio::action]]
void blend::blendlog( const name owner,
                      const name collection_name,
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

    blend::blends_table _blends( get_self(), scope ? scope->value : get_self().value );
    blend::recipes_table _recipes( get_self(), scope ? scope->value : get_self().value );
    blend::status_table _status( get_self(), get_self().value );
    blend::collections_table _collections( get_self(), get_self().value );

    if ( table == "blends"_n ) clear_table( _blends );
    else if ( table == "recipes"_n ) clear_table( _recipes );
    else if ( table == "status"_n ) _status.remove();
    else if ( table == "collections"_n ) _collections.remove();
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

name blend::get_ram_payer( const atomic::nft id )
{
    if ( has_auth( get_self() ) ) return get_self();
    return get_author( id );
}

[[eosio::action]]
void blend::addrecipe( const atomic::nft id, vector<atomic::nft> templates )
{
    if ( !has_auth( get_self() ) ) require_auth( get_author( id ) );

    // tables
    blend::recipes_table _recipes( get_self(), id.collection_name.value );
    blend::blends_table _blends( get_self(), id.collection_name.value );

    // validate
    check( templates.size() >= 1, "blend::addrecipe: [templates] cannot be empty");
    validate_templates( templates, true );

    // pre-sort ingredients for detect_recipe efficiency
    sort( templates.begin(), templates.end() );

    // disallow duplicate recipes within same blend
    auto & blend = _blends.get(id.template_id, "blend::addrecipe: [id.template_id] cannot find any blends" );
    for ( const uint64_t recipe_id : blend.recipe_ids ) {
        auto recipe = _recipes.get( recipe_id, "blend::addrecipe: [recipe_id] does not exists" );
        check( !is_match( templates, recipe.templates ), "blend::addrecipe: recipe already exists" );
    }

    // add recipe to blend
    const uint64_t recipe_id = _recipes.available_primary_key();
    const name ram_payer = get_ram_payer( id );
    _blends.modify( blend, ram_payer, [&]( auto & row ) {
        row.recipe_ids.insert( recipe_id );
    });

    // recipe content
    auto insert = [&]( auto & row ) {
        row.id = recipe_id;
        row.templates = templates;
    };
    _recipes.emplace( ram_payer, insert );
}

[[eosio::action]]
void blend::setblend( const atomic::nft id, const optional<string> description, const optional<time_point_sec> start_time, const optional<time_point_sec> end_time )
{
    if ( !has_auth( get_self() ) ) require_auth( get_author( id ) );

    blend::blends_table _blends( get_self(), id.collection_name.value );
    blend::recipes_table _recipes( get_self(), id.collection_name.value );
    blend::collections_table _collections( get_self(), get_self().value );

    // validate
    validate_templates( { id }, false );
    const set<name> authorized_accounts = atomic::get_authorized_accounts( id );
    check( authorized_accounts.find(get_self()) != authorized_accounts.end(), "blend::setblend: contract must be included in [atomic::authorized_accounts]" );

    // recipe content
    auto insert = [&]( auto & row ) {
        row.id = id;
        if ( description ) row.description = *description;
        row.start_time = start_time ? *start_time : static_cast<time_point_sec>( current_time_point() );
        row.end_time = end_time ? *end_time : static_cast<time_point_sec>( current_time_point().sec_since_epoch() + 365 * 86400 );
    };

    // create/modify blend
    const name ram_payer = get_ram_payer( id );
    auto itr = _blends.find( id.template_id );
    if ( itr == _blends.end() ) _blends.emplace( ram_payer, insert );
    else  _blends.modify( itr, ram_payer, insert );

    // add scope
    auto collections = _collections.get_or_default();
    collections.collection_names.insert(id.collection_name);
    _collections.set( collections, get_self() );
}

[[eosio::action]]
void blend::delblend( const atomic::nft id )
{
    if ( !has_auth( get_self() ) ) require_auth( get_author( id ) );

    blend::blends_table _blends( get_self(), id.collection_name.value );
    blend::recipes_table _recipes( get_self(), id.collection_name.value );
    blend::collections_table _collections( get_self(), get_self().value );

    // delete any recipes connected to blend
    auto & blend = _blends.get( id.template_id, "blend::delblend: [id.template_id] does not exist" );
    for ( const uint64_t recipe_id : blend.recipe_ids ) {
        auto recipe = _recipes.find( recipe_id );
        if ( recipe != _recipes.end() ) _recipes.erase( recipe );
    }
    _blends.erase( blend );

    // remove scope if empty
    if ( _blends.begin() == _blends.end() ) {
        auto collections = _collections.get_or_default();
        collections.collection_names.erase(id.collection_name);
        _collections.set( collections, get_self() );
    }
}

[[eosio::action]]
void blend::delrecipe( const atomic::nft id, const uint64_t recipe_id )
{
    if ( !has_auth( get_self() ) ) require_auth( get_author( id ) );

    // tables
    blend::blends_table _blends( get_self(), id.collection_name.value );
    blend::recipes_table _recipes( get_self(), id.collection_name.value );

    // erase recipe from existing blends
    auto & blend = _blends.get( id.template_id, "blend::delrecipe: [id.template_id] does not exist" );
    if ( blend.recipe_ids.count( recipe_id ) ) {
        _blends.modify( blend, get_ram_payer( id ), [&]( auto & row ) {
            row.recipe_ids.erase( recipe_id );
        });
    }

    // erase recipe
    auto & recipe = _recipes.get( recipe_id, "blend::delrecipe: [recipe_id] does not exist" );
    _recipes.erase( recipe );
}

}
