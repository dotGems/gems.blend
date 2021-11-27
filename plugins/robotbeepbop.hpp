#include <gems/atomic.gems.hpp>

namespace robotbeepbop {
    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomicassets::assets_s>& in_assets )
    {
        ATTRIBUTE_MAP immutable_data = {};
        ATTRIBUTE_MAP mutable_data = {};

        uint64_t counter = 0;
        for ( const auto asset : in_assets ) {
            const ATOMIC_ATTRIBUTE name = atomic::get_template_attribute( asset, "name" );
            if ( std::get<string>(name) == "MSTR BTC ETF" ) counter += 100;
        }
        immutable_data["counter"] = ATOMIC_ATTRIBUTE{ counter };
        immutable_data["img"] = ATOMIC_ATTRIBUTE{"Qme7FuzMzusD2YYyZRjHAsXvfNUAZviDsjQhLTv1hRWoTh"};

        return { immutable_data, mutable_data };
    }
}
