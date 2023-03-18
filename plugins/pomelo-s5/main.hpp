#pragma once
#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {
namespace s5 {
namespace main {
    // rarity
    const string RARITY_COMMON = "Common";
    const string RARITY_RARE = "Rare";
    const string RARITY_ULTRA = "Ultra Rare";

    // colors
    const string COLOR_1 = "Red";
    const string COLOR_2 = "Green";
    const string COLOR_3 = "Pink";
    const string COLOR_4 = "Navy";
    const string COLOR_5 = "Golden";
    // const string COLOR_6 = "Multi";

    // names - common
    const string NAME_COMMON_1 = "Dual Plasma Guns";
    const string NAME_COMMON_2 = "Agriculture Lab";
    const string NAME_COMMON_3 = "DJ Booth";
    const string NAME_COMMON_4 = "Science Lab";
    const string NAME_COMMON_5 = "Construction Unit";
    // const string NAME_COMMON_6 = "Cryo Chamber";

    // names - rare
    const string NAME_RARE_1 = "Thrusters";
    const string NAME_RARE_2 = "Biodome";
    const string NAME_RARE_3 = "BFG";

    // names - ultra rare
    const string NAME_ULTRA_1 = "Militaris - Citrus Maxima";
    const string NAME_ULTRA_2 = "Natura - Citrus Maxima";
    const string NAME_ULTRA_3 = "Otium - Citrus Maxima";
    const string NAME_ULTRA_4 = "Scientia - Citrus Maxima";
    const string NAME_ULTRA_5 = "Operarius - Citrus Maxima";

    const vector<string> NAME_COMMONS = {
        NAME_COMMON_1,
        NAME_COMMON_2,
        NAME_COMMON_3,
        NAME_COMMON_4,
        NAME_COMMON_5,
        // NAME_COMMON_6,
    };

    const vector<string> NAME_RARES = {
        NAME_RARE_1,
        NAME_RARE_2,
        NAME_RARE_3,
    };

    const map<string, string> COLORS = {
        // Common
        {NAME_COMMON_1, COLOR_1},
        {NAME_COMMON_2, COLOR_2},
        {NAME_COMMON_3, COLOR_3},
        {NAME_COMMON_4, COLOR_4},
        {NAME_COMMON_5, COLOR_5},
        // {NAME_COMMON_6, COLOR_6},

        // Rare
        {NAME_RARE_1, COLOR_4},
        {NAME_RARE_2, COLOR_2},
        {NAME_RARE_3, COLOR_1},

        // Ultra
        {NAME_ULTRA_1, COLOR_1},
        {NAME_ULTRA_2, COLOR_2},
        {NAME_ULTRA_3, COLOR_3},
        {NAME_ULTRA_4, COLOR_4},
        {NAME_ULTRA_5, COLOR_5},
    };

    const map<string, string> PURE_COLORS_ULTRAS = {
        {COLOR_1, NAME_ULTRA_1},
        {COLOR_2, NAME_ULTRA_2},
        {COLOR_3, NAME_ULTRA_3},
        {COLOR_4, NAME_ULTRA_4},
        {COLOR_5, NAME_ULTRA_5},
    };

    // IPFS images
    const map<string, string> IPFS_IMG = {
        {NAME_COMMON_1, "QmUGb1Yu1iGiRD2UzzDB2n4VL55vcQGJZfu8GyzfgCiqqP"},
        {NAME_COMMON_2, "QmS2wxvhsTwebwMC9kgZn3DvykXyBagu8q5LHqpnKeB2m2"},
        {NAME_COMMON_3, "QmWqEK68SFUkasn84UmcueSch7SKovrJE2H4pCGXfDAhA8"},
        {NAME_COMMON_4, "Qma9tEPUjJFv5dVaCpgdbCjdFv8aSKrBp4oGJqiyX8kK4v"},
        {NAME_COMMON_5, "QmQrsFUPiVDuk4ARkfzADgr8qSexvF1rwWMXdASJyU2FsD"},
        // {NAME_COMMON_6, "QmdU1v66eTM1EnGtZo7cKAdcL1fCXX4m8pz2FVStT6Agzn"},
        {NAME_RARE_1, "QmUBy2MGSEoLv77spYeTQDqckEhzywA9fKF5FASBRYEdvp"},
        {NAME_RARE_2, "QmVFiGhfxU5fNmc79rArYV6JmtxPN7xJYN86EKe485MQ3A"},
        {NAME_RARE_3, "QmV9Zt3vQAsQACzoDZFmF77cvqBTnEJACAaQK8R12Ep88s"},
        {NAME_ULTRA_1, "QmW7iVi7rbzKqdVwRYhKs2rbmgDwjuLqaqTAzPpdaZ5gWU"},
        {NAME_ULTRA_2, "QmTBR4VrFCS3snG1iBVZ6WPUu2fAVKUWXtkQs2wVveaAYW"},
        {NAME_ULTRA_3, "QmcjG1MavqBtHURk6wejwYZZbRzobWdWktAvNxNheFRhfa"},
        {NAME_ULTRA_4, "QmcxtfWgyivsXkSAZM8EpKw5zaEKw8r26d4tK4SZr4h3pF"},
        {NAME_ULTRA_5, "QmTUARksawV4HLgwWwrSMe76RqKYMk1KEapJ4szkmYpR4V"},
    };

