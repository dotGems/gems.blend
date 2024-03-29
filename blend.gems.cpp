#include <sx.utils/utils.hpp>
#include <gems/atomic.gems.hpp>

#include "blend.gems.hpp"
#include "src/helpers.cpp"
#include "plugins/plugins.cpp"

// static values
static constexpr uint16_t MAX_PROTOCOL_FEE = 2000;

namespace gems {

/**
 * Notify contract when any token transfer notifiers relay contract
 */
[[eosio::on_notify("*::transfer")]]
void blend::on_transfer( const name from, const name to, const asset quantity, const string memo )
{
    require_auth( from );

    // ignore transfers
    if ( from == get_self() || from == "eosio.ram"_n ) return;

    // validate
    check_status();

    // parse memo
    const auto [ collection_name, template_id ] = parse_memo( memo );

    // validate deposit
    const extended_asset ext_in = { quantity, get_first_receiver() };

    // deposit
    add_quantity( from, atomic::nft{ collection_name, template_id }, ext_in );

    // stats
    update_status(2, 1); // deposit
}

[[eosio::on_notify("atomicassets::transfer")]]
void blend::on_nft_transfer( const name from, const name to, const vector<uint64_t> asset_ids, const std::string memo )
{
    require_auth( from );

    // ignore transfers
    if ( is_account( "notify.gems"_n )) require_recipient( "notify.gems"_n );
    if ( from == get_self() || memo == get_self().to_string() ) return;

    // validate
    check_status();

    // parse memo
    const auto [ collection_name, template_id ] = parse_memo( memo );

    // !!! TO-DO <START> !!!
    // check if token quantity deposit is first required
    // check if valid quantity exists in order prior to attempting blend
    // !!! TO-DO <END> !!!

    // attempt to blend
    attempt_to_blend( from, collection_name, template_id, asset_ids );

    // stats
    update_status(0, 1); // mint
    update_status(1, asset_ids.size()); // burn
}

void blend::deduct_token_payment( const name from, const name collection_name, const int32_t template_id )
{
    blend::orders_table _orders( get_self(), from.value );
    blend::config_table _config( get_self(), get_self().value );
    blend::blends_table _blends( get_self(), get_self().value );
    auto config = _config.get();

    // get required payment for blend recipe
    const extended_asset required = *_blends.get( template_id ).quantity;
    if ( !required.quantity.amount ) return; // skip if not required token payment

    // check if user has valid deposit
    auto & orders = _orders.get( template_id, "blend.gems::cancel: no deposits for this user in this NFT ID");
    check( orders.quantity == required, "blend::deduct_token_payment: invalid quantity, must be exactly " + required.quantity.to_string() );
    _orders.erase( orders );

    // send asset to author account
    const name author = atomic::get_author( collection_name );
    const extended_symbol ext_sym = required.get_extended_symbol();
    const int64_t to_protocol = required.quantity.amount * config.protocol_fee / 10000;
    const int64_t to_author = required.quantity.amount - to_protocol;
    transfer( get_self(), author, { to_author, ext_sym }, "blend fee");
    transfer( get_self(), config.fee_account, { to_protocol, ext_sym }, "protocol fee");
}

std::pair<name, int32_t> blend::parse_memo( const string memo )
{
    check(memo.length(), "blend::parse_memo: memo is required (ex: 'mycollection:12345'");
    const auto memo_parts = sx::utils::split(memo, ":");
    check( sx::utils::is_digit(memo_parts[1]), "blend::parse_memo: invalid [template_id] in the memo (ex: 'mycollection:12345')");

    const name collection_name = sx::utils::parse_name(memo_parts[0]);
    check( collection_name.value, "blend::parse_memo: invalid [collection_name=" + memo_parts[0] + "] in the memo (ex: 'mycollection:12345')");
    const int32_t template_id = std::stol(memo_parts[1]);

    return { collection_name, template_id };
}

void blend::add_quantity( const name owner, const atomic::nft id, const extended_asset value )
{
    blend::orders_table _orders( get_self(), owner.value );
    blend::blends_table _blends( get_self(), get_self().value );

    // validate deposit
    const extended_asset required = *_blends.get( id.template_id ).quantity;

    check( value.quantity.amount, "blend::on_transfer: empty input quantity" );
    check( value == required, "blend::on_transfer: invalid quantity, must be exactly " + required.quantity.to_string() );

    // add deposit order
    auto itr = _orders.find( id.template_id );
    check( itr == _orders.end(), "gems::add_quantity: quantity deposit already exists, proceed with blend or cancel deposit");
    _orders.emplace( get_self(), [&]( auto & row ) {
        row.id = id;
        row.quantity = value;
    });

    // !!! TO-DO !!!
    // Log deposit to user
}

void blend::check_time( const optional<time_point_sec> start_time, const optional<time_point_sec> end_time )
{
    const int64_t remaining = (*start_time - current_time_point()).to_seconds();
    const int64_t hours = remaining / 60 / 60;
    const int64_t minutes = (remaining - hours * 60 * 60) / 60;
    const int64_t seconds = remaining - hours * 60 * 60 - minutes * 60;
    if ( start_time->sec_since_epoch() ) check( remaining <= 0, "blend::check_time: not yet availabe, opening in " + to_string(hours) + "h " + to_string(minutes) + "m " + to_string(seconds) + "s");
    if ( end_time->sec_since_epoch() ) check( *end_time > current_time_point(), "blend::check_time: has ended");
}

uint64_t blend::detect_recipe( const name collection_name, const vector<uint64_t> asset_ids, const set<uint64_t> recipe_ids, vector<atomic::nft_extra> received_templates )
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

bool blend::is_match( const vector<atomic::nft_extra>& sorted_templates, vector<atomic::nft_extra>& templates )
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
    blend::limits_table _limits( get_self(), collection_name.value );

