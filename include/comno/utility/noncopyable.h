#ifndef _COMNO_UTILS_NONCOPYABLE_H_
#define _COMNO_UTILS_NONCOPYABLE_H_

namespace comno
{
namespace utility
{
class noncopyable
{
protected:
    noncopyable() = default;
    virtual ~ noncopyable() = default;

public:
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator= (const noncopyable&) = delete;
};

}
}
#endif