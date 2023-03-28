/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
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
