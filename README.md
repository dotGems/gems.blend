# 💎`.Gems` Blend technology

## Quickstart

1. Admin setup NFT blend recipe (ex: "myblend").
2. User sends AtomicAssets NFT assets to `blend.gems` with desired blend id in the memo.

```bash
# setup NFT blend recipe
$ cleos push action blend.gems setblend '["myblend", [["mycollection", 123], ["mycollection", 456]], [["mycollection", 789]], ["1.00000000 WAX"], "2021-07-02T00:00:00"]' -p blend.gems

# user send NFT's to be blended (memo contains blend id)
$ cleos push action atomicassets transfer '["myaccount", "blend.gems", [1099512167123, 1099512167124], "myblend"]' -p myaccount
```

## Table of Content

- [TABLE `blends`](#table-blends)
- [TABLE `ontransfer`](#table-ontransfer)
- [ACTION `setblend`](#action-setblend)
- [ACTION `delblend`](#action-delblend)

## TABLE `blends`

### multi-indexes

| `param`        | `index_position` | `key_type` |
|--------------- |------------------|------------|
| `byupdated`    | 2                | i64        |

### params

- `{name} blend_id` - (primary key) blend ID (ex: `myblend`)
- `{vector<atomic::nft>} in_templates` - input AtomicHub NFT template IDs (ex: [{mycol1, `21881`}, {mycol2, `21882`}])
- `{name} out_collection_name` - output AtomicHub Collection Name (ex: `mycollection`)
- `{vector<atomic::nft>} out_templates` - output AtomicHub NFT template IDs (ex: [{mycol1, `21883`}])
- `{vector<asset>} backed_tokens` - backed token (ex: `["1.00000000 WAX"]`)
- `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
- `{time_point_sec} last_updated` - last updated time (ex: "2021-07-01T00:00:00")
- `{uint64_t} total_mint` - total minted
- `{uint64_t} total_burn` - total burned
- `{vector<asset>} total_backed_token` - total backed

### example

```json
{
    "blend_id": "myblend",
    "in_template_ids": [{"collection_name": "mycollection", "template_id": 21881}, {"collection_name": "mycollection", "template_id": 21882}],
    "out_template_ids": [{"collection_name": "mycollection","template_id": 21883}],
    "backed_tokens": ["1.00000000 WAX"],
    "start_time": "2021-07-01T00:00:00",
    "last_updated": "2021-07-01T00:00:00",
    "total_mint": 0,
    "total_burn": 0,
    "total_backed_tokens": ["0.00000000 WAX"]
    }
```

## TABLE `ontransfer`

### multi-indexes

| `param`        | `index_position` | `key_type` |
|--------------- |------------------|------------|
| `byupdated`    | 2                | i64        |

### params

- `{name} owner` - (primary key) account name
- `{name} blend_id` - blend ID (ex: `myblend`)
- `{vector<uint64_t>} asset_ids` - received AtomicHub NFT asset IDs
- `{time_point_sec} last_updated` - last updated time (ex: "2021-07-01T00:00:00")
*
### example

```json
{
    "owner": "myaccount",
    "blend_id": "myblend",
    "asset_ids": [ 1099512167124, 1099512167125 ],
    "last_updated": "2021-07-01T00:00:00"
}
```

## TABLE `global`

### params

- `{uint64_t} total_mint` - total mint
- `{uint64_t} total_burn` - total burn
- `{vector<asset>} total_backed_tokens` - total backed tokens

### example

```json
{
    "total_mint": 1,
    "total_burn": 3,
    "total_backed_tokens": ["1.00000000 WAX", "0.5000 EOS"]
}
```

## ACTION `setblend`

Create/update NFT blend recipe

- **authority**: `get_self()`

### params

- `{name} blend_id` - proposed blend ID (ex: `myblend`)
- `{vector<atomic::nft>} in_templates` - input AtomicHub NFT template IDs (ex: [{mycollection, 21881}, {mycollection, 21882}])
- `{vector<atomic::nft>} out_templates` - output AtomicHub NFT template IDs (ex: [{mycollection, 21883}])
- `{asset} [backed_token=null]` - (optional) token to back (ex: `"1.00000000 WAX"`)
- `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")

### Example

```bash
$ cleos push action blend.gems setblend '["myblend", [["mycollection", 123], ["mycollection", 456]], [["mycollection", 789]], "1.00000000 WAX", "2021-07-02T00:00:00"]' -p blend.gems
```

## ACTION `delblend`

Delete NFT blend recipe

- **authority**: `get_self()`

### params

- `{name} blend_id` - blend blend ID (ex: `myblend`)

### Example

```bash
$ cleos push action blend.gems delblend '["myblend"]' -p blend.gems
```
