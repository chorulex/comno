#ifndef _QT_ERROR_CODE_H_
#define _QT_ERROR_CODE_H_

#include <errno.h>

namespace QtSocket
{
class ErrorCode
{
public:
    explicit ErrorCode(int err) : _errno (err){}
    ErrorCode(const ErrorCode& src) : _errno(src._errno){}
    ErrorCode& operator= (const ErrorCode& src)
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
