#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>

#include <optional>
#include <string>

using namespace eosio;
using namespace std;

namespace gems {

class [[eosio::contract("blend.gems")]] blend : public eosio::contract {
public:
    using contract::contract;

    /**
     * ## TABLE `config`
     *
     * - `{name} status` - contract status ("ok", "testing", "maintenance")
     * - `{uint16_t} protocol_fee` - trading fee (pips 1/100 of 1%)
     * - `{name} fee_account` - protocol fee are transfered to account
     *
     * ### example
     *
     * ```json
     * {
     *   "status": "ok",
     *   "protocol_fee": 2000,
     *   "fee_account": "fee.gems"
     * }
     * ```
     */
    struct [[eosio::table("config")]] config_row {
        name                status = "testing"_n;
        uint16_t            protocol_fee = 2000;
        name                fee_account = "fee.gems"_n;
    };
    typedef eosio::singleton< "config"_n, config_row > config_table;

    /**
     * ## TABLE `status`
     *
     * ## params
     *
     * - `{vector<uint32_t>} counters` - counters
     *   - `{uint32_t} counters[0]` - total mint
     *   - `{uint32_t} counters[1]` - total burn
     *   - `{uint32_t} counters[2]` - total deposits
     * - `{time_point_sec} last_updated` - last updated
     *
     * ### example
     *
     * ```json
     * {
     *     "status": "ok"
     *     "counters": [10, 30],
     *     "last_updated": "2021-04-12T12:23:42"
     * }
     * ```
     */
    struct [[eosio::table("status")]] status_row {
        vector<uint32_t>        counters;
        time_point_sec          last_updated;
    };
    typedef eosio::singleton< "status"_n, status_row > status_table;

    /**
     * ## TABLE `collections`
     *
     * ## params
     *
     * - `{set<name>} collection_names` - collection names
     *
     * ### example
     *
     * ```json
     * {
     *     "collection_names": ["mycollection"]
     * }
     * ```
     */
    struct [[eosio::table("collections")]] collections_row {
        set<name>     collection_names;
    };
    typedef eosio::singleton< "collections"_n, collections_row > collections_table;

    /**
     * ## TABLE `blends`
     *
     * - scope: `{name} collection_name`
     *
     * ### params
     *
     * - `{atomic::nft_extra} id` - output AtomicAsset NFT template
     * - `{set<uint64_t>} recipe_ids` - one or many input recipes ID's
     * - `{string} [description=null]]` - (optional) blend description
     * - `{name} [plugin=null]` - (optional) plugin (custom attributes)
     * - `{extended_asset} [quantity=[]]` - (optional) token deposit required
     * - `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
     * - `{time_point_sec} [end_time=null]` - (optional) end time (ex: "2021-08-01T00:00:00")
     *
     * ### example
     *
     * ```json
     * {
     *     "id": {"collection_name": "mycollection", "schema_name": "myschema", "template_id": 21883},
     *     "recipe_ids": [1, 2],
     *     "description": "My Blend",
     *     "plugin": "myplugin",
     *     "quantity": {"contract": "eosio.token", "quantity": "1.0000 EOS"},
     *     "start_time": "2021-07-01T00:00:00",
     *     "end_time": "2021-10-01T00:00:00"
     * }
     * ```
     */
    struct [[eosio::table("blends")]] blends_row {
        atomic::nft_extra           id;
        set<uint64_t>               recipe_ids;
        optional<string>            description;
        optional<name>              plugin;
        optional<extended_asset>    quantity;
        optional<time_point_sec>    start_time;
        optional<time_point_sec>    end_time;

        uint64_t primary_key() const { return id.template_id; }
    };
    typedef eosio::multi_index< "blends"_n, blends_row> blends_table;

