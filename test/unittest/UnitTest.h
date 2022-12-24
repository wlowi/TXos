
#ifndef _UnitTest_
#define _UnitTest_

#include <iostream>

#define ASSERT_UINT8_T( r, e, m) \
do { \
uint8_t res = (r); \
uint8_t exp = (e); \
if( res == exp ) { \
    std::cout << "OK " << m << std::endl; \
} else { \
    std::cout << "FAIL " << m << " result:" << (int)res << " expected:" << (int)exp << std::endl; \
} \
} while( false )

#define ASSERT_INT8_T( r, e, m) \
do { \
int8_t res = (r); \
int8_t exp = (e); \
if( res == exp ) { \
    std::cout << "OK " << m << std::endl; \
} else { \
    std::cout << "FAIL " << m << " result:" << (int)res << " expected:" << (int)exp << std::endl; \
} \
} while( false )

#define ASSERT_UINT16_T( r, e, m) \
do { \
uint16_t res = (r); \
uint16_t exp = (e); \
if( res == exp ) { \
    std::cout << "OK " << m << std::endl; \
} else { \
    std::cout << "FAIL " << m << " result:" << (int)res << " expected:" << (int)exp << std::endl; \
} \
} while( false )

#define ASSERT_INT16_T( r, e, m) \
do { \
int16_t res = (r); \
int16_t exp = (e); \
if( res == exp ) { \
    std::cout << "OK " << m << std::endl; \
} else { \
    std::cout << "FAIL " << m << " result:" << (int)res << " expected:" << (int)exp << std::endl; \
} \
} while( false )

#define ASSERT_TEXT_T( r, e, m) \
do { \
const char* res = (r); \
const char* exp = (e); \
if( strcmp( res, exp) == 0 ) { \
    std::cout << "OK " << m << std::endl; \
} else { \
    std::cout << "FAIL " << m << " result:" << res << " expected:" << exp << std::endl; \
} \
} while( false )

class UnitTest {

    public:
        virtual void run() = 0;
};

#endif
