#ifndef _COMNO_SOCKET_TYPE_H_
#define _COMNO_SOCKET_TYPE_H_

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
using socket_t = int;
}

#endif
