#ifndef _COMNO_SOCKET_OPTION_H_
#define _COMNO_SOCKET_OPTION_H_

#include <stdexcept> // C++11 std exception
#include <ctime>
#include <cstddef>
#include <errno.h>

#include "comno/impl/socket_t.h"

namespace comno
{
namespace option
{
template <int level_val, int name_val>
class option_context
{
public:
    int level() const
    {
        return level_val;
    }
    int name() const
    {
        return name_val;
    }
};

template <int level_val, int name_val>
class boolean_t : public option_context<level_val, name_val>
{
public:
    boolean_t() : _value(0)
    {
    }
    explicit boolean_t(bool val) : _value(val?1:0)
    {
    }
    boolean_t& operator= (bool val)
    {
        _value = val ? 1 : 0;
        return *this;
    }

public:
    std::size_t size() const 
    {
        return sizeof(_value);
    }
    void resize(const std::size_t& size_v)
    {
        if( size_v != size() ){
            throw std::length_error("integer socket option resize");
        }
    }
    int* data()
    {
        return &_value;
    }
    const int* data() const
    {
        return &_value;
    }
    bool value() const
    {
        return !!_value;
    }
    operator bool()
    {
        return !!_value;
    }
    bool operator! ()
    {
        return !_value;
    }

private:
    int _value;
};

template <int level_val, int name_val>
class integer_t : public option_context<level_val, name_val>
{
public:
    integer_t() : _value(0)
    {
    }
    explicit integer_t(int val) : _value(val)
    {
    }
    integer_t& operator= (int val)
    {
        _value = val;
        return *this;
    }

public:
    std::size_t size() const 
    {
        return sizeof(_value);
    }
    void resize(const std::size_t& size_v)
    {
        if( size_v != size() ){
            throw std::length_error("integer socket option resize");
        }
    }
    int* data()
    {
        return &_value;
    }
    const int* data() const
    {
        return &_value;
    }
    int value() const
    {
        return _value;
    }
    operator int()
    {
        return _value;
    }

private:
    int _value;
};

template <int level_val, int name_val>
class timeval_t : public option_context<level_val, name_val>
{
public:
    timeval_t()
    {
        _value.tv_sec = 0;
        _value.tv_usec = 0;
    }
    explicit timeval_t(std::time_t sec)
    {
        _value.tv_sec = sec;
        _value.tv_usec = 0;
    }
    timeval_t& operator= (std::time_t sec)
    {
        _value.tv_sec = sec;
        _value.tv_usec = 0;
        return *this;
    }

public:
    std::size_t size() const 
    {
        return sizeof(_value);
    }
    void resize(const std::size_t& size_v)
    {
        if( size_v != size() ){
            throw std::length_error("integer socket option resize");
        }
    }
    struct timeval* data()
    {
        return &_value;
    }
    const struct timeval* data() const
    {
        return &_value;
    }
    struct timeval value() const
    {
        return _value;
    }
    operator struct timeval()
    {
        return _value;
    }

private:
    struct timeval _value;
};
}
}

#endif