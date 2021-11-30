#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {

    const map<string, int> COLOR_SCORE = {
        // colorless
        { "D", 100 },
        { "E", 100 },
        { "F", 100 },
        // near colorless
        { "G", 80 },
        { "H", 80 },
        { "I", 80 },
        { "J", 80 },
        // faint yellow
        { "K", 60 },
        { "L", 60 },
        { "M", 60 },
        // very light yellow
        { "N", 40 },
        { "O", 40 },
        { "P", 40 },
        { "R", 40 },
        // light yellow
        { "S", 20 },
        { "T", 20 },
        { "U", 20 },
        { "V", 20 },
        { "W", 20 },
        { "X", 20 },
        { "Y", 20 },
        { "Z", 20 }
    };

    const map<string, int> CLARITY_SCORE = {
        { "FL", 100 },
        { "IF", 100 },
        { "VVS1", 80 },
        { "VVS2", 80 },
        { "VS1", 60 },
        { "VS2", 60 },
        { "SI1", 40 },
        { "SI2", 40 },
        { "I1", 20 },
        { "I2", 20 },
        { "I3", 20 }
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

    string select_color( const int color_score )
    {
        if ( color_score >= 340 ) return random_select( {"D", "E", "F"});
        if ( color_score >= 280 ) return random_select( {"G", "H", "I", "J"});
        if ( color_score >= 220 ) return random_select( {"K", "L", "M"});
        if ( color_score >= 160 ) return random_select( {"N", "O", "P", "Q", "R"});
        return random_select({"S", "T", "U", "V", "W", "X", "Y", "Z"});
    }

    string select_clarity( const int clarity_score )
    {
        if ( clarity_score >= 340 ) return random_select( {"FL", "IF"});
        if ( clarity_score >= 280 ) return random_select( {"VVS1", "VVS2"});
        if ( clarity_score >= 220 ) return random_select( {"VS1", "VS2"});
        if ( clarity_score >= 160 ) return random_select( {"SI1", "SI2"});
        return random_select({"I1", "I2", "I3"});
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
            const ATTRIBUTE_MAP immutable = atomic::get_template_immutable( asset );
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