    /**
     * ## TABLE `recipes`
     *
     * - scope: `{name} collection_name`
     *
     * ### params
     *
     * - `{uint64_t} id` - (auto-incremental primary key) recipe ID
     * - `{vector<atomic::nft_extra>} templates` - AtomicAsset NFT templates
     *
     * ### example
     *
     * ```json
     * {
     *     "id": 1,
     *     "templates": [{"collection_name": "mycollection", "schema_name": "myschema", "template_id": 21883}]
     * }
     * ```
     */
    struct [[eosio::table("recipes")]] recipes_row {
        uint64_t                        id;
        vector<atomic::nft_extra>       templates;

        uint64_t primary_key() const { return id; }
    };
    typedef eosio::multi_index< "recipes"_n, recipes_row> recipes_table;

    /**
     * ## TABLE `orders`
     *
     * *scope*: `owner` (name)
     *
     * - `{atomic::nft} id` - output AtomicAsset NFT template
     * - `{extended_asset} quantity` - quantity asset
     *
     * ### example
     *
     * ```json
     * {
     *   "id": {"collection_name": "mycollection", "schema_name": "myschema", "template_id": 21883},
     *   "quantity": {"contract": "eosio.token", "quantity": "1.0000 EOS"}
     * }
     * ```
     */
    struct [[eosio::table("orders")]] orders_row {
        atomic::nft             id;
        extended_asset          quantity;

        uint64_t primary_key() const { return id.template_id; }
    };
    typedef eosio::multi_index< "orders"_n, orders_row> orders_table;

    /**
     * ## ACTION `setblend`
     *
     * Set NFT blend
     *
     * - **authority**: `atomicassets::author` or `get_self()`
     *
     * ### params
     *
     * - `{name} collection_name` - AtomicAsset NFT collection name
     * - `{int32_t} template_id` - AtomicAsset NFT template ID
     * - `{string} [description=""]` - (optional) blend description
     * - `{name} [plugin=""]` - (optional) plugin (custom attributes)
     * - `{extended_asset} [quantity=null]` - (optional) token deposit required
     * - `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
     * - `{time_point_sec} [end_time=null]` - (optional) end time (ex: "2021-08-01T00:00:00")
     *
     * ### Example
     *
     * ```bash
     * # basic
     * $ cleos push action blend.gems setblend '["mycollection", 789, "My Blend", null, null, null, null]' -p myaccount
     *
     * # advanced
     * $ cleos push action blend.gems setblend '["mycollection", 789, "My Blend", "myplugin, {"contract": "eosio.token", "quantity": "0.1000 EOS"}, "2021-11-01T00:00:00", "2021-12-01T00:00:00"]' -p myaccount
     * ```
     */
    [[eosio::action]]
    void setblend( const name collection_name, const int32_t template_id, const optional<string> description, const optional<name> plugin, const optional<extended_asset> quantity, const optional<time_point_sec> start_time, const optional<time_point_sec> end_time );

    /**
     * ## ACTION `addrecipe`
     *
     * Add NFT recipe to blend
     *
     * - **authority**: `atomicassets::author` or `get_self()`
     *
     * ### params
     *
     * - `{name} collection_name` - AtomicAsset NFT collection name
     * - `{int32_t} template_id` - AtomicAsset NFT template ID
     * - `{vector<atomic::nft>} templates` - AtomicHub NFT templates
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems addrecipe '["mycollection", 789, [["mycollection", 123], ["mycollection", 456]]]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void addrecipe(  const name collection_name, const int32_t template_id, vector<atomic::nft> templates );

    /**
     * ## ACTION `delblend`
     *
     * Delete NFT blend
     *
     * - **authority**: `atomicassets::author` or `get_self()`
     *
     * ### params
     *
     * - `{name} collection_name` - AtomicAsset NFT collection name
     * - `{int32_t} template_id` - AtomicAsset NFT template ID
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems delblend '["mycollection", 789]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void delblend( const name collection_name, const int32_t template_id );

    /**
     * ## ACTION `delrecipe`
     *
     * Delete NFT recipe
     *
     * - **authority**: `atomicassets::author` or `get_self()`
     *
     * ### params
     *
     * - `{name} collection_name` - AtomicAsset NFT collection name
     * - `{int32_t} template_id` - AtomicAsset NFT template ID
     * - `{uint64_t} recipe_id` - recipe ID
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems delrecipe '["mycollection", 789, 1]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void delrecipe( const name collection_name, const int32_t template_id, const uint64_t recipe_id );

    /**
     * ## ACTION `cancel`
     *
     * Returns any remaining orders to owner account
     *
     * - **authority**: `owner` or `get_self()`
     *
     * ### params
     *
     * - `{name} owner` - owner account to claim
     * - `{int32_t} template_id` - AtomicAsset NFT template ID
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems cancel '["myaccount", 789]' -p myaccount
     * ```
     */
    [[eosio::action]]
    void cancel( const name owner, const int32_t template_id );

