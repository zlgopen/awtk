#include "misc/new.hpp"

/*
 * 由于mem.c中导出了malloc等函数，没有必要重载new/delete等操作符，所以去掉misc/new.cpp|.hpp中的代码，但为了兼容保留文件。
*/
#if 0
#define MAX_SIZE 2 * 1024 * 1024

#ifndef WITH_SDL
void* operator new(std::size_t size) __TK_THROW_BAD_ALLOC {
  if (size >= MAX_SIZE) {
    log_debug("size is too large\n");
  }

  return TKMEM_ALLOC(size);
}

void* operator new[](std::size_t size) __TK_THROW_BAD_ALLOC {
  if (size >= MAX_SIZE) {
    log_debug("size is too large\n");
  }

  return TKMEM_ALLOC(size);
}

void operator delete(void* obj) throw() {
  TKMEM_FREE(obj);
}

void operator delete[](void* obj) throw() {
  TKMEM_FREE(obj);
}
#endif /*WITH_SDL*/

#endif /*HAS_STD_MALLOC*/
