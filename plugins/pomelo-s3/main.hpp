#pragma once
#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {
namespace s3 {
namespace main {
    // rarity
    const string RARITY_COMMON = "Common";
    const string RARITY_RARE = "Rare";
    const string RARITY_ULTRA = "Ultra Rare";

    // colors
    const string COLOR_0 = "Red";
    const string COLOR_1 = "Green";
    const string COLOR_2 = "Pink";
    const string COLOR_3 = "Navy";
    const string COLOR_4 = "Golden";

    // colors - common & rare
    const string COLOR_5 = "White";
    const string COLOR_6 = "Black";

    // moon phases - common
    const string PHASE_COMMON_0 = "Waning Crescent Moon";
    const string PHASE_COMMON_1 = "Waxing Crescent Moon";
    const string PHASE_COMMON_2 = "First Quarter Moon";
    const string PHASE_COMMON_3 = "Waxing Gibbous Moon";
    const string PHASE_COMMON_4 = "Waning Gibbous Moon";
    const string PHASE_COMMON_5 = "Third Quarter Moon";

    // moon phases - rare
    const string PHASE_RARE_0 = "New Moon";
    const string PHASE_RARE_1 = "Full Moon";
    const string PHASE_RARE_2 = "Eclipse";

    // moon phases - ultra rare
    const string PHASE_ULTRA_0 = "Blood Moon";
    const string PHASE_ULTRA_1 = "Super Harvest Moon";
    const string PHASE_ULTRA_2 = "Strawberry Supermoon";
    const string PHASE_ULTRA_3 = "Wolf Moon";
    const string PHASE_ULTRA_4 = "Ring of Fire Eclipse";

    const vector<string> PHASE_RARITY_COMMON = {PHASE_COMMON_0, PHASE_COMMON_1, PHASE_COMMON_2, PHASE_COMMON_3, PHASE_COMMON_4, PHASE_COMMON_5};
    const vector<string> PHASE_RARITY_RARE = {PHASE_RARE_0, PHASE_RARE_1, PHASE_RARE_2};
    const map<string, string> PHASE_RARITY_ULTRA = {
        {COLOR_0, PHASE_ULTRA_0},
        {COLOR_1, PHASE_ULTRA_1},
        {COLOR_2, PHASE_ULTRA_2},
        {COLOR_3, PHASE_ULTRA_3},
        {COLOR_4, PHASE_ULTRA_4},
    };

    string random_select( const vector<string>& values )
    {
        check( values.size(), "pomelo::random_select: [values] must contain at least one item");
        return values.at(gems::random::generate(1, 0, values.size() -1 )[0]);
    }

    string select_color( const string phase, const eosio::string pure )
    {
        if ( pure.size() ) return pure;
        if ( phase == PHASE_RARE_2 ) return COLOR_6;
        return COLOR_5;
    }

    string select_phase( const string rarity, const string pure )
    {
        // ultra rare
        if ( rarity == RARITY_ULTRA ) {
            check( PHASE_RARITY_ULTRA.count(pure), "pomelo::select_phase: [pure] invalid value");
            return PHASE_RARITY_ULTRA.at(pure);
        }

        // rare & common phases
        if ( rarity == RARITY_RARE ) return random_select( PHASE_RARITY_RARE );
        if ( rarity == RARITY_COMMON ) return random_select( PHASE_RARITY_COMMON );
        check( false,  "pomelo::select_phase: [rarity] invalid");
        return "";
    }

    string select_rarity( const int fuel_total, const string pure )
    {
        if ( pure.size() && fuel_total >= 25 ) return RARITY_ULTRA;
        if ( fuel_total >= 21 ) return RARITY_RARE;
        return RARITY_COMMON;
    }

