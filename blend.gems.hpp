#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>
#include <gems.atomic/atomic.gems.hpp>

#include <optional>
#include <string>

using namespace eosio;
using namespace std;

namespace gems {

class [[eosio::contract("blend.gems")]] blend : public eosio::contract {
public:
    using contract::contract;

    /**
     * ## TABLE `status`
     *
     * - `vector<uint32_t>` counters;   // 0 - total recipes blended, 1 - total recipes
     * - `time_point_sec` last_updated;
     *
     * ### example
     *
     * ```json
     * {
     *   "counters": [1234, 12],
     *   "last_updated": "2021-04-12T12:23:42"
     * }
     * ```
     */
    struct [[eosio::table("status")]] status_row {
        vector<uint32_t>        counters;
        time_point_sec          last_updated;
    };
    typedef eosio::singleton< "status"_n, status_row > status_table;

    /**
     * ## TABLE `blends`
     *
     * | `param`        | `index_position` | `key_type` |
     * |--------------- |------------------|------------|
     * | `bycollection` | 2                | i64        |
     *
     * ### params
     *
     * - `{atomic::nft} blend` - output AtomicAsset NFT template
     * - `{set<uint64_t>} recipe_ids` - one or many input recipes ID's
     * - `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
     * - `{time_point_sec} [end_time=null]` - (optional) end time (ex: "2021-08-01T00:00:00")
     *
     * ### example
     *
     * ```json
     * {
     *     "blend": {"collection_name": "mycollection", "template_id": 21883},
     *     "recipe_ids": [1, 2],
     *     "start_time": "2021-07-01T00:00:00",
     *     "end_time": "2021-10-01T00:00:00"
     * }
     * ```
     */
    struct [[eosio::table("blends")]] blends_row {
        atomic::nft         blend;
        set<uint64_t>       recipe_ids;
        time_point_sec      start_time;
        time_point_sec      end_time;

        uint64_t primary_key() const { return blend.template_id; }
        uint64_t by_collection() const { return blend.collection_name; }
    };
    typedef eosio::multi_index< "blends"_n, blends_row,
        indexed_by<"bycollection"_n, const_mem_fun<blends_row, uint64_t, &blends_row::by_collection>>
    > blends_table;

    /**
     * ## TABLE `recipes`
     *
     * ### params
     *
     * - `{uint64_t} recipe_id` - (auto-incremental primary key) recipe ID
     * - `{vector<atomic::nft>} templates` - AtomicAsset NFT templates
     *
     * ### example
     *
     * ```json
     * {
     *     "recipe_id": 1,
     *     "templates": [{"collection_name": "mycollection", "template_id": 21883}]
     * }
     * ```
     */
    struct [[eosio::table("recipes")]] recipes_row {
        uint64_t                recipe_id;
        vector<atomic::nft>     templates;

        uint64_t primary_key() const { return recipe_id; }
    };
    typedef eosio::multi_index< "recipes"_n, recipes_row> recipes_table;

    /**
     * ## ACTION `setblend`
     *
     * Set NFT blend
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{atomic::nft} blend` - blend AtomicAsset NFT template
     * - `{set<uint64_t>} recipe_ids` - input recipes ID's
     * - `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
     * - `{time_point_sec} [end_time=null]` - (optional) end time (ex: "2021-08-01T00:00:00")
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems setblend '[["mycollection", 789], [1, 2], "2021-11-01T00:00:00", "2021-12-01T00:00:00"]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void setblend( const atomic::nft blend, const set<uint64_t> recipe_ids, const optional<time_point_sec> start_time, const optional<time_point_sec> end_time );

    /**
     * ## ACTION `initrecipe`
     *
     * Initialize NFT recipe
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{vector<atomic::nft>} templates` - AtomicHub NFT templates
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems initrecipe '[[["mycollection", 123], ["mycollection", 456]]]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void initrecipe( const vector<atomic::nft> templates );

    /**
     * ## ACTION `delblend`
     *
     * Delete NFT blend
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{atomic::nft} blend` - blend AtomicAsset NFT
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems delblend '[["mycollection", 789]]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void delblend( const atomic::nft blend );

    /**
     * ## ACTION `delrecipe`
     *
     * Delete NFT recipe
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{uint64_t} recipe_id` - recipe ID
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems delrecipe '[1]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void delrecipe( const uint64_t recipe_id );

    [[eosio::action]]
    void reset( const name table, const optional<name> scope  );

    [[eosio::action]]
    void blendlog( const name owner,
                   const vector<uint64_t> in_asset_ids,
                   const uint64_t out_asset_id
                   const vector<atomic::nft> in_templates,
                   const atomic::nft out_template,
                   const int total_mint,
                   const int total_burn );

    /**
     * Notify contract when AtomicAssets NFT token transfer notifiers relay contract
     */
    [[eosio::on_notify("atomicassets::transfer")]]
    void on_nft_transfer( const name from, const name to, const vector<uint64_t> asset_ids, const std::string memo );

    // static actions
    using setblend_action = eosio::action_wrapper<"setblend"_n, &gems::blend::setblend>;
    using initrecipe_action = eosio::action_wrapper<"initrecipe"_n, &gems::blend::initrecipe>;
    using delblend_action = eosio::action_wrapper<"delblend"_n, &gems::blend::delblend>;
    using delrecipe_action = eosio::action_wrapper<"delrecipe"_n, &gems::blend::delrecipe>;
    using reset_action = eosio::action_wrapper<"reset"_n, &gems::blend::reset>;
    using blendlog_action = eosio::action_wrapper<"blendlog"_n, &gems::blend::blendlog>;

private:
    // eosio.token helper
    void transfer( const name from, const name to, const extended_asset quantity, const string memo );

    // blend
    void validate_template( const atomic::nft template, const bool burnable );
    void validate_templates( const vector<atomic::nft> templates, const bool burnable );
    void attempt_to_blend( const name owner, const name blend_id, const vector<uint64_t>& asset_ids, const vector<atomic::nft>& received_nfts );
    void check_time( const time_point_sec start_time, const time_point_sec end_time );
    name detect_recipe( const set<name> recipe_ids, vector<atomic::nft> received_templates );

    // update counters in status singleton
    void update_status( const uint32_t index, const uint32_t count );

    // maintenance
    template <typename T>
    void clear_table( T& table );
};
}
