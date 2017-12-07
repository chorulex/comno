#ifndef _QT_END_POINT_H_
#define _QT_END_POINT_H_

#include <string>

namespace QtSocket
{
struct EndPoint
{
    EndPoint() : port(0){}
    EndPoint(const EndPoint& src) : ip(src.ip), port(src.port){}
    EndPoint(const std::string& ip_, unsigned int port_) : ip(ip_), port(port_){}

    std::string ip;
    unsigned int port;
};

}
#endif
