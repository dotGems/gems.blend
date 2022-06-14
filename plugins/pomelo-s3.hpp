#pragma once
#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {
namespace s3 {
    // rarity
    const string RARITY_COMMON = "Common";
    const string RARITY_RARE = "Rare";
    const string RARITY_LEGENDARY = "Legendary";

    // colors
    const string COLOR_0 = "Red";
    const string COLOR_1 = "Orange";
    const string COLOR_2 = "Pink";
    const string COLOR_3 = "Blue";
    const string COLOR_4 = "Yellow";

    // moon phases - common
    const string PHASE_COMMON_0 = "Waning Crescent Moon";
    const string PHASE_COMMON_1 = "Waxing Crescent Moon";
    const string PHASE_COMMON_2 = "First Quarter Moon";
    const string PHASE_COMMON_3 = "Waxing Gibbous Moon";
    const string PHASE_COMMON_4 = "Waning Gibbous Moon";
    const string PHASE_COMMON_5 = "Last Quarter Moon";

    // moon phases - rare
    const string PHASE_RARE_0 = "New Moon";
    const string PHASE_RARE_1 = "Full Moon";
    const string PHASE_RARE_2 = "Eclipse";

    // moon phases - ultra rare
    const string PHASE_LEGENDARY_0 = "Blood Moon";
    const string PHASE_LEGENDARY_1 = "Ring of Fire Eclipse";
    const string PHASE_LEGENDARY_2 = "Strawberry Supermoon";
    const string PHASE_LEGENDARY_3 = "3 Wolf Moon";
    const string PHASE_LEGENDARY_4 = "Super Harvest Moon";

    const vector<string> PHASE_RARITY_COMMON = {PHASE_COMMON_0, PHASE_COMMON_1, PHASE_COMMON_2, PHASE_COMMON_3, PHASE_COMMON_4, PHASE_COMMON_5};
    const vector<string> PHASE_RARITY_RARE = {PHASE_RARE_0, PHASE_RARE_1, PHASE_RARE_2};
    const map<string, string> PHASE_RARITY_LEGENDARY = {
        {COLOR_0, PHASE_LEGENDARY_0},
        {COLOR_1, PHASE_LEGENDARY_1},
        {COLOR_2, PHASE_LEGENDARY_2},
        {COLOR_3, PHASE_LEGENDARY_3},
        {COLOR_4, PHASE_LEGENDARY_4},
    };

    string random_select( const vector<string>& values )
    {
        check( values.size(), "pomelo::random_select: [values] must contain at least one item");
        return values.at(gems::random::generate(1, 0, values.size() -1 )[0]);
    }

    string select_color( const string phase, const eosio::string pure )
    {
        if ( pure.size() ) return pure;
        if ( phase == PHASE_RARE_2 ) return "black";
        return "white";
    }

    string select_phase( const string rarity, const string pure )
    {
        // ultra rare
        if ( rarity == RARITY_LEGENDARY ) {
            check( PHASE_RARITY_LEGENDARY.count(pure), "pomelo::select_phase: [pure] invalid value");
            return PHASE_RARITY_LEGENDARY.at(pure);
        }

        // rare & common phases
        if ( rarity == RARITY_RARE ) return random_select( PHASE_RARITY_RARE );
        if ( rarity == RARITY_COMMON ) return random_select( PHASE_RARITY_COMMON );
        check( false,  "pomelo::select_phase: [rarity] invalid");
        return "";
    }

    string select_rarity( const int fuel_total, const string pure )
    {
        if ( fuel_total >= 21 ) {
            if ( pure.size() ) return RARITY_LEGENDARY;
            else return RARITY_RARE;
        }
        return RARITY_COMMON;
    }

