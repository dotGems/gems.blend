#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>

#include <optional>
#include <string>

using namespace eosio;
using namespace std;

static constexpr symbol WAX{"WAX", 8};
static constexpr symbol EOS{"EOS", 8};
static constexpr uint32_t DAY = 86400;

namespace gems {

class [[eosio::contract("blend.gems")]] blend : public eosio::contract {
public:
    using contract::contract;

    /**
     * ## TABLE `recipes`
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
     * - `{name} recipe_id` - (primary key) recipe ID (ex: `myrecipe`)
     * - `{name} collection_name` - AtomicHub Collection Name (ex: `mycollection`)
     * - `{set<int32_t>} template_ids` - AtomicHub NFT template ID (ex: [`21881`, `21882`])
     * - `{int32_t} blend_template_id` - AtomicHub NFT template ID (ex: `21883`)
     * - `{asset} [backed_token=null]` - (optional) backed token (ex: `"1.00000000 WAX"`)
     * - `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
     * - `{time_point_sec} last_updated` - last updated time (ex: "2021-07-01T00:00:00")
     * - `{uint64_t} counter_blend` - total counter blend
     *
     * ### example
     *
     * ```json
     * {
     *     "recipe_id": "myrecipe",
     *     "collection_name": "mycollection",
     *     "in_template_ids": [21881, 21882],
     *     "out_template_ids": [21883],
     *     "backed_token": "1.00000000 WAX",
     *     "start_time": "2021-07-01T00:00:00",
     *     "last_updated": "2021-07-01T00:00:00",
     *     "counter_blend": 0
     * }
     * ```
     */
    struct [[eosio::table("recipes")]] recipes_row {
        name                recipe_id;
        name                collection_name;
        vector<int32_t>     in_template_ids;
        vector<int32_t>     out_template_ids;
        asset               backed_token;
        time_point_sec      start_time;
        time_point_sec      last_updated;
        uint64_t            counter_blend;

        uint64_t primary_key() const { return recipe_id.value; }
        uint64_t by_collection() const { return collection_name.value; }
        uint64_t by_updated() const { return last_updated.sec_since_epoch(); }
    };
    typedef eosio::multi_index< "recipes"_n, recipes_row,
        indexed_by<"bycollection"_n, const_mem_fun<recipes_row, uint64_t, &recipes_row::by_collection>>,
        indexed_by<"byupdated"_n, const_mem_fun<recipes_row, uint64_t, &recipes_row::by_updated>>
    > recipes_table;

    /**
     * ## TABLE `templates`
     *
     * ### multi-indexes
     *
     * | `param`        | `index_position` | `key_type` |
     * |--------------- |------------------|------------|
     * | `bycollection` | 2                | i64        |
     * | `byrecipe`     | 2                | i64        |
     *
     * ### params
     *
     * - `{int32_t} template_id` - (primary key) AtomicHub NFT template ID (ex: `21881`)
     * - `{name} collection_name` - AtomicHub Collection Name (ex: `mycollection`)
     * - `{name} schema_name` - AtomicHub Schema Name (ex: `myschema`)
     * - `{name} recipe_id` - recipe ID (ex: `myrecipe`)
     *
     * ### example
     *
     * ```json
     * {
     *     "template_id": 21881,
     *     "collection_name": "mycollection",
     *     "schema_name": "myschema",
     *     "recipe_id": "myrecipe"
     * }
     * ```
     */
    struct [[eosio::table("templates")]] templates_row {
        int32_t             template_id;
        name                collection_name;
        name                schema_name;
        name                recipe_id;

        uint64_t primary_key() const { return template_id; }
        uint64_t by_collection() const { return collection_name.value; }
        uint64_t by_recipe_id() const { return recipe_id.value; }
    };
    typedef eosio::multi_index< "templates"_n, templates_row,
        indexed_by<"bycollection"_n, const_mem_fun<templates_row, uint64_t, &templates_row::by_collection>>,
        indexed_by<"byrecipe"_n, const_mem_fun<templates_row, uint64_t, &templates_row::by_recipe_id>>
    > templates_table;

