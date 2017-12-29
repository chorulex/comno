#ifndef _COMNO_ERROR_CODE_H_
#define _COMNO_ERROR_CODE_H_

#include <errno.h>

namespace comno
{
namespace system
{
class error_code
{
public:
    error_code() :_errno(0){}
    explicit error_code(int err) : _errno (err){}
    error_code(const error_code& src) : _errno(src._errno){}
    error_code& operator= (const error_code& src)
    {
        _errno = src._errno;
        return *this;
    }
    error_code& operator= (const int ec)
    {
        _errno = ec;
        return *this;
    }

public:
    operator int() { return _errno;}
    operator bool() { return _errno == 0;}
    bool operator! () { return _errno != 0;}

    int value() const { return _errno;}
    const char* to_string() const;
    const char* details() const;

private:
    int _errno;
};

}
}
#endif
