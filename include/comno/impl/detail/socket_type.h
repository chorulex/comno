#ifndef _COMNO_SOCKET_TYPES_H_
#define _COMNO_SOCKET_TYPES_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace comno
{

namespace type
{
    // network byte order address.
    using in_v4_addr_t = in_addr_t;

    using sockaddr_base  = struct sockaddr;
    using sockaddr_in_v4 = struct sockaddr_in;
};

}
#endif