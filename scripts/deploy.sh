#!/bin/bash

# unlock wallet
cleos wallet unlock --password $(cat ~/eosio-wallet/.pass)

# create accounts
cleos create account eosio blend.gems EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio myaccount EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio toaccount EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio atomicassets EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio mycollection EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

# deploy
cleos set contract blend.gems . blend.gems.wasm blend.gems.abi
cleos set contract eosio.token include/eosio.token eosio.token.wasm eosio.token.abi
cleos set contract atomicassets include/atomicassets atomicassets.wasm atomicassets.abi

# permission
cleos set account permission blend.gems active --add-code
cleos set account permission atomicassets active --add-code

# used only for local testing
cleos set account permission mycollection active blend.gems --add-code

# create EOS & WAX token
cleos push action eosio.token create '["eosio", "100000000.00000000 WAX"]' -p eosio.token
cleos push action eosio.token issue '["eosio", "100000000.00000000 WAX", "init"]' -p eosio
cleos transfer eosio mycollection "500.00000000 WAX" ""
cleos transfer eosio blend.gems "500.00000000 WAX" ""
