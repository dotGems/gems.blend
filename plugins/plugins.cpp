#include "example.hpp"
#include "robotbeepbop.hpp"
#include "pomelo/pomelo.hpp"

namespace gems {
    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> blend::mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids )
    {
        // prepare assets
        vector<atomicassets::assets_s> in_assets;
        for ( const uint64_t asset_id : in_asset_ids ) {
            in_assets.push_back( atomic::get_asset( get_self(), asset_id ) );
        }

        // add plugins
        switch ( collection_name.value ) {
            case "example"_n.value:
                return example::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
            case "robotbeepbop"_n.value:
                return robotbeepbop::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
            case "pomelo"_n.value:
                return pomelo::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        }
        return { };
    }
}