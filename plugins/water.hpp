#pragma once
#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace test {

namespace water {
    // Clarity
    // quality 5 - Internally Flawless
    // quality 4 - Very Very Slight Inclusions
    // quality 3 - Very Slight Inclusions
    // quality 2 - Slight Inclusions
    // quality 1 - Imperfect
    const vector<string> CLARITIES = {"FL", "IF","VVS1", "VVS2","VS1", "VS2","SI1", "SI2", "I1", "I2", "I3"};
    const vector<string> CLARITY_QUALITY_5 = {"FL", "IF"};
    const vector<string> CLARITY_QUALITY_4 = {"VVS1", "VVS2"};
    const vector<string> CLARITY_QUALITY_3 = {"VS1", "VS2"};
    const vector<string> CLARITY_QUALITY_2 = {"SI1", "SI2"};
    const vector<string> CLARITY_QUALITY_1 = {"I1", "I2", "I3"};

    const map<string, int> CLARITY_QUALITY = {
        { "FL", 5 }, { "IF", 5 },               // Rare
        { "VVS1", 4 }, { "VVS2", 4 },           // Rare
        { "VS1", 3 }, { "VS2", 3 },             // Common
        { "SI1", 2 }, { "SI2", 2 },             // Common
        { "I1", 1 }, { "I2", 1 }, { "I3", 1 }   // Common
    };

    // Color
    const vector<string> COLORS = {"White", "Rose", "Amber", "Mint", "Ice Blue"};

    // Shape
    const vector<string> SHAPES = {"Round", "Pear", "Heart"};

    int calculate_clarity( const string& clarity )
    {
        check( CLARITY_QUALITY.count(clarity), "test::water::calculate_clarity: [clarity] invalid value");
        return CLARITY_QUALITY.at(clarity);
    }

    string random_select( const vector<string>& values )
    {
        check( values.size(), "test::water::random_select: [values] must contain at least one item");
        return values.at(gems::random::generate(1, 0, values.size() -1 )[0]);
    }

    string select_clarity( const int quality )
    {
        if ( quality == 5 ) return random_select( CLARITY_QUALITY_5 );
        if ( quality == 4 ) return random_select( CLARITY_QUALITY_4 );
        if ( quality == 3 ) return random_select( CLARITY_QUALITY_3 );
        if ( quality == 2 ) return random_select( CLARITY_QUALITY_2 );
        if ( quality == 1 ) return random_select( CLARITY_QUALITY_1 );
        check( false,  "test::water::select_clarity: [quality] invalid");
        return "";
    }

    string select_rarity( const int quality )
    {
        if ( quality == 5 ) return "Rare";
        if ( quality == 4 ) return "Rare";
        if ( quality == 3 ) return "Common";
        if ( quality == 2 ) return "Common";
        if ( quality == 1 ) return "Common";
        check( false,  "test::water::select_clarity: [quality] invalid");
        return "";
    }

