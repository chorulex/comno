#include <sys/un.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "tcp_socket.h"
#include "utility.h"


bool EtSocket::SetNoBlock(SocketFd fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if( flags < 0 )
        return false;
        
    return fcntl(fd, F_SETFL, O_NONBLOCK) != -1;
}

bool EtSocket::SetBlock(SocketFd fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if( flags < 0 )
        return false;
        
    return fcntl(fd, F_SETFL, O_NONBLOCK) != -1;
}

void EtSocket::SetRecvTimeOut(int socket, time_t timeout)//seconds
{
    struct timeval time_out = {timeout,0};
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&time_out, sizeof(struct timeval));
}

void EtSocket::SetSendTimeOut(int socket, time_t timeout)//seconds
{
    struct timeval time_out = {timeout,0};
    setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&time_out, sizeof(struct timeval));
}

int EtSocket::Recv(SocketFd& socket_df, char* buffer, int max_len)
{
    int res = recv(socket_df, buffer, max_len, 0);
    if (res == -1 ) {
        //LOG_WARN(main_logger, "Failed to recv. err=%s.", GetLastError().c_str());
    }
    else{
        //LOG_DEBUG(main_logger, "size:%d.", res);
    }

    return res;
}

int EtSocket::Send(SocketFd& socket_df, const char* buffer, int size)
{
    int res = send(socket_df, buffer, size, 0);
    if (res == -1 ) {
        //LOG_WARN(main_logger, "Failed to send. err=%s.", GetLastError().c_str());
    }
    else{
        //LOG_DEBUG(main_logger, "size:%d.", res);
    }

    return res;
}

int EtTCPSocket::CreateSocket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

void EtTCPSocket::Close()
{
    close(m_sock_fd);
}

bool EtTCPServerSocket::Listen(const int port)
{
    m_sock_fd = CreateSocket();
    if (m_sock_fd < 0) {
        //LOG_FATAL(main_logger, "Failed to open listen socket.");
        return false;
    }

    ReuseAddr(true);
    if ( Bind(port) ) {
        //LOG_FATAL(main_logger, "Failed to bind the server port %d! err=%s.", port, GetLastError().c_str());
        return false;
    }

    int res = listen(m_sock_fd, 10);
    if (res < 0) {
        //LOG_FATAL(main_logger, "Failed to listen to the server. err=%s.", GetLastError().c_str());
        return false;
    }

    return true;
}

bool EtTCPServerSocket::Bind(const int port)
{
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    return bind(m_sock_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1;
}

void EtTCPServerSocket::ReuseAddr(bool use)
{
    int yes = use ? 1 : 0;
    setsockopt(m_sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (yes));
}

SocketFd EtTCPServerSocket::Accept()
{
    struct sockaddr_in clientAddr;
    socklen_t cliLen = sizeof(clientAddr);

    int clientSocket = accept(m_sock_fd, (struct sockaddr *)&clientAddr, &cliLen);
    if (clientSocket == -1 ) {
        //LOG_WARN(main_logger, "Failed to accept. err=%s.", GetLastError().c_str());
        return -1;
    }

    return clientSocket;
}

// host: ip:port
bool EtTCPClientSocket::Connect(const std::string& host, time_t timeout_sec)
{
    std::string::size_type pos = host.find(':');
    if( pos == std::string::npos ) {
        return false;
    }

    if( pos == host.size() - 1 )
        return false;

    std::string ip = host.substr(0, pos);
    int port = 0;
    try{
        port = std::stoi(host.substr(pos+1));
    }
    catch(std::exception& ex){
        throw ex;
    }

    return Connect(ip.c_str(), port, timeout_sec);
}

bool EtTCPClientSocket::Connect(const char* ip, const int port, time_t timeout_sec)
{
    int orig_sock = EtTCPSocket::CreateSocket();
    if ( orig_sock == -1 )
        return false;

    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port    = htons(port);
    if (inet_aton(ip, &server_address.sin_addr) == 0)
        return false;

    // until to success or fail
    if( timeout_sec == 0 ){
        if (connect(orig_sock, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
            close(orig_sock);
            return false;
        }
    }else{
        EtSocket::SetNoBlock(orig_sock);
        int ret = connect(orig_sock, (sockaddr*)&server_address, sizeof(server_address));
        if( ret == -1 ){
            if( ConnectTimeout(orig_sock, timeout_sec) ){
                close(orig_sock);
                return false;
            }
        }

        EtSocket::SetBlock(orig_sock);
    }

    m_sock_fd = orig_sock;
    return true;
}

bool EtTCPClientSocket::ConnectTimeout(int orig_sock, time_t timeout_sec)
{
    struct timeval time_val;
    time_val.tv_sec = timeout_sec;
    time_val.tv_usec = 0;

    fd_set fset;
    FD_ZERO(&fset);
    FD_SET(orig_sock, &fset);

    int res = select(orig_sock+1, NULL, &fset, NULL, &time_val);
    if( res > 0 ){
        int error = -1;
        size_t err_len = sizeof(error);
        getsockopt( orig_sock, SOL_SOCKET, SO_ERROR, &error, (socklen_t*)&err_len);
        if( error != 0 ){
            return true;
        }
    }

    return false;
}
