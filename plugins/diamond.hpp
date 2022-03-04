#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {

namespace diamond {
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
                if ( shape == "Round" ) return "IPFS";
                if ( shape == "Pear" ) return "IPFS";
                if ( shape == "Heart" ) return "IPFS";
            } else {
                if ( shape == "Round" ) return "IPFS";
                if ( shape == "Pear" ) return "IPFS";
                if ( shape == "Heart" ) return "IPFS";
            }
        }
        if ( color == "Rose" ) {
            if ( noise ) {
                if ( shape == "Round" ) return "IPFS";
                if ( shape == "Pear" ) return "IPFS";
                if ( shape == "Heart" ) return "IPFS";
            } else {
                if ( shape == "Round" ) return "IPFS";
                if ( shape == "Pear" ) return "IPFS";
                if ( shape == "Heart" ) return "IPFS";
            }
        }
        if ( color == "Amber" ) {
            if ( noise ) {
                if ( shape == "Round" ) return "IPFS";
                if ( shape == "Pear" ) return "IPFS";
                if ( shape == "Heart" ) return "IPFS";
            } else {
                if ( shape == "Round" ) return "IPFS";
                if ( shape == "Pear" ) return "IPFS";
                if ( shape == "Heart" ) return "IPFS";
            }
        }
        if ( color == "Mint" ) {
            if ( noise ) {
                if ( shape == "Round" ) return "IPFS";
                if ( shape == "Pear" ) return "IPFS";
                if ( shape == "Heart" ) return "IPFS";
            } else {
                if ( shape == "Round" ) return "IPFS";
                if ( shape == "Pear" ) return "IPFS";
                if ( shape == "Heart" ) return "IPFS";
            }
        }
        if ( color == "Ice Blue" ) {
            if ( noise ) {
                if ( shape == "Round" ) return "IPFS";
                if ( shape == "Pear" ) return "IPFS";
                if ( shape == "Heart" ) return "IPFS";
            } else {
                if ( shape == "Round" ) return "IPFS";
                if ( shape == "Pear" ) return "IPFS";
                if ( shape == "Heart" ) return "IPFS";
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

        // add shapes & sum clarity quality
        for ( const auto& asset : in_assets ) {
            const ATTRIBUTE_MAP immutable = atomic::get_asset_immutable( asset );
            const string shape = atomic::attribute_to_string( immutable, "shape" );
            const string color = atomic::attribute_to_string( immutable, "color" );
            const string clarity = atomic::attribute_to_string( immutable, "clarity" );

            // accumulate all shapes & colors (higher chance of selecting the more as input)
            shapes.push_back( shape );
            colors.push_back( color );

            // sum clarity quality
            total += calculate_clarity( clarity );
        }

        // calculate clarity quality average (round down)
        // (lowest) 1-2-3-4-5 (highest)
        int average = total / 5;

        // immutable
        ATTRIBUTE_MAP immutable_data = {};
        const string shape = random_select( shapes );
        const string color = random_select( colors );
        const string rarity = select_rarity( average );
        immutable_data["rarity"] = rarity;
        immutable_data["shape"] = shape;
        immutable_data["color"] = color;
        immutable_data["clarity"] = select_clarity( average );
        immutable_data["img"] = select_img( shape, color, rarity );

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
} // namespace diamond
} // namespace pomelo