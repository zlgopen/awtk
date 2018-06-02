#ifndef TK_NEW_H
#define TK_NEW_H

#include <new>
#include <cstddef>
#include "base/mem.h"

void* operator new(std::size_t size) throw(std::bad_alloc);

void* operator new[](std::size_t size) throw(std::bad_alloc);

void  operator delete(void* obj) throw();

void  operator delete[](void* obj) throw();

#endif/*TK_NEW_H*/

