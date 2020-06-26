#include "tkc/mem_allocator_pool.h"
#include "tkc/mem_allocator_simple.h"

void allocator_test_basic() {
  uint32_t i = 0;
  char buff[102400];
  void* addr = NULL;
  void* addrs[100];
  mem_allocator_simple_t simple;
  mem_allocator_pool_t pool;
  mem_allocator_t* allocator = mem_allocator_simple_init(&simple, buff, sizeof(buff));
  allocator = mem_allocator_pool_init(&pool, allocator, 10, 10, 10, 10, 10);

  for (i = 0; i < ARRAY_SIZE(addrs); i++) {
    addr = mem_allocator_alloc(allocator, i + 1, __FUNCTION__, __LINE__);
    assert(addr != NULL);
    addr = mem_allocator_realloc(allocator, addr, i + 1, __FUNCTION__, __LINE__);
    assert(addr != NULL);
    addr = mem_allocator_realloc(allocator, addr, i + 5, __FUNCTION__, __LINE__);
    assert(addr != NULL);
    addrs[i] = addr;
  }

  for (i = 0; i < ARRAY_SIZE(addrs); i++) {
    mem_allocator_free(allocator, addrs[i]);
  }

  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    assert(pool.pools[i]->used == 0);
  }
}

void allocator_test_rand() {
  uint32_t k = 0;
  uint32_t i = 0;
  char buff[1000 * 1000];
  void* addr = NULL;
  void* addrs[1000];
  mem_allocator_simple_t simple;
  mem_allocator_pool_t pool;
  mem_allocator_t* allocator = mem_allocator_simple_init(&simple, buff, sizeof(buff));
  allocator = mem_allocator_pool_init(&pool, allocator, 10, 10, 10, 10, 10);

  for (k = 0; k < 1000; k++) {
    for (i = 0; i < ARRAY_SIZE(addrs); i++) {
      uint32_t size = random() % 100;
      addr = mem_allocator_alloc(allocator, size, __FUNCTION__, __LINE__);
      assert(addr != NULL);
      addr = mem_allocator_realloc(allocator, addr, size, __FUNCTION__, __LINE__);
      assert(addr != NULL);
      addr = mem_allocator_realloc(allocator, addr, size + 10, __FUNCTION__, __LINE__);
      assert(addr != NULL);
      addrs[i] = addr;
    }

    for (i = 0; i < ARRAY_SIZE(addrs); i++) {
      mem_allocator_free(allocator, addrs[i]);
    }
  }
  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    assert(pool.pools[i]->used == 0);
  }
}

void allocator_test() {
  allocator_test_basic();
  allocator_test_rand();
}
