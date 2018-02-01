#include <cstring>

#include "comno/impl/detail/socket_ops.h"
#include "comno/impl/exception/throw_exception.hpp"
#include "address_v4.h"

namespace comno
{
address_v4::address_v4()
{
    _addr = comno::detail::socket_ops::host_to_network_long(INADDR_ANY);
}

address_v4::address_v4(comno::type::in_v4_addr_t addr)
{
    _addr = addr;
}

address_v4::address_v4(const address_v4& src) : _addr(src._addr)
{
}

address_v4& address_v4::operator= (const address_v4& src)
{
    _addr = src._addr;
    return *this;
}

std::string address_v4::to_string() const
{
    struct in_addr addr = {_addr};
    return ::inet_ntoa(addr);
}

comno::type::in_v4_addr_t address_v4::to_uint() const
{
    return _addr;
}

address_v4 address_v4::any()
{
    return address_v4();
}

address_v4 address_v4::from_string(const std::string& addr_str)
{
    return from_string(addr_str.c_str());
}

address_v4 address_v4::from_string(const char* addr_str)
{
    struct in_addr addr;
    if(::inet_aton(addr_str, &addr) == 0){
        comno::throw_exception(EINVAL);
    }

    return address_v4(addr.s_addr);
}

bool address_v4::operator== (const address_v4& other)
{
    return _addr == other._addr;
}

bool address_v4::operator!= (const address_v4& other)
{
    return !this->operator==(other);
}
}