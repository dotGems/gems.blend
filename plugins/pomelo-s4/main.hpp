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
    const string COLOR_1 = "Red";
    const string COLOR_2 = "Green";
    const string COLOR_3 = "Pink";
    const string COLOR_4 = "Navy";
    const string COLOR_5 = "Golden";
    const string COLOR_6 = "Mixed";

    // moon phases - common
    const string NAME_COMMON_1 = "Medical Unit";
    const string NAME_COMMON_2 = "Command Unit";
    const string NAME_COMMON_3 = "Food Unit";
    const string NAME_COMMON_4 = "Service Unit";
    const string NAME_COMMON_5 = "Crew Unit";
    const string NAME_COMMON_6 = "Communication Tower";

    // moon phases - rare
    const string NAME_RARE_1 = "Glider";
    const string NAME_RARE_2 = "Trike";
    const string NAME_RARE_3 = "Rover";

    // moon phases - ultra rare
    const string NAME_ULTRA_1 = "Pomelo Armory";
    const string NAME_ULTRA_2 = "Pomelo Farm";
    const string NAME_ULTRA_3 = "Pomelo Disco";
    const string NAME_ULTRA_4 = "Pomelo Mining";
    const string NAME_ULTRA_5 = "Pomelo Power";

    const vector<string> NAME_COMMONS = {
        NAME_COMMON_1,
        NAME_COMMON_2,
        NAME_COMMON_3,
        NAME_COMMON_4,
        NAME_COMMON_5,
        NAME_COMMON_6,
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
        {NAME_COMMON_6, COLOR_6},

        // Rare
        {NAME_RARE_1, COLOR_6},
        {NAME_RARE_2, COLOR_6},
        {NAME_RARE_3, COLOR_6},

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
        {NAME_COMMON_1, "QmXFrKbDnieCpmqQgsUByskKmJ97h38CNfGn2X4nzLdrGV"},
        {NAME_COMMON_2, "QmVje7m6me2sMX41Y8DyQ6yAyjPs5ZjTEPfXD1SnDnqDtH"},
        {NAME_COMMON_3, "QmeCpBFzCsVKpC5xJE16ihktuLMQgqNL7p1uJdirbC7tHL"},
        {NAME_COMMON_4, "QmPZHmbCGxXgAgyKKuS4Nh4qUJfZA1xtfJMgrNJWcC7CKk"},
        {NAME_COMMON_5, "QmZ8prTh3RMM9KFtGsabkbMDT2pV8YZVfo2TG1CstFZuCp"},
        {NAME_COMMON_6, "QmcDd49nQ9rKqX6P2dJysA1C2cnoFEJerPh91FubjV76iJ"},
        {NAME_RARE_1, "QmYTwGGV4p6hmziaeeZku43mAN7dPwShDR8LpgzqnsQcWT"},
        {NAME_RARE_2, "QmZroLCGTVJN46F5Ma92e4YDrcZSVqLBf5DuUYQgGq7w6W"},
        {NAME_RARE_3, "Qma77Pu3CCJziCRG8VVavwEX3bmNXLHuPMQab2fVJMBgNZ"},
        {NAME_ULTRA_1, "QmaT77rFmFR75FtPK7t68Gd3pJS7HMzRyLcfA4Q9qCGVpg"},
        {NAME_ULTRA_2, "QmUHpHVUZtquYT5jRyrfCQR1KbTAZVjvxh9v3AePs1gUBa"},
        {NAME_ULTRA_3, "QmdkwQrhdwSf2t94abpgwX17Hv2cJut428MKSjytcDH1qe"},
        {NAME_ULTRA_4, "QmU7TmyVcU7zby8ym7qJ9v1hzyxudhHxsZmQYVVWvBfdHv"},
        {NAME_ULTRA_5, "Qmf73aqYE3eHFzLdFi4aFT37e3J26BNU7KDFZybRiTh4wf"},
    };

    const map<string, string> IPFS_BANNER = {
        {NAME_RARE_1,  "QmSKPccbseRjY1f3aepGM7iSu3j8Xs6fB3aZ13LByxWLfn"},
        {NAME_RARE_2,  "QmURjDaT6M4FPZKNwoMRqbHLBCC4obW4ucoy2sxXuuGCRX"},
        {NAME_RARE_3,  "QmWyTa3Cs5LaJ4uW2KMKKv3DC2gfshD2bHFvPHK8QDpDQK"},
        {NAME_ULTRA_1, "QmX2f4b7nmnbDHByQUTfUETeXQ4T2MF64hyGyfxeLpt3hP"},
        {NAME_ULTRA_2, "QmXuq9AodZMxG5HLYuT1RqUVonuE2GoRGoDXK9Y3KFMDgh"},
        {NAME_ULTRA_3, "QmVEE4VTeEVMxCMWeD71JZWCmZxjFY2MvcRXvVumPi9B24"},
        {NAME_ULTRA_4, "QmRZQJmnt8pYFEM6CvkRBNZ9UQYekQkQ9KmxM6DWuRcjRR"},
        {NAME_ULTRA_5, "QmWc8xtu4wayXJGcArqiWpmN2GPGVb4nRezaCb1Bov4PE1"},
    };

    const map<string, string> IPFS_3D = {
        {NAME_COMMON_1, "QmPrajnMriYPAyKsyYTdgrNcWH79HQ56Q9yYuTHuytKaVa"},
        {NAME_COMMON_2, "QmbjLA5pf1nZcSKvhPz4MmNT5HEGrv9GtxxPS2nGXUn3tQ"},
        {NAME_COMMON_3, "QmPhXdj6PWGpcWYK7rtVxqnwYacMGEEAqdQfYsHsYjVtq1"},
        {NAME_COMMON_4, "QmTJ94pgvy17WqzkjzEcKmpMSBtXsdhYWSVE2tiWMrSDhy"},
        {NAME_COMMON_5, "QmNsdUHLhoHf7ftSRAPTgq7RnJKtaFG7XE1JKfKufeSCkB"},
        {NAME_COMMON_6, "QmYyW8Sy4hvpNz6t8kraBivPzSGRLHgDUqwGfac4PRqCxr"},
        {NAME_RARE_1,   "QmWqTzD15zSgGCRBSDh17VFKUVyr8XFDbSpaHkmDopCGLn"},
        {NAME_RARE_2,   "QmPF7mRc81nmWxt4urvDQUWSocU7vrqvvGmhLadnZZ9ake"},
        {NAME_RARE_3,   "QmTs6va345rjmyX52oqA1g5iMMYPdVaLo2cD4bimbKS4zv"},
        {NAME_ULTRA_1,  "QmXqe3dbbDjr44fFFK8EPjmtUn54s1NU9yt7YvW3gdB2hY"},
        {NAME_ULTRA_2,  "QmaD2AGReZdhZDkjBFKr4GeeaBCkGLpmMvDqyBa4YN8x19"},
        {NAME_ULTRA_3,  "QmXMRLdmgcp2HV6d8agZhHfMGmybNxDLvuYaxp8tuiuYip"},
        {NAME_ULTRA_4,  "QmdBEiwtHR4rYpsFNj5R51kmWgX9E2BAXfG9F5GSyZEc5w"},
        {NAME_ULTRA_5,  "QmSpyBz2XEwkCBqFLH7mmfbaMPkomxkL2bxzQXZeMDEQYr"},
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

        // computed attributes
        const string rarity = select_rarity( power_total, pure );
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
        print("power_total: ", power_total, "\n");
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
} // namespace main
} // namespace s4
} // namespace pomelo
