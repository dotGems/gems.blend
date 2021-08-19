#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>
#include <gems.atomic/atomic.gems.hpp>

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
     * ### multi-indexes
     *
     * | `param`        | `index_position` | `key_type` |
     * |--------------- |------------------|------------|
     * | `byupdated`    | 2                | i64        |
     *
     * ### params
     *
     * - `{name} blend_id` - (primary key) blend ID (ex: `myblend`)
     * - `{vector<atomic::nft>} in_templates` - input AtomicHub NFT template IDs (ex: [{mycol1, `21881`}, {mycol2, `21882`}])
     * - `{name} out_collection_name` - output AtomicHub Collection Name (ex: `mycollection`)
     * - `{vector<atomic::nft>} out_templates` - output AtomicHub NFT template IDs (ex: [{mycol1, `21883`}])
     * - `{vector<asset>} backed_tokens` - backed token (ex: `["1.00000000 WAX"]`)
     * - `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
     * - `{time_point_sec} last_updated` - last updated time (ex: "2021-07-01T00:00:00")
     * - `{uint64_t} total_mint` - total minted
     * - `{uint64_t} total_burn` - total burned
     * - `{vector<asset>} total_backed_token` - total backed
     *
     * ### example
     *
     * ```json
     * {
     *     "blend_id": "myblend",
     *     "in_template_ids": [{mycollection, 21881}, {mycollection, 21882}],
     *     "out_template_ids": [{mycollection, 21883}],
     *     "backed_tokens": ["1.00000000 WAX"],
     *     "start_time": "2021-07-01T00:00:00",
     *     "last_updated": "2021-07-01T00:00:00",
     *     "total_mint": 0,
     *     "total_burn": 0,
     *     "total_backed_tokens": ["0.00000000 WAX"]
     * }
     * ```
     */
    struct [[eosio::table("blends")]] blends_row {
        name                blend_id;
        vector<atomic::nft> in_templates;
        vector<atomic::nft> out_templates;
        vector<asset>       backed_tokens;
        time_point_sec      start_time;
        time_point_sec      last_updated;
        uint64_t            total_mint;
        uint64_t            total_burn;
        vector<asset>       total_backed_tokens;

        uint64_t primary_key() const { return blend_id.value; }
        uint64_t by_updated() const { return last_updated.sec_since_epoch(); }
    };
    typedef eosio::multi_index< "blends"_n, blends_row,
        indexed_by<"byupdated"_n, const_mem_fun<blends_row, uint64_t, &blends_row::by_updated>>
    > blends_table;

    /**
     * ## TABLE `ontransfer`
     *
     * ### multi-indexes
     *
     * | `param`        | `index_position` | `key_type` |
     * |--------------- |------------------|------------|
     * | `byblend`      | 2                | i64        |
     * | `byupdated`    | 3                | i64        |
     *
     * ### params
     *
     * - `{name} owner` - (primary key) account name
     * - `{name} blend_id` - blend ID (ex: `myblend`)
     * - `{vector<uint64_t>} asset_ids` - received AtomicHub NFT asset IDs
     * - `{time_point_sec} last_updated` - last updated time (ex: "2021-07-01T00:00:00")
     *
     * ### example
     *
     * ```json
     * {
     *     "owner": "myaccount",
     *     "blend_id": "myblend",
     *     "asset_ids": [{ "collection_name": "mycol1", "template_id": 1099512167124 }, {"collection_name": "mycol1", "template_id": 1099512167125}],
     *     "last_updated": "2021-07-01T00:00:00"
     * }
     * ```
     */
    struct [[eosio::table("ontransfer")]] ontransfer_row {
        name                owner;
        name                blend_id;
        vector<uint64_t>    asset_ids;
        time_point_sec      last_updated;

        uint64_t primary_key() const { return owner.value; }
        uint64_t by_blend() const { return blend_id.value; }
        uint64_t by_updated() const { return last_updated.sec_since_epoch(); }
    };
    typedef eosio::multi_index< "ontransfer"_n, ontransfer_row,
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
     * - `{vector<asset>} total_backed_tokens` - total backed tokens
     *
     * ### example
     *
     * ```json
     * {
     *     "total_mint": 0,
     *     "total_burn": 0,
     *     "total_backed_tokens": { "0.00000000 WAX" }
     * }
     * ```
     */
    struct [[eosio::table("global")]] global_row {
        uint64_t            total_mint;
        uint64_t            total_burn;
        vector<asset>       total_backed_tokens;
    };
    typedef eosio::singleton< "global"_n, global_row> global_table;

    /**
     * ## ACTION `setblend`
     *
     * Set NFT blend recipe
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{name} blend_id` - blend blend ID (ex: `myblend`)
     * - `{vector<atomic::nft>} in_templates` - input AtomicHub NFT template IDs (ex: [{mycol1, `21881`}, {mycol2, `21882`}])
     * - `{vector<atomic::nft>} out_templates` - output AtomicHub NFT template IDs (ex: [{mycol1, `21883`}])
     * - `{asset} [backed_token=null]` - (optional) token to back (ex: `"1.00000000 WAX"`)
     * - `{optional<time_point_sec>} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems setblend '["myblend", [["mycollection", 123], ["mycollection", 456]], [["mycollection", 789]], ["1.00000000 WAX"], "2021-07-02T00:00:00"]' -p blend.gems
     * ```
     */
    [[eosio::action]]
    void setblend( const name blend_id, const vector<atomic::nft> in_templates, const vector<atomic::nft> out_templates, const optional<asset> backed_token, const optional<time_point_sec> start_time );

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
     * ## ACTION `reset`
     *
     * Reset table
     *
     * - **authority**: `get_self()`
     *
     * ### params
     *
     * - `{name} table` - table to clear
     *
     * ### Example
     *
     * ```bash
     * $ cleos push action blend.gems reset '["blends"]' -p blend.gems
     * ```
     */

    [[eosio::action]]
    void reset( const name table );

    [[eosio::action]]
    void blendlog(  const name owner,
                    const name blend_id,
                    const uint64_t total_mint,
                    const uint64_t total_burn,
                    const vector<asset> total_backed_tokens,
                    const vector<uint64_t> in_asset_ids,
                    const vector<atomic::nft> burned_nfts,
                    const vector<atomic::nft> minted_nfts,
                    const vector<uint64_t> refund_asset_ids );

    /**
     * Notify contract when AtomicAssets NFT token transfer notifiers relay contract
     */
    [[eosio::on_notify("atomicassets::transfer")]]
    void on_nft_transfer( const name from, const name to, const vector<uint64_t> asset_ids, const std::string memo );

    // static actions
    using setblend_action = eosio::action_wrapper<"setblend"_n, &gems::blend::setblend>;
    using delblend_action = eosio::action_wrapper<"delblend"_n, &gems::blend::delblend>;
    using reset_action = eosio::action_wrapper<"reset"_n, &gems::blend::reset>;
    using blendlog_action = eosio::action_wrapper<"blendlog"_n, &gems::blend::blendlog>;

private:
    // eosio.token helper
    void transfer( const name from, const name to, const extended_asset quantity, const string memo );

    // blend
    void validate_templates( const vector<atomic::nft> templates, const bool burnable );
    void add_transfer( const name owner, const name blend_id, const uint64_t asset_id );
    void attempt_to_blend( const name owner );

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
