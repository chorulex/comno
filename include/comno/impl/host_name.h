#ifndef _COMNO_HOST_NAME_H_
#define _COMNO_HOST_NAME_H_

#include <unistd.h>
#include <string>

#include "comno/impl/exception/exception.h"

namespace comno
{

std::string host_name()
{
    char name[1024];
    if( ::gethostname(name, sizeof(name)-1) != 0 ){
        throw comno::exception(system::error_code(errno));
        return "";
    }

    return std::string(name);
}

}
#endif