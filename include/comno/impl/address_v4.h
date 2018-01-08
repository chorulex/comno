#ifndef _COMNO_ADDRESS_V4_H_
#define _COMNO_ADDRESS_V4_H_

#include <string>

#include <comno/detail/detail.h>

namespace comno
{
class address_v4
{
public:
    address_v4();
    address_v4(const address_v4&);

    // construct from network byte order address.
    explicit address_v4(detail::in_v4_addr_t);

    address_v4& operator= (const address_v4&);
    bool operator== (const address_v4&);
    bool operator!= (const address_v4&);

    /**
     * if ip not to set, or set to INADDR_ANY,
     * return "0.0.0.0"
     */
    std::string to_string() const;
    detail::in_v4_addr_t to_uint() const;

    static address_v4 any();
    static address_v4 from_string(const char* addr_str);
    static address_v4 from_string(const std::string& addr_str);

    template <typename Elem, typename Traits>
    friend std::basic_ostream<Elem, Traits>& operator<<(
        std::basic_ostream<Elem, Traits>& os, const address_v4& addr)
    {
        return os << addr.to_string().c_str();
    }

private:
    detail::in_v4_addr_t _addr;
};

}
#endif