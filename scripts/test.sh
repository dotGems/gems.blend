# #!/bin/bash

# AtomicAssets - init configurations
cleos push action atomicassets init '[]' -p atomicassets

# AtomicAssets - create collection & schema
cleos push action atomicassets createcol '["mycollection", "mycollection", true, ["blend.gems", "mycollection"], [], 0, []]' -p mycollection
cleos push action atomicassets createschema '["mycollection", "mycollection", "myitems", [{"name": "name", "type": "string"}]]' -p mycollection
cleos push action atomicassets createschema '["mycollection", "mycollection", "myblended", [{"name": "name", "type": "string"}]]' -p mycollection

# create NFT templates
cleos push action atomicassets createtempl '["mycollection", "mycollection", "myitems", true, true, 0, [{"key": "name","value": ["string", "pack"]}]]' -p mycollection -f
cleos push action atomicassets createtempl '["mycollection", "mycollection", "myitems", true, true, 0, [{"key": "name","value": ["string", "pack"]}]]' -p mycollection -f
cleos push action atomicassets createtempl '["mycollection", "mycollection", "myitems", true, true, 0, [{"key": "name","value": ["string", "pack"]}]]' -p mycollection -f
cleos push action atomicassets createtempl '["mycollection", "mycollection", "myblended", true, true, 0, [{"key": "name","value": ["string", "pack"]}]]' -p mycollection -f

# mint blendable
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myitems", 1, "myaccount", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myitems", 2, "myaccount", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myitems", 3, "myaccount", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myitems", 3, "myaccount", [], [], []]' -p mycollection -f

# setup NFT blend recipe
cleos push action blend.gems setrecipe '["myrecipe", [["mycollection", 1], ["mycollection", 2], ["mycollection", 3]]]' -p blend.gems
cleos push action blend.gems setblend '["myblend", ["myrecipe"], ["mycollection", 2], null, null]' -p blend.gems

# user send NFT's to be blended (memo optional)
cleos push action atomicassets transfer '["myaccount", "blend.gems", [1099511627776, 1099511627777, 1099511627778, 1099511627779], "myblend"]' -p myaccount -p mycollection
