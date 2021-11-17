# 💎`.Gems` Blend technology

## Quickstart

1. Admin setup NFT recipe & blend
2. User sends AtomicAssets NFT assets to `blend.gems` with desired blend id in the memo.

```bash
# setup NFT recipe
$ cleos push action blend.gems setblend '["myrecipe", [["mycollection", 123], ["mycollection", 456]]]' -p blend.gems

# setup NFT blend
$ cleos push action blend.gems setblend '["myblend", ["myrecipe"], [["mycollection", 789]], "2021-11-16T00:00:00", "2021-12-01T00:00:00"]' -p blend.gems

# user send NFT's to be blended (memo contains Blend id)
$ cleos push action atomicassets transfer '["myaccount", "blend.gems", [1099512167123, 1099512167124], "myblend"]' -p myaccount
```

## Table of Content

- [TABLE `blends`](#table-blends)
- [TABLE `recipes`](#table-recipes)
- [TABLE `ontransfer`](#table-ontransfer)
- [TABLE `status`](#table-status)
- [ACTION `setblend`](#action-setblend)
- [ACTION `setrecipe`](#action-setrecipe)
- [ACTION `delblend`](#action-delblend)
- [ACTION `delrecipe`](#action-delrecipe)

## TABLE `blends`

### params

- `{name} blend_id` - (primary key) blend ID (ex: `myblend`)
- `{set<name>} in_recipe_ids` - one or many input recipes ID's
- `{atomic::nft} out_template` - output AtomicAsset NFT template
- `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
- `{time_point_sec} [end_time=null]` - (optional) end time (ex: "2021-08-01T00:00:00")

### example

```json
{
    "blend_id": "myblend",
    "in_recipe_ids": ["myrecipe1", "myrecipe2"],
    "out_template": {"collection_name": "mycollection", "template_id": 21883},
    "start_time": "2021-07-01T00:00:00",
    "end_time": "2021-10-01T00:00:00"
}
```

## TABLE `recipes`

### params

- `{name} recipe_id` - (primary key) recipe ID (ex: `myrecipe`)
- `{vector<atomic::nft>} templates` - AtomicAsset NFT templates

### example

```json
{
    "recipe_id": "myrecipe",
    "templates": [{"collection_name": "mycollection", "template_id": 21883}]
}
```

## TABLE `ontransfer`

- scope: `{name} owner`

### params

- `{name} owner` - (primary key) account name
- `{vector<uint64_t>} asset_ids` - received AtomicHub NFT asset IDs

### example

```json
{
    "owner": "myaccount",
    "asset_ids": [1099511627776, 1099511627777, 1099511627778, 1099511627779]
}
```

## TABLE `status`

- `vector<uint32_t>` counters;   // 0 - total recipes blended
- `time_point_sec` last_updated;

### example

```json
{
    "counters": [1234, 12],
    "last_updated": "2021-04-12T12:23:42"
}
```

## ACTION `setblend`

Set NFT blend

- **authority**: `get_self()`

### params

- `{name} blend_id` - (primary key) blend ID (ex: `myblend`)
- `{set<name>} in_recipe_ids` - input recipes ID's
- `{atomic::nft} out_template` - output AtomicAsset NFT template
- `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
- `{time_point_sec} [end_time=null]` - (optional) end time (ex: "2021-08-01T00:00:00")

### Example

```bash
$ cleos push action blend.gems setblend '["myblend", ["myrecipe1", "myrecipe2"], ["mycollection", 789], "2021-11-01T00:00:00", "2021-12-01T00:00:00"]' -p blend.gems
```

## ACTION `setrecipe`

Set NFT recipe

- **authority**: `get_self()`

### params

- `{name} blend_id` - blend blend ID (ex: `myblend`)
- `{vector<atomic::nft>} templates` - AtomicHub NFT templates

### Example

```bash
$ cleos push action blend.gems setrecipe '["myrecipe", [["mycollection", 123], ["mycollection", 456]]]' -p blend.gems
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

## ACTION `delrecipe`

Delete NFT recipe

- **authority**: `get_self()`

### params

- `{name} recipe_id` - recipe ID (ex: `myrecipe`)

### Example

```bash
$ cleos push action blend.gems delrecipe '["myrecipe"]' -p blend.gems
```