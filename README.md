# 💎`.Gems` Blend technology

## Quickstart

1. Admin setup NFT blend recipe (ex: "myblend").
1. User sends AtomicAssets NFT assets to `blend.gems`.

```bash
# setup NFT blend recipe
$ cleos push action blend.gems setblend '["myblend", "mycollection", [123, 456], "mycollection", [789], "1.00000000 WAX", "2021-07-02T00:00:00"]' -p blend.gems

# user send NFT's to be blended (memo optional)
$ cleos push action atomicassets transfer '["myaccount", "blend.gems", [1099512167123, 1099512167124], "blend"]' -p myaccount
```

## Table of Content

- [TABLE `blends`](#table-blends)
- [TABLE `templates`](#table-templates)
- [TABLE `ontransfer`](#table-ontransfer)
- [ACTION `setblend`](#action-setblend)
- [ACTION `delblend`](#action-delblend)
- [ACTION `refund`](#action-refund)

## TABLE `blends`

### multi-indexes

| `param`        | `index_position` | `key_type` |
|--------------- |------------------|------------|
| `bycollection` | 2                | i64        |
| `byupdated`    | 2                | i64        |

### params

- `{name} blend_id` - (primary key) blend ID (ex: `myblend`)
- `{name} in_collection_name` - input AtomicHub Collection Name (ex: `mycollection`)
- `{vector<int32_t>} in_template_ids` - input AtomicHub NFT template ID (ex: [`21881`, `21882`])
- `{name} out_collection_name` - output AtomicHub Collection Name (ex: `mycollection`)
- `{vector<int32_t>} out_template_id` - output AtomicHub NFT template ID (ex: [`21883`])
- `{asset} [backed_tokens=null]` - (optional) backed token (ex: `"1.00000000 WAX"`)
- `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")
- `{time_point_sec} last_updated` - last updated time (ex: "2021-07-01T00:00:00")
- `{uint64_t} total_mint` - total mint
- `{uint64_t} total_burn` - total burn
- `{asset} total_backed_tokens` - total backed token

### example

```json
{
    "blend_id": "myblend",
    "in_collection_name": "mycollection",
    "in_template_ids": [21881, 21882],
    "out_collection_name": "mycollection",
    "out_template_ids": [21883],
    "backed_tokens": "1.00000000 WAX",
    "start_time": "2021-07-01T00:00:00",
    "last_updated": "2021-07-01T00:00:00",
    "total_mint": 0,
    "total_burn": 0,
    "total_backed_tokens": "0.00000000 WAX"
}
```

## TABLE `templates`

### multi-indexes

| `param`        | `index_position` | `key_type` |
|--------------- |------------------|------------|
| `bycollection` | 2                | i64        |

### params

- `{int32_t} template_id` - (primary key) AtomicHub NFT template ID (ex: `21881`)
- `{name} collection_name` - AtomicHub Collection Name (ex: `mycollection`)
- `{name} schema_name` - AtomicHub Schema Name (ex: `myschema`)
- `{name} blend_id` - blend ID (ex: `myblend`)

### example

```json
{
    "template_id": 21881,
    "collection_name": "mycollection",
    "schema_name": "myschema",
    "blend_id": "myblend"
}
```

## TABLE `ontransfer`

### multi-indexes

| `param`        | `index_position` | `key_type` |
|--------------- |------------------|------------|
| `bycollection` | 2                | i64        |
| `byupdated`    | 3                | i64        |

### params

- `{name} owner` - (primary key) account name
- `{name} collection_name` - AtomicHub Collection Name (ex: `mycollection`)
- `{vector<uint64_t>} asset_ids` - received AtomicHub NFT asset IDs
- `{set<int32_t>} template_ids` - received AtomicHub NFT template ID (ex: [`21881`, `21882`])
- `{time_point_sec} last_updated` - last updated time (ex: "2021-07-01T00:00:00")

### example

```json
{
    "owner": "myaccount",
    "collection_name": "mycollection",
    "asset_ids": [1099512167124, 1099512167125],
    "template_ids": [21881, 21882],
    "last_updated": "2021-07-01T00:00:00"
}
```

## TABLE `global`

### params

- `{uint64_t} total_mint` - total mint
- `{uint64_t} total_burn` - total burn
- `{asset} total_backed_tokens` - total backed token

### example

```json
{
    "total_mint": 0,
    "total_burn": 0,
    "total_backed_tokens": "0.00000000 WAX"
}
```

## ACTION `setblend`

Set NFT blend recipe

- **authority**: `get_self()`

### params

- `{name} blend_id` - blend blend ID (ex: `myblend`)
- `{name} in_collection_name` - input AtomicHub Collection Name (ex: `mycollection`)
- `{vector<int32_t>} in_template_ids` - input AtomicHub NFT template ID (ex: [`21881`, `21882`])
- `{name} out_collection_name` - output AtomicHub Collection Name (ex: `mycollection`)
- `{vector<int32_t>} out_template_ids` - output AtomicHub NFT template ID (ex: [`21883`])
- `{vector<asset>} [backed_tokens=null]` - (optional) tokens to back (ex: `"1.00000000 WAX"`)
- `{time_point_sec} [start_time=null]` - (optional) start time (ex: "2021-07-01T00:00:00")

### Example

```bash
$ cleos push action blend.gems setblend '["myblend", "mycollection", [123, 456], "mycollection", [789], "1.00000000 WAX", "2021-07-02T00:00:00"]' -p blend.gems
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

## ACTION `refund`

Refund user

- **authority**: `get_self()`

### params

- `{name} owner` - account name to refund

### Example

```bash
$ cleos push action blend.gems refund '["myaccount"]' -p blend.gems
```