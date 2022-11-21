#pragma once
#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {
namespace s4 {
namespace main {
    // rarity
    const string RARITY_COMMON = "Common";
    const string RARITY_RARE = "Rare";
    const string RARITY_ULTRA = "Ultra Rare";

    // rocket colors
    const string COLOR_0 = "Red";
    const string COLOR_1 = "Green";
    const string COLOR_2 = "Pink";
    const string COLOR_3 = "Navy";
    const string COLOR_4 = "Golden";
    const string COLOR_5 = "All";

    // moon phases - common
    const string COMMON_NAME_0 = "Medical Unit";
    const string COMMON_NAME_1 = "Command Unit";
    const string COMMON_NAME_2 = "Food Unit";
    const string COMMON_NAME_3 = "Service Unit";
    const string COMMON_NAME_4 = "Crew Unit";
    const string COMMON_NAME_5 = "Communication Tower";

    // moon phases - rare
    const string NAME_RARE_0 = "Glider";
    const string NAME_RARE_1 = "Trike";
    const string NAME_RARE_2 = "Rover";

    // moon phases - ultra rare
    const string NAME_ULTRA_0 = "Pomelo Armory";
    const string NAME_ULTRA_1 = "Pomelo Farm";
    const string NAME_ULTRA_2 = "Pomelo Disco";
    const string NAME_ULTRA_3 = "Pomelo Mining";
    const string NAME_ULTRA_4 = "Pomelo Power";

    const map<string, string> RARITY_COMMONS = {
        {COLOR_0, NAME_COMMON_0},
        {COLOR_1, NAME_COMMON_1},
        {COLOR_2, NAME_COMMON_2},
        {COLOR_3, NAME_COMMON_3},
        {COLOR_4, NAME_COMMON_4},
    };
    const vector<string> RARITY_RARES = {
        NAME_RARE_0,
        NAME_RARE_1,
        NAME_RARE_2
    };
    const map<string, string> RARITY_ULTRAS = {
        {COLOR_0, NAME_ULTRA_0},
        {COLOR_1, NAME_ULTRA_1},
        {COLOR_2, NAME_ULTRA_2},
        {COLOR_3, NAME_ULTRA_3},
        {COLOR_4, NAME_ULTRA_4},
    };

    string random_select( const vector<string>& values )
    {
        check( values.size(), "pomelo::random_select: [values] must contain at least one item");
        return values.at(gems::random::generate(1, 0, values.size() -1 )[0]);
    }

    string select_name( const string rarity, const string pure, const string color )
    {
        // ultra rare
        if ( rarity == RARITY_ULTRA ) {
            check( RARITY_ULTRAS.count(pure), "pomelo::select_name: [pure] invalid value");
            return RARITY_ULTRAS.at(pure);
        }

        // rare & common phases
        if ( rarity == RARITY_RARE ) return random_select( RARITY_RARES );

        // common
        if ( rarity == RARITY_COMMON ) {
            check( RARITY_COMMONS.count(color), "pomelo::select_name: [color] invalid value");
            return RARITY_COMMONS.at(color);
        }
        check( false,  "pomelo::select_name: [rarity] invalid");
        return "";
    }

    string select_rarity( const int power_total, const string pure )
    {
        if ( pure.size() && power_total >= 25 ) return RARITY_ULTRA;
        if ( power_total >= 13 ) return RARITY_RARE;
        return RARITY_COMMON;
    }

    string select_img( const string phase )
    {
        // ultra rare
        if ( phase == NAME_ULTRA_0 ) return "ipfs";
        if ( phase == NAME_ULTRA_1 ) return "ipfs";
        if ( phase == NAME_ULTRA_2 ) return "ipfs";
        if ( phase == NAME_ULTRA_3 ) return "ipfs";
        if ( phase == NAME_ULTRA_4 ) return "ipfs";

        // rare
        if ( phase == NAME_RARE_0 ) return "ipfs";
        if ( phase == NAME_RARE_1 ) return "ipfs";
        if ( phase == NAME_RARE_2 ) return "ipfs";

        // common
        if ( phase == COMMON_NAME_0 ) return "ipfs";
        if ( phase == COMMON_NAME_1 ) return "ipfs";
        if ( phase == COMMON_NAME_2 ) return "ipfs";
        if ( phase == COMMON_NAME_3 ) return "ipfs";
        if ( phase == COMMON_NAME_4 ) return "ipfs";
        if ( phase == COMMON_NAME_5 ) return "ipfs";

        check( false, "pomelo::select_img: invalid values");
        return "";
    }