    string select_img( const string shape, const string color, const string quality )
    {
        const bool noise = (quality == "Rare") ? false : true;
        if ( color == "White") {
            if ( noise ) {
                if ( shape == "Round" ) return "QmQHcoEKbK9sqkTFUxtWMoA2LWGA5o2nZ2JEEM4AEtPZ5D";
                if ( shape == "Pear" ) return "QmQHcoEKbK9sqkTFUxtWMoA2LWGA5o2nZ2JEEM4AEtPZ5D";
                if ( shape == "Heart" ) return "QmQHcoEKbK9sqkTFUxtWMoA2LWGA5o2nZ2JEEM4AEtPZ5D";
            } else {
                if ( shape == "Round" ) return "QmQHcoEKbK9sqkTFUxtWMoA2LWGA5o2nZ2JEEM4AEtPZ5D";
                if ( shape == "Pear" ) return "QmQHcoEKbK9sqkTFUxtWMoA2LWGA5o2nZ2JEEM4AEtPZ5D";
                if ( shape == "Heart" ) return "QmQHcoEKbK9sqkTFUxtWMoA2LWGA5o2nZ2JEEM4AEtPZ5D";
            }
        }
        if ( color == "Rose" ) {
            if ( noise ) {
                if ( shape == "Round" ) return "QmU7imr2si99q3xwirNfyiAXKY2qcxCg7UhdfCSwiY8Hgy";
                if ( shape == "Pear" ) return "QmU7imr2si99q3xwirNfyiAXKY2qcxCg7UhdfCSwiY8Hgy";
                if ( shape == "Heart" ) return "QmU7imr2si99q3xwirNfyiAXKY2qcxCg7UhdfCSwiY8Hgy";
            } else {
                if ( shape == "Round" ) return "QmU7imr2si99q3xwirNfyiAXKY2qcxCg7UhdfCSwiY8Hgy";
                if ( shape == "Pear" ) return "QmU7imr2si99q3xwirNfyiAXKY2qcxCg7UhdfCSwiY8Hgy";
                if ( shape == "Heart" ) return "QmU7imr2si99q3xwirNfyiAXKY2qcxCg7UhdfCSwiY8Hgy";
            }
        }
        if ( color == "Amber" ) {
            if ( noise ) {
                if ( shape == "Round" ) return "QmQHcoEKbK9sqkTFUxtWMoA2LWGA5o2nZ2JEEM4AEtPZ5D";
                if ( shape == "Pear" ) return "QmQHcoEKbK9sqkTFUxtWMoA2LWGA5o2nZ2JEEM4AEtPZ5D";
                if ( shape == "Heart" ) return "QmQHcoEKbK9sqkTFUxtWMoA2LWGA5o2nZ2JEEM4AEtPZ5D";
            } else {
                if ( shape == "Round" ) return "QmQHcoEKbK9sqkTFUxtWMoA2LWGA5o2nZ2JEEM4AEtPZ5D";
                if ( shape == "Pear" ) return "QmQHcoEKbK9sqkTFUxtWMoA2LWGA5o2nZ2JEEM4AEtPZ5D";
                if ( shape == "Heart" ) return "QmQHcoEKbK9sqkTFUxtWMoA2LWGA5o2nZ2JEEM4AEtPZ5D";
            }
        }
        if ( color == "Mint" ) {
            if ( noise ) {
                if ( shape == "Round" ) return "QmSdXzTKTgqrzeMbsojtuSgnnkeSKqhTT1sp85m59MrNCu";
                if ( shape == "Pear" ) return "QmSdXzTKTgqrzeMbsojtuSgnnkeSKqhTT1sp85m59MrNCu";
                if ( shape == "Heart" ) return "QmSdXzTKTgqrzeMbsojtuSgnnkeSKqhTT1sp85m59MrNCu";
            } else {
                if ( shape == "Round" ) return "QmSdXzTKTgqrzeMbsojtuSgnnkeSKqhTT1sp85m59MrNCu";
                if ( shape == "Pear" ) return "QmSdXzTKTgqrzeMbsojtuSgnnkeSKqhTT1sp85m59MrNCu";
                if ( shape == "Heart" ) return "QmSdXzTKTgqrzeMbsojtuSgnnkeSKqhTT1sp85m59MrNCu";
            }
        }
        if ( color == "Ice Blue" ) {
            if ( noise ) {
                if ( shape == "Round" ) return "QmQgMaHpPgR1dxAYe3QRaeHr5xR3bxBT5KpcbruWT9W8bm";
                if ( shape == "Pear" ) return "QmQgMaHpPgR1dxAYe3QRaeHr5xR3bxBT5KpcbruWT9W8bm";
                if ( shape == "Heart" ) return "QmQgMaHpPgR1dxAYe3QRaeHr5xR3bxBT5KpcbruWT9W8bm";
            } else {
                if ( shape == "Round" ) return "QmQgMaHpPgR1dxAYe3QRaeHr5xR3bxBT5KpcbruWT9W8bm";
                if ( shape == "Pear" ) return "QmQgMaHpPgR1dxAYe3QRaeHr5xR3bxBT5KpcbruWT9W8bm";
                if ( shape == "Heart" ) return "QmQgMaHpPgR1dxAYe3QRaeHr5xR3bxBT5KpcbruWT9W8bm";
            }
        }
        check( false, "pomelo::water::select_img: invalid values");
        return "";
    }

    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomicassets::assets_s>& in_assets )
    {
        // containers
        vector<string> shapes;
        vector<string> colors;
        int total = 0;

        print("GET ATTRIBUTES\n");
        print("==============\n");
        // add shapes & sum clarity quality
        for ( const auto& asset : in_assets ) {
            print("\nASSET\n");
            print("-----\n");
            print("asset_id: ", asset.asset_id, "\n");
            print("schema_name: ", asset.schema_name, "\n");
            if ( asset.schema_name != "water"_n ) continue; // skip if not water
            ATTRIBUTE_MAP immutable = atomic::get_asset_immutable( asset );
            const string shape = std::get<string>(immutable["shape"]);
            const string color = std::get<string>(immutable["color"]);
            const string clarity = std::get<string>(immutable["clarity"]);

            // accumulate all shapes & colors (higher chance of selecting the more as input)
            shapes.push_back( shape );
            colors.push_back( color );

            // sum clarity quality
            const int score = calculate_clarity( clarity );
            total += score;

            print("get.shape: ", shape, "\n");
            print("get.color: ", color, "\n");
            print("get.clarity: ", clarity, "\n");
            print("score: ", score, "\n");
        }
        // calculate clarity quality average (round down)
        // (lowest) 1-2-3-4-5 (highest)
        int average = total / 5;

        // immutable
        ATTRIBUTE_MAP immutable_data = {};
        const string rarity = select_rarity( average );
        const string shape = random_select( shapes );
        const string color = random_select( colors );
        const string clarity = select_clarity( average );
        const string img = select_img( shape, color, rarity );
        immutable_data["rarity"] = rarity;
        immutable_data["shape"] = shape;
        immutable_data["color"] = color;
        immutable_data["clarity"] = clarity;
        immutable_data["img"] = img;

        print("\n🎉 RESULT 🎉\n");
        print("===========\n");
        print("total: ", total, "\n");
        print("average: ", average, "\n");
        print("rarity: ", rarity, "\n");
        print("shape: ", shape, "\n");
        print("color: ", color, "\n");
        print("clarity: ", clarity, "\n");
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
        check( atomic::attribute_exists(schema.format, { "shape", "string" }), "blend::validate_attributes: [nft] must have 'shape' as String");
        check( atomic::attribute_exists(schema.format, { "color", "string" }), "blend::validate_attributes: [nft] must have 'color' as String");
        check( atomic::attribute_exists(schema.format, { "clarity", "string" }), "blend::validate_attributes: [nft] must have 'clarity' as String");
    }
} // namespace water
} // namespace test
