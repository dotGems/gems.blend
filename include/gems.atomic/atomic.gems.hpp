#define assert(condition) ((void)0)

#include <string>
#include <atomicassets/atomicassets.hpp>
#include <eosio.token/eosio.token.hpp>

namespace atomic {

const name ATOMIC_ASSETS_CONTRACT = "atomicassets"_n;

struct nft {
    name collection_name;
    int32_t template_id;
};

void transfer_nft( const name from, const name to, const vector<uint64_t> asset_ids, const string memo )
{
    vector<permission_level> permission = {{ from, "active"_n }};
    if (!is_account("eosio.ram"_n)) permission.push_back( { "mycollection"_n, "active"_n } );

    atomicassets::transfer_action transfer( ATOMIC_ASSETS_CONTRACT, permission );
    transfer.send( from, to, asset_ids, memo );
}

void announce_deposit( const name owner, const symbol symbol_to_announce )
{
    atomicassets::announcedepo_action announcedepo( ATOMIC_ASSETS_CONTRACT, { owner, "active"_n });
    announcedepo.send( owner, symbol_to_announce );
}

void burnasset( const name asset_owner, const uint64_t asset_id )
{
    atomicassets::burnasset_action burnasset( ATOMIC_ASSETS_CONTRACT, { asset_owner, "active"_n });
    burnasset.send( asset_owner, asset_id );
}

void mintasset( const name authorized_minter, const name collection_name, const name schema_name, const int32_t template_id, const name new_asset_owner, const ATTRIBUTE_MAP immutable_data, const ATTRIBUTE_MAP mutable_data, const vector<asset> tokens_to_back )
{
    for( const auto backed_token: tokens_to_back ){
        eosio::token::transfer_action transfer( "eosio.token"_n, { authorized_minter, "active"_n });
        announce_deposit( authorized_minter, backed_token.symbol );
        transfer.send( authorized_minter, ATOMIC_ASSETS_CONTRACT, backed_token, "deposit" );
    }
    atomicassets::mintasset_action mintasset( ATOMIC_ASSETS_CONTRACT, { authorized_minter, "active"_n });
    mintasset.send( authorized_minter, collection_name, schema_name, template_id, new_asset_owner, immutable_data, mutable_data, tokens_to_back );
}

atomicassets::collections_s get_collection( const name collection_name )
{
    atomicassets::collections_t _collections( ATOMIC_ASSETS_CONTRACT, ATOMIC_ASSETS_CONTRACT.value );
    return _collections.get( collection_name.value, "get_collection: `collection_name` does not exist" );
}

atomicassets::templates_s get_template( const name collection_name, const int32_t template_id )
{
    atomicassets::templates_t _templates( ATOMIC_ASSETS_CONTRACT, collection_name.value );
    return _templates.get( template_id, "get_template: `template_id` does not exist in `collection_name`" );
}

atomicassets::assets_s get_asset( const name owner, const uint64_t asset_id )
{
    atomicassets::assets_t _assets( ATOMIC_ASSETS_CONTRACT, owner.value );
    return _assets.get( asset_id, "get_asset: `asset_id` does not belong to `owner`" );
}

name get_collection_name( const name owner, const uint64_t asset_id )
{
    atomicassets::assets_t _assets( ATOMIC_ASSETS_CONTRACT, owner.value );
    auto itr = _assets.find( asset_id );
    if ( itr != _assets.end() ) return itr->collection_name;
    return {};
}

int32_t get_template_id( const name owner, const uint64_t asset_id )
{
    atomicassets::assets_t _assets( ATOMIC_ASSETS_CONTRACT, owner.value );
    auto itr = _assets.find( asset_id );
    if ( itr != _assets.end() ) return itr->template_id;
    return 0;
}

name get_schema_name( const name owner, const uint64_t asset_id )
{
    atomicassets::assets_t _assets( ATOMIC_ASSETS_CONTRACT, owner.value );
    auto itr = _assets.find( asset_id );
    if ( itr != _assets.end() ) return itr->schema_name;
    return {};
}

uint64_t get_next_asset_id( )
{
    atomicassets::config_t config( ATOMIC_ASSETS_CONTRACT, ATOMIC_ASSETS_CONTRACT.value );
    return config.get().asset_counter;
}

uint32_t get_issued_supply( const name collection_name, const int32_t template_id )
{
    return get_template( collection_name, template_id ).issued_supply;
}

uint32_t get_max_supply( const name collection_name, const int32_t template_id )
{
    return get_template( collection_name, template_id ).max_supply;
}

}   // end atomic