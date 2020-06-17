#include "gtest/gtest.h"
#include "tkc/mem_pool.h"

extern "C" void allocator_test();

TEST(MemPool, basic) {
  allocator_test();
}

TEST(MemPool, basic1) {
  uint8_t buff[256];
  void* addr = NULL;
  mem_pool_t* pool = mem_pool_init(buff, sizeof(buff), 8, 1);
  ASSERT_EQ(pool->block_nr, 1);
  ASSERT_EQ(pool->block_size, 8);
  ASSERT_EQ(pool->used, 0);
  ASSERT_EQ(pool->bits_size, 2);

  addr = mem_pool_get(pool);
  ASSERT_EQ(addr != NULL, true);
  ASSERT_EQ(mem_pool_put(pool, addr), RET_OK);
  ASSERT_NE(mem_pool_put(pool, addr), RET_OK);
}

TEST(MemPool, basic2) {
  uint32_t i = 0;
  uint32_t nr = 100;
  uint8_t buff[2560];
  void* addr = NULL;
  void* addrs[100];

  mem_pool_t* pool = mem_pool_init(buff, sizeof(buff), 16, nr);
  ASSERT_EQ(pool->block_nr, 100);
  ASSERT_EQ(pool->block_size, 16);
  ASSERT_EQ(pool->used, 0);
  ASSERT_EQ(pool->bits_size, 4);

  addr = mem_pool_get(pool);
  ASSERT_EQ(addr != NULL, true);
  ASSERT_EQ(mem_pool_put(pool, addr), RET_OK);
  ASSERT_NE(mem_pool_put(pool, addr), RET_OK);

  for (i = 0; i < nr; i++) {
    ASSERT_EQ(pool->used, i);
    addr = mem_pool_get(pool);
    ASSERT_EQ(addr != NULL, true);
    addrs[i] = addr;
  }

  addr = mem_pool_get(pool);
  ASSERT_EQ(addr == NULL, true);

  for (i = 0; i < nr; i++) {
    ASSERT_EQ(mem_pool_put(pool, addrs[i]), RET_OK);
    ASSERT_NE(mem_pool_put(pool, addrs[i]), RET_OK);
  }
  ASSERT_EQ(pool->used, 0);
}
