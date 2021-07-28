#!/usr/bin/env bats

@test "create collections, templates and assets" {
  # AtomicAssets - init and config
  run cleos push action atomicassets init '[]' -p atomicassets
  [ $status -eq 0 ]
  run cleos push action atomicassets addconftoken '["eosio.token", "8,WAX"]' -p atomicassets
  [ $status -eq 0 ]
  run cleos push action atomicassets addconftoken '["eosio.token", "4,EOS"]' -p atomicassets
  [ $status -eq 0 ]

  # AtomicAssets - create collection & schema
  run cleos push action atomicassets createcol '["mycollection", "mycollectio1", true, ["blend.gems", "mycollection"], [], 0, []]' -p mycollection
  [ $status -eq 0 ]
  run cleos push action atomicassets createcol '["mycollection", "mycollectio2", true, ["blend.gems", "mycollection"], [], 0, []]' -p mycollection
  [ $status -eq 0 ]
  run cleos push action atomicassets createschema '["mycollection", "mycollectio1", "myitems1", [{"name": "name", "type": "string"}]]' -p mycollection
  [ $status -eq 0 ]
  run cleos push action atomicassets createschema '["mycollection", "mycollectio1", "myblended1", [{"name": "name", "type": "string"}]]' -p mycollection
  [ $status -eq 0 ]
  run cleos push action atomicassets createschema '["mycollection", "mycollectio2", "myitems2", [{"name": "name", "type": "string"}]]' -p mycollection
  [ $status -eq 0 ]
  run cleos push action atomicassets createschema '["mycollection", "mycollectio2", "myblended2", [{"name": "name", "type": "string"}]]' -p mycollection
  [ $status -eq 0 ]

  # create NFT templates
  run cleos push action atomicassets createtempl '["mycollection", "mycollectio1", "myitems1", true, true, 0, [{"key": "name","value": ["string", "pack"]}]]' -p mycollection -f
  [ $status -eq 0 ]
  run cleos push action atomicassets createtempl '["mycollection", "mycollectio1", "myitems1", true, true, 0, [{"key": "name","value": ["string", "pack"]}]]' -p mycollection -f
  [ $status -eq 0 ]
  run cleos push action atomicassets createtempl '["mycollection", "mycollectio1", "myitems1", true, true, 0, [{"key": "name","value": ["string", "pack"]}]]' -p mycollection -f
  [ $status -eq 0 ]
  run cleos push action atomicassets createtempl '["mycollection", "mycollectio1", "myblended1", true, true, 0, [{"key": "name","value": ["string", "pack"]}]]' -p mycollection -f
  [ $status -eq 0 ]

  # mint blendable
  run cleos push action atomicassets mintasset '["mycollection", "mycollectio1", "myitems1", 1, "myaccount", [], [], []]' -p mycollection -f
  [ $status -eq 0 ]
  run cleos push action atomicassets mintasset '["mycollection", "mycollectio1", "myitems1", 2, "myaccount", [], [], []]' -p mycollection -f
  [ $status -eq 0 ]
  run cleos push action atomicassets mintasset '["mycollection", "mycollectio1", "myitems1", 3, "myaccount", [], [], []]' -p mycollection -f
  [ $status -eq 0 ]
  run cleos push action atomicassets mintasset '["mycollection", "mycollectio1", "myitems1", 3, "myaccount", [], [], []]' -p mycollection -f
  [ $status -eq 0 ]

}

@test "invalid set blends" {

  run cleos push action blend.gems setblend '["badblend", [["mycollectio1", 1], ["mycollectio1", 2]], [["mycollectio1", 4]], ["1.00000000 XXX"], "2021-07-05T00:00:00"]' -p blend.gems
  [ $status -eq 1 ]
  [[ "$output" =~ "`backed_token` symbol must match" ]]

  run cleos push action blend.gems setblend '["badtemplate", [["mycollectio1", 1], ["mycollectio1", 200]], [["mycollectio1", 4]], ["1.00000000 WAX"], "2021-07-05T00:00:00"]' -p blend.gems
  [ $status -eq 1 ]
  [[ "$output" =~ "`template_id` does not exist" ]]

}

@test "valid set blend" {

  run cleos push action blend.gems setblend '["myblend1", [["mycollectio1", 1], ["mycollectio1", 2], ["mycollectio1", 3], ["mycollectio1", 3]], [["mycollectio1", 4]], ["1.00000000 WAX", "0.5000 EOS"], "2021-07-05T00:00:00"]' -p blend.gems
  [ $status -eq 0 ]

}

@test "override set blend" {

  run cleos push action blend.gems setblend '["myblend1", [["mycollectio1", 1], ["mycollectio1", 2], ["mycollectio1", 3]], [["mycollectio1", 4]], ["1.00000000 WAX", "0.5000 EOS"], "2021-07-05T00:00:00"]' -p blend.gems
  [ $status -eq 0 ]

}

@test "blend with refund" {

  run cleos push action atomicassets transfer '["myaccount", "blend.gems", [1099511627776, 1099511627777, 1099511627778, 1099511627779], "myblend1"]' -p myaccount -p mycollection
  [ $status -eq 0 ]

  result=$(cleos get table atomicassets myaccount assets | jq -r '.rows[0].asset_id + " " + .rows[1].asset_id + " " + .rows[1].backed_tokens[0] + " " + .rows[1].backed_tokens[1]')
  [ "$result" = "1099511627779 1099511627780 1.00000000 WAX 0.5000 EOS" ]

}

@test "delete blend" {

  run cleos push action blend.gems delblend '["myblend1"]' -p blend.gems
  [ $status -eq 0 ]

  result=$(cleos get table blend.gems blend.gems blends | jq -r '.rows[0]')
  [ "$result" = "null" ]
}
