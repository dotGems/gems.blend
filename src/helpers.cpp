
namespace gems {

template <typename T>
void blend::clear_table( T& table )
{
    auto itr = table.begin();
    while ( itr != table.end() ) {
        itr = table.erase( itr );
    }
}

void blend::transfer( const name from, const name to, const extended_asset value, const string memo )
{
    eosio::token::transfer_action transfer( value.contract, { from, "active"_n });
    transfer.send( from, to, value.quantity, memo );
}

int blend::get_index( const vector<name> vec, const name value )
{
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == value ) return i;
    }
    return -1;
}

int blend::get_index( const vector<int32_t> vec, const int32_t value )
{
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == value ) return i;
    }
    return -1;
}

int blend::get_index( const vector<uint64_t> vec, const uint64_t value )
{
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == value ) return i;
    }
    return -1;
}

int blend::get_index(const vector<atomic::nft> vec, const atomic::nft value)
{
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i].collection_name == value.collection_name && vec[i].template_id == value.template_id ) return i;
    }
    return -1;
}

int blend::get_index(const vector<asset> vec, const symbol sym)
{
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i].symbol == sym ) return i;
    }
    return -1;
}

}