    [[eosio::action]]
    void setfee( const optional<uint16_t> protocol_fee, const optional<name> fee_account );

    [[eosio::action]]
    void setstatus( const name status );

    [[eosio::action]]
    void reset( const name table, const optional<name> scope  );

    [[eosio::action]]
    void blendlog( const name owner,
                   const string description,
                   const vector<uint64_t> in_asset_ids,
                   const uint64_t out_asset_id,
                   const vector<atomic::nft_extra> in_templates,
                   const atomic::nft_extra out_template,
                   const int total_mint,
                   const int total_burn );

    /**
     * Notify contract when AtomicAssets NFT token transfer notifiers relay contract
     */
    [[eosio::on_notify("atomicassets::transfer")]]
    void on_nft_transfer( const name from, const name to, const vector<uint64_t> asset_ids, const std::string memo );

    [[eosio::on_notify("*::transfer")]]
    void on_transfer( const name from, const name to, const asset quantity, const string memo );

    // static actions
    using setblend_action = eosio::action_wrapper<"setblend"_n, &gems::blend::setblend>;
    using addrecipe_action = eosio::action_wrapper<"addrecipe"_n, &gems::blend::addrecipe>;
    using delblend_action = eosio::action_wrapper<"delblend"_n, &gems::blend::delblend>;
    using delrecipe_action = eosio::action_wrapper<"delrecipe"_n, &gems::blend::delrecipe>;
    using reset_action = eosio::action_wrapper<"reset"_n, &gems::blend::reset>;
    using blendlog_action = eosio::action_wrapper<"blendlog"_n, &gems::blend::blendlog>;

private:
    // eosio.token helper
    void transfer( const name from, const name to, const extended_asset quantity, const string memo );

    // blend
    void validate_templates( const vector<atomic::nft> templates, const bool burnable, const bool transferable );
    void attempt_to_blend( const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& asset_ids );
    void check_time( const optional<time_point_sec> start_time, const optional<time_point_sec> end_time );
    uint64_t detect_recipe( const name collection_name, const vector<uint64_t> asset_ids, const set<uint64_t> recipe_ids, const vector<atomic::nft_extra> received_templates );
    bool is_match( const vector<atomic::nft>& sorted_templates, vector<atomic::nft>& templates );
    bool is_match( const vector<atomic::nft_extra>& sorted_templates, vector<atomic::nft_extra>& templates );
    std::pair<name, int32_t> parse_memo( const string memo );
    vector<atomic::nft> sort_templates( vector<atomic::nft> templates );
    name get_ram_payer( const name collection_name );
    void deduct_token_payment( const name from, const name collection_name, const int32_t template_id );
    void add_quantity( const name owner, const atomic::nft id, const extended_asset value );
    void check_status();

    // plugins
    void check_plugin( const name plugin );
    void validate_attributes( const name plugin, const atomic::nft id );
    pair<ATTRIBUTE_MAP, ATTRIBUTE_MAP> mint_attributes( const name plugin, const name owner, const name collection_name, const int32_t template_id, const vector<uint64_t>& in_asset_ids );

    // update counters in status singleton
    void update_status( const uint32_t index, const uint32_t count );

    // maintenance
    template <typename T>
    void clear_table( T& table );
};
}
