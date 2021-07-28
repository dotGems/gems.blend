#include <eosio.token/eosio.token.hpp>
#include <sx.utils/utils.hpp>

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

    const auto blend_id = sx::utils::parse_name(memo);
    check(blend_id.value, "blend::on_nft_transfer: invalid `blend_id` in the memo");

    // iterate over each NFT received
    for ( const uint64_t asset_id : asset_ids ) {
        add_transfer( from, blend_id, asset_id );
    }
    attempt_to_blend( from );
}

void blend::add_transfer( const name owner, const name blend_id, const uint64_t asset_id )
{
    blend::ontransfer_table _ontransfer( get_self(), get_self().value );

    auto my_asset = atomic::get_assets( get_self(), asset_id );
    const atomic::nft templ { my_asset.collection_name, my_asset.template_id };
    auto itr = _ontransfer.find( owner.value );

    // if owner has already started recipe
    if ( itr != _ontransfer.end() ) {
        check( blend_id == itr->blend_id, "blend::add_transfer: owner has already started blend, must send from same `blend_id` or call `refund` ACTION");
    }

    // insert data
    auto insert = [&]( auto & row ) {
        row.owner = owner;
        row.blend_id = blend_id;
        row.asset_ids.push_back( asset_id );
        row.templates.push_back( templ );
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
    const auto & blend = _blends.get( ontransfer.blend_id.value, "blend::attempt_to_blend: `blend_id` does not exists");

    // enforce start time if available
    const int64_t remaining = (blend.start_time - current_time_point()).to_seconds();
    const int64_t hours = remaining / 60 / 60;
    const int64_t minutes = (remaining - hours * 60 * 60) / 60;
    const int64_t seconds = remaining - hours * 60 * 60 - minutes * 60;
    check( remaining <= 0, "blend::attempt_to_blend: not yet availabe, opening in " + to_string(hours) + "h " + to_string(minutes) + "m " + to_string(seconds) + "s");

    // containers to blend
    const auto asset_ids = ontransfer.asset_ids;
    auto refund_asset_ids = ontransfer.asset_ids;
    auto in_templates = blend.in_templates;

    // counters
    uint64_t total_mint = 0;
    uint64_t total_burn = 0;
    vector<asset> total_backed_tokens;

    // iterate owner incoming NFT transfers
    for ( const uint64_t asset_id : asset_ids ) {
        // if completed, stop and refund any excess asset ids
        if ( in_templates.size() == 0 ) break;
        auto my_asset = atomic::get_assets( get_self(), asset_id );
        const atomic::nft templ = { my_asset.collection_name, my_asset.template_id };
        const int i = get_index( in_templates, templ );
        check( i >= 0, "blend::attempt_to_blend: `template` is not part of this `blend_id`");

        // erase from previous containers
        atomic::burnasset( get_self(), asset_id );
        total_burn += 1;
        refund_asset_ids.erase( refund_asset_ids.begin() + get_index( refund_asset_ids, asset_id ));
        in_templates.erase( in_templates.begin() + get_index( in_templates, templ ));
    }

    // error if remaining template ids not blended
    check( in_templates.size() == 0, "blend::attempt_to_blend: not providing enough `asset_ids` for this `blend_id`");

    // mint blended NFT asset to owner
    for ( const auto& out_template : blend.out_templates ) {
        const name schema = atomic::get_template( out_template.collection_name, out_template.template_id ).schema_name;
        const auto& backed_tokens = blend.backed_tokens;
        for( const auto& backed_token: backed_tokens ){
            atomic::announce_deposit( get_self(), backed_token.symbol );
            transfer( get_self(), "atomicassets"_n, { backed_token, "eosio.token"_n }, "deposit");
            auto index = get_index(total_backed_tokens, backed_token.symbol);
            if(index == -1) total_backed_tokens.push_back(backed_token);
            else total_backed_tokens[index] += backed_token;
        }
        atomic::mintasset( get_self(), out_template.collection_name, schema, out_template.template_id, owner, {}, {}, backed_tokens );
        total_mint += 1;
    }
    // update mints & burn statistics counters
    _blends.modify( blend, get_self(), [&]( auto & row ) {
        row.total_mint += total_mint;
        row.total_burn += total_burn;
        for( const auto& backed_token: total_backed_tokens ){
            auto index = get_index(row.total_backed_tokens, backed_token.symbol);
            if(index == -1) row.total_backed_tokens.push_back(backed_token);
            else row.total_backed_tokens[index] += backed_token;
        }
        row.last_updated = current_time_point();
    });
    auto global = _global.get_or_default();
    global.total_mint += total_mint;
    global.total_burn += total_burn;
    for( const auto& backed_token: total_backed_tokens ){
        auto index = get_index(global.total_backed_tokens, backed_token.symbol);
        if(index == -1) global.total_backed_tokens.push_back(backed_token);
        else global.total_backed_tokens[index] += backed_token;
    }
    _global.set( global, get_self() );

    // return & erase any excess asset_ids
    if ( refund_asset_ids.size() ) atomic::transfer_nft( get_self(), owner, refund_asset_ids, "blend refund" );
    _ontransfer.erase( ontransfer );

    // logging
    blend::blendlog_action blendlog( get_self(), { get_self(), "active"_n });
    blendlog.send( owner,
                   blend.blend_id,
                   total_mint,
                   total_burn,
                   total_backed_tokens,
                   asset_ids,
                   blend.out_templates,
                   refund_asset_ids );
}

[[eosio::action]]
void blend::blendlog( const name owner,
                      const name blend_id,
                      const uint64_t total_mint,
                      const uint64_t total_burn,
                      const vector<asset> total_backed_tokens,
                      const vector<uint64_t> in_asset_ids,
                      const vector<atomic::nft> blend_templates,
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
    blend::ontransfer_table _ontransfer( get_self(), get_self().value );
    blend::global_table _global( get_self(), get_self().value );

    if ( table == "blends"_n ) clear_table( _blends );
    else if ( table == "ontransfer"_n ) clear_table( _ontransfer );
    else if ( table == "global"_n ) _global.remove();
    else check( false, "invalid table name");
}

void blend::validate_templates( const vector<atomic::nft> templates, const bool burnable )
{
    for ( const auto& templ : templates ) {
        const auto my_template = atomic::get_template( templ.collection_name, templ.template_id );
        if ( burnable ) check( my_template.burnable, "blend::validate_templates: `template` must be `burnable`");
        check( my_template.transferable, "blend::validate_templates: `template` must be `transferable`");
    }
}

[[eosio::action]]
void blend::setblend( const name blend_id, const vector<atomic::nft> in_templates, const vector<atomic::nft> out_templates, const vector<asset> backed_tokens, const optional<time_point_sec> start_time )
{
    require_auth( get_self() );

    blend::blends_table _blends( get_self(), get_self().value );
    vector<asset> total_backed_tokens;

    // validate
    validate_templates( in_templates, true );
    validate_templates( out_templates, false );

    // enforce tokens to back
    for(const auto backed_token: backed_tokens){
        check( backed_token.symbol == EOS || backed_token.symbol == WAX, "blend::setblend: `backed_token` symbol must match 8,WAX or 4,EOS");
        check( backed_token.amount, "blend::setblend: `backed_token` should have value");
        total_backed_tokens.push_back({ 0, backed_token.symbol });
    }

    // recipe content
    auto insert = [&]( auto & row ) {
        row.blend_id = blend_id;
        row.in_templates = in_templates;
        row.out_templates = out_templates;
        row.backed_tokens = backed_tokens;
        row.start_time = *start_time;
        row.last_updated = current_time_point();
        row.total_backed_tokens = total_backed_tokens;
    };

    // create/modify row
    auto itr = _blends.find( blend_id.value );
    if ( itr == _blends.end() ) _blends.emplace( get_self(), insert );
    else  _blends.modify( itr, get_self(), insert );
}


[[eosio::action]]
void blend::delblend( const name blend_id )
{
    require_auth( get_self() );

    blend::blends_table _blends( get_self(), get_self().value );
    auto & blends = _blends.get( blend_id.value, "blend::delblend: `blend_id` does not exist" );
    _blends.erase( blends );
}

[[eosio::action]]
void blend::refund( const name owner )
{
    if ( !has_auth( get_self() )) require_auth( owner );

    blend::ontransfer_table _ontransfer( get_self(), get_self().value );
    auto &itr = _ontransfer.get( owner.value, "blend::refund: `owner` does not have any refundable NFT assets" );

    atomic::transfer_nft( get_self(), owner, itr.asset_ids, "blend::refund" );

    _ontransfer.erase( itr );
}

}