    const map<string, string> IPFS_BANNER = {
        {NAME_RARE_1,  "QmNzqSVXxLGKopL6c8Vu2f1gVyF9NTqtSJ9UmAWr4poYKr"},
        {NAME_RARE_2,  "QmYPvwaJQEJUiy2H33SKb3mrXhDg2rUmuf3RMApstnoe1c"},
        {NAME_RARE_3,  "QmadEiyLzSKajabWMoDbdQrVnitmN2C1b8S3TLJ8VFp7yG"},
        {NAME_ULTRA_1, "QmdebiMJuqSV34q1zUj6oJU1pqzLEQJfYoZNVZPF2UvcBd"},
        {NAME_ULTRA_2, "QmXB8kZFxgTBiDD9bj5y6mMPSi7beR7TUJ1gFWjzzBLsng"},
        {NAME_ULTRA_3, "QmU7NKsFoUwmkJJcHg6B8vyceqPAvUUpmnkdZzLYnQbby1"},
        {NAME_ULTRA_4, "QmSGXnK2yPXXntWFdo1QeXAfxGgUBxCi5YRo7ibT9kc4Y8"},
        {NAME_ULTRA_5, "QmTTh2UX3A3MYmXJ6L675UuHhSuPR6Ke95mmCLqRMbKe1P"},
    };

    const map<string, string> IPFS_3D = {
        {NAME_COMMON_1, "QmWenoTJGFPHFKg9Dk4E65hAKcwt5855bdHqXtkmARpyLy"},
        {NAME_COMMON_2, "QmbAVpAwVFv4pbQ4oxZDDE8uo1FENN1LFNqfZwa18Gy62e"},
        {NAME_COMMON_3, "QmWqEK68SFUkasn84UmcueSch7SKovrJE2H4pCGXfDAhA8"},
        {NAME_COMMON_4, "QmXSUBd8QTk6tPssLyoDVR2W3eNFzrrDTHx54fcG1JZtr1"},
        {NAME_COMMON_5, "QmPw8w67Tu9N6Rx7FuLMwzQjay7KkEEktFd2jSsc4gpry2"},
        // {NAME_COMMON_6, "QmQXNh1aJwrVcQVfZyjDTsjpn3pkbYBq2k7xzqxtkD1Kq7"},
        {NAME_RARE_1,   "QmTdRymJZsMvj46ZTpv5sGXmF36EZKz8g4hiuw2LEfsNDo"},
        {NAME_RARE_2,   "QmZoS2E4UWnk2BVB9e8kC95dT6KTigavRSzEb3NSSsTJJm"},
        {NAME_RARE_3,   "QmP6xm773gBxo2bfswVQ1Pb4d8xXZKLFpRK9VJ6HwSzc6u"},
        {NAME_ULTRA_1,  "QmWW17CNfQdLLNTUVyuht65A4aEgjC2gmnHyBsdE6GP9Zt"}, // TO FIX
        {NAME_ULTRA_2,  "QmNbmQjrxthKDRgAoHUNNQEFtHxpYj5N6T8UHpdiqMm9Vj"},
        {NAME_ULTRA_3,  "QmVoo31ceD3btw5N7Re1sFAHpYt54YQCodx7spfqdXXCdN"},
        {NAME_ULTRA_4,  "QmWTJRVYn7B7R3D1dvDihs8whMpcfgpaDtw8CKxUArNvB3"},
        {NAME_ULTRA_5,  "Qmc1K9h6Jmoww2t75qvhwdqCAdd25MrC4mSTX83KGp9NiL"},
    };