    /**
     * ## TABLE `ontransfer`
     *
     * ### multi-indexes
     *
     * | `param`        | `index_position` | `key_type` |
     * |--------------- |------------------|------------|
     * | `bycollection` | 2                | i64        |
     * | `byrecipe`     | 3                | i64        |
     * | `byupdated`    | 4                | i64        |
     *
     * ### params
     *
     * - `{name} owner` - (primary key) account name
     * - `{name} collection_name` - AtomicHub Collection Name (ex: `mycollection`)
     * - `{name} recipe_id` - recipe ID (ex: `myrecipe`)
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
     *     "recipe_id": "myrecipe",
     *     "asset_ids": [1099512167124, 1099512167125],
     *     "template_ids": [21881, 21882],
     *     "last_updated": "2021-07-01T00:00:00"
     * }
     * ```
     */
    struct [[eosio::table("ontransfer")]] ontransfer_row {
        name                owner;
        name                collection_name;
        name                recipe_id;
        vector<uint64_t>    asset_ids;
        vector<int32_t>     template_ids;
        time_point_sec      last_updated;

        uint64_t primary_key() const { return owner.value; }
        uint64_t by_collection() const { return collection_name.value; }
        uint64_t by_recipe() const { return recipe_id.value; }
        uint64_t by_updated() const { return last_updated.sec_since_epoch(); }
    };
    typedef eosio::multi_index< "ontransfer"_n, ontransfer_row,
        indexed_by<"bycollection"_n, const_mem_fun<ontransfer_row, uint64_t, &ontransfer_row::by_collection>>,
        indexed_by<"byrecipe"_n, const_mem_fun<ontransfer_row, uint64_t, &ontransfer_row::by_recipe>>,
        indexed_by<"byupdated"_n, const_mem_fun<ontransfer_row, uint64_t, &ontransfer_row::by_updated>>
    > ontransfer_table;

    /**
     * ## TABLE `global`
     *
     * ### params
     *
     * - `{uint64_t} counter_blends` - counter blends
     *
     * ### example
     *
     * ```json
     * {
     *     "counter_blend": 1
     * }
     * ```
     */
    struct [[eosio::table("global")]] global_row {
        uint64_t                    counter_blend;
    };
    typedef eosio::singleton< "global"_n, global_row> global_table;

    /**
     * ## ACTION `setrecipe`
     *
     * Set NFT blend recipe
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{name} recipe_id` - blend recipe ID (ex: `myrecipe`)
     * - `{name} collection_name` - AtomicHub Collection Name (ex: `mycollection`)
     * - `{vector<int32_t>} in_template_ids` - input AtomicHub NFT template ID (ex: [`21881`, `21882`])
     * - `{vector<int32_t>} out_template_ids` - output AtomicHub NFT template ID (ex: [`21883`])
     * - `{asset} [backed_token=null]` - (optional) backed token (ex: `"1.00000000 WAX"`)
     * - `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems setrecipe '["myrecipe", "mycollection", [123, 456], [789], "1.00000000 WAX", "2021-07-02T00:00:00"]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void setrecipe( const name recipe_id, const name collection_name, const vector<int32_t> in_template_ids, const vector<int32_t> out_template_ids, const asset backed_token, const time_point_sec start_time );

    /**
     * ## ACTION `delrecipe`
     *
     * Delete NFT blend recipe
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{name} recipe_id` - blend recipe ID (ex: `myrecipe`)
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems delrecipe '["myrecipe"]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void delrecipe( const name recipe_id );

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
                   const name recipe_id,
                   const vector<uint64_t> in_asset_ids,
                   const vector<uint64_t> out_asset_ids,
                   const vector<int32_t> in_template_ids,
                   const vector<int32_t> out_template_id );

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
    using setrecipe_action = eosio::action_wrapper<"setrecipe"_n, &gems::blend::setrecipe>;
    using delrecipe_action = eosio::action_wrapper<"delrecipe"_n, &gems::blend::delrecipe>;
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
    void add_template( const name collection_name, const int32_t template_id, const name recipe_id );

    // AtomicAssets action helper
    void transfer_nft( const name from, const name to, const vector<uint64_t> asset_ids, const string memo );
    void burnasset( const name asset_owner, const uint64_t asset_id );
    void mintasset( const name authorized_minter, const name collection_name, const name schema_name, const int32_t template_id, const name new_asset_owner, const ATTRIBUTE_MAP immutable_data, const ATTRIBUTE_MAP mutable_data, const vector <asset> tokens_to_back );

    // AtomicAssets lookup helpers
    atomicassets::collections_s get_collection( const name collection_name );
    atomicassets::templates_s get_template( const name collection_name, const int32_t template_id );
    atomicassets::assets_s get_assets( const name owner, const uint64_t asset_id );

    // maintenance
    template <typename T>
    void clear_table( T& table );
};
}
