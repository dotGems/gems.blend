# #!/bin/bash

# AtomicAssets - init configurations
cleos push action atomicassets init '[]' -p atomicassets

# AtomicAssets - create collection & schema
cleos push action atomicassets createcol '["mycollection", "mycollection", true, ["blend.gems", "mycollection"], [], 0, []]' -p mycollection
cleos push action atomicassets createschema '["mycollection", "mycollection", "myitems", [{"name": "name", "type": "string"}]]' -p mycollection
cleos push action atomicassets createschema '["mycollection", "mycollection", "myblended", [{"name": "name", "type": "string"}]]' -p mycollection
cleos push action atomicassets createschema '["mycollection", "mycollection", "potion", [{"name": "name", "type": "string"}, {"name": "mana", "type": "uint64"}, {"name": "timestamp", "type": "uint64"}]]' -p mycollection

# create NFT templates
cleos push action atomicassets createtempl '["mycollection", "mycollection", "myitems", true, true, 0, [{"key": "name","value": ["string", "pack"]}]]' -p mycollection -f
cleos push action atomicassets createtempl '["mycollection", "mycollection", "myitems", true, true, 0, [{"key": "name","value": ["string", "pack"]}]]' -p mycollection -f
cleos push action atomicassets createtempl '["mycollection", "mycollection", "myitems", true, true, 0, [{"key": "name","value": ["string", "pack"]}]]' -p mycollection -f
cleos push action atomicassets createtempl '["mycollection", "mycollection", "myblended", true, true, 0, [{"key": "name","value": ["string", "pack"]}]]' -p mycollection -f
cleos push action atomicassets createtempl '["mycollection", "mycollection", "potion", true, true, 0, [{"key": "name","value": ["string", "potion"]}]]' -p mycollection -f

# mint blendable
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myitems", 1, "myaccount", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myitems", 2, "myaccount", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myitems", 3, "myaccount", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myitems", 3, "myaccount", [], [], []]' -p mycollection -f
cleos push action atomicassets mintasset '["mycollection", "mycollection", "myitems", 1, "myaccount", [], [], []]' -p mycollection -f

# setup NFT blend recipe
cleos push action blend.gems setblend '[["mycollection", 2], "My Blend", null, null, null, null]' -p mycollection
cleos push action blend.gems addrecipe '[["mycollection", 2], [["mycollection", 1], ["mycollection", 2], ["mycollection", 3]]]' -p mycollection

# Potion
cleos push action blend.gems setblend '[["mycollection", 5], "Potion", "potion", null, null, null]' -p mycollection
cleos push action blend.gems addrecipe '[["mycollection", 5], [["mycollection", 1]]]' -p mycollection

# user send NFT's to be blended (memo optional)
cleos push action atomicassets transfer '["myaccount", "blend.gems", [1099511627776, 1099511627777, 1099511627778], "mycollection:2"]' -p myaccount -p mycollection