    string select_img( const string phase )
    {
        // ultra rare
        if ( phase == PHASE_ULTRA_0 ) return "QmcPZs9XtSRVK9Sy88q5DNzphfvHvjAQSBpAixzR5mDjqH";
        if ( phase == PHASE_ULTRA_1 ) return "QmbF4A5KNTDsXDZUHXDLMk4sMUyU2RHrRLj8TZzs1QPo58";
        if ( phase == PHASE_ULTRA_2 ) return "QmRDjcUjPyndNHPUVZ1w8bypHFfm3Ryn26LLtdNpPKNbt3";
        if ( phase == PHASE_ULTRA_3 ) return "QmTk61BppLXm8y8RMzUC42F7R9VYDs3VVWeTzruNjEcgE8";
        if ( phase == PHASE_ULTRA_4 ) return "QmPKRsZLoRFBkwhyoGDfej8RaEGcpaunBZGdWZUwtaV2sY";

        // rare
        if ( phase == PHASE_RARE_0 ) return "QmdwuzJosFVn6mkTUaSePCDxpp3gXS3FMQNUVJLPbkY4Ye";
        if ( phase == PHASE_RARE_1 ) return "QmWM4qiVfRqxc8wVDduHxDcRjatnrZ8afT1ax13bqbKGiz";
        if ( phase == PHASE_RARE_2 ) return "QmctL2iZKAzV6xWNbb43yCCLATHFScuHZQqPHthTM8TMom";

        // common
        if ( phase == PHASE_COMMON_0 ) return "QmVwFKSR6HAC3DqySpcLQ7U9XFsajTd7k44f446dXfGnf3";
        if ( phase == PHASE_COMMON_1 ) return "QmTYDuT8fkRfHo6sokcLqCHdwb1jFmSy2f1RrhwaBWucwG";
        if ( phase == PHASE_COMMON_2 ) return "QmNp4iKMesMps1SH4xj6tUT6pLHHWVH1JWVNLt23Pw4zDd";
        if ( phase == PHASE_COMMON_3 ) return "QmVVD1K9szC9o2MiB1pvwsK1wNs8ju5UCio5fuKcrNRfrr";
        if ( phase == PHASE_COMMON_4 ) return "QmRagZeX7y8qgy1bSirrHgVDukmppnyxXfgwfQyRmyhy1V";
        if ( phase == PHASE_COMMON_5 ) return "Qmf8dkkVN437Wd8REJ2Ac4oLsa7v1iVGKYWSNbMP14ixmn";

        check( false, "pomelo::select_img: invalid values");
        return "";
    }

    string select_video( const string phase )
    {
        // ultra rare
        if ( phase == PHASE_ULTRA_0 ) return "QmVg78u4WHpgf9uEK8nAD9Dh3frBtnupPFatUpN61rLykT";
        if ( phase == PHASE_ULTRA_1 ) return "QmdsjMNWzNjfr58s5VbDoB2jergKQHzPfr8cSYozHydPb1";
        if ( phase == PHASE_ULTRA_2 ) return "QmZrQYsyyyLeVLKpqDuSdm9UyUw2H3vMT83jHzF8f7WuAL";
        if ( phase == PHASE_ULTRA_3 ) return "QmUiesVupCAsF73dqSUN9KZS5JezXwo48vCyci7YhSdSjo";
        if ( phase == PHASE_ULTRA_4 ) return "QmPbMWPV1xUDyW3bfs3NgXwKPynBP7hSSGP4iVYb4F8A3F";

        // rare
        if ( phase == PHASE_RARE_0 ) return "QmNXZ3M3Q3YqhLLTUfTBjGHtNFFjaaewtZtnrCmmqNtgVt";
        if ( phase == PHASE_RARE_1 ) return "QmThV9NxPUfQbVBxxnLoATazLdaBPegijNkt4g1KH1dq4D";
        if ( phase == PHASE_RARE_2 ) return "QmNdiA3yahXaPr7HrHaFFShh3pYZkg9zMzanrjTZo3ZVTT";
        return "";
    }

    string select_backimg( const string phase )
    {
        // ultra rare
        if ( phase == PHASE_ULTRA_0 ) return "QmRscH3bV2e2i3navXy2xEhLoGxfnc2hRSmkR2kty6pnZL";
        if ( phase == PHASE_ULTRA_1 ) return "QmY3eQugTsfZRiEZRB1bNLpnd8w2aJZ9WgcyYJLGdYBHen";
        if ( phase == PHASE_ULTRA_2 ) return "QmQFECu2xjsSkrZ1BPC1g4waYeJgF7yAqn4tHzwAWa9kPE";
        if ( phase == PHASE_ULTRA_3 ) return "QmZJ48bDScZ32PNN1kKdVW9FF3gM7zXX1ndPNkiMqpA1C9";
        if ( phase == PHASE_ULTRA_4 ) return "QmTZ52bFU6tFpEqDVSPDX1imjSVnQNL1hvGS5shvdeSRQ9";
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
        const string video = select_video( phase );
        const string backimg = select_backimg( phase );
        immutable_data["rarity"] = rarity;
        immutable_data["color"] = color;
        immutable_data["name"] = phase;
        immutable_data["img"] = img;
        if ( video.size() ) immutable_data["video"] = video;
        if ( backimg.size() ) immutable_data["backimg"] = backimg;

        print("\n🎉 RESULT 🎉\n");
        print("===========\n");
        print("fuel_total: ", fuel_total, "\n");
        print("rarity: ", rarity, "\n");
        print("phase: ", phase, "\n");
        print("color: ", color, "\n");
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
        check( atomic::attribute_exists(schema.format, { "color", "string" }), "blend::validate_attributes: [nft] must have 'color' as String");
        check( atomic::attribute_exists(schema.format, { "video", "string" }), "blend::validate_attributes: [nft] must have 'video' as String");
        check( atomic::attribute_exists(schema.format, { "backimg", "string" }), "blend::validate_attributes: [nft] must have 'backimg' as String");
    }
} // namespace main
} // namespace s3
} // namespace pomelo
