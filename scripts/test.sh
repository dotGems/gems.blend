# #!/bin/bash

# AtomicAssets - init configurations
cleos push action atomicassets init '[]' -p atomicassets

# AtomicAssets - create collection & schema
cleos push action atomicassets createcol '["mycollection", "mycollection", true, ["blend.gems", "mycollection"], [], 0, []]' -p mycollection
cleos push action atomicassets createschema '["mycollection", "mycollection", "potion", [{"name": "name", "type": "string"}, {"name": "mana", "type": "uint64"}, {"name": "timestamp", "type": "uint64"}]]' -p mycollection
cleos push action atomicassets createschema '["mycollection", "mycollection", "diamond", [{"name": "name", "type": "string"}, {"name": "img", "type": "string"}, {"name": "rarity", "type": "string"}, {"name": "color", "type": "string"}, {"name": "shape", "type": "string"}, {"name": "clarity", "type": "string"}]]' -p mycollection

# create NFT templates
cleos push action atomicassets createtempl '["mycollection", "mycollection", "potion", true, true, 0, [{"key": "name","value": ["string", "diamond Bottle"]}]]' -p mycollection -f
cleos push action atomicassets createtempl '["mycollection", "mycollection", "diamond", true, true, 0, [{"key": "name", "value": ["string", "diamond"]}]]' -p mycollection -f
cleos push action atomicassets createtempl '["mycollection", "mycollection", "diamond", true, true, 0, [{"key": "name", "value": ["string", "diamond"]}]]' -p mycollection -f

# mint diamond Bottle
cleos push action atomicassets mintasset '["mycollection", "mycollection", "potion", 1, "myaccount", [], [], []]' -p mycollection -f

# mint diamond Drops
cleos push action atomicassets mintasset '["mycollection", "mycollection", "diamond", 2, "myaccount", [{"key": "clarity", "value": ["string", "FL"]}, {"key": "shape", "value": ["string", "Round"]}, {"key": "color", "value": ["string", "Rose"]}], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "diamond", 2, "myaccount", [{"key": "clarity", "value": ["string", "FL"]}, {"key": "shape", "value": ["string", "Round"]}, {"key": "color", "value": ["string", "Rose"]}], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "diamond", 2, "myaccount", [{"key": "clarity", "value": ["string", "FL"]}, {"key": "shape", "value": ["string", "Round"]}, {"key": "color", "value": ["string", "Rose"]}], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "diamond", 2, "myaccount", [{"key": "clarity", "value": ["string", "FL"]}, {"key": "shape", "value": ["string", "Round"]}, {"key": "color", "value": ["string", "Rose"]}], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "diamond", 2, "myaccount", [{"key": "clarity", "value": ["string", "FL"]}, {"key": "shape", "value": ["string", "Round"]}, {"key": "color", "value": ["string", "Rose"]}], [], []]' -p mycollection -f

# set status
cleos push action blend.gems setstatus '["ok"]' -p blend.gems

# setup NFT blend recipe
cleos push action blend.gems setblend '["mycollection", 3, "Diamond Pond", null, null, null, null]' -p mycollection
cleos push action blend.gems addrecipe '["mycollection", 3, [["mycollection", 1], ["mycollection", 2], ["mycollection", 2], ["mycollection", 2], ["mycollection", 2], ["mycollection", 2]]]' -p mycollection

# # # user send NFT's to be blended (memo optional)
# cleos -v push action atomicassets transfer '["myaccount", "blend.gems", [1099511627776, 1099511627777, 1099511627778, 1099511627779, 1099511627780, 1099511627781], "mycollection:3"]' -p myaccount -p mycollection