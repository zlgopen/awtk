#ifndef TK_NEW_H
#define TK_NEW_H

#include <cstddef>
#include "base/mem.h"

inline void* operator new(std::size_t size) {
  return TKMEM_ALLOC(size);
}

inline void* operator new[](std::size_t size) {
  return TKMEM_ALLOC(size);
}

inline void  operator delete(void* obj) {
  TKMEM_FREE(obj);
}

inline void  operator delete[](void* obj) {
  TKMEM_FREE(obj);
}

#endif/*TK_NEW_H*/

