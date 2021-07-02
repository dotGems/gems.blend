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
    }
    attempt_to_blend( from );
}

void blend::add_transfer( const name owner, const uint64_t asset_id )
{
    blend::ontransfer_table _ontransfer( get_self(), get_self().value );
    blend::templates_table _templates( get_self(), get_self().value );

    auto itr = _ontransfer.find( owner.value );
    auto my_asset = get_assets( get_self(), asset_id );
    const name collection_name = my_asset.collection_name;
    const int32_t template_id = my_asset.template_id;
    const name blend_id = _templates.get( template_id, "blend::add_transfer: `template_id` does not belong to any blends").blend_id;

    // if owner has already started recipe
    if ( itr != _ontransfer.end() ) {
        check( collection_name == itr->collection_name, "blend::add_transfer: owner has already started blend, must send same collection or call `refund` ACTION");
        check( blend_id == itr->blend_id, "blend::add_transfer: owner has already started blend, must send same `blend_id` or call `refund` ACTION");
    }

    // insert data
    auto insert = [&]( auto & row ) {
        row.owner = owner;
        row.collection_name = collection_name;
        row.blend_id = blend_id;
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
    // check( false, "to-do");
    print("blending");
}

[[eosio::action]]
void blend::blendlog( const name owner,
                      const name collection_name,
                      const name blend_id,
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

    blend::blends_table _blends( get_self(), get_self().value );
    blend::templates_table _templates( get_self(), get_self().value );
    blend::ontransfer_table _ontransfer( get_self(), get_self().value );
    blend::global_table _global( get_self(), get_self().value );

    if ( table == "blends"_n ) clear_table( _blends );
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
void blend::setblend( const name blend_id, const name collection_name, const vector<int32_t> in_template_ids, const vector<int32_t> out_template_ids, const optional<asset> backed_token, const optional<time_point_sec> start_time )
{
    require_auth( get_self() );

    blend::blends_table _blends( get_self(), get_self().value );
    blend::templates_table templates( get_self(), get_self().value );

    // validate
    validate_template_ids( collection_name, in_template_ids, true );
    validate_template_ids( collection_name, out_template_ids, false );

    // recipe content
    auto insert = [&]( auto & row ) {
        row.blend_id = blend_id;
        row.collection_name = collection_name;
        row.in_template_ids = in_template_ids;
        row.out_template_ids = out_template_ids;
        row.backed_token = *backed_token;
        row.start_time = *start_time;
        row.last_updated = current_time_point();
    };

    // add input template
    for ( const int32_t template_id : in_template_ids ) {
        add_template( collection_name, template_id, blend_id );
    }

    // create/modify asset
    auto itr = _blends.find( blend_id.value );
    if ( itr == _blends.end() ) _blends.emplace( get_self(), insert );
    else _blends.modify( itr, get_self(), insert );

    // TO-DO erase any existing `template_id`
    // check( false, "NOT IMPLEMENTED: to modify first call `delblend` ACTION");
}

void blend::add_template( const name collection_name, const int32_t template_id, const name blend_id )
{
    blend::templates_table _templates( get_self(), get_self().value );

    // template id matches existing recipe
    auto itr = _templates.find( template_id );
    // check( itr == _templates.end(), "blend::add_template: `template_id` already exists for another recipe, must call `delblend` ACTION");

    // insert data
    auto insert = [&]( auto & row ) {
        row.template_id = template_id;
        row.collection_name = collection_name;
        row.schema_name = get_template( collection_name, template_id ).schema_name;
        row.blend_id = blend_id;
    };

    // create/modify row
    if ( itr == _templates.end() ) _templates.emplace( get_self(), insert );
    else _templates.modify( itr, get_self(), insert );
}

[[eosio::action]]
void blend::delblend( const name blend_id )
{
    require_auth( get_self() );

    blend::blends_table _blends( get_self(), get_self().value );
    blend::templates_table _templates( get_self(), get_self().value );
    auto & blends = _blends.get( blend_id.value, "blend::delblend: `blend_id` does not exist" );

    // erase any linked template id to recipe
    for ( const int32_t template_id : blends.in_template_ids ) {
        auto & row = _templates.get( template_id, "blend::delblend: `template_id` does not exist" );
        _templates.erase( row );
    }
    _blends.erase( blends );
}

[[eosio::action]]
void blend::refund( const name owner )
{
    if ( !has_auth( get_self() )) require_auth( owner );

    blend::ontransfer_table _ontransfer( get_self(), get_self().value );
    auto &itr = _ontransfer.get( owner.value, "blend::refund: `owner` does not have any refundable NFT assets" );

    transfer_nft( get_self(), owner, itr.asset_ids, "blend::refund" );

    _ontransfer.erase( itr );
}

}