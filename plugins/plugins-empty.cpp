namespace gems {
    void blend::check_plugin( const name plugin )
    {
        check( false, "blend::check_plugin: invalid [plugin]");
    }

    void blend::validate_attributes( const name plugin, const atomic::nft id )
    {

    }

    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> blend::mint_attributes( const name plugin, const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids )
    {
        return { };
    }
}