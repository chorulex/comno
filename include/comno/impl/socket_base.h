#ifndef _COMNO_SOCKET_BASE_H_
#define _COMNO_SOCKET_BASE_H_

#include <sys/types.h>
#include <sys/socket.h>

#include "socket_t.h"

namespace comno
{

class socket_base
{
public:
    const static int max_listen_connections = SOMAXCONN;

protected:

};

}
#endif