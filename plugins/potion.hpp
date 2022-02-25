#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {

namespace potion {
    void validate_attributes( const atomic::nft id )
    {
        const auto my_template = atomic::get_template( id.collection_name, id.template_id );
        const auto schema = atomic::get_schema( id.collection_name, my_template.schema_name );
        check( my_template.transferable, "blend::validate_attributes: [nft] must be `transferable`");
        check( my_template.burnable, "blend::validate_attributes: [nft] must be `burnable`");
        check( atomic::attribute_exists(schema.format, { "mana", "uint64" }), "blend::validate_attributes: [nft] must have 'mana' as Integer Number (uint64)");
        check( atomic::attribute_exists(schema.format, { "timestamp", "uint64" }), "blend::validate_attributes: [nft] must have 'timestamp' as Integer Number (uint64)");
    }

    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomicassets::assets_s>& in_assets )
    {
        // mutable
        ATTRIBUTE_MAP mutable_data = {};
        mutable_data["mana"] = static_cast<uint64_t>(7); // Integer Number (uint64)
        mutable_data["timestamp"] = static_cast<uint64_t>(0); // Integer Number (uint64)

        return { {}, mutable_data };
    }
} // namespace diamond
} // namespace pomelo
