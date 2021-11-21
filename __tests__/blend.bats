#!/usr/bin/env bats

@test "create collections, templates and assets" {
  # AtomicAssets - init and config
  run cleos push action atomicassets init '[]' -p atomicassets
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
  run cleos push action atomicassets mintasset '["mycollection", "mycollectio1", "myitems1", 3, "myaccount", [], [], []]' -p mycollection -f
  [ $status -eq 0 ]
  run cleos push action atomicassets mintasset '["mycollection", "mycollectio1", "myitems1", 1, "myaccount", [], [], []]' -p mycollection -f
  [ $status -eq 0 ]
  run cleos push action atomicassets mintasset '["mycollection", "mycollectio1", "myitems1", 2, "myaccount", [], [], []]' -p mycollection -f
  [ $status -eq 0 ]
  run cleos push action atomicassets mintasset '["mycollection", "mycollectio1", "myitems1", 3, "myaccount", [], [], []]' -p mycollection -f
  [ $status -eq 0 ]

}

@test "invalid set recipes" {

  run cleos push action blend.gems initrecipe '[[["mycollectio1", 999]]]' -p blend.gems
  [ $status -eq 1 ]
  [[ "$output" =~ "`template_id` does not exist" ]]

}

@test "valid set recipe & blend" {

  run cleos push action blend.gems initrecipe '[[["mycollectio1", 1], ["mycollectio1", 2], ["mycollectio1", 3], ["mycollectio1", 3]]]' -p blend.gems
  [ $status -eq 0 ]

  run cleos push action blend.gems setblend '[["mycollectio1", 4], [0], "2021-07-05T00:00:00", null]' -p blend.gems
  [ $status -eq 0 ]

}

@test "override set recipe & blend" {

  run cleos push action blend.gems setblend '[["mycollectio1", 4], [0], "2021-07-05T00:00:00", null]' -p blend.gems -f
  [ $status -eq 0 ]

}

@test "set recipe #2" {

  run cleos push action blend.gems initrecipe '[[["mycollectio1", 1], ["mycollectio1", 2]]]' -p blend.gems
  [ $status -eq 0 ]

  run cleos push action blend.gems setblend '[["mycollectio1", 4], [0, 1], "2021-07-05T00:00:00", null]' -p blend.gems
  [ $status -eq 0 ]

}

@test "provided too many" {

  run cleos push action atomicassets transfer '["myaccount", "blend.gems", [1099511627776, 1099511627777, 1099511627778, 1099511627779, 1099511627780], "mycollectio1:4"]' -p myaccount -p mycollection
  [ $status -eq 1 ]
  [[ "$output" =~ "could not detect any valid recipes" ]]

}

@test "blend exact" {

  run cleos push action atomicassets transfer '["myaccount", "blend.gems", [1099511627776, 1099511627777, 1099511627778, 1099511627779], "mycollectio1:4"]' -p myaccount -p mycollection
  [ $status -eq 0 ]

}

@test "provided not enough" {

  run cleos push action atomicassets transfer '["myaccount", "blend.gems", [1099511627781, 1099511627782, 1099511627783], "mycollectio1:4"]' -p myaccount -p mycollection
  [ $status -eq 1 ]
  [[ "$output" =~ "could not detect any valid recipes" ]]

}

@test "blend recipe #2" {

  run cleos push action atomicassets transfer '["myaccount", "blend.gems", [1099511627781, 1099511627782], "mycollectio1:4"]' -p myaccount -p mycollection
  [ $status -eq 0 ]

}

@test "status check" {
  result=$(cleos get table blend.gems blend.gems status | jq -r '.rows[0].counters[0]')
  [ "$result" = "2" ]
}


@test "delete recipe #2" {

  run cleos push action blend.gems delrecipe '[1]' -p blend.gems
  [ $status -eq 0 ]

  result=$(cleos get table blend.gems blend.gems blends | jq -r '.rows[0].recipe_ids | length')
  [ "$result" = "1" ]

  result=$(cleos get table blend.gems blend.gems recipes | jq -r '.rows | length')
  [ "$result" = "1" ]
}

@test "delete recipe #1" {

  run cleos push action blend.gems delrecipe '[0]' -p blend.gems
  [ $status -eq 0 ]

  result=$(cleos get table blend.gems blend.gems recipes | jq -r '.rows[0] | length')
  [ "$result" = "0" ]

  result=$(cleos get table blend.gems blend.gems blends | jq -r '.rows[0].recipe_ids | length')
  [ "$result" = "0" ]

}

@test "delete blend" {

  run cleos push action blend.gems delblend '[["mycollectio1", 4]]' -p blend.gems
  [ $status -eq 0 ]

  result=$(cleos get table blend.gems blend.gems blends | jq -r '.rows[0] | length')
  [ "$result" = "0" ]
}