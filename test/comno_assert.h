#ifndef _COMNO_ASSERT_SET_H_
#define _COMNO_ASSERT_SET_H_

#include <iostream>

#define ASSERT_PROMPT() \
        std::cout << "test failed >>>>>>" << std::endl;

#define ASSERT_LOCATION() \
        std::cout << " location:" << __FILE__ << ", line: " << __LINE__ << std::endl;\
        std::cout << " func    :" << __PRETTY_FUNCTION__ << std::endl;

#define ASSERT_REGULAR_VAL(expr, expect, expect_bool) \
    if( (expr != expect) == expect_bool ){\
        ASSERT_PROMPT();\
        ASSERT_LOCATION();\
        std::cout << " detail:  " << "expr:\"" << #expr << "\" expect-value: " << expect <<", real-value: " << expr<< std::endl;\
        exit(-1);\
    }
#define EQUAL(expr, expect) ASSERT_REGULAR_VAL(expr, expect, true)
#define NOT_EQUAL(expr, expect) ASSERT_REGULAR_VAL(expr, expect, false)

#define ASSERT_POINTER(expr, expect_null) \
    if( (expr == nullptr) == expect_null ){\
        ASSERT_PROMPT();\
        ASSERT_LOCATION();\
        if( expr ){\
            std::cout << " detail:  " << "expr:\"" << #expr << "\" result is null!" << std::endl;\
        }else{ \
            std::cout << " detail:  " << "expr:\"" << #expr << "\" result is not null!" << std::endl;\
        }\
        \
        exit(-1);\
    }
#define NULL_POINTER(expr) ASSERT_POINTER(expr, false)
#define NOT_NULL_POINTER(expr) ASSERT_POINTER(expr, true)


#endif