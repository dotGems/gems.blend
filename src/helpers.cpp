
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

void blend::update_status( const uint32_t index, const uint32_t count )
{
    status_table _status( get_self(), get_self().value );
    auto status = _status.get_or_default();

    if ( status.counters.size() <= index ) status.counters.resize( index + 1);
    status.counters[index] += count;
    status.last_updated = current_time_point();
    _status.set( status, get_self() );
}

}