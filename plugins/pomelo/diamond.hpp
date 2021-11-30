#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {

    // Color grading
    // 100 - Rank 5 - colorless
    //  80 - Rank 4 - near colorless
    //  60 - Rank 3 - faint yellow
    //  40 - Rank 2 - very light yellow
    //  20 - Rank 1 - light yellow

    // Clarity Grading
    // 100 - Rank 5 - Internally Flawless
    //  80 - Rank 4 - Very Very Slight Inclusions
    //  60 - Rank 3 - Very Slight Inclusions
    //  40 - Rank 2 - Slight Inclusions
    //  20 - Rank 1 - Imperfect

    // Ranks
    const vector<string> COLOR_RANK_5 = {"D", "E", "F"};
    const vector<string> COLOR_RANK_4 = {"G", "H", "I", "J"};
    const vector<string> COLOR_RANK_3 = {"K", "L", "M"};
    const vector<string> COLOR_RANK_2 = {"N", "O", "P", "Q", "R"};
    const vector<string> COLOR_RANK_1 = {"S", "T", "U", "V", "W", "X", "Y", "Z"};

    const vector<string> CLARITY_RANK_5 = {"FL", "IF"};
    const vector<string> CLARITY_RANK_4 = {"VVS1", "VVS2"};
    const vector<string> CLARITY_RANK_3 = {"VS1", "VS2"};
    const vector<string> CLARITY_RANK_2 = {"SI1", "SI2"};
    const vector<string> CLARITY_RANK_1 = {"I1", "I2", "I3"};

    // Scores
    const map<string, int> COLOR_SCORE = {
        { "D", 100 }, { "E", 100 }, { "F", 100 },
        { "G", 80 }, { "H", 80 }, { "I", 80 }, { "J", 80 },
        { "K", 60 }, { "L", 60 }, { "M", 60 },
        { "N", 40 }, { "O", 40 }, { "P", 40 }, { "Q", 40 }, { "R", 40 },
        { "S", 20 }, { "T", 20 }, { "U", 20 }, { "V", 20 }, { "W", 20 }, { "X", 20 }, { "Y", 20 }, { "Z", 20 }
    };
    const map<string, int> CLARITY_SCORE = {
        { "FL", 100 }, { "IF", 100 },
        { "VVS1", 80 }, { "VVS2", 80 },
        { "VS1", 60 }, { "VS2", 60 },
        { "SI1", 40 }, { "SI2", 40 },
        { "I1", 20 }, { "I2", 20 }, { "I3", 20 }
    };

    int calculate_color( const string& color )
    {
        check( COLOR_SCORE.count(color), "pomelo::diamond::calculate_color: [color] invalid value");
        return COLOR_SCORE.at(color);
    }

    int calculate_clarity( const string& clarity )
    {
        check( CLARITY_SCORE.count(clarity), "pomelo::diamond::calculate_clarity: [clarity] invalid value");
        return CLARITY_SCORE.at(clarity);
    }

    string random_select( const vector<string>& values )
    {
        check( values.size(), "pomelo::diamond::random_select: [values] must contain at least one item");
        return values.at(gems::random::generate(1, 0, values.size() -1 )[0]);
    }

    int calculate_min_score( const int rank ) {
        return 40 + 60 * rank;
    }

    string select_color( const int color_score )
    {
        if ( color_score >= calculate_min_score( 5 ) ) return random_select( COLOR_RANK_5 );
        if ( color_score >= calculate_min_score( 4 ) ) return random_select( COLOR_RANK_4 );
        if ( color_score >= calculate_min_score( 3 ) ) return random_select( COLOR_RANK_3 );
        if ( color_score >= calculate_min_score( 2 ) ) return random_select( COLOR_RANK_2 );
        return random_select( COLOR_RANK_1 );
    }

    string select_clarity( const int clarity_score )
    {
        if ( clarity_score >= calculate_min_score( 5 ) ) return random_select( CLARITY_RANK_5 );
        if ( clarity_score >= calculate_min_score( 4 ) ) return random_select( CLARITY_RANK_4 );
        if ( clarity_score >= calculate_min_score( 3 ) ) return random_select( CLARITY_RANK_3 );
        if ( clarity_score >= calculate_min_score( 2 ) ) return random_select( CLARITY_RANK_2 );
        return random_select( CLARITY_RANK_1 );
    }

    string select_img( const ATTRIBUTE_MAP& immutable_data )
    {
        const string shape = atomic::attribute_to_string( immutable_data, "shape" );
        const int color_score = calculate_color( atomic::attribute_to_string( immutable_data, "color" ) );
        const int clarity_score = calculate_clarity( atomic::attribute_to_string( immutable_data, "clarity" ) );

        if ( shape == "Round") {
            switch ( color_score ) {
                case 100:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
                case 80:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
                case 60:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
                case 40:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
                case 20:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
            }
        }
        if ( shape == "Square" ) {
            switch ( color_score ) {
                case 100:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
                case 80:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
                case 60:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
                case 40:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
                case 20:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
            }
        }
        if ( shape == "Triangle" ) {
            switch ( color_score ) {
                case 100:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
                case 80:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
                case 60:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
                case 40:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
                case 20:
                    switch ( clarity_score ) {
                        case 100: return "IPFS";
                        case 80: return "IPFS";
                        case 60: return "IPFS";
                        case 40: return "IPFS";
                        case 20: return "IPFS";
                    }
            }
        }
        check( false, "pomelo::diamond::select_img: invalid value");
        return "";
    }

    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> diamond( const vector<atomicassets::assets_s>& in_assets )
    {
        // containers
        vector<string> shapes;
        int color_score = 0;
        int clarity_score = 0;

        // add shapes & sum scores
        for ( const auto& asset : in_assets ) {
            const ATTRIBUTE_MAP immutable = atomic::get_asset_immutable( asset );
            const string shape = atomic::attribute_to_string( immutable, "shape" );
            const string color = atomic::attribute_to_string( immutable, "color" );
            const string clarity = atomic::attribute_to_string( immutable, "clarity" );

            // accumulate all shapes (higher chance of selecting the more as input)
            shapes.push_back( shape );

            // color & clarity scores
            color_score += calculate_color( color );
            clarity_score += calculate_clarity( clarity );
        }

        // immutable
        ATTRIBUTE_MAP immutable_data = {};
        immutable_data["shape"] = random_select( shapes );
        immutable_data["color"] = select_color( color_score );
        immutable_data["clarity"] = select_clarity( clarity_score );
        immutable_data["img"] = select_img( immutable_data );

        return { immutable_data, {} };
    }
}
