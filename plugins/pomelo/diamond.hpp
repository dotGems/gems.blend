#include <gems/atomic.gems.hpp>
#include <gems/random.gems.hpp>

namespace pomelo {

    string random_select( const vector<string> values )
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

    int calculate_color( const name color )
    {
        switch ( color.value ) {
            // colorless
            case "d"_n.value:
            case "e"_n.value:
            case "f"_n.value:
                return 100;
            // near colorless
            case "g"_n.value:
            case "h"_n.value:
            case "i"_n.value:
            case "j"_n.value:
                return 80;
            // faint yellow
            case "k"_n.value:
            case "l"_n.value:
            case "m"_n.value:
                return 60;
            // very light yellow
            case "n"_n.value:
            case "o"_n.value:
            case "p"_n.value:
            case "q"_n.value:
            case "r"_n.value:
                return 40;
            // light yellow
            case "s"_n.value:
            case "t"_n.value:
            case "u"_n.value:
            case "v"_n.value:
            case "w"_n.value:
            case "x"_n.value:
            case "y"_n.value:
            case "z"_n.value:
                return 20;
        }
        check( false, "pomelo::diamond::calculate_color: [color] invalid value");
        return 0;
    }
    int calculate_clarity( const name clarity )
    {
        switch ( clarity.value ) {
            // Internally Flawless
            case "fl"_n.value:
            case "if"_n.value:
                return 100;

            // Very Very Slight Inclusions
            case "vvs1"_n.value:
            case "vvs2"_n.value:
                return 80;

            // Very Slight Inclusions
            case "vs1"_n.value:
            case "vs2"_n.value:
                return 60;

            // Slight Inclusions
            case "si1"_n.value:
            case "si2"_n.value:
                return 40;

            // Imperfect
            case "i1"_n.value:
            case "i2"_n.value:
            case "i3"_n.value:
                return 20;
        }
        check( false, "pomelo::diamond::calculate_clarity: [clarity] invalid value");
        return 0;
    }

    string select_img( const ATTRIBUTE_MAP& immutable_data )
    {
        const name shape = atomic::attribute_to_name( immutable_data, "shape" );
        const int color_score = calculate_color( atomic::attribute_to_name( immutable_data, "color" ) );
        const int clarity_score = calculate_clarity( atomic::attribute_to_name( immutable_data, "clarity" ) );

        switch ( shape.value ) {
            case "round"_n.value:
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
            case "square"_n.value:
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

            case "triangle"_n.value:
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
        for ( const auto asset : in_assets ) {
            const ATTRIBUTE_MAP immutable = atomic::get_template_immutable( asset );
            const string shape = atomic::attribute_to_string( immutable, "shape" );
            const name color = atomic::attribute_to_name( immutable, "color" );
            const name clarity = atomic::attribute_to_name( immutable, "clarity" );

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
