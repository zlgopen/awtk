#include "misc/new.hpp"

void* operator new(std::size_t size) throw(std::bad_alloc)
{
  return TKMEM_ALLOC(size);
}

void* operator new[](std::size_t size) throw(std::bad_alloc)
{
  return TKMEM_ALLOC(size);
}

void  operator delete(void* obj) throw() {
  TKMEM_FREE(obj);
}

void  operator delete[](void* obj) throw() {
  TKMEM_FREE(obj);
}

