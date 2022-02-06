#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {

namespace diamond {
    // Clarity
    // score 5 - Internally Flawless
    // score 4 - Very Very Slight Inclusions
    // score 3 - Very Slight Inclusions
    // score 2 - Slight Inclusions
    // score 1 - Imperfect
    const vector<string> CLARITIES = {"FL", "IF","VVS1", "VVS2","VS1", "VS2","SI1", "SI2", "I1", "I2", "I3"};
    const vector<string> CLARITY_SCORE_5 = {"FL", "IF"};
    const vector<string> CLARITY_SCORE_4 = {"VVS1", "VVS2"};
    const vector<string> CLARITY_SCORE_3 = {"VS1", "VS2"};
    const vector<string> CLARITY_SCORE_2 = {"SI1", "SI2"};
    const vector<string> CLARITY_SCORE_1 = {"I1", "I2", "I3"};

    // Colors
    const vector<string> COLORS = {"White", "Rose", "Amber", "Mint", "Ice Blue"};

    // Shapes
    const vector<string> SHAPES = {"Round", "Pear", "Heart"};

    const map<string, int> CLARITY_SCORES = {
        { "FL", 5 }, { "IF", 5 },
        { "VVS1", 4 }, { "VVS2", 4 },
        { "VS1", 3 }, { "VS2", 3 },
        { "SI1", 2 }, { "SI2", 2 },
        { "I1", 1 }, { "I2", 1 }, { "I3", 1 }
    };

    int calculate_clarity( const string& clarity )
    {
        check( CLARITY_SCORES.count(clarity), "pomelo::diamond::calculate_clarity: [clarity] invalid value");
        return CLARITY_SCORES.at(clarity);
    }

    string random_select( const vector<string>& values )
    {
        check( values.size(), "pomelo::diamond::random_select: [values] must contain at least one item");
        return values.at(gems::random::generate(1, 0, values.size() -1 )[0]);
    }

    string select_clarity( const int score )
    {
        if ( score == 5 ) return random_select( CLARITY_SCORE_5 );
        if ( score == 4 ) return random_select( CLARITY_SCORE_4 );
        if ( score == 3 ) return random_select( CLARITY_SCORE_3 );
        if ( score == 2 ) return random_select( CLARITY_SCORE_2 );
        if ( score == 1 ) return random_select( CLARITY_SCORE_1 );
        check( false,  "pomelo::diamond::select_clarity: [score] invalid");
        return "";
    }

    string select_img( const string shape, const string color, const int score )
    {
        const bool noise = (score == 5 || score == 4) ? false : true;
        const vector<string> COLORS = {"White", "Rose", "Amber", "Mint", "Ice Blue"};
        const vector<string> SHAPES = {"Round", "Pear", "Heart"};

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

        // add shapes & sum scores
        for ( const auto& asset : in_assets ) {
            const ATTRIBUTE_MAP immutable = atomic::get_asset_immutable( asset );
            const string shape = atomic::attribute_to_string( immutable, "shape" );
            const string color = atomic::attribute_to_string( immutable, "color" );
            const string clarity = atomic::attribute_to_string( immutable, "clarity" );

            // accumulate all shapes & colors (higher chance of selecting the more as input)
            shapes.push_back( shape );
            colors.push_back( color );

            // sum clarity scores
            total += calculate_clarity( clarity );
        }

        // sum scores, divide by 4 and round down
        // score (lowest) 1-2-3-4-5 (highest)
        int score = total / 4;

        // immutable
        ATTRIBUTE_MAP immutable_data = {};
        const string shape = random_select( shapes );
        const string color = random_select( colors );
        immutable_data["score"] = score;
        immutable_data["shape"] = shape;
        immutable_data["color"] = color;
        immutable_data["clarity"] = select_clarity( score );
        immutable_data["img"] = select_img( shape, color, score );

        return { immutable_data, {} };
    }
} // namespace diamond
} // namespace pomelo
