#ifndef _COMNO_SOCKET_GLOBAL_H_
#define _COMNO_SOCKET_GLOBAL_H_

#include <string>

#include "socket_t.h"

namespace comno
{
extern std::string host_name();

extern void close(const socket_t sock_fd);

extern void set_block(const socket_t sock_fd, bool blocked);
extern int send(const socket_t sock_fd, const std::string& buffer);
extern int send(const socket_t sock_fd, const char* buffer, int size);
}

#endif
