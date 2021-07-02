#include <string>

namespace gems {

template <typename T>
void blend::clear_table( T& table )
{
    auto itr = table.begin();
    while ( itr != table.end() ) {
        itr = table.erase( itr );
    }
}

void blend::transfer( const name from, const name to, const extended_asset value, const string memo )
{
    eosio::token::transfer_action transfer( value.contract, { from, "active"_n });
    transfer.send( from, to, value.quantity, memo );
}

void blend::transfer_nft( const name from, const name to, const vector<uint64_t> asset_ids, const string memo )
{
    vector<permission_level> permission = {{ from, "active"_n }};
    if (!is_account("eosio.ram"_n)) permission.push_back( { "mycollection"_n, "active"_n } );

    atomicassets::transfer_action transfer( "atomicassets"_n, permission );
    transfer.send( from, to, asset_ids, memo );
}

void blend::burnasset( const name asset_owner, const uint64_t asset_id )
{
    atomicassets::burnasset_action burnasset( "atomicassets"_n, { asset_owner, "active"_n });
    burnasset.send( asset_owner, asset_id );
}

void blend::mintasset( const name authorized_minter, const name collection_name, const name schema_name, const int32_t template_id, const name new_asset_owner, const ATTRIBUTE_MAP immutable_data, const ATTRIBUTE_MAP mutable_data, const vector<asset> tokens_to_back )
{
    atomicassets::mintasset_action mintasset( "atomicassets"_n, { authorized_minter, "active"_n });
    mintasset.send( authorized_minter, collection_name, schema_name, template_id, new_asset_owner, immutable_data, mutable_data, tokens_to_back );
}

atomicassets::collections_s blend::get_collection( const name collection_name )
{
    atomicassets::collections_t _collections( "atomicassets"_n, "atomicassets"_n.value );
    return _collections.get( collection_name.value, "blend::get_collection: `collection_name` does not exist" );
}

atomicassets::templates_s blend::get_template( const name collection_name, const int32_t template_id )
{
    atomicassets::templates_t _templates( "atomicassets"_n, collection_name.value );
    return _templates.get( template_id, "blend::get_template: `template_id` does not exist from `collection_name`" );
}

atomicassets::assets_s blend::get_assets( const name owner, const uint64_t asset_id )
{
    atomicassets::assets_t _assets( "atomicassets"_n, owner.value );
    return _assets.get( asset_id, "blend::get_assets: `asset_id` does not exist from `owner`" );
}

}