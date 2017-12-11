#ifndef _QT_SOCKET_H_
#define _QT_SOCKET_H_

#include <string>
#include <sys/socket.h>

#include "socket_global.h"

namespace QtSocket
{
#define REPORT_ERR() printf("errno=%d:%s\n", errno, strerror(errno));

class Socket
{
protected:
    Socket() = default;

public:
    virtual ~Socket() = default;

public:
    virtual void Close();
    SocketFd FD() const { return _sock_fd;}

    void SetNoBlock();
    void SetBlock();

    void SetSockOpt(int level, int opt_name, const unsigned char opt_val);
    void SetSockOpt(int level, int opt_name, const int opt_val);
    void SetSockOpt(int level, int opt_name, const unsigned int opt_val);
    void SetSockOpt(int level, int opt_name, const struct timeval &opt_val);

    void GetSockOpt(int level, int opt_name, unsigned char &opt_val);
    void GetSockOpt(int level, int opt_name, int& opt_val);
    void GetSockOpt(int level, int opt_name, unsigned int &opt_val);
    void GetSockOpt(int level, int opt_name, struct timeval &opt_val);

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
    time_t GetRecvTimeOut(); //seconds

    void SetSendTimeOut(time_t timeout); //seconds
    time_t GetSendTimeOut(); //seconds

protected:
    virtual int CreateFD() override;
};

}
#endif
