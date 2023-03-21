#pragma once
#include <gems/atomic.gems.hpp>

#include "main.hpp"

namespace pomelo {
namespace s5 {
namespace commons {
    const name plugin = "commons.s5"_n;
    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomicassets::assets_s>& in_assets )
    {
        int total = 0;
        int required_total = 5;
        string required_rarity = pomelo::s5::main::RARITY_COMMON;
        set<string> names;
        string last_rarity;

        for ( const auto& asset : in_assets ) {
            ATTRIBUTE_MAP immutable = atomic::get_asset_immutable( asset );
            check( immutable.find("name") != immutable.end(), "missing `name` attribute from " + to_string(asset.asset_id) );
            check( immutable.find("rarity") != immutable.end(), "missing `rarity` attribute from " + to_string(asset.asset_id) );

            const string name = std::get<string>(immutable["name"]);
            const string rarity = std::get<string>(immutable["rarity"]);

            // sum rarities
            if ( rarity == required_rarity ) total += 1;
            else check( false, "invalid [rarity=" + rarity + "] provided");

            // all uniques
            check( names.find( name ) == names.end(), "all assets for this blend must be unique");
            names.insert( name );

            // all same rarity types
            if ( last_rarity.size() ) check( last_rarity == rarity, "cannot mix commons and rares for this blend");
            last_rarity = rarity;
        }
        check( total == required_total, "this blend requires " + to_string(required_total) + "x unique " + required_rarity + "s");

        // immutable
        ATTRIBUTE_MAP immutable_data = {};
        immutable_data["color"] = pomelo::s5::main::COLOR_6;

        return { immutable_data, {} };
    }

    void validate_input_attributes( const atomic::nft id )
    {
        const auto my_template = atomic::get_template( id.collection_name, id.template_id );
        const auto schema = atomic::get_schema( id.collection_name, my_template.schema_name );
        check( my_template.transferable, "blend::validate_attributes: [nft] must be `transferable`");
        check( my_template.burnable, "blend::validate_attributes: [nft] must be `burnable`");
        check( my_template.burnable, "blend::validate_attributes: [nft] must be `burnable`");
        check( atomic::attribute_exists(schema.format, { "color", "string" }), "blend::validate_attributes: [nft] must have 'color' as String");
    }
}
}
}
