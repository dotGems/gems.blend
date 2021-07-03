#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>

#include <optional>
#include <string>

using namespace eosio;
using namespace std;

static constexpr symbol WAX{"WAX", 8};
static constexpr symbol EOS{"EOS", 4};
static constexpr uint32_t DAY = 86400;

namespace gems {

class [[eosio::contract("blend.gems")]] blend : public eosio::contract {
public:
    using contract::contract;

    /**
     * ## TABLE `blends`
     *
     * ### multi-indexes
     *
     * | `param`        | `index_position` | `key_type` |
     * |--------------- |------------------|------------|
     * | `bycollection` | 2                | i64        |
     * | `byupdated`    | 2                | i64        |
     *
     * ### params
     *
     * - `{name} blend_id` - (primary key) blend ID (ex: `myblend`)
     * - `{name} collection_name` - AtomicHub Collection Name (ex: `mycollection`)
     * - `{vector<int32_t>} in_template_ids` - input AtomicHub NFT template ID (ex: [`21881`, `21882`])
     * - `{vector<int32_t>} out_template_id` - output AtomicHub NFT template ID (ex: [`21883`])
     * - `{asset} backed_tokens` - backed token (ex: `"1.00000000 WAX"`)
     * - `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
     * - `{time_point_sec} last_updated` - last updated time (ex: "2021-07-01T00:00:00")
     * - `{uint64_t} counter_blend` - total counter blend
     *
     * ### example
     *
     * ```json
     * {
     *     "blend_id": "myblend",
     *     "collection_name": "mycollection",
     *     "in_template_ids": [21881, 21882],
     *     "out_template_ids": [21883],
     *     "backed_tokens": "1.00000000 WAX",
     *     "start_time": "2021-07-01T00:00:00",
     *     "last_updated": "2021-07-01T00:00:00",
     *     "total_mint": 0,
     *     "total_burn": 0,
     *     "total_backed_tokens": "0.00000000 WAX"
     * }
     * ```
     */
    struct [[eosio::table("blends")]] blends_row {
        name                blend_id;
        name                collection_name;
        vector<int32_t>     in_template_ids;
        vector<int32_t>     out_template_ids;
        asset               backed_tokens;
        time_point_sec      start_time;
        time_point_sec      last_updated;
        uint64_t            total_mint;
        uint64_t            total_burn;
        asset               total_backed_tokens;

        uint64_t primary_key() const { return blend_id.value; }
        uint64_t by_collection() const { return collection_name.value; }
        uint64_t by_updated() const { return last_updated.sec_since_epoch(); }
    };
    typedef eosio::multi_index< "blends"_n, blends_row,
        indexed_by<"bycollection"_n, const_mem_fun<blends_row, uint64_t, &blends_row::by_collection>>,
        indexed_by<"byupdated"_n, const_mem_fun<blends_row, uint64_t, &blends_row::by_updated>>
    > blends_table;

    /**
     * ## TABLE `templates`
     *
     * ### multi-indexes
     *
     * | `param`        | `index_position` | `key_type` |
     * |--------------- |------------------|------------|
     * | `bycollection` | 2                | i64        |
     * | `byblend`     | 2                | i64        |
     *
     * ### params
     *
     * - `{int32_t} template_id` - (primary key) AtomicHub NFT template ID (ex: `21881`)
     * - `{name} collection_name` - AtomicHub Collection Name (ex: `mycollection`)
     * - `{name} schema_name` - AtomicHub Schema Name (ex: `myschema`)
     * - `{name} blend_id` - blend ID (ex: `myblend`)
     *
     * ### example
     *
     * ```json
     * {
     *     "template_id": 21881,
     *     "collection_name": "mycollection",
     *     "schema_name": "myschema",
     *     "blend_id": "myblend"
     * }
     * ```
     */
    struct [[eosio::table("templates")]] templates_row {
        int32_t             template_id;
        name                collection_name;
        name                schema_name;
        name                blend_id;

