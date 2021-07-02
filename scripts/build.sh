#!/bin/bash

# blanc++ blend.gems.cpp -I include
eosio-cpp blend.gems.cpp -I include

# unlock wallet & deploy
cleos wallet unlock --password $(cat ~/eosio-wallet/.pass)
cleos set contract blend.gems . blend.gems.wasm blend.gems.abi
