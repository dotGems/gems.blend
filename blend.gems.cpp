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
        check( collection_name == itr->collection_name, "blend::add_transfer: owner has already started blend, must send from same `collection_name` or call `refund` ACTION");
        check( blend_id == itr->blend_id, "blend::add_transfer: owner has already started blend, must send from same `blend_id` or call `refund` ACTION");
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
    blend::ontransfer_table _ontransfer( get_self(), get_self().value );
    blend::blends_table _blends( get_self(), get_self().value );
    blend::global_table _global( get_self(), get_self().value );

    const auto & ontransfer = _ontransfer.get( owner.value, "blend::attempt_to_blend: `owner` does not exists");
    const auto & blends = _blends.get( ontransfer.blend_id.value, "blend::attempt_to_blend: `blend_id` does not exists");

    // enforce start time if available
    const int64_t remaining = (blends.start_time - current_time_point()).to_seconds();
    const int64_t hours = remaining / 60 / 60;
    const int64_t minutes = (remaining - hours * 60 * 60) / 60;
    const int64_t seconds = remaining - hours * 60 * 60 - minutes * 60;
    check( remaining <= 0, "blend::attempt_to_blend: not yet availabe, opening in " + to_string(hours) + "h " + to_string(minutes) + "m " + to_string(seconds) + "s");

    // containers to blend
    vector<uint64_t> asset_ids = ontransfer.asset_ids;
    vector<uint64_t> refund_asset_ids = ontransfer.asset_ids;
    vector<int32_t> in_template_ids = blends.in_template_ids;

    // counters
    uint64_t total_mint = 0;
    uint64_t total_burn = 0;
    asset total_backed_token = { 0, blends.backed_token.symbol };

    // iterate owner incoming NFT transfers
    for ( const uint64_t asset_id : asset_ids ) {
        // if completed, stop and refund any excess asset ids
        if ( in_template_ids.size() == 0 ) break;
        auto my_asset = get_assets( get_self(), asset_id );
        const int32_t template_id = my_asset.template_id;
        const int i = get_index( in_template_ids, template_id );
        check( i >= 0, "blend::attempt_to_blend: missing `template_id` from `blends::in_template_ids`");

        // erase from previous containers
        burnasset( get_self(), asset_id );
        total_burn += 1;
        refund_asset_ids.erase( refund_asset_ids.begin() + get_index( refund_asset_ids, asset_id ));
        in_template_ids.erase( in_template_ids.begin() + get_index( in_template_ids, template_id ));
    }

    // error if remaining template ids not blended
    check( in_template_ids.size() == 0, "blend::attempt_to_blend: not providing enough `template_id` for `blends::in_template_ids`");

    // mint blended NFT asset to owner
    for ( const int32_t out_template_id : blends.out_template_ids ) {
        const name schema = get_template( blends.in_collection_name, out_template_id ).schema_name;
        const asset backed_token = blends.backed_token;
        const vector<asset> tokens_to_back = backed_token.amount ? vector<asset>{ backed_token } : vector<asset>{};
        if ( backed_token.amount ) {
            announce_deposit( get_self(), backed_token.symbol );
            transfer( get_self(), "atomicassets"_n, { backed_token, "eosio.token"_n }, "deposit");
            total_backed_token += backed_token;
        }
        mintasset( get_self(), blends.out_collection_name, schema, out_template_id, owner, {}, {}, tokens_to_back );
        total_mint += 1;
    }
    // update mints & burn statistics counters
    _blends.modify( blends, get_self(), [&]( auto & row ) {
        row.total_mint += total_mint;
        row.total_burn += total_burn;
        row.total_backed_token += total_backed_token;
        row.last_updated = current_time_point();
    });
    auto global = _global.get_or_default();
    global.total_mint += total_mint;
    global.total_burn += total_burn;
    global.total_backed_token.symbol = total_backed_token.symbol;
    global.total_backed_token += total_backed_token;
    _global.set( global, get_self() );

    // return & erase any excess asset_ids
    if ( refund_asset_ids.size() ) transfer_nft( get_self(), owner, refund_asset_ids, "blend refund" );
    _ontransfer.erase( ontransfer );

    // logging
    blend::blendlog_action blendlog( get_self(), { get_self(), "active"_n });
    blendlog.send( owner,
                   blends.in_collection_name,
                   blends.out_collection_name,
                   blends.blend_id,
                   total_mint,
                   total_burn,
                   total_backed_token,
                   asset_ids,
                   blends.out_template_ids,
                   refund_asset_ids );
}

