#ifndef _COMNO_DOMAIN_BASIC_ENDPOINT_H_
#define _COMNO_DOMAIN_BASIC_ENDPOINT_H_

#include "comno/impl/domain/endpoint.h"

namespace comno
{
namespace domain
{

class basic_endpoint
{
public:
    basic_endpoint() : _impl()
    {
    }
    basic_endpoint(const char* path) : _impl(path)
    {
    }
    basic_endpoint(const std::string& path) : _impl(path)
    {
    }
    basic_endpoint(const basic_endpoint& src) : _impl(src._impl)
    {
    }

public:
    const struct sockaddr* data() const
    {
        return _impl.data();
    }
    const char* path() const
    {
        return _impl.path();
    }
    std::size_t size() const
    {
        return _impl.size();
    }
    void path(const char* path)
    {
        _impl.path(path);
    }
    void path(const std::string& path)
    {
        _impl.path(path);
    }

private:
    comno::domain::detail::endpoint _impl;
};

}
}

#endif