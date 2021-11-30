#include "diamond.hpp"

namespace pomelo {
    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomicassets::assets_s>& in_assets )
    {
        switch ( template_id ) {
            case 123:
                return pomelo::diamond::mint_attributes( in_assets );
        }
        return { };
    }
}
