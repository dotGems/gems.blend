#include "mint_attributes/pomelo/diamond.hpp"

namespace gems {
    void blend::check_plugin( const name plugin )
    {
        const set<name> plugins = { "pomelo"_n };
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
        switch ( plugin.value ) {
            case "diamond"_n.value:
                return pomelo::diamond::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        }
        check( false, "blend::mint_attributes: invalid [plugin]");
        return { };
    }
}