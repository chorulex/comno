#ifndef _COMNO_TCP_SOCKET_H_
#define _COMNO_TCP_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>

#include "comno/impl/ip/basic_endpoint.h"
#include "acceptor.h"
#include "basic_socket.h"

namespace comno
{

class tcp
{
    #define PROTOCOL_TCP 6

public:
    using endpoint = comno::ip::basic_endpoint;

    using socket = comno::basic_socket<tcp>;
    using acceptor = comno::basic_acceptor<tcp>;

    int family() const
    {
        // AF_INET6;
        return AF_INET;
    }
    int type() const
    {
        return SOCK_STREAM;
    }
    int protocol() const
    {
        return PROTOCOL_TCP;
    }

protected:
};

}

#endif // end _COMNO_TCP_SOCKET_BASE_H_