        uint64_t primary_key() const { return template_id; }
        uint64_t by_collection() const { return collection_name.value; }
        uint64_t by_blend_id() const { return blend_id.value; }
    };
    typedef eosio::multi_index< "templates"_n, templates_row,
        indexed_by<"bycollection"_n, const_mem_fun<templates_row, uint64_t, &templates_row::by_collection>>,
        indexed_by<"byblend"_n, const_mem_fun<templates_row, uint64_t, &templates_row::by_blend_id>>
    > templates_table;

    /**
     * ## TABLE `ontransfer`
     *
     * ### multi-indexes
     *
     * | `param`        | `index_position` | `key_type` |
     * |--------------- |------------------|------------|
     * | `bycollection` | 2                | i64        |
     * | `byblend`      | 3                | i64        |
     * | `byupdated`    | 4                | i64        |
     *
     * ### params
     *
     * - `{name} owner` - (primary key) account name
     * - `{name} collection_name` - AtomicHub Collection Name (ex: `mycollection`)
     * - `{name} blend_id` - blend ID (ex: `myblend`)
     * - `{vector<uint64_t>} asset_ids` - received AtomicHub NFT asset IDs
     * - `{vector<int32_t>} template_ids` - received AtomicHub NFT template ID (ex: [`21881`, `21882`])
     * - `{time_point_sec} last_updated` - last updated time (ex: "2021-07-01T00:00:00")
     *
     * ### example
     *
     * ```json
     * {
     *     "owner": "myaccount",
     *     "collection_name": "mycollection",
     *     "blend_id": "myblend",
     *     "asset_ids": [1099512167124, 1099512167125],
     *     "template_ids": [21881, 21882],
     *     "last_updated": "2021-07-01T00:00:00"
     * }
     * ```
     */
    struct [[eosio::table("ontransfer")]] ontransfer_row {
        name                owner;
        name                collection_name;
        name                blend_id;
        vector<uint64_t>    asset_ids;
        vector<int32_t>     template_ids;
        time_point_sec      last_updated;

        uint64_t primary_key() const { return owner.value; }
        uint64_t by_collection() const { return collection_name.value; }
        uint64_t by_blend() const { return blend_id.value; }
        uint64_t by_updated() const { return last_updated.sec_since_epoch(); }
    };
    typedef eosio::multi_index< "ontransfer"_n, ontransfer_row,
        indexed_by<"bycollection"_n, const_mem_fun<ontransfer_row, uint64_t, &ontransfer_row::by_collection>>,
        indexed_by<"byblend"_n, const_mem_fun<ontransfer_row, uint64_t, &ontransfer_row::by_blend>>,
        indexed_by<"byupdated"_n, const_mem_fun<ontransfer_row, uint64_t, &ontransfer_row::by_updated>>
    > ontransfer_table;

    /**
     * ## TABLE `global`
     *
     * ### params
     *
     * - `{uint64_t} total_mint` - total mint
     * - `{uint64_t} total_burn` - total burn
     * - `{asset} total_backed_tokens` - total backed token
     *
     * ### example
     *
     * ```json
     * {
     *     "total_mint": 0,
     *     "total_burn": 0,
     *     "total_backed_tokens": "0.00000000 WAX"
     * }
     * ```
     */
    struct [[eosio::table("global")]] global_row {
        uint64_t            total_mint;
        uint64_t            total_burn;
        asset               total_backed_tokens;
    };
    typedef eosio::singleton< "global"_n, global_row> global_table;

