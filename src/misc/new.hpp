#ifndef TK_NEW_H
#define TK_NEW_H

#include <new>
#include <cstddef>
#include "tkc/mem.h"

/*
 * 由于mem.c中导出了malloc等函数，没有必要重载new/delete等操作符，所以去掉misc/new.cpp|.hpp中的代码，但为了兼容保留文件。
*/
#if 0

#if __cplusplus < 201103L || defined(__ARMCC_VERSION)
#define __TK_THROW_BAD_ALLOC throw(std::bad_alloc)
#else
#define __TK_THROW_BAD_ALLOC
#endif

void* operator new(std::size_t size) __TK_THROW_BAD_ALLOC;

void* operator new[](std::size_t size) __TK_THROW_BAD_ALLOC;

void  operator delete(void* obj) throw();

void  operator delete[](void* obj) throw();

#endif/*HAS_STD_MALLOC*/

#endif/*TK_NEW_H*/

