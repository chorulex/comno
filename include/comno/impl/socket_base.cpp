#include <memory>

#include "socket_base.h"
#include "socket_exception.h"

namespace comno
{

void socket_base::set_no_block()
{
    comno::set_block(_sock_fd, true);
}

void socket_base::set_block()
{
    comno::set_block(_sock_fd, false);
}

void socket_base::close()
{
    if( _sock_fd != -1 ){
        comno::close(_sock_fd);
        _sock_fd = -1;
    }
}

}
