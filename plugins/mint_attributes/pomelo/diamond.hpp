#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {

namespace diamond {

    // Color
    // Grade 5 - Colorless
    // Grade 4 - Near Colorless
    // Grade 3 - Faint Yellow
    // Grade 2 - Very Light Yellow
    // Grade 1 - Light Yellow

    // Clarity
    // Grade 5 - Internally Flawless
    // Grade 4 - Very Very Slight Inclusions
    // Grade 3 - Very Slight Inclusions
    // Grade 2 - Slight Inclusions
    // Grade 1 - Imperfect

    const vector<string> COLOR_GRADE_5 = {"D", "E", "F"};
    const vector<string> COLOR_GRADE_4 = {"G", "H", "I", "J"};
    const vector<string> COLOR_GRADE_3 = {"K", "L", "M"};
    const vector<string> COLOR_GRADE_2 = {"N", "O", "P", "Q", "R"};
    const vector<string> COLOR_GRADE_1 = {"S", "T", "U", "V", "W", "X", "Y", "Z"};

    const vector<string> CLARITY_GRADE_5 = {"FL", "IF"};
    const vector<string> CLARITY_GRADE_4 = {"VVS1", "VVS2"};
    const vector<string> CLARITY_GRADE_3 = {"VS1", "VS2"};
    const vector<string> CLARITY_GRADE_2 = {"SI1", "SI2"};
    const vector<string> CLARITY_GRADE_1 = {"I1", "I2", "I3"};

    const map<string, int> COLOR_GRADES = {
        { "D", 5 }, { "E", 5 }, { "F", 5 },
        { "G", 4 }, { "H", 4 }, { "I", 4 }, { "J", 4 },
        { "K", 3 }, { "L", 3 }, { "M", 3 },
        { "N", 2 }, { "O", 2 }, { "P", 2 }, { "Q", 2 }, { "R", 2 },
        { "S", 1 }, { "T", 1 }, { "U", 1 }, { "V", 1 }, { "W", 1 }, { "X", 1 }, { "Y", 1 }, { "Z", 1 }
    };
    const map<string, int> CLARITY_GRADES = {
        { "FL", 5 }, { "IF", 5 },
        { "VVS1", 4 }, { "VVS2", 4 },
        { "VS1", 3 }, { "VS2", 3 },
        { "SI1", 2 }, { "SI2", 2 },
        { "I1", 1 }, { "I2", 1 }, { "I3", 1 }
    };

    int calculate_color( const string& color )
    {
        check( COLOR_GRADES.count(color), "pomelo::diamond::calculate_color: [color] invalid value");
        return COLOR_GRADES.at(color);
    }

    int calculate_clarity( const string& clarity )
    {
        check( CLARITY_GRADES.count(clarity), "pomelo::diamond::calculate_clarity: [clarity] invalid value");
        return CLARITY_GRADES.at(clarity);
    }

    string random_select( const vector<string>& values )
    {
        check( values.size(), "pomelo::diamond::random_select: [values] must contain at least one item");
        return values.at(gems::random::generate(1, 0, values.size() -1 )[0]);
    }

    string select_color( const int grade )
    {
        if ( grade == 5 ) return random_select( COLOR_GRADE_5 );
        if ( grade == 4 ) return random_select( COLOR_GRADE_4 );
        if ( grade == 3 ) return random_select( COLOR_GRADE_3 );
        if ( grade == 2 ) return random_select( COLOR_GRADE_2 );
        return random_select( COLOR_GRADE_1 );
    }

    string select_clarity( const int grade )
    {
        if ( grade == 5 ) return random_select( CLARITY_GRADE_5 );
        if ( grade == 4 ) return random_select( CLARITY_GRADE_4 );
        if ( grade == 3 ) return random_select( CLARITY_GRADE_3 );
        if ( grade == 2 ) return random_select( CLARITY_GRADE_2 );
        return random_select( CLARITY_GRADE_1 );
    }

    string select_img( const string shape, int grade )
    {
        if ( shape == "Round") {
            switch ( grade ) {
                case 5: return "IPFS";
                case 4: return "IPFS";
                case 3: return "IPFS";
                case 2: return "IPFS";
                case 1: return "IPFS";
            }
        }
        if ( shape == "Circle" ) {
            switch ( grade ) {
                case 5: return "IPFS";
                case 4: return "IPFS";
                case 3: return "IPFS";
                case 2: return "IPFS";
                case 1: return "IPFS";
            }
        }
        if ( shape == "Heart" ) {
            switch ( grade ) {
                case 5: return "IPFS";
                case 4: return "IPFS";
                case 3: return "IPFS";
                case 2: return "IPFS";
                case 1: return "IPFS";
            }
        }
        check( false, "pomelo::diamond::select_img: invalid value");
        return "";
    }

    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids, const vector<atomicassets::assets_s>& in_assets )
    {
        // containers
        vector<string> shapes;
        int total = 0;

        // add shapes & sum scores
        for ( const auto& asset : in_assets ) {
            const ATTRIBUTE_MAP immutable = atomic::get_asset_immutable( asset );
            const string shape = atomic::attribute_to_string( immutable, "shape" );
            const string color = atomic::attribute_to_string( immutable, "color" );
            const string clarity = atomic::attribute_to_string( immutable, "clarity" );

            // accumulate all shapes (higher chance of selecting the more as input)
            shapes.push_back( shape );

            // color & clarity scores
            total += calculate_color( color );
            total += calculate_clarity( clarity );
        }

        // grade (lowest) 1-2-3-4-5 (highest)
        // 4x ingredients (1-5)
        int grade = total / 8;

        // immutable
        ATTRIBUTE_MAP immutable_data = {};
        const string shape = random_select( shapes );
        immutable_data["grade"] = grade;
        immutable_data["shape"] = shape;
        immutable_data["color"] = select_color( grade );
        immutable_data["clarity"] = select_clarity( grade );
        immutable_data["img"] = select_img( shape, grade );

        return { immutable_data, {} };
    }
} // namespace diamond
} // namespace pomelo
