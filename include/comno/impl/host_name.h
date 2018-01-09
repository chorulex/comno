#ifndef _COMNO_HOST_NAME_H_
#define _COMNO_HOST_NAME_H_

#include <unistd.h>
#include <string>

#include "comno/impl/exception/throw_exception.hpp"

namespace comno
{

std::string host_name()
{
    char name[1024];
    ::gethostname(name, sizeof(name)-1);
    comno::throw_exception(errno);

    return std::string(name);
}

}
#endif