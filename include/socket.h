#ifndef _QT_SOCKET_H_
#define _QT_SOCKET_H_

#include <string>
#include <sys/socket.h>

#include "socket_global.h"

namespace comno
{
class socket
{
protected:
    socket() = default;

public:
    virtual ~socket() = default;

public:
    virtual void close();
    SocketFd fd() const { return _sock_fd;}

    void set_no_block();
    void set_block();

    void set_sock_opt(int level, int opt_name, const unsigned char opt_val);
    void set_sock_opt(int level, int opt_name, const int opt_val);
    void set_sock_opt(int level, int opt_name, const unsigned int opt_val);
    void set_sock_opt(int level, int opt_name, const struct timeval &opt_val);

    void get_sock_opt(int level, int opt_name, unsigned char &opt_val);
    void get_sock_opt(int level, int opt_name, int& opt_val);
    void get_sock_opt(int level, int opt_name, unsigned int &opt_val);
    void get_sock_opt(int level, int opt_name, struct timeval &opt_val);

protected:
    virtual int create_fd() = 0;

protected:
    SocketFd _sock_fd;
};

class tcp_socket : public socket
{
protected:
    tcp_socket() = default;

public:
    virtual ~tcp_socket() = default;

    int recv(char* buffer, int max_len);
    int send(const std::string& buffer);
    int send(const char* buffer, int size);

    void   set_recv_timeout(time_t timeout); //seconds
    time_t get_recv_timeout(); //seconds

    void   set_send_timeout(time_t timeout); //seconds
    time_t get_send_timeout(); //seconds

protected:
    virtual int create_fd() override;
};

}
#endif
