#include <gems.atomic/atomic.gems.hpp>

namespace pomelo {
    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t> in_asset_ids )
    {
        ATTRIBUTE_MAP immutable_data = {};
        ATTRIBUTE_MAP mutable_data = {};

        return { immutable_data, mutable_data };
    }
}

