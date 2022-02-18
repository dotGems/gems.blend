#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {

namespace potion {
    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomicassets::assets_s>& in_assets )
    {
        // mutable
        ATTRIBUTE_MAP mutable_data = {};
        mutable_data["mana"] = 7; // int16
        mutable_data["timestamp"] = 0; // uint32

        return { {}, mutable_data };
    }
} // namespace diamond
} // namespace pomelo