    // get blend
    const auto & blend = _blends.get( template_id, "blend::attempt_to_blend: [template_id] in the memo does not exists");
    check( blend.id.collection_name == collection_name, "blend::attempt_to_blend: [collection_name] in the memo does not match blend");

    // build vector of received templates
    vector<atomic::nft_extra> received_templates;
    for ( const uint64_t asset_id : in_asset_ids ) {
        received_templates.push_back( atomic::get_nft_extra( get_self(), asset_id ) );
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
    const auto [ immutable_attributes, mutable_attributes ] = gems::blend::mint_attributes( *blend.plugin, owner, collection_name, template_id, in_asset_ids );

    // mint blended NFT asset to owner
    const uint64_t next_asset_id = atomic::get_next_asset_id();
    const name schema = atomic::get_template( collection_name, template_id ).schema_name;
    atomic::mintasset( get_self(), collection_name, schema, template_id, owner, immutable_attributes, mutable_attributes, {} );

    // detect if limit is reached
    const auto itr = _limits.find( template_id );
    if ( itr != _limits.end() ) {
        check( itr->mint_assets < itr->max_mint_assets, "blend::attempt_to_blend: maximum limit reached");
        _limits.modify( itr, same_payer, [&]( auto & row ) {
            row.mint_assets += 1;
        });
    }

    // Extra options
    gems::blend::extras(*blend.plugin, owner, collection_name, template_id);

    // logging
    blend::blendlog_action blendlog( get_self(), { get_self(), "active"_n });
    blendlog.send( owner,
                   *blend.description,
                   in_asset_ids,
                   next_asset_id,
                   received_templates,
                   blend.id,
                   1,
                   in_asset_ids.size() );
}

[[eosio::action]]
void blend::blendlog( const name owner,
                      const string description,
                      const vector<uint64_t> in_asset_ids,
                      const uint64_t out_asset_id,
                      const vector<atomic::nft_extra> in_templates,
                      const atomic::nft_extra out_template,
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
    blend::orders_table _orders( get_self(), scope ? scope->value : get_self().value );
    blend::status_table _status( get_self(), get_self().value );
    blend::config_table _config( get_self(), get_self().value );
    blend::collections_table _collections( get_self(), get_self().value );

    if ( table == "blends"_n ) clear_table( _blends );
    else if ( table == "recipes"_n ) clear_table( _recipes );
    else if ( table == "orders"_n ) clear_table( _orders );
    else if ( table == "status"_n ) _status.remove();
    else if ( table == "config"_n ) _config.remove();
    else if ( table == "collections"_n ) _collections.remove();
    else check( false, "invalid table name");
}

void blend::validate_templates( const vector<atomic::nft> templates, const bool burnable, const bool transferable )
{
    for ( const atomic::nft item : templates ) {
        check( item.collection_name.value, "blend::validate_templates: [collection_name] is required");
        check( item.template_id, "blend::validate_templates: [template_id] is required");
        const auto my_template = atomic::get_template( item.collection_name, item.template_id );
        if ( burnable ) check( my_template.burnable, "blend::validate_templates: [template] must be `burnable`");
        if ( transferable ) check( my_template.transferable, "blend::validate_templates: [template] must be `transferable`");
    }
}

name blend::get_ram_payer( const name collection_name )
{
    if ( has_auth( get_self() ) ) return get_self();
    return atomic::get_author( collection_name );
}

[[eosio::action]]
void blend::setrecipes( const name collection_name, const int32_t template_id, const set<uint64_t> recipe_ids )
{
    if ( !has_auth( get_self() ) ) require_auth( atomic::get_author( collection_name ) );

    blend::blends_table _blends( get_self(), collection_name.value );
    blend::recipes_table _recipes( get_self(), collection_name.value );

    // maximum unique templates (prevent overloading UI)
    check( recipe_ids.size() <= 10, "blend::setrecipes: [recipe_ids] cannot exceed 10 recipes");

    // make sure recipe exists
    for ( const uint64_t recipe_id : recipe_ids ) {
        _recipes.get( recipe_id, "blend::setrecipes: [recipe_id] does not exists" );
    }
    // add recipe to blend
    const name ram_payer = get_ram_payer( collection_name );
    auto & blend = _blends.get(template_id, "blend::setrecipes: [template_id] does not exists" );
    _blends.modify( blend, ram_payer, [&]( auto & row ) {
        row.recipe_ids = recipe_ids;
    });
}

[[eosio::action]]
void blend::addrecipe( const name collection_name, const int32_t template_id, vector<atomic::nft> templates )
{
    if ( !has_auth( get_self() ) ) require_auth( atomic::get_author( collection_name ) );

    // tables
    blend::recipes_table _recipes( get_self(), collection_name.value );
    blend::blends_table _blends( get_self(), collection_name.value );

    // validate
    check( templates.size() >= 1, "blend::addrecipe: [templates] cannot be empty");
    validate_templates( templates, true, true );

    // maximum unique templates (prevent overloading UI)
    set<int32_t> uniques;
    vector<atomic::nft_extra> templates_extra;
    for ( const auto row : templates ) {
        const name schema_name = atomic::get_template( row.collection_name, row.template_id ).schema_name;
        uniques.insert( row.template_id );
        templates_extra.push_back( atomic::nft_extra{ row.collection_name, row.template_id, schema_name });
    }
    check( uniques.size() <= 10, "blend::addrecipe: [templates] cannot exceed 10 unique templates");

    // pre-sort ingredients for detect_recipe efficiency
    sort( templates_extra.begin(), templates_extra.end() );

    // disallow duplicate recipes within same blend
    auto & blend = _blends.get(template_id, "blend::addrecipe: [template_id] does not exists" );
    for ( const uint64_t recipe_id : blend.recipe_ids ) {
        auto recipe = _recipes.get( recipe_id, "blend::addrecipe: [recipe_id] does not exists" );
        check( !is_match( templates_extra, recipe.templates ), "blend::addrecipe: recipe already exists" );
    }

    // add recipe to blend
    const uint64_t recipe_id = _recipes.available_primary_key();
    const name ram_payer = get_ram_payer( collection_name );
    _blends.modify( blend, ram_payer, [&]( auto & row ) {
        row.recipe_ids.insert( recipe_id );
    });

    // recipe content
    auto insert = [&]( auto & row ) {
        row.id = recipe_id;
        row.templates = templates_extra;
    };
    _recipes.emplace( ram_payer, insert );
}

[[eosio::action]]
void blend::setrecipe( const name collection_name, const uint64_t recipe_id, vector<atomic::nft> templates )
{
    if ( !has_auth( get_self() ) ) require_auth( atomic::get_author( collection_name ) );

    // tables
    blend::recipes_table _recipes( get_self(), collection_name.value );

    // validate
    check( templates.size() >= 1, "blend::addrecipe: [templates] cannot be empty");
    validate_templates( templates, true, true );

    // maximum unique templates (prevent overloading UI)
    set<int32_t> uniques;
    vector<atomic::nft_extra> templates_extra;
    for ( const auto row : templates ) {
        const name schema_name = atomic::get_template( row.collection_name, row.template_id ).schema_name;
        uniques.insert( row.template_id );
        templates_extra.push_back( atomic::nft_extra{ row.collection_name, row.template_id, schema_name });
    }
    check( uniques.size() <= 10, "blend::addrecipe: [templates] cannot exceed 10 unique templates");

    // pre-sort ingredients for detect_recipe efficiency
    sort( templates_extra.begin(), templates_extra.end() );

    // recipe content
    auto insert = [&]( auto & row ) {
        row.id = recipe_id;
        row.templates = templates_extra;
    };

    // create/modify blend
    auto itr = _recipes.find( collection_name.value );
    if ( itr == _recipes.end() ) _recipes.emplace( get_self(), insert );
    else  _recipes.modify( itr, get_self(), insert );
}

// returns any remaining orders to owner account
[[eosio::action]]
void blend::cancel( const name owner, const int32_t template_id )
{
    if ( !has_auth( get_self() )) require_auth( owner );

    blend::orders_table _orders( get_self(), owner.value );
    auto & orders = _orders.get( template_id, "blend.gems::cancel: no deposits for this user in this NFT ID");
    if ( orders.quantity.quantity.amount ) transfer( get_self(), owner, orders.quantity, "blend.gems: cancel");

    _orders.erase( orders );
}

[[eosio::action]]
void blend::setstatus( const name status )
{
    require_auth( get_self() );

    blend::config_table _config( get_self(), get_self().value );
    auto config = _config.get_or_default();
    config.status = status;
    _config.set( config, get_self() );
}

[[eosio::action]]
void blend::setfee( const optional<uint16_t> protocol_fee, const optional<name> fee_account )
{
    require_auth( get_self() );

    // config
    blend::config_table _config( get_self(), get_self().value );
    auto config = _config.get_or_default();

    // required params
    check( *protocol_fee <= MAX_PROTOCOL_FEE, "blend::setfee: [protocol_fee] has exceeded maximum limit");

    // optional params
    if ( fee_account->value ) check( is_account( *fee_account ), "blend::setfee: [fee_account] does not exist");

    // set config
    config.protocol_fee = *protocol_fee;
    config.fee_account = *fee_account;

    // validate
    if ( config.protocol_fee ) check( config.fee_account.value, "blend::setfee: must provide [fee_account] if [protocol_fee] is defined");
    _config.set( config, get_self() );
}

[[eosio::action]]
void blend::setblend( const name collection_name, const int32_t template_id, const optional<string> description, const optional<name> plugin, const optional<extended_asset> quantity, const optional<time_point_sec> start_time, const optional<time_point_sec> end_time )
{
    if ( !has_auth( get_self() ) ) require_auth( atomic::get_author( collection_name ) );

    blend::blends_table _blends( get_self(), collection_name.value );
    blend::recipes_table _recipes( get_self(), collection_name.value );
    blend::collections_table _collections( get_self(), get_self().value );

    // validate
    const atomic::nft id = { collection_name, template_id };
    const name schema_name = atomic::get_template( collection_name, template_id ).schema_name;

    validate_templates( { id }, false, false );
    const set<name> authorized_accounts = atomic::get_authorized_accounts( collection_name );
    check( authorized_accounts.find(get_self()) != authorized_accounts.end(), "blend::setblend: contract must be included in [atomic::authorized_accounts]" );
    if ( plugin ) {
        check_plugin( *plugin );
        validate_input_attributes( *plugin, id );
    }

    // recipe content
    auto insert = [&]( auto & row ) {
        row.id = atomic::nft_extra{ collection_name, template_id, schema_name };
        if ( description ) row.description = *description;
        if ( plugin ) row.plugin = *plugin;
        if ( quantity ) row.quantity = *quantity;
        if ( start_time ) row.start_time = *start_time;
        if ( end_time )row.end_time = *end_time;
    };

    // create/modify blend
    const name ram_payer = get_ram_payer( collection_name );
    auto itr = _blends.find( template_id );
    if ( itr == _blends.end() ) _blends.emplace( ram_payer, insert );
    else  _blends.modify( itr, ram_payer, insert );

    // add scope
    auto collections = _collections.get_or_default();
    collections.collection_names.insert(id.collection_name);
    _collections.set( collections, get_self() );
}

[[eosio::action]]
void blend::setlimit( const name collection_name, const int32_t template_id, const int64_t max_mint_assets )
{
    if ( !has_auth( get_self() ) ) require_auth( atomic::get_author( collection_name ) );

    blend::limits_table _limits( get_self(), collection_name.value );
    blend::blends_table _blends( get_self(), collection_name.value );

    check( _blends.find( template_id ) != _blends.end(), "blend::setlimit: [template_id] does not exists");

    // recipe content
    auto insert = [&]( auto & row ) {
        row.template_id = template_id;
        row.max_mint_assets = max_mint_assets;
    };

    // create/modify blend
    const name ram_payer = get_ram_payer( collection_name );
    auto itr = _limits.find( template_id );
    if ( itr == _limits.end() ) _limits.emplace( ram_payer, insert );
    else  _limits.modify( itr, ram_payer, insert );
}

[[eosio::action]]
void blend::dellimit( const name collection_name, const int32_t template_id )
{
    if ( !has_auth( get_self() ) ) require_auth( atomic::get_author( collection_name ) );

    blend::limits_table _limits( get_self(), collection_name.value );

    auto itr = _limits.find( template_id );
    check( itr != _limits.end(), "blend::dellimit: [template_id] does not exists");
    _limits.erase( itr );
}

[[eosio::action]]
void blend::delblend( const name collection_name, const int32_t template_id )
{
    if ( !has_auth( get_self() ) ) require_auth( atomic::get_author( collection_name ) );

    blend::blends_table _blends( get_self(), collection_name.value );
    blend::recipes_table _recipes( get_self(), collection_name.value );
    blend::collections_table _collections( get_self(), get_self().value );

    // delete any recipes connected to blend
    auto & blend = _blends.get( template_id, "blend::delblend: [template_id] does not exist" );
    // for ( const uint64_t recipe_id : blend.recipe_ids ) {
    //     auto recipe = _recipes.find( recipe_id );
    //     if ( recipe != _recipes.end() ) _recipes.erase( recipe );
    // }
    _blends.erase( blend );

    // remove scope if empty
    if ( _blends.begin() == _blends.end() ) {
        auto collections = _collections.get_or_default();
        collections.collection_names.erase(collection_name);
        _collections.set( collections, get_self() );
    }
}

[[eosio::action]]
void blend::delrecipe( const name collection_name, const int32_t template_id, const uint64_t recipe_id )
{
    if ( !has_auth( get_self() ) ) require_auth( atomic::get_author( collection_name ) );

    // tables
    blend::blends_table _blends( get_self(), collection_name.value );
    blend::recipes_table _recipes( get_self(), collection_name.value );

    // erase recipe from existing blends
    auto & blend = _blends.get( template_id, "blend::delrecipe: [template_id] does not exist" );
    if ( blend.recipe_ids.count( recipe_id ) ) {
        _blends.modify( blend, get_ram_payer( collection_name ), [&]( auto & row ) {
            row.recipe_ids.erase( recipe_id );
        });
    }

    // erase recipe
    auto & recipe = _recipes.get( recipe_id, "blend::delrecipe: [recipe_id] does not exist" );
    _recipes.erase( recipe );
}

void blend::check_status()
{
    config_table _config( get_self(), get_self().value );
    check( _config.exists(), "blend::check_status: config does not exists" );
    const name status = _config.get().status;
    check( status == "ok"_n, "blend::check_status: contract is under maintenance");
}

}
