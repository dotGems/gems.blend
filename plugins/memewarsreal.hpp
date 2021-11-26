#include <gems.atomic/atomic.gems.hpp>

namespace memewarsreal {
    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomicassets::assets_s>& in_assets )
    {
        ATTRIBUTE_MAP immutable_data = {};
        ATTRIBUTE_MAP mutable_data = {};

        int count = 0;
        for ( const auto asset : in_assets ) {
            const ATOMIC_ATTRIBUTE name = atomic::get_template_attribute( asset.collection_name, asset.schema_name, asset.template_id, "name" );
            if ( std::get<string>(name) == "MSTR BTC ETF" ) count += 1;
        }
        immutable_data["name"] = ATOMIC_ATTRIBUTE{count};

        return { immutable_data, mutable_data };
    }
}

