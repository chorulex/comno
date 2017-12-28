#ifndef _COMNO_END_POINT_H_
#define _COMNO_END_POINT_H_

#include <string>

namespace comno
{
struct end_point
{
    end_point() : port(0){}
    end_point(const end_point& src) : ip(src.ip), port(src.port){}
    end_point(const std::string& ip_, unsigned int port_) : ip(ip_), port(port_){}

    std::string ip;
    unsigned int port;
};

}
#endif