    /**
     * ## ACTION `setblend`
     *
     * Set NFT blend
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{name} blend_id` - blend blend ID (ex: `myblend`)
     * - `{name} collection_name` - AtomicHub Collection Name (ex: `mycollection`)
     * - `{vector<int32_t>} in_template_ids` - input AtomicHub NFT template ID (ex: [`21881`, `21882`])
     * - `{vector<int32_t>} out_template_ids` - output AtomicHub NFT template ID (ex: [`21883`])
     * - `{asset} [backed_tokens=null]` - (optional) tokens to back (ex: `"1.00000000 WAX"`)
     * - `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems setblend '["myblend", "mycollection", [123, 456], [789], ["1.00000000 WAX"], "2021-07-02T00:00:00"]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void setblend( const name blend_id, const name collection_name, const vector<int32_t> in_template_ids, const vector<int32_t> out_template_ids, const asset backed_tokens, const optional<time_point_sec> start_time );

    /**
     * ## ACTION `delblend`
     *
     * Delete NFT blend
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{name} blend_id` - blend blend ID (ex: `myblend`)
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems delblend '["myblend"]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void delblend( const name blend_id );

    /**
     * ## ACTION `refund`
     *
     * Refund account
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{name} owner` - account name to refund
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems refund '["myaccount"]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void refund( const name owner );

    [[eosio::action]]
    void reset( const name table );

    [[eosio::action]]
    void blendlog( const name owner,
                   const name collection_name,
                   const name blend_id,
                   const uint64_t total_mint,
                   const uint64_t total_burn,
                   const asset total_backed_tokens,
                   const vector<uint64_t> in_asset_ids,
                   const vector<int32_t> blend_template_ids,
                   const vector<uint64_t> refund_asset_ids );

    /**
     * Notify contract when AtomicAssets NFT token transfer notifiers relay contract
     */
    [[eosio::on_notify("atomicassets::transfer")]]
    void on_nft_transfer( const name from, const name to, const vector<uint64_t> asset_ids, const std::string memo );

    // [[eosio::on_notify("atomicassets::logmint")]]
    // void on_logmint( uint64_t asset_id,
    //                  name authorized_minter,
    //                  name collection_name,
    //                  name schema_name,
    //                  int32_t template_id,
    //                  name new_asset_owner,
    //                  ATTRIBUTE_MAP immutable_data,
    //                  ATTRIBUTE_MAP mutable_data,
    //                  vector <asset> backed_tokens,
    //                  ATTRIBUTE_MAP immutable_template_data );

    // static actions
    using setblend_action = eosio::action_wrapper<"setblend"_n, &gems::blend::setblend>;
    using delblend_action = eosio::action_wrapper<"delblend"_n, &gems::blend::delblend>;
    using refund_action = eosio::action_wrapper<"refund"_n, &gems::blend::refund>;
    using reset_action = eosio::action_wrapper<"reset"_n, &gems::blend::reset>;
    using blendlog_action = eosio::action_wrapper<"blendlog"_n, &gems::blend::blendlog>;

private:
    // eosio.token helper
    void transfer( const name from, const name to, const extended_asset quantity, const string memo );

    // blend
    void validate_template_ids( const name collection_name, const vector<int32_t> template_ids, const bool burnable );
    void add_transfer( const name owner, const uint64_t asset_id );
    void attempt_to_blend( const name owner );
    void add_template( const name collection_name, const int32_t template_id, const name blend_id );
    void erase_blend_templates( const name blend_id );
    void check_templates_already_exists( const name blend_id, const vector<int32_t> template_ids );

    // AtomicAssets action helper
    void transfer_nft( const name from, const name to, const vector<uint64_t> asset_ids, const string memo );
    void burnasset( const name asset_owner, const uint64_t asset_id );
    void mintasset( const name authorized_minter, const name collection_name, const name schema_name, const int32_t template_id, const name new_asset_owner, const ATTRIBUTE_MAP immutable_data, const ATTRIBUTE_MAP mutable_data, const vector <asset> backed_token );
    void announce_deposit( const name owner, const symbol symbol_to_announce );

    // utils
    int get_index(const vector<name> vec, const name value);
    int get_index(const vector<int32_t> vec, const int32_t value);
    int get_index(const vector<uint64_t> vec, const uint64_t value);

    // AtomicAssets lookup helpers
    atomicassets::collections_s get_collection( const name collection_name );
    atomicassets::templates_s get_template( const name collection_name, const int32_t template_id );
    atomicassets::assets_s get_assets( const name owner, const uint64_t asset_id );

    // maintenance
    template <typename T>
    void clear_table( T& table );
};
}
