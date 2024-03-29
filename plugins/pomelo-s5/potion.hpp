#pragma once

namespace pomelo {
namespace s5 {
namespace potion {
    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomicassets::assets_s>& in_assets )
    {
        const uint64_t mana = 6; // initial

        // mutable
        ATTRIBUTE_MAP mutable_data = {};
        int32_t now = current_time_point().sec_since_epoch() + 1; // now() + 1 sec (prevents flash claim)
        mutable_data["mana"] = static_cast<uint64_t>(mana); // Integer Number (uint64)
        mutable_data["timestamp"] = static_cast<uint64_t>(now); // Integer Number (uint64)

        return { {}, mutable_data };
    }
}
}
}
