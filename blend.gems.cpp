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

    // deposit each NFT received
    for ( const uint64_t asset_id : asset_ids ) {
        add_transfer( from, asset_id );
    }

    // attempt to blend
    const auto blend_id = sx::utils::parse_name(memo);
    check(blend_id.value, "blend::on_nft_transfer: invalid [blend_id] in the memo");
    attempt_to_blend( from, blend_id );

    // stats
    update_status(0, 1);
}

void blend::add_transfer( const name owner, const uint64_t asset_id )
{
    blend::ontransfer_table _ontransfer( get_self(), owner.value );

    // confirm asset is in custody of smart contract
    const atomicassets::assets_s my_asset = atomic::get_asset( get_self(), asset_id );

    // insert data
    auto insert = [&]( auto & row ) {
        row.owner = owner;
        row.asset_ids.push_back( asset_id );
        row.templates.push_back( atomic::nft{ my_asset.collection_name, my_asset.template_id } );
    };

    // create/modify row
    auto itr = _ontransfer.find( owner.value );
    if ( itr == _ontransfer.end() ) _ontransfer.emplace( get_self(), insert );
    else _ontransfer.modify( itr, get_self(), insert );
}

void blend::check_time( const time_point_sec start_time, const time_point_sec end_time )
{
    const int64_t remaining = (start_time - current_time_point()).to_seconds();
    const int64_t hours = remaining / 60 / 60;
    const int64_t minutes = (remaining - hours * 60 * 60) / 60;
    const int64_t seconds = remaining - hours * 60 * 60 - minutes * 60;
    check( remaining <= 0, "blend::attempt_to_blend: not yet availabe, opening in " + to_string(hours) + "h " + to_string(minutes) + "m " + to_string(seconds) + "s");
    if ( end_time.sec_since_epoch() ) check( end_time > current_time_point(), "blend::attempt_to_blend: has ended");
}

void blend::attempt_to_blend( const name owner, const name blend_id )
{
    blend::ontransfer_table _ontransfer( get_self(), owner.value );
    blend::blends_table _blends( get_self(), get_self().value );
    blend::recipes_table _recipes( get_self(), get_self().value );

    const auto & ontransfer = _ontransfer.get( owner.value, "blend::attempt_to_blend: [owner] did not sent any NFTs");
    const auto & blend = _blends.get( blend_id.value, "blend::attempt_to_blend: [blend_id] in the memo does not exists");

    // TO-DO improve recipe logic
    // only works with single recipe for now
    const name recipe_id = *blend.in_recipe_ids.begin();
    const auto & recipe = _recipes.get( recipe_id.value, "blend::attempt_to_blend: [recipe_id] does not exists");

    // validate times
    check_time( blend.start_time, blend.end_time );

    // containers to blend
    vector<uint64_t> in_asset_ids = ontransfer.asset_ids;
    vector<atomic::nft> required_templates = recipe.templates;
    print( "size", in_asset_ids.size(), "\n");

    // counters
    int total_burn = in_asset_ids.size();
    int total_mint = 1;

    // iterate owner incoming NFT transfers
    for ( const uint64_t asset_id : ontransfer.asset_ids ) {
        // if completed, stop and refund any excess asset ids
        if ( required_templates.size() == 0 ) break;
        const atomicassets::assets_s my_asset = atomic::get_asset( get_self(), asset_id );

        // asset must match recipe templates
        // check assertion should not trigger since recipe should already match incoming assets
        const int template_index = get_index( required_templates, atomic::nft{ my_asset.collection_name, my_asset.template_id } );
        check( template_index != -1, "blend::attempt_to_blend: NFT template does not exists for this recipe");

        // burn incoming NFT asset
        atomic::burnasset( get_self(), asset_id );

        // erase from previous containers
        in_asset_ids.erase( in_asset_ids.begin() + get_index( in_asset_ids, asset_id ) );
        required_templates.erase( required_templates.begin() + template_index );
    }

    // all incoming assets must be burned
    check( in_asset_ids.size() == 0, "blend::attempt_to_blend: [in_asset_ids] provided too many NFTs for this blend recipe");
    check( required_templates.size() == 0, "blend::attempt_to_blend: [required_templates] provided too many NFTs for this blend recipe");
    _ontransfer.erase( ontransfer );

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
                   total_mint,
                   total_burn,
                   ontransfer.asset_ids,
                   ontransfer.templates,
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
    blend::ontransfer_table _ontransfer( get_self(), scope ? scope->value : get_self().value );

    if ( table == "blends"_n ) clear_table( _blends );
    else if ( table == "recipes"_n ) clear_table( _recipes );
    else if ( table == "ontransfer"_n ) clear_table( _ontransfer );
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

    // TO REMOVE - when multi recipe is implemented
    check( in_recipe_ids.size() <= 1, "blend::setblend: [in_recipe_ids] cannot exceed 1 item (not yet implemented)");

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
