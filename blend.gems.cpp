#include <eosio.token/eosio.token.hpp>
#include <atomicassets/atomicassets.hpp>

#include "blend.gems.hpp"
#include "src/helpers.cpp"

namespace gems {

[[eosio::on_notify("atomicassets::transfer")]]
void blend::on_nft_transfer( const name from, const name to, const vector<uint64_t> asset_ids, const std::string memo )
{
    require_auth( from );

    // tables
    blend::ontransfer_table _ontransfer( get_self(), get_self().value );

    // ignore transfers
    if ( from == get_self() || memo == get_self().to_string() ) return;

    // iterate over each NFT received
    for ( const uint64_t asset_id : asset_ids ) {
        add_transfer( from, asset_id );
        attempt_to_blend( from );
    }
}

void blend::add_transfer( const name owner, const uint64_t asset_id )
{
    blend::ontransfer_table _ontransfer( get_self(), get_self().value );
    blend::templates_table _templates( get_self(), get_self().value );

    auto itr = _ontransfer.find( owner.value );
    auto my_asset = get_assets( owner, asset_id );
    const name collection_name = my_asset.collection_name;
    const int32_t template_id = my_asset.template_id;
    const name recipe_id = _templates.get( template_id, "blend::add_transfer: `template_id` does not belong to any blend recipes").recipe_id;

    // if owner has already started recipe
    if ( itr != _ontransfer.end() ) {
        check( collection_name == itr->collection_name, "blend::add_transfer: owner has already started blend, must send same collection or call `refund` ACTION");
        check( recipe_id == itr->recipe_id, "blend::add_transfer: owner has already started blend, must send same `recipe_id` or call `refund` ACTION");
    }

    // insert data
    auto insert = [&]( auto & row ) {
        row.collection_name = collection_name;
        row.recipe_id = recipe_id;
        row.asset_ids.push_back( asset_id );
        row.template_ids.push_back( template_id );
        row.last_updated = current_time_point();
    };

    // create/modify row
    if ( itr == _ontransfer.end() ) _ontransfer.emplace( get_self(), insert );
    else _ontransfer.modify( itr, get_self(), insert );
}

void blend::attempt_to_blend( const name owner )
{
    check( false, "to-do");
}

[[eosio::action]]
void blend::blendlog( const name owner,
                      const name collection_name,
                      const name recipe_id,
                      const vector<uint64_t> in_asset_ids,
                      const vector<uint64_t> out_asset_ids,
                      const vector<int32_t> in_template_ids,
                      const vector<int32_t> out_template_id )
{
    require_auth( get_self() );
    require_recipient( owner );
}

[[eosio::action]]
void blend::reset( const name table )
{
    require_auth( get_self() );

    blend::recipes_table _recipes( get_self(), get_self().value );
    blend::templates_table _templates( get_self(), get_self().value );
    blend::ontransfer_table _ontransfer( get_self(), get_self().value );
    blend::global_table _global( get_self(), get_self().value );

    if ( table == "recipes"_n ) clear_table( _recipes );
    else if ( table == "templates"_n ) clear_table( _templates );
    else if ( table == "ontransfer"_n ) clear_table( _ontransfer );
    else if ( table == "global"_n ) _global.remove();
    else check( false, "invalid table name");
}

void blend::validate_template_ids( const name collection_name, const vector<int32_t> template_ids, const bool burnable )
{
    for ( const int32_t template_id : template_ids ) {
        const auto my_template = get_template( collection_name, template_id );
        if ( burnable ) check( my_template.burnable, "`template_id` must be `burnable`");
        check( my_template.transferable, "`template_id` must be `transferable`");
    }
}

[[eosio::action]]
void blend::setrecipe( const name recipe_id, const name collection_name, const vector<int32_t> in_template_ids, const vector<int32_t> out_template_ids, const asset backed_token, const time_point_sec start_time )
{
    require_auth( get_self() );

    blend::recipes_table _recipes( get_self(), get_self().value );

    // validate
    validate_template_ids( collection_name, in_template_ids, true );
    validate_template_ids( collection_name, out_template_ids, false );

    // recipe content
    auto insert = [&]( auto & row ) {
        row.recipe_id = recipe_id;
        row.collection_name = collection_name;
        row.in_template_ids = in_template_ids;
        row.out_template_ids = out_template_ids;
        row.backed_token = backed_token;
        row.start_time = start_time;
        row.last_updated = current_time_point();
    };

    // create/modify asset
    auto itr = _recipes.find( recipe_id.value );
    if ( itr == _recipes.end() ) _recipes.emplace( get_self(), insert );
    else _recipes.modify( itr, get_self(), insert );
}

[[eosio::action]]
void blend::delrecipe( const name recipe_id )
{
    require_auth( get_self() );

    blend::recipes_table _recipes( get_self(), get_self().value );
    blend::templates_table _templates( get_self(), get_self().value );
    auto & recipes = _recipes.get( recipe_id.value, "blend::delrecipe: `recipe_id` does not exist" );

    // erase any linked template id to recipe
    for ( const int32_t template_id : recipes.in_template_ids ) {
        auto & row = _templates.get( template_id, "blend::delrecipe: `template_id` does not exist" );
        _templates.erase( row );
    }
    _recipes.erase( recipes );
}

[[eosio::action]]
void blend::refund( const name owner )
{
    require_auth( get_self() );

    blend::ontransfer_table _ontransfer( get_self(), get_self().value );
    auto &itr = _ontransfer.get( owner.value, "blend::refund: `owner` does not exist" );

    check( false, "to-do");
}

}