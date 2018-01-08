#ifndef _COMNO_BASE_H_
#define _COMNO_BASE_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace comno
{
namespace detail
{
    // network byte order address.
    using in_v4_addr_t = in_addr_t;
}
}
#endif