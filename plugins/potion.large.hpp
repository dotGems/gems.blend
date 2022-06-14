#pragma once
#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {

namespace potionLarge {
    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomicassets::assets_s>& in_assets )
    {
        uint64_t mana = 5; // initial

        // accumulate mana from blend inputs
        for ( const auto in_asset : in_assets ) {
            const auto schema = atomic::get_schema( in_asset.collection_name, in_asset.schema_name );
            if ( atomic::attribute_exists( schema.format, { "mana", "uint64" } )) {
                atomicdata::ATTRIBUTE_MAP mutable_data = atomic::get_asset_mutable( in_asset );
                mana += get<uint64_t>(mutable_data["mana"]);
            }
        }

        // mutable
        ATTRIBUTE_MAP mutable_data = {};
        int32_t now = current_time_point().sec_since_epoch() + 1; // now() + 1 sec (prevents flash claim)
        mutable_data["mana"] = mana; // Integer Number (uint64)
        mutable_data["timestamp"] = static_cast<uint64_t>(now); // Integer Number (uint64)

        return { {}, mutable_data };
    }
} // namespace potionLarge
} // namespace pomelo
