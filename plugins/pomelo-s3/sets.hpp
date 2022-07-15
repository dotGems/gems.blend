#pragma once
#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

#include "main.hpp"

namespace pomelo {
namespace s3 {
namespace sets {
    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomicassets::assets_s>& in_assets )
    {
        int rares = 0;
        int commons = 0;
        set<string> names;
        set<string> rarities;

        for ( const auto& asset : in_assets ) {
            check( asset.schema_name == "moons"_n, "only accepts `moons` schema");
            ATTRIBUTE_MAP immutable = atomic::get_asset_immutable( asset );
            const string name = std::get<string>(immutable["name"]);
            const string rarity = std::get<string>(immutable["rarity"]);

            // sum rarities
            if ( rarity == pomelo::s3::main::RARITY_RARE ) rares += 1;
            else if ( rarity == pomelo::s3::main::RARITY_COMMON ) commons += 1;
            else check( false, "invalid [rarity=" + rarity + "] provided");

            check( names.find( name ) == names.end(), "all assets for this blend must be unique");
            check( rarities.find( rarity ) == rarities.end(), "cannot mix commons and rares for this blend");
            names.insert( name );
            rarities.insert( rarity );
        }
        check( rares == 3 || commons == 6, "this blend requires either 6x commons or 3x rares");
        return { {}, {} };
    }

    void validate_attributes( const atomic::nft id )
    {
        const auto my_template = atomic::get_template( id.collection_name, id.template_id );
        const auto schema = atomic::get_schema( id.collection_name, my_template.schema_name );
        check( my_template.transferable, "blend::validate_attributes: [nft] must be `transferable`");
        check( my_template.burnable, "blend::validate_attributes: [nft] must be `burnable`");
    }
} // namespace sets
} // namespace s3
} // namespace pomelo
