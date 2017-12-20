#ifndef _QT_SOCKET_GLOBAL_H_
#define _QT_SOCKET_GLOBAL_H_

#include <string>

namespace comno
{
typedef int SocketFd;

extern void close(const SocketFd sock_fd);

extern void set_block(const SocketFd sock_fd, bool blocked);
extern int send(const SocketFd sock_fd, const std::string& buffer);
extern int send(const SocketFd sock_fd, const char* buffer, int size);

extern void set_sock_opt(const SocketFd sock_fd, int level, int opt_name, const void* opt_val, socklen_t opt_len);
extern void get_sock_opt(const SocketFd sock_fd, int level, int opt_name, void* opt_val, socklen_t* opt_len);
}

#endif
