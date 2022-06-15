#pragma once

#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

#include "main.hpp"


namespace pomelo {
namespace s3 {
namespace dev {
    string select_img( const string phase )
    {
        // legendary
        if ( phase == pomelo::s3::main::PHASE_LEGENDARY_0 ) return "QmXapCV6Z3pHqxQU1QtTbcaA1vskVKHUXSRBncQK9mvowW";
        if ( phase == pomelo::s3::main::PHASE_LEGENDARY_1 ) return "QmPuok9ytkmCe6G6sNSzbiAHSYqvwoHLwApPNwW75gYTD9";
        if ( phase == pomelo::s3::main::PHASE_LEGENDARY_2 ) return "QmWWPc6jddtZDubHNwptjqqDR7C2i3unfK7KJhxgGduMbc";
        if ( phase == pomelo::s3::main::PHASE_LEGENDARY_3 ) return "QmVjaSxE1rvFCfLGbLDpbfn977dJyYeXbtKAgD7amGwVKy";
        if ( phase == pomelo::s3::main::PHASE_LEGENDARY_4 ) return "Qmcrno4RwGuBPSzFmATJX7sWvqJkXYCfWVk1ppux3qCgNj";

        // rare
        if ( phase == pomelo::s3::main::PHASE_RARE_0 ) return "QmUQzTp2QE59gb35qJYkPpm7NT5cASG6piLCY3SRrWhRkS";
        if ( phase == pomelo::s3::main::PHASE_RARE_1 ) return "QmUQzTp2QE59gb35qJYkPpm7NT5cASG6piLCY3SRrWhRkS";
        if ( phase == pomelo::s3::main::PHASE_RARE_2 ) return "QmUQzTp2QE59gb35qJYkPpm7NT5cASG6piLCY3SRrWhRkS";

        // common
        if ( phase == pomelo::s3::main::PHASE_COMMON_0 ) return "QmQpBwV6UvmdKZYiUerbSy1aWi1ski41D3qqTRDVBLqSj2";
        if ( phase == pomelo::s3::main::PHASE_COMMON_1 ) return "QmQpBwV6UvmdKZYiUerbSy1aWi1ski41D3qqTRDVBLqSj2";
        if ( phase == pomelo::s3::main::PHASE_COMMON_2 ) return "QmQpBwV6UvmdKZYiUerbSy1aWi1ski41D3qqTRDVBLqSj2";
        if ( phase == pomelo::s3::main::PHASE_COMMON_3 ) return "QmQpBwV6UvmdKZYiUerbSy1aWi1ski41D3qqTRDVBLqSj2";
        if ( phase == pomelo::s3::main::PHASE_COMMON_4 ) return "QmQpBwV6UvmdKZYiUerbSy1aWi1ski41D3qqTRDVBLqSj2";
        if ( phase == pomelo::s3::main::PHASE_COMMON_5 ) return "QmQpBwV6UvmdKZYiUerbSy1aWi1ski41D3qqTRDVBLqSj2";

        check( false, "pomelo::select_img: invalid values");
        return "";
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
        const string pure = pomelo::s3::main::is_pure( colors );

        // immutable
        ATTRIBUTE_MAP immutable_data = {};
        const string rarity = pomelo::s3::main::select_rarity( fuel_total, pure );
        const string phase = pomelo::s3::main::select_phase( rarity, pure );
        const string color = pomelo::s3::main::select_color( phase, pure );
        const string img = select_img( phase );
        immutable_data["rarity"] = rarity;
        immutable_data["color"] = color;
        immutable_data["name"] = phase;
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
} // namespace dev
} // namespace s3
} // namespace pomelo