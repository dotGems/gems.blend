# 💎`.Gems` Blend technology

## Quickstart

1. Admin setup NFT blend recipe (ex: "myrecipe").
1. User sends AtomicAssets NFT assets to `blend.gems`.

```bash
# setup NFT blend recipe
$ cleos push action blend.gems setrecipe '["myrecipe", "mycollection", [123, 456], 789, "1.00000000 WAX", "2021-07-02T00:00:00"]' -p blend.gems

# user send NFT's to be blended (memo optional)
$ cleos push action atomicassets transfer '["myaccount", "blend.gems", [1099512167123, 1099512167124], "blend"]' -p myaccount
```

## Table of Content

- [TABLE `recipes`](#table-recipes)
- [TABLE `templateids`](#table-templateids)
- [TABLE `blends`](#table-blends)
- [ACTION `setrecipe`](#action-setrecipe)
- [ACTION `delrecipe`](#action-delrecipe)
- [ACTION `refund`](#action-refund)

## TABLE `recipes`

### params

- `{name} recipe_id` - (primary key) recipe ID (ex: `myrecipe`)
- `{name} collection_name` - AtomicHub Collection Name (ex: `mycollection`)
- `{set<int32_t>} template_ids` - AtomicHub NFT template ID (ex: [`21881`, `21882`])
- `{int32_t} blend_template_id` - AtomicHub NFT template ID (ex: `21883`)
- `{asset} [backed_token=null]` - (optional) backed token (ex: `"1.00000000 WAX"`)
- `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
- `{time_point_sec} last_blend` - last blend time (ex: "2021-07-01T00:00:00")
- `{uint64_t} counter_blend` - total counter blend

### example

```json
{
    "recipe_id": "myrecipe",
    "collection_name": "mycollection",
    "template_ids": [21881, 21882],
    "blend_template_id": 21883,
    "backed_token": "1.00000000 WAX",
    "start_time": "2021-07-01T00:00:00",
    "last_blend": "2021-07-01T00:00:00",
    "counter_blend": 0
}
```

## TABLE `templateids`

### multi-indexes

| `param`        | `index_position` | `key_type` |
|--------------- |------------------|------------|
| `bycollection` | 2                | i64        |

### params

- `{int32_t} template_id` - (primary key) AtomicHub NFT template ID (ex: `21881`)
- `{name} collection_name` - AtomicHub Collection Name (ex: `mycollection`)
- `{name} schema_name` - AtomicHub Schema Name (ex: `myschema`)
- `{name} recipe_id` - recipe ID (ex: `myrecipe`)

### example

```json
{
    "template_id": 21881,
    "collection_name": "mycollection",
    "schema_name": "myschema",
    "recipe_id": "myrecipe"
}
```

## TABLE `blends`

### params

- `{name} account` - (primary key) account name
- `{vector<uint64_t>} asset_ids` - received AtomicHub NFT asset IDs
- `{set<int32_t>} template_ids` - received AtomicHub NFT template ID (ex: [`21881`, `21882`])

### example

```json
{
    "id": "mypool",
    "asset_ids": [1099512167124, 1099512167125],
    "template_ids": [21881, 21882]
}
```

## TABLE `global`

### params

- `{uint64_t} counter_blends` - counter blends

### example

```json
{
    "counter_blends": 1
}
```

## ACTION `setrecipe`

Set NFT blend recipe

- **authority**: `get_self()`

### params

- `{name} recipe_id` - blend recipe ID (ex: `myrecipe`)
- `{name} collection_name` - AtomicHub Collection Name (ex: `mycollection`)
- `{set<int32_t>} template_ids` - AtomicHub NFT template ID (ex: [`21881`, `21882`])
- `{int32_t} blend_template_id` - AtomicHub NFT template ID (ex: `21883`)
- `{asset} [backed_token=null]` - (optional) backed token (ex: `"1.00000000 WAX"`)
- `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")

### Example

```bash
$ cleos push action blend.gems setrecipe '["myrecipe", "mycollection", [123, 456], 789, "1.00000000 WAX", "2021-07-02T00:00:00"]' -p blend.gems
```

## ACTION `delrecipe`

Delete NFT blend recipe

- **authority**: `get_self()`

### params

- `{name} recipe_id` - blend recipe ID (ex: `myrecipe`)

### Example

```bash
$ cleos push action blend.gems delrecipe '["myrecipe"]' -p blend.gems
```

## ACTION `refund`

Refund user

- **authority**: `get_self()`

### params

- `{name} account` - account name to refund

### Example

```bash
$ cleos push action blend.gems refund '["myaccount"]' -p blend.gems
```