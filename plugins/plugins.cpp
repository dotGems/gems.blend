// Utils
#include "potion.hpp"

// Pomelo Season 2
#include "pomelo-s2/main.hpp"
#include "pomelo-s2/potion_large.hpp"
#include "pomelo-s2/potion.hpp"

// Pomelo Season 3
#include "pomelo-s3/main.hpp"
#include "pomelo-s3/dev.hpp"
#include "pomelo-s3/potion.hpp"
#include "pomelo-s3/sets.hpp"

namespace gems {
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
            "sets.s3"_n
        };
        check( plugins.find( plugin ) != plugins.end(), "blend::check_plugin: invalid [plugin]");
    }

    void blend::validate_input_attributes( const name plugin, const atomic::nft id )
    {
        if ( plugin == "potion"_n || plugin == "potion.large"_n || plugin == "potion.s3"_n ) potion::validate_input_attributes( id );
        if ( plugin == "diamond"_n ) pomelo::s2::main::validate_input_attributes( id );
        if ( plugin == "moon"_n || plugin == "moon.dev"_n ) pomelo::s3::main::validate_input_attributes( id );
        if ( plugin == "sets.s3"_n ) pomelo::s3::sets::validate_input_attributes( id );
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
        // potion large is the same as Season 2

        check( false, "blend::mint_attributes: invalid [plugin]");
        return { };
    }
}