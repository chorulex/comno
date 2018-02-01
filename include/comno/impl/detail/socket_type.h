#ifndef _COMNO_SOCKET_TYPES_H_
#define _COMNO_SOCKET_TYPES_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

namespace comno
{

namespace type
{
    // socket fd type.
    using socket_type = int;
    const static socket_type invalid_socket_value = -1;

    // network byte order address.
    using in_v4_addr_t = in_addr_t;

    using socket_len_t = socklen_t;
    using sockaddr_base  = struct sockaddr;
    using sockaddr_in_v4 = struct sockaddr_in;
};

}
#endif