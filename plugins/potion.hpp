#pragma once
#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace potion {
    void validate_input_attributes( const atomic::nft id )
    {
        const auto my_template = atomic::get_template( id.collection_name, id.template_id );
        const auto schema = atomic::get_schema( id.collection_name, my_template.schema_name );
        check( my_template.transferable, "blend::validate_attributes: [nft] must be `transferable`");
        check( my_template.burnable, "blend::validate_attributes: [nft] must be `burnable`");
        check( atomic::attribute_exists(schema.format, { "mana", "uint64" }), "blend::validate_attributes: [nft] must have 'mana' as Integer Number (uint64)");
        check( atomic::attribute_exists(schema.format, { "timestamp", "uint64" }), "blend::validate_attributes: [nft] must have 'timestamp' as Integer Number (uint64)");
    }
} // namespace potion
