#ifndef _COMNO_THROW_EXCEPTION_HPP_
#define _COMNO_THROW_EXCEPTION_HPP_

#include "comno/impl/exception/exception.h"

namespace comno
{
/**
 * helper for throw comno::exception with errno
 */
void throw_exception(int error_no)
{
    if( error_no != 0 )
        throw comno::exception(comno::system::error_code(error_no));
}

/**
 * helper for throw comno::exception with comno::system::error_code
 */
void throw_exception(const comno::system::error_code& err)
{
    if( !err )
        throw comno::exception(err);
}

}
#endif