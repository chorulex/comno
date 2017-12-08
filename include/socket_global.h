#ifndef _QT_SOCKET_GLOBAL_H_
#define _QT_SOCKET_GLOBAL_H_

#include <string>

namespace QtSocket
{
typedef int SocketFd;

extern void Close(const SocketFd sock_fd);

extern void SetBlock(const SocketFd sock_fd, bool blocked);
extern int Send(const SocketFd sock_fd, const std::string& buffer);
extern int Send(const SocketFd sock_fd, const char* buffer, int size);

extern void SetSockOpt(const SocketFd sock_fd, int level, int opt_name, const void* opt_val, socklen_t opt_len);
extern void GetSockOpt(const SocketFd sock_fd, int level, int opt_name, void* opt_val, socklen_t* opt_len);
}

#endif
