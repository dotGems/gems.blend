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
     * - `vector<uint32_t>` counters;   // 0 - total recipes blended
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
     * ### params
     *
     * - `{name} blend_id` - (primary key) blend ID (ex: `myblend`)
     * - `{set<name>} in_recipe_ids` - one or many input recipes ID's
     * - `{atomic::nft} out_template` - output AtomicAsset NFT template
     * - `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
     * - `{time_point_sec} [end_time=null]` - (optional) end time (ex: "2021-08-01T00:00:00")
     *
     * ### example
     *
     * ```json
     * {
     *     "blend_id": "myblend",
     *     "in_recipe_ids": ["myrecipe1", "myrecipe2"],
     *     "out_template": {"collection_name": "mycollection", "template_id": 21883},
     *     "start_time": "2021-07-01T00:00:00",
     *     "end_time": "2021-10-01T00:00:00"
     * }
     * ```
     */
    struct [[eosio::table("blends")]] blends_row {
        name                blend_id;
        set<name>           in_recipe_ids;
        atomic::nft         out_template;
        time_point_sec      start_time;
        time_point_sec      end_time;

        uint64_t primary_key() const { return blend_id.value; }
    };
    typedef eosio::multi_index< "blends"_n, blends_row> blends_table;

    /**
     * ## TABLE `recipes`
     *
     * ### params
     *
     * - `{name} recipe_id` - (primary key) recipe ID (ex: `myrecipe`)
     * - `{vector<atomic::nft>} templates` - AtomicAsset NFT templates
     *
     * ### example
     *
     * ```json
     * {
     *     "recipe_id": "myrecipe",
     *     "templates": [{"collection_name": "mycollection", "template_id": 21883}]
     * }
     * ```
     */
    struct [[eosio::table("recipes")]] recipes_row {
        name                    recipe_id;
        vector<atomic::nft>     templates;

        uint64_t primary_key() const { return recipe_id.value; }
    };
    typedef eosio::multi_index< "recipes"_n, recipes_row> recipes_table;

    /**
     * ## TABLE `ontransfer`
     *
     * - scope: `{name} owner`
     *
     * ### params
     *
     * - `{name} owner` - (primary key) account name
     * - `{vector<uint64_t>} asset_ids` - received AtomicHub NFT asset IDs
     *
     * ### example
     *
     * ```json
     * {
     *     "owner": "myaccount",
     *     "asset_ids": [1099511627776, 1099511627777, 1099511627778, 1099511627779]
     * }
     * ```
     */
    struct [[eosio::table("ontransfer")]] ontransfer_row {
        name                    owner;
        vector<uint64_t>        asset_ids;

        uint64_t primary_key() const { return owner.value; }
    };
    typedef eosio::multi_index< "ontransfer"_n, ontransfer_row> ontransfer_table;

    /**
     * ## ACTION `setblend`
     *
     * Set NFT blend
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{name} blend_id` - (primary key) blend ID (ex: `myblend`)
     * - `{set<name>} in_recipe_ids` - input recipes ID's
     * - `{atomic::nft} out_template` - output AtomicAsset NFT template
     * - `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
     * - `{time_point_sec} [end_time=null]` - (optional) end time (ex: "2021-08-01T00:00:00")
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems setblend '["myblend", ["myrecipe1", "myrecipe2"], ["mycollection", 789], "2021-11-01T00:00:00", "2021-12-01T00:00:00"]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void setblend( const name blend_id, const set<name> in_recipe_ids, const atomic::nft out_template, const optional<time_point_sec> start_time, const optional<time_point_sec> end_time );

    /**
     * ## ACTION `setrecipe`
     *
     * Set NFT recipe
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{name} blend_id` - blend blend ID (ex: `myblend`)
     * - `{vector<atomic::nft>} templates` - AtomicHub NFT templates
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems setrecipe '["myrecipe", [["mycollection", 123], ["mycollection", 456]]]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void setrecipe( const name recipe_id, const vector<atomic::nft> templates );

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
     * ## ACTION `delrecipe`
     *
     * Delete NFT recipe
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{name} recipe_id` - recipe ID (ex: `myrecipe`)
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems delrecipe '["myrecipe"]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void delrecipe( const name recipe_id );

    [[eosio::action]]
    void reset( const name table, const optional<name> scope  );

    [[eosio::action]]
    void blendlog( const name owner,
                   const name blend_id,
                   const name recipe_id,
                   const uint64_t total_mint,
                   const uint64_t total_burn,
                   const vector<uint64_t> in_asset_ids,
                   const uint64_t out_asset_id,
                   const vector<uint64_t> refund_asset_ids );

    /**
     * Notify contract when AtomicAssets NFT token transfer notifiers relay contract
     */
    [[eosio::on_notify("atomicassets::transfer")]]
    void on_nft_transfer( const name from, const name to, const vector<uint64_t> asset_ids, const std::string memo );

    // static actions
    using setblend_action = eosio::action_wrapper<"setblend"_n, &gems::blend::setblend>;
    using setrecipe_action = eosio::action_wrapper<"setrecipe"_n, &gems::blend::setrecipe>;
    using delblend_action = eosio::action_wrapper<"delblend"_n, &gems::blend::delblend>;
    using delrecipe_action = eosio::action_wrapper<"delrecipe"_n, &gems::blend::delrecipe>;
    using reset_action = eosio::action_wrapper<"reset"_n, &gems::blend::reset>;
    using blendlog_action = eosio::action_wrapper<"blendlog"_n, &gems::blend::blendlog>;

private:
    // eosio.token helper
    void transfer( const name from, const name to, const extended_asset quantity, const string memo );

    // blend
    void validate_templates( const vector<atomic::nft> templates, const bool burnable );
    void add_transfer( const name owner, const uint64_t asset_id );
    void attempt_to_blend( const name owner, const name blend_id );
    void check_time( const time_point_sec start_time, const time_point_sec end_time );

    // utils
    int get_index(const vector<name> vec, const name value);
    int get_index(const vector<int32_t> vec, const int32_t value);
    int get_index(const vector<uint64_t> vec, const uint64_t value);
    int get_index(const vector<atomic::nft> vec, const atomic::nft value);
    int get_index(const vector<asset> vec, const symbol sym);

    // update counters in status singleton
    void update_status( const uint32_t index, const uint32_t count );

    // maintenance
    template <typename T>
    void clear_table( T& table );
};
}
