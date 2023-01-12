/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef _UnitTest_
#define _UnitTest_

#include <iostream>

#define DECLARE_ASSERT_COUNTER \
uint16_t okCount = 0; \
uint16_t failCount = 0;

#define ASSERT_UINT8_T( r, e, m) \
do { \
uint8_t res = (r); \
uint8_t exp = (e); \
if( res == exp ) { \
    std::cout << "OK " << m << std::endl; \
    okCount++; \
} else { \
    std::cout << "FAIL " << m << " result:" << (int)res << " expected:" << (int)exp << std::endl; \
    failCount++; \
} \
} while( false )

#define ASSERT_INT8_T( r, e, m) \
do { \
int8_t res = (r); \
int8_t exp = (e); \
if( res == exp ) { \
    std::cout << "OK " << m << std::endl; \
    okCount++; \
} else { \
    std::cout << "FAIL " << m << " result:" << (int)res << " expected:" << (int)exp << std::endl; \
    failCount++; \
} \
} while( false )

#define ASSERT_UINT16_T( r, e, m) \
do { \
uint16_t res = (r); \
uint16_t exp = (e); \
if( res == exp ) { \
    std::cout << "OK " << m << std::endl; \
    okCount++; \
} else { \
    std::cout << "FAIL " << m << " result:" << (int)res << " expected:" << (int)exp << std::endl; \
    failCount++; \
} \
} while( false )

#define ASSERT_INT16_T( r, e, m) \
do { \
int16_t res = (r); \
int16_t exp = (e); \
if( res == exp ) { \
    std::cout << "OK " << m << std::endl; \
    okCount++; \
} else { \
    std::cout << "FAIL " << m << " result:" << (int)res << " expected:" << (int)exp << std::endl; \
    failCount++; \
} \
} while( false )

#define ASSERT_TEXT_T( r, e, m) \
do { \
const char* res = (r); \
const char* exp = (e); \
if( strcmp( res, exp) == 0 ) { \
    std::cout << "OK " << m << std::endl; \
    okCount++; \
} else { \
    std::cout << "FAIL " << m << " result:" << res << " expected:" << exp << std::endl; \
    failCount++; \
} \
} while( false )

class UnitTest {

    public:
        virtual void run() = 0;
};

#endif