    string select_video( const string phase )
    {
        // ultra rare
        if ( phase == NAME_ULTRA_0 ) return "ipfs";
        if ( phase == NAME_ULTRA_1 ) return "ipfs";
        if ( phase == NAME_ULTRA_2 ) return "ipfs";
        if ( phase == NAME_ULTRA_3 ) return "ipfs";
        if ( phase == NAME_ULTRA_4 ) return "ipfs";

        // rare
        if ( phase == NAME_RARE_0 ) return "ipfs";
        if ( phase == NAME_RARE_1 ) return "ipfs";
        if ( phase == NAME_RARE_2 ) return "ipfs";
        return "";
    }

    string select_backimg( const string phase )
    {
        // ultra rare
        if ( phase == NAME_ULTRA_0 ) return "ipfs";
        if ( phase == NAME_ULTRA_1 ) return "ipfs";
        if ( phase == NAME_ULTRA_2 ) return "ipfs";
        if ( phase == NAME_ULTRA_3 ) return "ipfs";
        if ( phase == NAME_ULTRA_4 ) return "ipfs";
        return "";
    }

    // detects if all items match the same
    string is_pure( const vector<string> items )
    {
        if ( !items.size() ) return ""; // empty
        string last;
        for ( const string item : items ) {
            if ( !last.size() ) last = item;
            if ( item != last ) return ""; // one item does not match
        }
        return last;
    }

    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomicassets::assets_s>& in_assets )
    {
        // containers
        vector<string> colors;
        uint8_t power_total = 0;

        print("GET ATTRIBUTES\n");
        print("==============\n");

        // aggregate attributes from assets
        for ( const auto& asset : in_assets ) {
            print("\nASSET\n");
            print("-----\n");
            print("asset.asset_id: ", asset.asset_id, "\n");
            print("asset.schema_name: ", asset.schema_name, "\n");
            if ( asset.schema_name != "rockets"_n ) continue; // skip if not rockets
            ATTRIBUTE_MAP immutable = atomic::get_asset_immutable( asset );
            const uint8_t power = std::get<uint8_t>(immutable["power"]);
            const string color = std::get<string>(immutable["color"]);

            // accumulate all colors (higher chance of selecting the more as input)
            colors.push_back( color );

            // sum power
            power_total += power;

            print("get.color: ", color, "\n");
            print("get.power: ", power, "\n");
        }

        // ultra rare requirement (pure = all same color)
        const string pure = is_pure( colors );

        // immutable
        ATTRIBUTE_MAP immutable_data = {};
        const string rarity = select_rarity( power_total, pure );
        const string color = random_select( colors );
        const string name = select_name( rarity, pure, color );
        const string img = select_img( name );
        const string video = select_video( name );
        const string backimg = select_backimg( name );
        immutable_data["rarity"] = rarity;
        immutable_data["name"] = name;
        immutable_data["img"] = img;
        if ( video.size() ) immutable_data["video"] = video;
        if ( backimg.size() ) immutable_data["backimg"] = backimg;

        print("\n🎉 RESULT 🎉\n");
        print("===========\n");
        print("power_total: ", power_total, "\n");
        print("rarity: ", rarity, "\n");
        print("name: ", name, "\n");
        print("pure: ", pure, "\n");
        print("img: ", img, "\n");
        print("video: ", video, "\n");
        print("backimg: ", backimg, "\n");

        return { immutable_data, {} };
    }

    void validate_input_attributes( const atomic::nft id )
    {
        const auto my_template = atomic::get_template( id.collection_name, id.template_id );
        const auto schema = atomic::get_schema( id.collection_name, my_template.schema_name );
        check( my_template.transferable, "blend::validate_attributes: [nft] must be `transferable`");
        check( my_template.burnable, "blend::validate_attributes: [nft] must be `burnable`");
        check( atomic::attribute_exists(schema.format, { "rarity", "string" }), "blend::validate_attributes: [nft] must have 'rarity' as String");
        check( atomic::attribute_exists(schema.format, { "video", "string" }), "blend::validate_attributes: [nft] must have 'video' as String");
        check( atomic::attribute_exists(schema.format, { "backimg", "image" }), "blend::validate_attributes: [nft] must have 'backimg' as Image");
    }
} // namespace main
} // namespace s4
} // namespace pomelo
