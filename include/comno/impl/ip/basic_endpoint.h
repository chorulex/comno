#ifndef _COMNO_IP_BASIC_ENDPOINT_H_
#define _COMNO_IP_BASIC_ENDPOINT_H_

#include "endpoint.h"

namespace comno
{
namespace ip
{
class basic_endpoint
{
public:
    basic_endpoint() : _impl()
    {
    }
    basic_endpoint(const basic_endpoint& src) : _impl(src._impl)
    {
    }
    basic_endpoint(const detail::in_v4_addr_t& addr, unsigned short port) : _impl(addr, port)
    {
    }
    basic_endpoint(const address_v4& addr, unsigned short port) : _impl(addr, port)
    {
    }

    const struct sockaddr *data() const
    {
        return _impl.data();
    }

    std::size_t size() const
    {
        return _impl.size();
    }

    address_v4 address() const
    {
        return _impl.address();
    }
    void address(const address_v4& addr)
    {
        _impl.address(addr);
    }
    void address(const detail::in_v4_addr_t& addr)
    {
        _impl.address(addr);
    }

    unsigned short port() const
    {
        return _impl.port();        
    }
    void port(unsigned short val)
    {
        _impl.port(val);
    }

private:
    comno::ip::endpoint _impl;
};

}
}
#endif