[[eosio::action]]
void blend::blendlog( const name owner,
                      const name in_collection_name,
                      const name out_collection_name,
                      const name blend_id,
                      const uint64_t total_mint,
                      const uint64_t total_burn,
                      const asset total_backed_token,
                      const vector<uint64_t> in_asset_ids,
                      const vector<int32_t> blend_template_ids,
                      const vector<uint64_t> refund_asset_ids )
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
void blend::setblend( const name blend_id, const name in_collection_name, const vector<int32_t> in_template_ids, const name out_collection_name, const vector<int32_t> out_template_ids, const asset backed_token, const optional<time_point_sec> start_time )
{
    require_auth( get_self() );

    blend::blends_table _blends( get_self(), get_self().value );
    blend::templates_table templates( get_self(), get_self().value );

    // validate
    validate_template_ids( in_collection_name, in_template_ids, true );
    validate_template_ids( out_collection_name, out_template_ids, false );

    // enforce tokens to back
    check( backed_token.symbol == EOS || backed_token.symbol == WAX, "blend::setblend: `backed_token` symbol must match 8,WAX or 4,EOS");

    // recipe content
    auto insert = [&]( auto & row ) {
        row.blend_id = blend_id;
        row.in_collection_name = in_collection_name;
        row.in_template_ids = in_template_ids;
        row.out_collection_name = out_collection_name;
        row.out_template_ids = out_template_ids;
        row.backed_token = backed_token;
        row.start_time = *start_time;
        row.last_updated = current_time_point();
        row.total_backed_token.symbol = backed_token.symbol;
    };

    // create/modify row
    auto itr = _blends.find( blend_id.value );
    if ( itr == _blends.end() ) _blends.emplace( get_self(), insert );
    else {
        // modify previous data
        check_templates_already_exists( blend_id, in_template_ids );
        erase_blend_templates( blend_id );
        _blends.modify( itr, get_self(), insert );
    }

    // add input template
    for ( const int32_t template_id : in_template_ids ) {
        add_template( in_collection_name, template_id, blend_id );
    }
}

void blend::check_templates_already_exists( const name blend_id, const vector<int32_t> template_ids )
{
    blend::templates_table _templates( get_self(), get_self().value );

    for ( const int32_t template_id : template_ids ) {
        auto itr = _templates.find( template_id );
        if ( itr == _templates.end() ) continue;
        if ( itr->blend_id != blend_id ) check( itr == _templates.end(), "blend::check_templates_already_exists: `template_id` already exists for another blend, must first call `delblend` ACTION");
    }
}

void blend::add_template( const name collection_name, const int32_t template_id, const name blend_id )
{
    blend::templates_table _templates( get_self(), get_self().value );

    // template id matches existing recipe
    auto itr = _templates.find( template_id );

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
    auto & blends = _blends.get( blend_id.value, "blend::delblend: `blend_id` does not exist" );
    erase_blend_templates( blend_id );
    _blends.erase( blends );
}

void blend::erase_blend_templates( const name blend_id )
{
    blend::blends_table _blends( get_self(), get_self().value );
    blend::templates_table _templates( get_self(), get_self().value );
    auto & blends = _blends.get( blend_id.value, "blend::erase_blend_templates: `blend_id` does not exist" );

    for ( const int32_t template_id : blends.in_template_ids ) {
        auto itr = _templates.find( template_id );
        if ( itr != _templates.end() ) _templates.erase( itr );
    }
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

int blend::get_index( const vector<name> vec, const name value )
{
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == value ) return i;
    }
    return -1;
}

int blend::get_index( const vector<int32_t> vec, const int32_t value )
{
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == value ) return i;
    }
    return -1;
}

int blend::get_index( const vector<uint64_t> vec, const uint64_t value )
{
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == value ) return i;
    }
    return -1;
}

}
