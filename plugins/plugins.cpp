#include "example.hpp"

namespace gems {
    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> blend::mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomic::nft>& received_templates )
    {
        switch ( collection_name.value ) {
            case "example"_n.value:
                return example::mint_attributes(owner, collection_name, template_id, in_asset_ids, received_templates );
        }
        return { };
    }
}