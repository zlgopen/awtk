#include "misc/new.hpp"

#define MAX_SIZE 2 * 1024 * 1024

#ifndef HAS_STD_MALLOC

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
