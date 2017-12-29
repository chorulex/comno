#include <memory>

#include "socket_base.h"
#include "socket_exception.h"

namespace comno
{

void socket_base::block(bool val)
{
    comno::set_block(_sock_fd, val);
}

void socket_base::close()
{
    if( _sock_fd != -1 ){
        comno::close(_sock_fd);
        _sock_fd = -1;
    }
}

}
