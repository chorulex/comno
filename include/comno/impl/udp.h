#ifndef _COMNO_UDP_H_
#define _COMNO_UDP_H_

#include "comno/impl/ip/basic_endpoint.h"
#include "comno/impl/basic_datagram_socket.h"

namespace comno
{
class udp
{
public:
    using endpoint = comno::ip::basic_endpoint;
    using socket = comno::basic_datagram_socket<udp>;

    int family() const
    {
        // AF_INET6;
        return AF_INET;
    }
    int type() const
    {
        return SOCK_DGRAM;
    }
    int protocol() const
    {
        return IPPROTO_UDP;
    }
};

}

#endif