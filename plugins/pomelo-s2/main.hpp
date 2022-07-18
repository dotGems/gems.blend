#pragma once
#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {
namespace s2 {
namespace main {
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
        check( CLARITY_QUALITY.count(clarity), "pomelo::diamond::calculate_clarity: [clarity] invalid value");
        return CLARITY_QUALITY.at(clarity);
    }

    string random_select( const vector<string>& values )
    {
        check( values.size(), "pomelo::diamond::random_select: [values] must contain at least one item");
        return values.at(gems::random::generate(1, 0, values.size() -1 )[0]);
    }

    string select_clarity( const int quality )
    {
        if ( quality == 5 ) return random_select( CLARITY_QUALITY_5 );
        if ( quality == 4 ) return random_select( CLARITY_QUALITY_4 );
        if ( quality == 3 ) return random_select( CLARITY_QUALITY_3 );
        if ( quality == 2 ) return random_select( CLARITY_QUALITY_2 );
        if ( quality == 1 ) return random_select( CLARITY_QUALITY_1 );
        check( false,  "pomelo::diamond::select_clarity: [quality] invalid");
        return "";
    }

    string select_rarity( const int quality )
    {
        if ( quality == 5 ) return "Rare";
        if ( quality == 4 ) return "Rare";
        if ( quality == 3 ) return "Common";
        if ( quality == 2 ) return "Common";
        if ( quality == 1 ) return "Common";
        check( false,  "pomelo::diamond::select_clarity: [quality] invalid");
        return "";
    }

    string select_img( const string shape, const string color, const string quality )
    {
        const bool noise = (quality == "Rare") ? false : true;
        if ( color == "White") {
            if ( noise ) {
                if ( shape == "Round" ) return "QmNhV8Tnq1gRk6itp4STpW9ZJcMhiHMA7zTwzc6SGBA3tV";
                if ( shape == "Pear" ) return "Qmb9vtynjCeEQoDSFbh4WzhbD3yffm6u3Fm6S9TorbSTVJ";
                if ( shape == "Heart" ) return "QmWQGMkE3icr3nVh1374WU81KzEY3HE97i3UsKoJdxfvfm";
            } else {
                if ( shape == "Round" ) return "QmZqmV6tgcvojWhb9HZcodzScjdBgPR7qFwREYyJ6QN6LX";
                if ( shape == "Pear" ) return "QmUWFfxKhAdDAuRt2YgQBTAhRRqpsnuRspJMtdGZhzd3Y8";
                if ( shape == "Heart" ) return "QmUAKaYi83DFsUHwjFE8TrxqPnSrJgTuaNgPqBApm9PHj8";
            }
        }
        if ( color == "Rose" ) {
            if ( noise ) {
                if ( shape == "Round" ) return "QmZu3rr38WFMFwn2dGibWaCnLroKJtqr9obqegSHZikuZX";
                if ( shape == "Pear" ) return "QmTwHorLGXB5MdcXwQsF5jbvYkM4DsGNUtwNwYFWj9j7N8";
                if ( shape == "Heart" ) return "QmRYhR2heXJKusPJvUmSiQtZ5gvgSng2gUPR1AnPmyQmoR";
            } else {
                if ( shape == "Round" ) return "QmUa7yVAC9YNDGd6Aa6EFRaSuwG6et6xdTqX19phiU8i7t";
                if ( shape == "Pear" ) return "QmRyGA5zMHgbqpEq7TaVHbsb6PfXyJ1UjgihWE6tX4U9XJ";
                if ( shape == "Heart" ) return "QmUikq1uXm7AiqWQcuj8QBcnS531zfST1jkaEq9Jcc7k4Z";
            }
        }
        if ( color == "Amber" ) {
            if ( noise ) {
                if ( shape == "Round" ) return "QmWspyWorrG8wD5bepaZfiNxvjLRuFAKMoMHkyrrNSeYGK";
                if ( shape == "Pear" ) return "QmNV2nyJhUpvxktH1izS1SP2fTtUNkPt85tuGVtBtcPGAj";
                if ( shape == "Heart" ) return "QmQqGfEPzD7uXgyrubFzDw94XtQbeYeJZ696cFCF35Z1h2";
            } else {
                if ( shape == "Round" ) return "QmReDTwuS4zkNNihfWDxAM78jt28GZVZjtDnz17Y2vQCky";
                if ( shape == "Pear" ) return "QmdKgCAqA2Gt1dypjNG2DxrcMqFm8i6rZsje65nmxSdXYq";
                if ( shape == "Heart" ) return "QmcXm8McmPArjhMwisjY3j51mgorvedJB6THr3YRjWKeF4";
            }
        }
        if ( color == "Mint" ) {
            if ( noise ) {
                if ( shape == "Round" ) return "QmZWV1xCdnEPyEYpfvNpqx5FgoxZwcWkL6bMprhFgxppHw";
                if ( shape == "Pear" ) return "QmQDUcEcW3JqmNx5ZJtamAViRbRiebqVu7YWhzur8Vkerp";
                if ( shape == "Heart" ) return "QmaNwC58nQitewgeKqnatDsZvNX3g5oprLQTVd43D6Tqs1";
            } else {
                if ( shape == "Round" ) return "QmbQEvxggYUJqaNqwQbgvnsapF8keYhuK3MayL8T2gmfdi";
                if ( shape == "Pear" ) return "QmcyDTMrREwo79h6BZxL4C3dsKG5JZEcGfe3h6FYyWF5w4";
                if ( shape == "Heart" ) return "Qma9iQSJNfGKjeJ8M4ZsVBwSgfQWGTUxuYYj9trQuJhZju";
            }
        }
        if ( color == "Ice Blue" ) {
            if ( noise ) {
                if ( shape == "Round" ) return "QmTsaHv4foW751rkV12ohuZTFHLf4wUrsrzMiDRdis3jez";
                if ( shape == "Pear" ) return "QmeCyhRStPtFXn8aGPxyJzAj7jvK4YRPHJVQvwfR4ehAXy";
                if ( shape == "Heart" ) return "QmSHHTAui11gz2DFubo12AfZy6pgeumHX7hk9ionpNXRZn";
            } else {
                if ( shape == "Round" ) return "QmeRM1s9b18fqP21LTU5H6h7AG6GrmLhqfS1qGVqeTuuar";
                if ( shape == "Pear" ) return "QmdsJKsQgzFcUSXDMnkiZYX4tHuwN5KSDiEB53jcr89i4r";
                if ( shape == "Heart" ) return "Qma3gj4NbXgXWoTxi97TCatRcQyfHy2RkAvh6LuTFNiKcR";
            }
        }
        check( false, "pomelo::diamond::select_img: invalid values");
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
            print("asset.asset_id: ", asset.asset_id, "\n");
            print("asset.schema_name: ", asset.schema_name, "\n");
            if ( asset.schema_name != "diamonds"_n ) continue; // skip if not diamonds
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

    void validate_input_attributes( const atomic::nft id )
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
} // namespace main
} // namespace s2
} // namespace pomelo
