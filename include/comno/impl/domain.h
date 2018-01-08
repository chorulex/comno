#ifndef _COMNO_DOMAIN_SOCKET_H_
#define _COMNO_DOMAIN_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>

#include "comno/impl/domain/basic_endpoint.h"
#include "acceptor.h"
#include "basic_socket.h"

namespace comno
{
namespace domain
{

class stream_protocol
{
public:
    using endpoint = comno::domain::basic_endpoint;

    using socket = comno::basic_socket<stream_protocol>;
    using acceptor = comno::basic_acceptor<stream_protocol>;

public:
    int family() const
    {
        return AF_UNIX;
    }
    int type() const
    {
        return SOCK_STREAM;
    }
    int protocol() const
    {
        return 0;
    }

};


}
}

#endif // end _COMNO_TCP_SOCKET_BASE_H_