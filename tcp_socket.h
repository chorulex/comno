#ifndef _ET_TCP_SOCKET_H_
#define _ET_TCP_SOCKET_H_

#include <string>

typedef int SocketFd;

class EtSocket
{
protected:
    EtSocket() = default;
public:
    virtual ~EtSocket()
    {
    }

    int Socket() const { return m_sock_fd;}

public:
    static bool SetNoBlock(SocketFd fd);
    static bool SetBlock(SocketFd fd);
    static void SetRecvTimeOut(int socket, time_t timeout); //seconds
    static void SetSendTimeOut(int socket, time_t timeout); //seconds

    static int Recv(SocketFd& socket_df, char* buffer, int max_len);
    static int Send(SocketFd& socket_df, const char* buffer, int size);

protected:
    virtual int CreateSocket() = 0;

protected:
    SocketFd m_sock_fd;
};

class EtTCPSocket : public EtSocket
{
protected:
    EtTCPSocket() = default;

public:
    virtual ~EtTCPSocket() {}

    void Close();
    int Recv(char* buffer, int max_len)
    {
        return EtSocket::Recv(m_sock_fd, buffer, max_len);
    }

    int Send(const char* buffer, int size)
    {
        return EtSocket::Send(m_sock_fd, buffer, size);
    }

    bool SetNoBlock()
    {
        return EtSocket::SetNoBlock(m_sock_fd);
    }
    bool SetBlock(SocketFd fd)
    {
        return EtSocket::SetBlock(m_sock_fd);
    }

    void SetRecvTimeOut(time_t timeout) //seconds
    {
        EtSocket::SetRecvTimeOut(m_sock_fd, timeout);
    }
    void SetSendTimeOut(time_t timeout) //seconds
    {
        EtSocket::SetSendTimeOut(m_sock_fd, timeout);
    }

protected:
    int CreateSocket() override;
};

class EtTCPClientSocket : public EtTCPSocket
{
public:
    // host: ip:port
    bool Connect(const std::string& host, time_t timeout_sec);
    bool Connect(const char* ip, const int port, time_t timeout_sec);

private:
    bool ConnectTimeout(int orig_sock, time_t timeout_sec);

private:
    std::string m_srv_ip;
    int m_srv_port;
};

class EtTCPServerSocket : public EtTCPSocket
{
public:
    bool Listen(const int port);
    SocketFd Accept();

private:
    bool Bind(const int port);
    void ReuseAddr(bool use);
};

#endif

