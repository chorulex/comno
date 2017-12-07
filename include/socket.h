#ifndef _QT_SOCKET_H_
#define _QT_SOCKET_H_

#include <string>
#include <sys/socket.h>

namespace QtSocket
{
#define REPORT_ERR() printf("errno=%d:%s\n", errno, strerror(errno));

typedef int SocketFd;

class Socket
{
protected:
    Socket() = default;

public:
    virtual ~Socket() = default;

public:
    void Close();
    SocketFd FD() const { return _sock_fd;}

    bool SetNoBlock();
    bool SetBlock();

    void SetSockOpt(int level, int opt_name, const void* opt_val, socklen_t opt_len);
    void GetSockOpt(int level, int opt_name, void* opt_val, socklen_t* opt_len);

protected:
    virtual int CreateFD() = 0;

protected:
    SocketFd _sock_fd;
};

class TCPSocket : public Socket
{
protected:
    TCPSocket() = default;

public:
    virtual ~TCPSocket() = default;

    int Recv(char* buffer, int max_len);
    int Send(const std::string& buffer);
    int Send(const char* buffer, int size);

    void SetRecvTimeOut(time_t timeout); //seconds
    void SetSendTimeOut(time_t timeout); //seconds

protected:
    int CreateFD() override;
};

}
#endif
