#include "diamond.hpp"
#include "potion.hpp"
#include "potion.large.hpp"

namespace gems {
    void blend::check_plugin( const name plugin )
    {
        const set<name> plugins = { "diamond"_n, "potion"_n, "potion.large"_n };
        check( plugins.find( plugin ) != plugins.end(), "blend::check_plugin: invalid [plugin]");
    }

    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> blend::mint_attributes( const name plugin, const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids )
    {
        // no plugin provided
        if ( !plugin ) return {};

        // prepare assets
        vector<atomicassets::assets_s> in_assets;
        for ( const uint64_t asset_id : in_asset_ids ) {
            in_assets.push_back( atomic::get_asset( get_self(), asset_id ) );
        }

        // supported plugins
        if ( plugin == "diamond"_n ) return pomelo::diamond::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        if ( plugin == "potion.large"_n ) return pomelo::potionLarge::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        if ( plugin == "potion"_n ) return pomelo::potion::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        check( false, "blend::mint_attributes: invalid [plugin]");
        return { };
    }
}