#include "diamond.hpp"
#include "potion.hpp"
#include "potion.large.hpp"
#include "water.hpp"
#include "pomelo-s3.hpp"
#include "pomelo-s3-dev.hpp"

namespace gems {
    void blend::check_plugin( const name plugin )
    {
        const set<name> plugins = { "diamond"_n, "potion"_n, "potion.large"_n, "water"_n, "pomelo.s3"_n };
        check( plugins.find( plugin ) != plugins.end(), "blend::check_plugin: invalid [plugin]");
    }

    void blend::validate_attributes( const name plugin, const atomic::nft id )
    {
        if ( plugin == "potion"_n || plugin == "potion.large"_n ) pomelo::potion::validate_attributes( id );
        if ( plugin == "diamond"_n ) pomelo::diamond::validate_attributes( id );
        if ( plugin == "pomelo.s3"_n ) pomelo::s3::validate_attributes( id );
        if ( plugin == "d.pomelo.s3"_n ) pomelo::s3::validate_attributes( id );

        // test plugins
        if ( plugin == "water"_n ) test::water::validate_attributes( id );
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

        // supported plugins
        if ( plugin == "diamond"_n ) return pomelo::diamond::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        if ( plugin == "potion.large"_n ) return pomelo::potionLarge::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        if ( plugin == "potion"_n ) return pomelo::potion::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        if ( plugin == "pomelo.s3"_n ) return pomelo::s3::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        if ( plugin == "d.pomelo.s3"_n ) return pomelo::s3_dev::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );

        // test plugins
        if ( plugin == "water"_n ) return test::water::mint_attributes(owner, collection_name, template_id, in_asset_ids, in_assets );
        check( false, "blend::mint_attributes: invalid [plugin]");
        return { };
    }
}