    string random_select( const vector<string>& values )
    {
        check( values.size(), "pomelo::random_select: [values] must contain at least one item");
        return values.at(gems::random::generate(1, 0, values.size() -1 )[0]);
    }

    string select_name( const string rarity, const string pure )
    {
        // ultra rare
        if ( rarity == RARITY_ULTRA ) {
            check( PURE_COLORS_ULTRAS.count(pure), "pomelo::select_name: [pure] invalid value");
            return PURE_COLORS_ULTRAS.at(pure);
        }

        // rare & common phases
        if ( rarity == RARITY_RARE ) return random_select( NAME_RARES );
        if ( rarity == RARITY_COMMON ) return random_select( NAME_COMMONS );

        check( false,  "pomelo::select_name: [rarity] invalid");
        return "";
    }

    // required
    string select_color( const string name )
    {
        check( COLORS.count(name), "pomelo::select_color: [name] invalid value");
        return COLORS.at(name);
    }

    string select_rarity( const int power_total, const string pure )
    {
        if ( pure.size() && power_total >= 25 ) return RARITY_ULTRA;
        if ( power_total >= 13 ) return RARITY_RARE;
        return RARITY_COMMON;
    }

    // required
    string select_img( const string name )
    {
        check( IPFS_IMG.count(name), "pomelo::select_img: [name] invalid value");
        return IPFS_IMG.at(name);
    }

    string select_banner( const string name )
    {
        if ( !IPFS_BANNER.count(name) ) return "";
        return IPFS_BANNER.at(name);
    }

    string select_3d( const string name )
    {
        if ( !IPFS_3D.count(name) ) return "";
        return IPFS_3D.at(name);
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
        uint8_t stars_total = 0;

        print("GET ATTRIBUTES\n");
        print("==============\n");

        // aggregate attributes from assets
        for ( const auto& asset : in_assets ) {
            print("\nASSET\n");
            print("-----\n");
            print("asset.asset_id: ", asset.asset_id, "\n");
            print("asset.schema_name: ", asset.schema_name, "\n");

            // Get immutable attributes
            if ( asset.schema_name == "potions"_n ) continue; // skip if potions
            ATTRIBUTE_MAP immutable = atomic::get_asset_immutable( asset );
            const uint8_t stars = std::get<uint8_t>(immutable["stars"]);
            const string color = std::get<string>(immutable["color"]);

            // accumulate all colors (higher chance of selecting the more as input)
            colors.push_back( color );

            // sum stars
            stars_total += stars;

            print("get.color: ", color, "\n");
            print("get.stars: ", stars, "\n");
        }

        // ultra rare requirement (pure = all same color)
        const string pure = is_pure( colors );

        // immutable
        ATTRIBUTE_MAP immutable_data = {};

        // computed attributes
        const string rarity = select_rarity( stars_total, pure );
        const string name = select_name( rarity, pure );
        immutable_data["rarity"] = rarity;
        immutable_data["name"] = name;

        // required
        const string color = select_color( name );
        const string img = select_img( name );
        immutable_data["img"] = img;

        // optional
        const string banner = select_banner( name );
        const string _3d = select_3d( name );
        if ( banner.size() ) immutable_data["banner"] = banner;
        if ( _3d.size() ) immutable_data["3d"] = _3d;

        print("\n🎉 RESULT 🎉\n");
        print("===========\n");
        print("stars_total: ", stars_total, "\n");
        print("rarity: ", rarity, "\n");
        print("name: ", name, "\n");
        print("pure: ", pure, "\n");
        print("color: ", color, "\n");
        print("img: ", img, "\n");
        print("banner: ", banner, "\n");
        print("3d: ", _3d, "\n");

        return { immutable_data, {} };
    }

    void validate_input_attributes( const atomic::nft id )
    {
        const auto my_template = atomic::get_template( id.collection_name, id.template_id );
        const auto schema = atomic::get_schema( id.collection_name, my_template.schema_name );
        check( my_template.transferable, "blend::validate_attributes: [nft] must be `transferable`");
        check( my_template.burnable, "blend::validate_attributes: [nft] must be `burnable`");
        check( atomic::attribute_exists(schema.format, { "rarity", "string" }), "blend::validate_attributes: [nft] must have 'rarity' as String");
        check( atomic::attribute_exists(schema.format, { "banner", "image" }), "blend::validate_attributes: [nft] must have 'banner' as Image");
        check( atomic::attribute_exists(schema.format, { "3d", "string" }), "blend::validate_attributes: [nft] must have '3d' as String");
    }
}
}
}