    string select_img( const string phase )
    {
        // legendary
        if ( phase == PHASE_LEGENDARY_0 ) return "<IPFS>";
        if ( phase == PHASE_LEGENDARY_1 ) return "<IPFS>";
        if ( phase == PHASE_LEGENDARY_2 ) return "<IPFS>";
        if ( phase == PHASE_LEGENDARY_3 ) return "<IPFS>";
        if ( phase == PHASE_LEGENDARY_4 ) return "<IPFS>";

        // rare
        if ( phase == PHASE_RARE_0 ) return "<IPFS>";
        if ( phase == PHASE_RARE_1 ) return "<IPFS>";
        if ( phase == PHASE_RARE_2 ) return "<IPFS>";

        // common
        if ( phase == PHASE_COMMON_0 ) return "<IPFS>";
        if ( phase == PHASE_COMMON_1 ) return "<IPFS>";
        if ( phase == PHASE_COMMON_2 ) return "<IPFS>";
        if ( phase == PHASE_COMMON_3 ) return "<IPFS>";
        if ( phase == PHASE_COMMON_4 ) return "<IPFS>";
        if ( phase == PHASE_COMMON_5 ) return "<IPFS>";

        check( false, "pomelo::select_img: invalid values");
        return "";
    }

    // detects if all items match the same
    string is_pure( const vector<string> items )
    {
        if ( !items.size() ) return ""; // empty
        string last;
        for ( const string item : items ) {
            if ( last.size() ) last = item;
            if ( item != last ) return ""; // one item does not match
        }
        return last;
    }

    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomicassets::assets_s>& in_assets )
    {
        // containers
        vector<string> colors;
        uint8_t fuel_total = 0;

        print("GET ATTRIBUTES\n");
        print("==============\n");
        // add shapes & sum clarity quality
        for ( const auto& asset : in_assets ) {
            print("\nASSET\n");
            print("-----\n");
            print("asset.asset_id: ", asset.asset_id, "\n");
            print("asset.schema_name: ", asset.schema_name, "\n");
            if ( asset.schema_name != "rockets"_n ) continue; // skip if not rockets
            ATTRIBUTE_MAP immutable = atomic::get_asset_immutable( asset );
            const uint8_t fuel = std::get<uint8_t>(immutable["fuel"]);
            const string color = std::get<string>(immutable["color"]);

            // accumulate all colors (higher chance of selecting the more as input)
            colors.push_back( color );

            // sum fuel
            fuel_total += fuel;

            print("get.color: ", color, "\n");
            print("get.fuel: ", fuel, "\n");
        }

        // ultra rare requirement (pure = all same color)
        const string pure = is_pure( colors );

        // immutable
        ATTRIBUTE_MAP immutable_data = {};
        const string rarity = select_rarity( fuel_total, pure );
        const string phase = select_phase( rarity, pure );
        const string color = select_color( phase, pure );
        const string img = select_img( phase );
        immutable_data["rarity"] = rarity;
        immutable_data["color"] = color;
        immutable_data["phase"] = phase;
        immutable_data["img"] = img;

        print("\n🎉 RESULT 🎉\n");
        print("===========\n");
        print("fuel_total: ", fuel_total, "\n");
        print("rarity: ", rarity, "\n");
        print("phase: ", phase, "\n");
        print("color: ", color, "\n");
        print("img: ", img, "\n");

        return { immutable_data, {} };
    }

    void validate_attributes( const atomic::nft id )
    {
        const auto my_template = atomic::get_template( id.collection_name, id.template_id );
        const auto schema = atomic::get_schema( id.collection_name, my_template.schema_name );
        check( my_template.transferable, "blend::validate_attributes: [nft] must be `transferable`");
        check( my_template.burnable, "blend::validate_attributes: [nft] must be `burnable`");
        check( atomic::attribute_exists(schema.format, { "rarity", "string" }), "blend::validate_attributes: [nft] must have 'rarity' as String");
        check( atomic::attribute_exists(schema.format, { "phase", "string" }), "blend::validate_attributes: [nft] must have 'phase' as String");
        check( atomic::attribute_exists(schema.format, { "color", "string" }), "blend::validate_attributes: [nft] must have 'color' as String");
    }
} // namespace s3
} // namespace pomelo
