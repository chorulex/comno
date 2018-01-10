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
template <int level_val, int name_val, typename T>
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

    T* data()
    {
        return &_value;
    }

    const T* data() const
    {
        return &_value;
    }

protected:
    T _value;
};

template <int level_val, int name_val>
class boolean_t : public option_context<level_val, name_val, int>
{
public:
    boolean_t()
    {
        this->_value = 0;
    }
    explicit boolean_t(bool val)
    {
        this->_value = val ? 1 : 0;
    }
    boolean_t& operator= (bool val)
    {
        this->_value = val ? 1 : 0;
        return *this;
    }

public:
    bool value() const
    {
        return !!this->_value;
    }
    operator bool()
    {
        return !!this->_value;
    }
    bool operator! ()
    {
        return !this->_value;
    }
};

template <int level_val, int name_val>
class integer_t : public option_context<level_val, name_val, int>
{
public:
    integer_t()
    {
        this->_value = 0;
    }
    explicit integer_t(int val)
    {
        this->_value = val;
    }
    integer_t& operator= (int val)
    {
        this->_value = val;
        return *this;
    }

public:
    int value() const
    {
        return this->_value;
    }
    operator int()
    {
        return this->_value;
    }
};

template <int level_val, int name_val>
class timeval_t : public option_context<level_val, name_val, struct timeval>
{
public:
    timeval_t()
    {
        this->_value.tv_sec = 0;
        this->_value.tv_usec = 0;
    }
    explicit timeval_t(std::time_t sec)
    {
        timeout(sec);
        this->_value.tv_usec = 0;
    }
    timeval_t& operator= (std::time_t sec)
    {
        timeout(sec);
        this->_value.tv_usec = 0;
        return *this;
    }

public:
    void timeout(std::time_t t)
    {
        this->_value.tv_sec = t;
    }

    std::time_t timeout() const
    {
        return this->_value.tv_sec;
    }

    struct timeval value() const
    {
        return this->_value;
    }
    operator struct timeval()
    {
        return this->_value;
    }
};

/**
 * SO_LINGER option,
 * For TCP/SCTP, not UDP. 
 * 
 * sepcify timeout units as seconds.
 */
template <int level_val, int name_val>
class linger_t : public option_context<level_val, name_val, struct linger>
{
public:
    linger_t()
    {
        this->_value.l_onoff = 0;
        this->_value.l_linger = 0;
    }
    linger_t(bool e, int t)
    {
        enable(e);
        timeout(t);
    }

public:
    void enable(bool e)
    {
        this->_value.l_onoff = enable ? 1 : 0;
    }

    bool enable() const
    {
        return this->_value.l_onoff != 0;
    }

    void timeout(int t)
    {
        this->_value.l_linger = t;
    }

    int timeout() const
    {
        return this->_value.l_linger;
    }

    struct linger value() const
    {
        return this->_value;
    }

    operator struct linger()
    {
        return this->_value;
    }
};

}
}

#endif