#ifndef _ET_TCP_CLIENT_SOCKET_H_
#define _ET_TCP_CLIENT_SOCKET_H_

#include "end_point.h"
#include "socket.h"

namespace QtSocket
{
class TCPClient : public TCPSocket
{
public:
    TCPClient() noexcept;
    TCPClient(const SocketFd sock_fd)noexcept;
    TCPClient(const SocketFd sock_fd, const EndPoint& src, const EndPoint& dest)noexcept;

    /**
     * not close socket fd on this calling.
     * close fd manually.
     */
    ~TCPClient() noexcept;

public:
    bool Connect(const EndPoint& host, time_t timeout_sec = 0);

    EndPoint DestEndPoint() const { return _dest_end_point;}
    std::string DestIP() const { return _dest_end_point.ip;}
    unsigned int DestPort() const { return _dest_end_point.port;}

    EndPoint SrcEndPoint() const { return _src_end_point;}
    std::string SrcIP() const { return _src_end_point.ip;}
    unsigned int SrcPort() const { return _src_end_point.port;}

private:
    bool ConnectTimeout(time_t timeout_sec);

private:
    EndPoint _dest_end_point;
    EndPoint _src_end_point;
};

}

#endif
