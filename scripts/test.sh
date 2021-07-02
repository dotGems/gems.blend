# #!/bin/bash

# AtomicAssets - init configurations
cleos push action atomicassets init '[]' -p atomicassets

# AtomicAssets - create collection & schema
cleos push action atomicassets createcol '["mycollection", "mycollection", true, ["blend.gems", "mycollection"], [], 0, []]' -p mycollection
cleos push action atomicassets createschema '["mycollection", "mycollection", "myschema", [{"name": "name", "type": "string"}]]' -p mycollection
cleos push action atomicassets createschema '["mycollection", "mycollection", "mypack", [{"name": "name", "type": "string"}]]' -p mycollection

# create NFT item
cleos push action atomicassets createtempl '["mycollection", "mycollection", "myschema", true, true, 0, [{"key": "name","value": ["string", "item"]}]]' -p mycollection

# create NFT pack
cleos push action atomicassets createtempl '["mycollection", "mycollection", "mypack", true, true, 0, [{"key": "name","value": ["string", "pack"]}]]' -p mycollection

# mint assets
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myschema", 1, "mycollection", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myschema", 1, "mycollection", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myschema", 1, "mycollection", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myschema", 1, "mycollection", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myschema", 1, "mycollection", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myschema", 1, "mycollection", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myschema", 1, "mycollection", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myschema", 1, "mycollection", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myschema", 1, "mycollection", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myschema", 1, "mycollection", [], [], []]' -p mycollection -f

# mint packs
cleos push action atomicassets mintasset '["mycollection", "mycollection", "mypack", 2, "myaccount", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "mypack", 2, "myaccount", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "mypack", 2, "myaccount", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "mypack", 2, "myaccount", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "mypack", 2, "myaccount", [], [], []]' -p mycollection -f

# setup NFT blend recipe
cleos push action blend.gems setrecipe '["myrecipe", "mycollection", [123, 456], 789, "1.00000000 WAX", "2021-07-02T00:00:00"]' -p blend.gems

# user send NFT's to be blended (memo optional)
cleos push action atomicassets transfer '["myaccount", "blend.gems", [1099511627786, 1099511627787], "blend"]' -p myaccount
