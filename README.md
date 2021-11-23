# 💎`.Gems` Blend technology

## Quickstart

1. Admin setup NFT recipe & blend
2. User sends AtomicAssets NFT assets to `blend.gems` with desired blend id in the memo.

```bash
# setup NFT blend
$ cleos push action blend.gems setblend '[["mycollection", 789], "2021-11-16T00:00:00", "2021-12-01T00:00:00"]' -p myauthor

# setup NFT recipe
$ cleos push action blend.gems addtrecipe '[["mycollection", 789], [["mycollection", 123], ["mycollection", 456]]]' -p myauthor

# user send NFT's to contract (memo schema `<collection_name>:<template_id>`)
$ cleos push action atomicassets transfer '["myaccount", "blend.gems", [1099512167123, 1099512167124], "mycollection:789"]' -p myaccount
```

## Table of Content

- [TABLE `status`](#table-status)
- [TABLE `collections`](#table-collections)
- [TABLE `blends`](#table-blends)
- [TABLE `recipes`](#table-recipes)
- [ACTION `setblend`](#action-setblend)
- [ACTION `addrecipe`](#action-addrecipe)
- [ACTION `delblend`](#action-delblend)
- [ACTION `delrecipe`](#action-delrecipe)

## TABLE `status`

## params

- `{vector<uint32_t>} counters` - counters
- `{uint32_t} counters[0]` - total mint
- `{uint32_t} counters[1]` - total burn
- `{time_point_sec} last_updated` - last updated

### example

```json
{
    "counters": [10, 30],
    "last_updated": "2021-04-12T12:23:42"
}
```

## TABLE `collections`

## params

- `{set<name>} collection_names` - collection names

### example

```json
{
    "collection_names": ["mycollection"]
}
```

## TABLE `blends`

- scope: `{name} collection_name`

### params

- `{atomic::nft} id` - output AtomicAsset NFT template
- `{set<uint64_t>} recipe_ids` - one or many input recipes ID's
- `{string} description` - blend description
- `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
- `{time_point_sec} [end_time=null]` - (optional) end time (ex: "2021-08-01T00:00:00")

### example

```json
{
    "id": {"collection_name": "mycollection", "template_id": 21883},
    "recipe_ids": [1, 2],
    "description": "My Blend",
    "start_time": "2021-07-01T00:00:00",
    "end_time": "2021-10-01T00:00:00"
}
```

## TABLE `recipes`

- scope: `{name} collection_name`

### params

- `{uint64_t} id` - (auto-incremental primary key) recipe ID
- `{vector<atomic::nft>} templates` - AtomicAsset NFT templates

### example

```json
{
    "id": 1,
    "templates": [{"collection_name": "mycollection", "template_id": 21883}]
}
```

## ACTION `setblend`

Set NFT blend

- **authority**: `atomicassets::author` or `get_self()`

### params

- `{atomic::nft} id` - AtomicAsset NFT template
- `{string} [description=""]` - (optional) blend description
- `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
- `{time_point_sec} [end_time=null]` - (optional) end time (ex: "2021-08-01T00:00:00")

### Example

```bash
$ cleos push action blend.gems setblend '[["mycollection", 789], "My Blend", "2021-11-01T00:00:00", "2021-12-01T00:00:00"]' -p blend.gems
```

## ACTION `addrecipe`

Add NFT recipe to blend

- **authority**: `atomicassets::author` or `get_self()`

### params

- `{atomic::nft} id` - AtomicAsset NFT template
- `{vector<atomic::nft>} templates` - AtomicHub NFT templates

### Example

```bash
$ cleos push action blend.gems addrecipe '[["mycollection", 789], [["mycollection", 123], ["mycollection", 456]]]' -p blend.gems
```

## ACTION `delblend`

Delete NFT blend

- **authority**: `atomicassets::author` or `get_self()`

### params

- `{atomic::nft} id` - blend AtomicAsset NFT

### Example

```bash
$ cleos push action blend.gems delblend '[["mycollection", 789]]' -p blend.gems
```

## ACTION `delrecipe`

Delete NFT recipe

- **authority**: `atomicassets::author` or `get_self()`

### params

- `{atomic::nft} id` - AtomicAsset NFT template
- `{uint64_t} recipe_id` - recipe ID

### Example

```bash
$ cleos push action blend.gems delrecipe '[["mycollection", 789], 1]' -p blend.gems
```