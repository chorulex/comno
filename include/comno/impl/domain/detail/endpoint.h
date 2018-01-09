#ifndef _COMNO_DOMAIN_ENDPOINT_H_
#define _COMNO_DOMAIN_ENDPOINT_H_

#include <string>
#include <cstddef>

#include <sys/un.h>
#include "comno/impl/exception/throw_exception.hpp"
#include "comno/impl/address_v4.h"

namespace comno
{
namespace domain
{
namespace detail
{

class endpoint
{
public:
    endpoint()
    {
        init(nullptr, 0);
    }
    endpoint(const char* path) : _path(path)
    {
        init(path, strlen(path));
    }
    endpoint(const std::string& path) : _path(path)
    {
        init(path.c_str(), path.size());
    }
    endpoint(const endpoint& src) : _path(src._path)
    {
        _path = src._path;
        memcpy(&_addr, &src._addr, src.size());
    }

public:
    const struct sockaddr* data() const
    {
        return &_addr.base;
    }
    const char* path() const
    {
        return _path.c_str();
    }
    std::size_t size() const
    {
        return _path.size() + offsetof(struct sockaddr_un, sun_path);
    }
    void path(const char* path)
    {
        init(path, strlen(path));
    }
    void path(const std::string& path)
    {
        init(path.c_str(), path.size());
    }

private:
    void init(const char* path, std::size_t size)
    {
        if( size > sizeof(_addr.local.sun_path) - 1)
            throw_exception(ENAVAIL);

        memset(&_addr, 0, sizeof(_addr));
        _addr.local.sun_family = AF_UNIX;

        if( path != nullptr && size > 0 )
            _path.assign(path, size);
        memcpy(_addr.local.sun_path, _path.c_str(), _path.size());
    }

private:
    union addr_union
    {
        struct sockaddr base;
        struct sockaddr_un local;
    }_addr;

    std::string _path;
};

}
}
} // end comno
#endif //_COMNO_ENDPOINT_H_
