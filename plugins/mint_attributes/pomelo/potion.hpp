#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {

namespace potion {
    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomicassets::assets_s>& in_assets )
    {
        ATTRIBUTE_MAP mutable_data = {};
        mutable_data["quantity"] = 10; // int16
        mutable_data["nonce"] = gems::random::generate(1)[0]; // int64
        mutable_data["timestamp"] = current_time_point().sec_since_epoch(); // uint32

        return { {}, mutable_data };
    }
} // namespace diamond
} // namespace pomelo
