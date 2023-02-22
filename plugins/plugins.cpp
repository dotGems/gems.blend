// Utils
#include "potion.hpp"
#include <gems/atomic.gems.hpp>

// Pomelo Season 2
#include "pomelo-s2/main.hpp"
#include "pomelo-s2/potion_large.hpp"
#include "pomelo-s2/potion.hpp"

// Pomelo Season 3
#include "pomelo-s3/main.hpp"
#include "pomelo-s3/dev.hpp"
#include "pomelo-s3/potion.hpp"
#include "pomelo-s3/sets.hpp"

// Pomelo Season 4
#include "pomelo-s4/potion.hpp"
#include "pomelo-s4/main.hpp"
#include "pomelo-s4/sets.hpp"

// Pomelo Season 5
#include "pomelo-s5/potion.hpp"
#include "pomelo-s5/main.hpp"

namespace gems {
    void blend::extras( const name plugin, const name owner, const name collection_name, int32_t template_id  )
    {
        if ( plugin == "sets.s4"_n) {
            atomic::mintasset( get_self(), "pomelo"_n, "astronauts"_n, 6845, owner, {}, {}, {} );
        }
    }

    void blend::check_plugin( const name plugin )
    {
        const set<name> plugins = {
            // Pomelo Season 2
            "diamond"_n,
            "potion"_n,
            "potion.large"_n,

            // Pomelo Season 3
            "moon"_n,
            "moon.dev"_n,
            "potion.s3"_n,
            "sets.s3"_n,

            // Pomelo Season 4
            "potion.s4"_n,
            "robot"_n,
            "robot.dev"_n,
            "sets.s4"_n,

            // Pomelo Season 5
            "potion.s5"_n,
            "crew"_n,
            "crew.dev"_n,

            // Astronanut bonus mint
            "astronaut"_n,
        };
        check( plugins.find( plugin ) != plugins.end(), "blend::check_plugin: invalid [plugin]");
    }

    void blend::validate_input_attributes( const name plugin, const atomic::nft id )
    {
        if ( plugin == "potion"_n || plugin == "potion.large"_n || plugin == "potion.s3"_n || plugin == "potion.s4"_n || plugin == "potion.s5"_n ) potion::validate_input_attributes( id );
        if ( plugin == "diamond"_n ) pomelo::s2::main::validate_input_attributes( id );
        if ( plugin == "moon"_n || plugin == "moon.dev"_n ) pomelo::s3::main::validate_input_attributes( id );
        if ( plugin == "robot"_n || plugin == "robot.dev"_n ) pomelo::s4::main::validate_input_attributes( id );
        if ( plugin == "crew"_n || plugin == "crew.dev"_n ) pomelo::s5::main::validate_input_attributes( id );
        if ( plugin == "sets.s3"_n ) pomelo::s3::sets::validate_input_attributes( id );
        if ( plugin == "sets.s4"_n ) pomelo::s4::sets::validate_input_attributes( id );
    }

    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> blend::mint_attributes( const name plugin, const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids )
    {
        // no plugin provided
        if ( !plugin ) return {};

        // prepare assets
        vector<atomicassets::assets_s> in_assets;
        for ( const uint64_t asset_id : in_asset_ids ) {
            in_assets.push_back( atomic::get_asset( get_self(), asset_id ) );
        }

        // Pomelo Seaosn 2
        if ( plugin == "diamond"_n ) return pomelo::s2::main::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        if ( plugin == "potion.large"_n ) return pomelo::s2::potion_large::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        if ( plugin == "potion"_n ) return pomelo::s2::potion::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );

        // Pomelo Season 3
        if ( plugin == "moon"_n ) return pomelo::s3::main::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        if ( plugin == "moon.dev"_n ) return pomelo::s3::dev::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        if ( plugin == "potion.s3"_n ) return pomelo::s3::potion::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        if ( plugin == "sets.s3"_n ) return pomelo::s3::sets::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );

        // Pomelo Season 4
        if ( plugin == "robot"_n ) return pomelo::s4::main::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        if ( plugin == "potion.s4"_n ) return pomelo::s4::potion::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        if ( plugin == "sets.s4"_n ) return pomelo::s4::sets::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );

        // Pomelo Season 5
        if ( plugin == "crew"_n ) return pomelo::s5::main::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        if ( plugin == "potion.s5"_n ) return pomelo::s5::potion::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );

        check( false, "blend::mint_attributes: invalid [plugin]");
        return { };
    }
}