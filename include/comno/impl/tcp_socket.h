#ifndef _COMNO_TCP_SOCKET_BASE_H_
#define _COMNO_TCP_SOCKET_BASE_H_

#include "socket_base.h"

namespace comno
{
class tcp_socket : public socket_base
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

#endif // end _COMNO_TCP_SOCKET_BASE_H_