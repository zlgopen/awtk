#include "misc/new.hpp"

#define MAX_SIZE 2 * 1024 * 1024

void* operator new(std::size_t size) throw(std::bad_alloc) {
  if (size >= MAX_SIZE) {
    size = size;
  }

  return TKMEM_ALLOC(size);
}

void* operator new[](std::size_t size) throw(std::bad_alloc) {
  if (size >= MAX_SIZE) {
    size = size;
  }

  return TKMEM_ALLOC(size);
}

void operator delete(void* obj) throw() {
  TKMEM_FREE(obj);
}

void operator delete[](void* obj) throw() {
  TKMEM_FREE(obj);
}
