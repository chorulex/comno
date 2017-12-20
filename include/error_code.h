#ifndef _COMNO_ERROR_CODE_H_
#define _COMNO_ERROR_CODE_H_

#include <errno.h>

namespace comno
{
class error_code
{
public:
    explicit error_code(int err) : _errno (err){}
    error_code(const error_code& src) : _errno(src._errno){}
    error_code& operator= (const error_code& src)
    {
        _errno = src._errno;
        return *this;
    }

public:
    operator int() { return _errno;}
    int value() const { return _errno;}
    const char* to_string() const;
    const char* details() const;

private:
    int _errno;
};

}
#endif
