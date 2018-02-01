#ifndef _COMNO_IP_ENDPOINT_H_
#define _COMNO_IP_ENDPOINT_H_

#include <cstring>
#include <string>

#include "comno/impl/detail/socket_ops.h"
#include "comno/impl/detail/socket_type.h"
#include "comno/impl/address_v4.h"

namespace comno
{
namespace ip
{
class endpoint
{
public:
    endpoint()
    {
        ::memset(&_addr, 0, sizeof(_addr));
        _addr.v4.sin_family = AF_INET;
    }
    endpoint(const endpoint& src) : _addr(src._addr)
    {
    }
    endpoint(const comno::type::in_v4_addr_t& addr, unsigned short port)
    {
        ::memset(&_addr, 0, sizeof(_addr));
        _addr.v4.sin_family = AF_INET;

        _addr.v4.sin_addr.s_addr = addr;
        _addr.v4.sin_port = comno::detail::socket_ops::host_to_network_short(port);
    }
    endpoint(const address_v4& addr, unsigned short port)
    {
        ::memset(&_addr, 0, sizeof(_addr));
        _addr.v4.sin_family = AF_INET;

        _addr.v4.sin_addr.s_addr = addr.to_uint();
        _addr.v4.sin_port = comno::detail::socket_ops::host_to_network_short(port);
    }

    struct sockaddr *data()
    {
        return &_addr.base;
    }

    const struct sockaddr *data() const
    {
        return &_addr.base;
    }

    std::size_t size() const
    {
        return sizeof(_addr.v4);
    }

    address_v4 address() const
    {
        return address_v4(_addr.v4.sin_addr.s_addr);
    }
    void address(const address_v4& addr)
    {
        _addr.v4.sin_addr.s_addr = addr.to_uint();
    }
    void address(const comno::type::in_v4_addr_t& addr)
    {
        _addr.v4.sin_addr.s_addr = addr;
    }

    unsigned short port() const
    {
        return comno::detail::socket_ops::network_to_host_short(_addr.v4.sin_port);
    }
    void port(unsigned short val)
    {
        _addr.v4.sin_port = comno::detail::socket_ops::host_to_network_short(val);
    }

private:
    union addr_union
    {
        comno::type::sockaddr_base    base;
        comno::type::sockaddr_in_v4   v4;
        // reserved for v6
    }_addr;
};

} // end ip
} // end comno
#endif //_COMNO_ENDPOINT_H_
