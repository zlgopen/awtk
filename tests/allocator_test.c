#include "tkc/mem.h"
#include "tkc/mem_allocator_pool.h"
#include "tkc/mem_allocator_simple.h"

#include "tkc/mem_allocator_composite.h"

static void allocator_test_simple(void) {
  char buff[64];
  void* addr1 = NULL;
  void* addr2 = NULL;
  mem_allocator_simple_t simple;
  mem_allocator_t* allocator = mem_allocator_simple_init(&simple, buff, sizeof(buff));

  assert(simple.info.buffer == buff);
  assert(simple.info.size == sizeof(buff));
  addr1 = MEM_ALLOCATOR_ALLOC(allocator, 10);
  assert(addr1 != NULL);
  assert(simple.info.used_block_nr == 1);

  addr2 = MEM_ALLOCATOR_ALLOC(allocator, 10);
  assert(addr2 != NULL);
  assert(simple.info.used_block_nr == 2);

  addr1 = MEM_ALLOCATOR_REALLOC(allocator, addr1, 12);
  assert(simple.info.used_block_nr == 2);

  addr2 = MEM_ALLOCATOR_REALLOC(allocator, addr2, 12);
  assert(simple.info.used_block_nr == 2);

  MEM_ALLOCATOR_FREE(allocator, addr1);
  assert(simple.info.used_block_nr == 1);

  MEM_ALLOCATOR_FREE(allocator, addr2);
  assert(simple.info.used_block_nr == 0);
  assert(simple.info.used_bytes == 0);
  assert(simple.info.used_max_block_nr == 2);
  assert(simple.info.free_list->size == sizeof(buff));

  mem_allocator_destroy(allocator);
}

void allocator_test_basic_ex(mem_allocator_t* allocator) {
  uint32_t i = 0;
  void* addr = NULL;
  void* addrs[100];

  for (i = 0; i < ARRAY_SIZE(addrs); i++) {
    addr = MEM_ALLOCATOR_ALLOC(allocator, i + 1);
    assert(addr != NULL);
    addr = MEM_ALLOCATOR_REALLOC(allocator, addr, i + 1);
    assert(addr != NULL);
    addr = MEM_ALLOCATOR_REALLOC(allocator, addr, i + 5);
    assert(addr != NULL);
    addrs[i] = addr;
  }

  for (i = 0; i < ARRAY_SIZE(addrs); i++) {
    MEM_ALLOCATOR_FREE(allocator, addrs[i]);
  }
}

void allocator_test_basic() {
  uint32_t i = 0;
  char buff[102400];
  mem_allocator_simple_t simple;
  mem_allocator_t* allocator = mem_allocator_simple_init(&simple, buff, sizeof(buff));
  mem_allocator_pool_t pool;
  allocator = mem_allocator_pool_init(&pool, allocator, 10, 10, 10, 10, 10);

  allocator_test_basic_ex(allocator);

  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    assert(pool.pools[i]->used == 0);
  }
  mem_allocator_destroy(allocator);

  return;
}

void allocator_test_rand_ex(mem_allocator_t* allocator) {
  uint32_t k = 0;
  uint32_t i = 0;
  void* addr = NULL;
  void* addrs[1000];

  for (k = 0; k < 1000; k++) {
    for (i = 0; i < ARRAY_SIZE(addrs); i++) {
      uint32_t size = random() % 100;
      addr = MEM_ALLOCATOR_ALLOC(allocator, size);
      assert(addr != NULL);
      addr = MEM_ALLOCATOR_REALLOC(allocator, addr, size);
      assert(addr != NULL);
      addr = MEM_ALLOCATOR_REALLOC(allocator, addr, size + 10);
      assert(addr != NULL);
      addrs[i] = addr;
    }

    for (i = 0; i < ARRAY_SIZE(addrs); i++) {
      MEM_ALLOCATOR_FREE(allocator, addrs[i]);
    }
  }
}

void allocator_test_rand() {
  uint32_t i = 0;
  char buff[1000 * 1000];
  mem_allocator_simple_t simple;
  mem_allocator_t* allocator = mem_allocator_simple_init(&simple, buff, sizeof(buff));
  mem_allocator_pool_t pool;

  allocator = mem_allocator_pool_init(&pool, allocator, 10, 10, 10, 10, 10);

  allocator_test_rand_ex(allocator);

  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    assert(pool.pools[i]->used == 0);
  }
  mem_allocator_destroy(allocator);
}

void allocator_test_composite0() {
  char mem1[100 * 1000];
  char mem2[1000 * 1000];
  mem_allocator_composite_t composite;
  mem_allocator_t* allocator =
      mem_allocator_composite_init(&composite, mem1, sizeof(mem1), mem2, sizeof(mem2), NULL);

  allocator_test_basic_ex(allocator);
  allocator_test_rand_ex(allocator);
  mem_allocator_destroy(allocator);
}

void allocator_test_composite1() {
  char mem1[36];
  /* 需要留出 "块大小 + MIN_SIZE" 的余量才能拆分，否则整块给出，realloc 时会搬迁 */
  char mem2[80];
  char mem3[1280];
  char* addr = NULL;
  mem_allocator_composite_t composite;
  mem_allocator_t* allocator = mem_allocator_composite_init(&composite, mem1, sizeof(mem1), mem2,
                                                            sizeof(mem2), mem3, sizeof(mem3), NULL);

  /*mem1*/
  addr = MEM_ALLOCATOR_ALLOC(allocator, 16);
  assert(addr >= mem1);
  assert(addr < (mem1 + sizeof(mem1)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  MEM_ALLOCATOR_FREE(allocator, addr);
  addr = MEM_ALLOCATOR_ALLOC(allocator, 16);
  assert(addr >= mem1);
  assert(addr < (mem1 + sizeof(mem1)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  addr = MEM_ALLOCATOR_REALLOC(allocator, addr, 16);
  assert(addr >= mem1);
  assert(addr < (mem1 + sizeof(mem1)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  addr = MEM_ALLOCATOR_REALLOC(allocator, addr, 16 + 2);
  assert(addr >= mem1);
  assert(addr < (mem1 + sizeof(mem1)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  /*mem2*/
  addr = MEM_ALLOCATOR_ALLOC(allocator, 36);
  assert(addr >= mem2);
  assert(addr < (mem2 + sizeof(mem2)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  MEM_ALLOCATOR_FREE(allocator, addr);
  addr = MEM_ALLOCATOR_ALLOC(allocator, 36);
  assert(addr >= mem2);
  assert(addr < (mem2 + sizeof(mem2)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  addr = MEM_ALLOCATOR_REALLOC(allocator, addr, 30);
  assert(addr >= mem2);
  assert(addr < (mem2 + sizeof(mem2)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  /*mem3*/
  addr = MEM_ALLOCATOR_ALLOC(allocator, 66);
  assert(addr >= mem3);
  assert(addr < (mem3 + sizeof(mem3)));

  MEM_ALLOCATOR_FREE(allocator, addr);
  addr = MEM_ALLOCATOR_ALLOC(allocator, 66);
  assert(addr >= mem3);
  assert(addr < (mem3 + sizeof(mem3)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  addr = MEM_ALLOCATOR_REALLOC(allocator, addr, 66 + 2);
  assert(addr >= mem3);
  assert(addr < (mem3 + sizeof(mem3)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  mem_allocator_destroy(allocator);
  return;
}

/* 分配、扩容、释放多种尺寸，检查返回的地址都按机器字对齐 */
static void allocator_test_alignment_impl(mem_allocator_t* allocator) {
  static const uint32_t s_k_align_test_sizes[] = {1, 4, 8, 12, 16, 24, 32, 40, 64};
  uint32_t i = 0;
  uint32_t round = 0;
  void* addrs[ARRAY_SIZE(s_k_align_test_sizes)];

  for (round = 0; round < 3; round++) {
    for (i = 0; i < ARRAY_SIZE(s_k_align_test_sizes); i++) {
      uint32_t size = s_k_align_test_sizes[i];

      addrs[i] = mem_allocator_alloc(allocator, size, __FUNCTION__, __LINE__);
      assert(addrs[i] != NULL);
      /* 载荷必须按机器字对齐，才能存放指针/double/uint64_t */
      assert(((uintptr_t)addrs[i] % sizeof(void*)) == 0);
      memset(addrs[i], 0x5a, size);
    }

    for (i = 0; i < ARRAY_SIZE(s_k_align_test_sizes); i++) {
      uint32_t size = s_k_align_test_sizes[i] + 8;
      void* addr = mem_allocator_realloc(allocator, addrs[i], size, __FUNCTION__, __LINE__);

      /* realloc 可能搬迁，但新地址仍必须对齐 */
      assert(addr != NULL);
      assert(((uintptr_t)addr % sizeof(void*)) == 0);
      memset(addr, 0x5a, size);
      addrs[i] = addr;
    }

    for (i = 0; i < ARRAY_SIZE(s_k_align_test_sizes); i++) {
      mem_allocator_free(allocator, addrs[i]);
    }
  }
}

void allocator_test_alignment(void) {
  /* 用按机器字对齐的缓冲区，满足分配器对基址的要求 */
  uintptr_t buff[2048];
  mem_allocator_pool_t pool;
  mem_allocator_simple_t simple;
  mem_allocator_t* allocator = mem_allocator_simple_init(&simple, buff, sizeof(buff));

  assert(allocator != NULL);

  /* 1.直接使用简单分配器 */
  allocator_test_alignment_impl(allocator);

  /* 2.经过 mem_pool 包装后仍应保持对齐 */
  allocator = mem_allocator_pool_init(&pool, allocator, 10, 10, 10, 10, 10);
  assert(allocator != NULL);
  allocator_test_alignment_impl(allocator);

  mem_allocator_destroy(allocator);
}

static uint32_t block_size_of_request(uint32_t user_size) {
  uint32_t size = user_size + SIMPLE_HEAD_SIZE;

  if (size <= MIN_SIZE) {
    size = MIN_SIZE;
  }

  return TK_ROUND_TO_MACH(size);
}

static void assert_bytes(const void* ptr, uint32_t n, uint8_t tag) {
  const uint8_t* bytes = (const uint8_t*)ptr;
  uint32_t i = 0;

  for (i = 0; i < n; i++) {
    assert(bytes[i] == tag);
  }
}

static int pool_index_for_size(uint32_t size) {
  static const uint32_t sizes[TK_MEM_POOLS_NR] = {8, 16, 32, 48, 64};
  uint32_t i = 0;

  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    if (size <= sizes[i]) {
      return (int)i;
    }
  }

  return -1;
}

/* simple：参数错误、耗尽、拆分/合并、realloc 失败时保留原块 */
static void allocator_test_simple_edges(void) {
  uintptr_t storage[64];
  uint8_t* base = (uint8_t*)storage;
  uint8_t* buf = base + 32;
  mem_allocator_simple_t simple;
  mem_allocator_t* allocator = NULL;
  uint32_t buf_size = 128;
  uint32_t exact = 0;
  uint32_t i = 0;
  void* ptr = NULL;
  void* other = NULL;
  free_node_t* free_list = NULL;

  assert(mem_allocator_simple_init(NULL, storage, sizeof(storage)) == NULL);

  assert(mem_allocator_simple_init(&simple, NULL, 128) == NULL);
  assert(mem_allocator_simple_is_valid(&simple.allocator) == FALSE);
  assert(simple.info.buffer == NULL);
  assert(mem_allocator_simple_alloc(&simple.allocator, 8, NULL, 0) == NULL);

  assert(mem_allocator_simple_init(&simple, storage, 0) == NULL);
  assert(mem_allocator_simple_init(&simple, storage, 1) == NULL);
  assert(mem_allocator_simple_init(&simple, storage, MIN_SIZE) == NULL);
  assert(mem_allocator_simple_init(&simple, storage, MIN_SIZE - 1) == NULL);

  allocator = mem_allocator_simple_init(&simple, storage, MIN_SIZE + 1);
  assert(allocator != NULL);
  assert(mem_allocator_simple_is_valid(allocator) == TRUE);
  ptr = MEM_ALLOCATOR_ALLOC(allocator, 1);
  assert(ptr != NULL);
  assert(mem_allocator_simple_get_mem_size(allocator, ptr) == MIN_SIZE + 1);
  assert(simple.info.free_list == NULL);
  assert(MEM_ALLOCATOR_ALLOC(allocator, 1) == NULL);
  assert(simple.info.used_block_nr == 1);
  MEM_ALLOCATOR_FREE(allocator, ptr);
  assert(simple.info.used_bytes == 0);
  assert(simple.info.free_list->size == MIN_SIZE + 1);
  mem_allocator_destroy(allocator);

  memset(&simple, 0x00, sizeof(simple));
  simple.allocator.vt = &s_mem_allocator_simple_vtable;
  assert(tk_mem_init_impl(&simple.allocator, NULL, sizeof(storage)) == RET_BAD_PARAMS);
  assert(tk_mem_init_impl(&simple.allocator, storage, MIN_SIZE) == RET_BAD_PARAMS);
  assert(tk_mem_init_impl(&simple.allocator, storage, sizeof(storage)) == RET_OK);
  assert(tk_mem_init_impl(&simple.allocator, storage, sizeof(storage)) == RET_FAIL);
  assert(simple.info.buffer == (char*)storage);

  allocator = mem_allocator_simple_init(&simple, buf, buf_size);
  assert(allocator != NULL);
  assert(mem_allocator_simple_contains(allocator, NULL) == FALSE);
  assert(mem_allocator_simple_contains(allocator, buf - 1) == FALSE);
  assert(mem_allocator_simple_contains(allocator, buf) == TRUE);
  assert(mem_allocator_simple_contains(allocator, buf + buf_size - 1) == TRUE);
  assert(mem_allocator_simple_contains(allocator, buf + buf_size) == FALSE);
  assert(mem_allocator_simple_get_mem_size(allocator, NULL) == 0);
  assert(mem_allocator_dump(allocator) == RET_OK);

  ptr = MEM_ALLOCATOR_ALLOC(allocator, 0);
  assert(ptr != NULL);
  assert(mem_allocator_simple_get_mem_size(allocator, ptr) == MIN_SIZE);
  assert(((uintptr_t)ptr % sizeof(void*)) == 0);
  MEM_ALLOCATOR_FREE(allocator, ptr);

  for (i = 0; i <= 48; i++) {
    uint32_t expect = block_size_of_request(i);

    ptr = MEM_ALLOCATOR_ALLOC(allocator, i);
    assert(ptr != NULL);
    assert(mem_allocator_simple_get_mem_size(allocator, ptr) == expect);
    if (i > 0) {
      memset(ptr, (int)(0x30 + (i & 15)), i);
      assert_bytes(ptr, i, (uint8_t)(0x30 + (i & 15)));
    }
    MEM_ALLOCATOR_FREE(allocator, ptr);
    assert(simple.info.used_bytes == 0);
    assert(simple.info.free_list->size == buf_size);
    assert(simple.info.free_list->next == NULL);
  }

  free_list = simple.info.free_list;
  assert(MEM_ALLOCATOR_ALLOC(allocator, buf_size) == NULL);
  assert(MEM_ALLOCATOR_ALLOC(allocator, UINT32_MAX - 15) == NULL);
  assert(simple.info.free_list == free_list);
  assert(simple.info.used_block_nr == 0);

  /* size 与块头相加回绕后变成最小块 */
  ptr = MEM_ALLOCATOR_ALLOC(allocator, UINT32_MAX);
  assert(ptr != NULL);
  assert(mem_allocator_simple_get_mem_size(allocator, ptr) == MIN_SIZE);
  memset(ptr, 0x46, 1);
  ptr = MEM_ALLOCATOR_REALLOC(allocator, ptr, UINT32_MAX);
  assert(ptr != NULL);
  assert(*(uint8_t*)ptr == 0x46);
  assert(mem_allocator_simple_get_mem_size(allocator, ptr) == MIN_SIZE);
  MEM_ALLOCATOR_FREE(allocator, ptr);

  exact = block_size_of_request(40);
  allocator = mem_allocator_simple_init(&simple, storage, exact);
  ptr = MEM_ALLOCATOR_ALLOC(allocator, 40);
  assert(ptr != NULL);
  assert(simple.info.free_list == NULL);
  memset(ptr, 0x21, 40);
  assert(MEM_ALLOCATOR_ALLOC(allocator, 1) == NULL);
  assert(MEM_ALLOCATOR_REALLOC(allocator, ptr, 1) == NULL);
  assert(MEM_ALLOCATOR_REALLOC(allocator, ptr, 40 + MIN_SIZE) == NULL);
  assert_bytes(ptr, 40, 0x21);
  assert(simple.info.used_block_nr == 1);
  MEM_ALLOCATOR_FREE(allocator, ptr);
  assert(simple.info.free_list->size == exact);

  allocator = mem_allocator_simple_init(&simple, storage, exact + MIN_SIZE);
  ptr = MEM_ALLOCATOR_ALLOC(allocator, 40);
  assert(mem_allocator_simple_get_mem_size(allocator, ptr) == exact);
  assert(simple.info.free_list->size == MIN_SIZE);
  other = ptr;
  memset(other, 0x22, 40);
  assert(MEM_ALLOCATOR_REALLOC(allocator, ptr, exact * 2) == NULL);
  assert_bytes(other, 40, 0x22);
  MEM_ALLOCATOR_FREE(allocator, ptr);
  ptr = MEM_ALLOCATOR_ALLOC(allocator, (exact + MIN_SIZE) - SIMPLE_HEAD_SIZE);
  assert(ptr != NULL);
  assert(simple.info.free_list == NULL);
  MEM_ALLOCATOR_FREE(allocator, ptr);

  /* 落在原地调整区间内的 realloc 不搬迁，再小一字节会换块，邻居数据保持不变 */
  {
    uint32_t block = block_size_of_request(100);
    uint32_t old_user = block - SIMPLE_HEAD_SIZE;
    uint32_t low = old_user > MIN_SIZE ? old_user - MIN_SIZE : 0;
    void* moved = NULL;

    assert(sizeof(storage) > block * 2);
    assert(low > 0);
    allocator = mem_allocator_simple_init(&simple, storage, sizeof(storage));
    ptr = MEM_ALLOCATOR_ALLOC(allocator, 100);
    other = MEM_ALLOCATOR_ALLOC(allocator, 8);
    memset(ptr, 0x23, 100);
    memset(other, 0x24, 8);
    assert(MEM_ALLOCATOR_REALLOC(allocator, ptr, old_user) == ptr);
    assert(MEM_ALLOCATOR_REALLOC(allocator, ptr, low) == ptr);
    assert_bytes(ptr, 100, 0x23);
    moved = MEM_ALLOCATOR_REALLOC(allocator, ptr, low - 1);
    assert(moved != NULL);
    assert(moved != ptr);
    assert_bytes(moved, low - 1, 0x23);
    assert_bytes(other, 8, 0x24);
    ptr = MEM_ALLOCATOR_REALLOC(allocator, moved, old_user + 1);
    assert(ptr != NULL);
    assert(ptr != moved);
    assert_bytes(ptr, low - 1, 0x23);
    assert_bytes(other, 8, 0x24);
    MEM_ALLOCATOR_FREE(allocator, ptr);
    MEM_ALLOCATOR_FREE(allocator, other);
    assert(simple.info.used_bytes == 0);
    assert(simple.info.free_list->size == sizeof(storage));
  }

  /* 填满后隔块释放，大块申请失败；全部释放后可以再申请 */
  {
    enum { kBlocks = 8 };
    void* ptrs[kBlocks];
    uint32_t total = MIN_SIZE * kBlocks;
    uint32_t user_cap = MIN_SIZE - SIMPLE_HEAD_SIZE;

    assert(sizeof(storage) >= total);
    allocator = mem_allocator_simple_init(&simple, storage, total);
    for (i = 0; i < kBlocks; i++) {
      ptrs[i] = MEM_ALLOCATOR_ALLOC(allocator, 1);
      assert(ptrs[i] != NULL);
      memset(ptrs[i], (int)(i + 1), user_cap);
    }
    assert(simple.info.free_list == NULL);
    assert(MEM_ALLOCATOR_ALLOC(allocator, 1) == NULL);
    for (i = 0; i < kBlocks; i += 2) {
      MEM_ALLOCATOR_FREE(allocator, ptrs[i]);
    }
    assert(MEM_ALLOCATOR_ALLOC(allocator, MIN_SIZE * 2 - SIMPLE_HEAD_SIZE) == NULL);
    for (i = 1; i < kBlocks; i += 2) {
      assert_bytes(ptrs[i], user_cap, (uint8_t)(i + 1));
      MEM_ALLOCATOR_FREE(allocator, ptrs[i]);
    }
    assert(simple.info.free_list->size == total);
    assert(simple.info.free_list->next == NULL);
    ptr = MEM_ALLOCATOR_ALLOC(allocator, MIN_SIZE * 2 - SIMPLE_HEAD_SIZE);
    assert(ptr != NULL);
    MEM_ALLOCATOR_FREE(allocator, ptr);
  }

  /* 按地址插入空闲链表，并与前后邻居合并 */
  {
    void* blocks[3];
    uint32_t total = MIN_SIZE * 3;
    static const uint8_t orders[6][3] = {
        {0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0},
    };
    uint32_t o = 0;

    assert(sizeof(storage) >= total);
    for (o = 0; o < 6; o++) {
      uint8_t alive[3] = {1, 1, 1};
      uint32_t step = 0;

      allocator = mem_allocator_simple_init(&simple, storage, total);
      for (i = 0; i < 3; i++) {
        blocks[i] = MEM_ALLOCATOR_ALLOC(allocator, 1);
        assert(blocks[i] != NULL);
        memset(blocks[i], (int)(0x40 + i), MIN_SIZE - SIMPLE_HEAD_SIZE);
      }
      for (step = 0; step < 3; step++) {
        uint32_t index = orders[o][step];
        uint32_t k = 0;

        MEM_ALLOCATOR_FREE(allocator, blocks[index]);
        alive[index] = 0;
        for (k = 0; k < 3; k++) {
          if (alive[k]) {
            assert_bytes(blocks[k], MIN_SIZE - SIMPLE_HEAD_SIZE, (uint8_t)(0x40 + k));
          }
        }
      }
      assert(simple.info.used_bytes == 0);
      assert(simple.info.free_list->size == total);
      assert(simple.info.free_list->next == NULL);
      assert(simple.info.used_max_block_nr == 3);
    }
  }

  ptr = MEM_ALLOCATOR_ALLOC(allocator, 4);
  assert(simple.info.used_block_nr == 1);
  mem_allocator_simple_free(allocator, NULL);
  assert(simple.info.used_block_nr == 1);
  MEM_ALLOCATOR_FREE(allocator, ptr);

  assert(mem_allocator_destroy(allocator) == RET_OK);
  assert(simple.allocator.vt == NULL);
  assert(mem_allocator_alloc(&simple.allocator, 4, __FUNCTION__, __LINE__) == NULL);
  assert(mem_allocator_realloc(&simple.allocator, NULL, 4, __FUNCTION__, __LINE__) == NULL);
  assert(mem_allocator_dump(&simple.allocator) == RET_BAD_PARAMS);
  assert(mem_allocator_destroy(&simple.allocator) == RET_BAD_PARAMS);
  assert(mem_allocator_alloc(NULL, 4, __FUNCTION__, __LINE__) == NULL);
  assert(mem_allocator_dump(NULL) == RET_BAD_PARAMS);
  assert(mem_allocator_destroy(NULL) == RET_BAD_PARAMS);

  {
    free_node_t prev;
    free_node_t next;
    mem_info_t info;

    memset(&info, 0x00, sizeof(info));
    memset(&prev, 0x00, sizeof(prev));
    memset(&next, 0x00, sizeof(next));
    prev.size = MIN_SIZE;
    next.size = MIN_SIZE;
    node_merge2(NULL, NULL, NULL);
    node_merge2(&info, NULL, &next);
    node_merge2(&info, &prev, NULL);
    node_merge2(&info, &prev, &next);
    assert(prev.size == MIN_SIZE);
    assert(next.size == MIN_SIZE);
    assert(prev.next == NULL);
  }
}

/* pool：尺寸落在哪一级、池耗尽后落到后备分配器、realloc 跨级保留数据 */
static void allocator_test_pool_edges(void) {
  uintptr_t storage[4096];
  mem_allocator_simple_t simple;
  mem_allocator_pool_t pool;
  mem_allocator_t* impl = mem_allocator_simple_init(&simple, storage, sizeof(storage));
  mem_allocator_t* allocator = NULL;
  uint32_t sizes[] = {0, 1, 8, 9, 16, 17, 32, 33, 48, 49, 64, 65};
  uint32_t i = 0;
  void* ptr = NULL;
  void* again = NULL;
  void* blocks[40];
  uint32_t used_before = 0;

  assert(impl != NULL);
  assert(mem_allocator_pool_init(NULL, impl, 4, 4, 4, 4, 4) == NULL);
  assert(mem_allocator_pool_init(&pool, NULL, 4, 4, 4, 4, 4) == NULL);

  allocator = mem_allocator_pool_init(&pool, impl, 4, 4, 4, 4, 4);
  assert(allocator != NULL);
  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    assert(pool.pools[i] != NULL);
    assert(pool.pools[i]->used == 0);
    assert(pool.pools[i]->block_nr >= 32);
  }

  mem_allocator_free(allocator, NULL);
  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    assert(pool.pools[i]->used == 0);
  }

  for (i = 0; i < ARRAY_SIZE(sizes); i++) {
    int index = pool_index_for_size(sizes[i]);
    uint32_t n = 0;
    uint32_t saved[TK_MEM_POOLS_NR];

    for (n = 0; n < TK_MEM_POOLS_NR; n++) {
      saved[n] = pool.pools[n]->used;
    }
    ptr = MEM_ALLOCATOR_ALLOC(allocator, sizes[i]);
    assert(ptr != NULL);
    assert(((uintptr_t)ptr % sizeof(void*)) == 0);
    if (index >= 0) {
      assert(pool.pools[index]->used == saved[index] + 1);
      assert(mem_pool_get_index(pool.pools[index], ptr) >= 0);
      for (n = 0; n < TK_MEM_POOLS_NR; n++) {
        if ((int)n != index) {
          assert(pool.pools[n]->used == saved[n]);
        }
      }
    } else {
      for (n = 0; n < TK_MEM_POOLS_NR; n++) {
        assert(pool.pools[n]->used == saved[n]);
        assert(mem_pool_get_index(pool.pools[n], ptr) < 0);
      }
    }
    if (sizes[i] > 0) {
      memset(ptr, 0x5a, sizes[i]);
    }
    MEM_ALLOCATOR_FREE(allocator, ptr);
    for (n = 0; n < TK_MEM_POOLS_NR; n++) {
      assert(pool.pools[n]->used == saved[n]);
    }
  }

  ptr = MEM_ALLOCATOR_REALLOC(allocator, NULL, 8);
  assert(ptr != NULL);
  assert(pool.pools[0]->used == 1);
  memset(ptr, 0x11, 8);
  again = MEM_ALLOCATOR_REALLOC(allocator, ptr, 8);
  assert(again == ptr);
  assert_bytes(again, 8, 0x11);
  again = MEM_ALLOCATOR_REALLOC(allocator, ptr, 9);
  assert(again != NULL);
  assert(again != ptr);
  assert_bytes(again, 8, 0x11);
  assert(pool.pools[0]->used == 0);
  assert(pool.pools[1]->used == 1);
  memset(again, 0x12, 9);
  ptr = MEM_ALLOCATOR_REALLOC(allocator, again, 80);
  assert(ptr != NULL);
  assert_bytes(ptr, 9, 0x12);
  assert(pool.pools[1]->used == 0);
  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    assert(mem_pool_get_index(pool.pools[i], ptr) < 0);
  }
  MEM_ALLOCATOR_FREE(allocator, ptr);

  for (i = 0; i < 32; i++) {
    blocks[i] = MEM_ALLOCATOR_ALLOC(allocator, 8);
    assert(blocks[i] != NULL);
    memset(blocks[i], (int)(i + 1), 8);
  }
  assert(pool.pools[0]->used == 32);
  ptr = MEM_ALLOCATOR_ALLOC(allocator, 8);
  assert(ptr != NULL);
  assert(mem_pool_get_index(pool.pools[0], ptr) < 0);
  assert(pool.pools[0]->used == 32);
  memset(ptr, 0x77, 8);
  MEM_ALLOCATOR_FREE(allocator, ptr);
  assert(pool.pools[0]->used == 32);
  assert_bytes(blocks[0], 8, 1);
  MEM_ALLOCATOR_FREE(allocator, blocks[0]);
  assert(pool.pools[0]->used == 31);
  blocks[0] = MEM_ALLOCATOR_ALLOC(allocator, 8);
  assert(mem_pool_get_index(pool.pools[0], blocks[0]) >= 0);
  assert(pool.pools[0]->used == 32);
  for (i = 0; i < 32; i++) {
    MEM_ALLOCATOR_FREE(allocator, blocks[i]);
  }
  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    assert(pool.pools[i]->used == 0);
  }

  used_before = simple.info.used_block_nr;
  assert(MEM_ALLOCATOR_ALLOC(allocator, 1024 * 1024) == NULL);
  assert(MEM_ALLOCATOR_REALLOC(allocator, NULL, 1024 * 1024) == NULL);
  assert(simple.info.used_block_nr == used_before);
  assert(mem_allocator_dump(allocator) == RET_OK);
  assert(mem_allocator_destroy(allocator) == RET_OK);
  assert(mem_allocator_alloc(allocator, 8, __FUNCTION__, __LINE__) == NULL);
}

/* composite：块太小被跳过、第一块耗尽后落到后面、搬迁失败保留原数据 */
static void allocator_test_composite_edges(void) {
  uintptr_t mem_a[32];
  uintptr_t mem_b[64];
  uintptr_t mem_c[64];
  uintptr_t extra[8];
  uint8_t tiny[4];
  mem_allocator_composite_t composite;
  mem_allocator_t* allocator = NULL;
  uint32_t total = MIN_SIZE * 4;
  uint32_t i = 0;
  void* ptrs[8];
  void* moved = NULL;
  int outside = 0;
  uint32_t exact = 0;

  assert(mem_allocator_composite_init(NULL, mem_b, sizeof(mem_b), NULL) == NULL);

  allocator = mem_allocator_composite_init(&composite, NULL, 128, NULL);
  assert(allocator != NULL);
  assert(MEM_ALLOCATOR_ALLOC(allocator, 8) == NULL);
  assert(mem_allocator_composite_is_valid_addr(allocator, mem_b) == FALSE);
  mem_allocator_destroy(allocator);

  assert(sizeof(mem_b) >= total);
  /* init 失败的槽 is_valid 仍为假，下一块内存会占用同一个槽，而不是往后排 */
  allocator = mem_allocator_composite_init(&composite, tiny, sizeof(tiny), mem_b, total, NULL);
  assert(allocator != NULL);
  assert(mem_allocator_simple_is_valid(MEM_ALLOCATOR(&composite.allocators[0])) == TRUE);
  assert(mem_allocator_simple_is_valid(MEM_ALLOCATOR(&composite.allocators[1])) == FALSE);
  ptrs[0] = MEM_ALLOCATOR_ALLOC(allocator, 1);
  assert(ptrs[0] != NULL);
  assert((uint8_t*)ptrs[0] >= (uint8_t*)mem_b);
  assert((uint8_t*)ptrs[0] < (uint8_t*)mem_b + total);
  MEM_ALLOCATOR_FREE(allocator, ptrs[0]);
  mem_allocator_destroy(allocator);

  allocator = mem_allocator_composite_init(&composite, mem_b, total, mem_c, sizeof(mem_c), NULL);
  assert(allocator != NULL);

  for (i = 0; i < 4; i++) {
    ptrs[i] = MEM_ALLOCATOR_ALLOC(allocator, 1);
    assert(ptrs[i] != NULL);
    assert((uint8_t*)ptrs[i] >= (uint8_t*)mem_b);
    assert((uint8_t*)ptrs[i] < (uint8_t*)mem_b + total);
    assert(mem_allocator_composite_is_valid_addr(allocator, ptrs[i]) == TRUE);
    memset(ptrs[i], (int)(0x50 + i), MIN_SIZE - SIMPLE_HEAD_SIZE);
  }
  assert(MEM_ALLOCATOR_ALLOC(allocator, total + sizeof(mem_c)) == NULL);
  ptrs[4] = MEM_ALLOCATOR_ALLOC(allocator, 1);
  assert(ptrs[4] != NULL);
  assert((uint8_t*)ptrs[4] >= (uint8_t*)mem_c);
  assert((uint8_t*)ptrs[4] < (uint8_t*)mem_c + sizeof(mem_c));
  for (i = 0; i < 4; i++) {
    assert_bytes(ptrs[i], MIN_SIZE - SIMPLE_HEAD_SIZE, (uint8_t)(0x50 + i));
  }

  mem_allocator_composite_free(allocator, NULL);
  mem_allocator_composite_free(allocator, &outside);
  assert(MEM_ALLOCATOR_REALLOC(allocator, &outside, 8) == NULL);
  assert_bytes(ptrs[0], MIN_SIZE - SIMPLE_HEAD_SIZE, 0x50);

  for (i = 0; i < 4; i++) {
    MEM_ALLOCATOR_FREE(allocator, ptrs[i]);
  }
  MEM_ALLOCATOR_FREE(allocator, ptrs[4]);
  mem_allocator_destroy(allocator);

  allocator =
      mem_allocator_composite_init(&composite, mem_a, sizeof(mem_a), mem_b, sizeof(mem_b), mem_c, sizeof(mem_c), NULL);
  assert(mem_allocator_composite_add_mem(allocator, (char*)extra, sizeof(extra)) == RET_BAD_PARAMS);
  assert(mem_allocator_dump(allocator) == RET_OK);
  mem_allocator_destroy(allocator);

  exact = block_size_of_request(40);
  assert(sizeof(mem_a) >= exact);
  allocator = mem_allocator_composite_init(&composite, mem_a, exact, mem_b, sizeof(mem_b), NULL);
  ptrs[0] = MEM_ALLOCATOR_ALLOC(allocator, 40);
  assert(ptrs[0] != NULL);
  assert((uint8_t*)ptrs[0] >= (uint8_t*)mem_a);
  assert((uint8_t*)ptrs[0] < (uint8_t*)mem_a + exact);
  memset(ptrs[0], 0x61, 40);
  moved = MEM_ALLOCATOR_REALLOC(allocator, ptrs[0], 1);
  assert(moved != NULL);
  assert((uint8_t*)moved >= (uint8_t*)mem_b);
  assert((uint8_t*)moved < (uint8_t*)mem_b + sizeof(mem_b));
  assert_bytes(moved, 1, 0x61);
  MEM_ALLOCATOR_FREE(allocator, moved);

  /* 两块都放不下时返回 NULL，原来的数据还在 */
  allocator = mem_allocator_composite_init(&composite, mem_a, exact, tiny, sizeof(tiny), NULL);
  ptrs[0] = MEM_ALLOCATOR_ALLOC(allocator, 40);
  memset(ptrs[0], 0x62, 40);
  assert(MEM_ALLOCATOR_REALLOC(allocator, ptrs[0], 200) == NULL);
  assert(MEM_ALLOCATOR_ALLOC(allocator, 200) == NULL);
  assert_bytes(ptrs[0], 40, 0x62);
  MEM_ALLOCATOR_FREE(allocator, ptrs[0]);

  /* 第一块剩下的空洞不够扩容，数据搬到第二块，前缀保留 */
  {
    uint32_t need = block_size_of_request(80);
    uint32_t mem1_size = need + MIN_SIZE;
    uint32_t old_user = need - SIMPLE_HEAD_SIZE;
    uint32_t bigger = old_user + MIN_SIZE;

    assert(sizeof(mem_b) >= mem1_size);
    assert(bigger > old_user);
    allocator =
        mem_allocator_composite_init(&composite, mem_b, mem1_size, mem_c, sizeof(mem_c), NULL);
    ptrs[0] = MEM_ALLOCATOR_ALLOC(allocator, 80);
    assert(mem_allocator_simple_get_mem_size(MEM_ALLOCATOR(&composite.allocators[0]), ptrs[0]) ==
           need);
    memset(ptrs[0], 0x63, 80);
    moved = MEM_ALLOCATOR_REALLOC(allocator, ptrs[0], bigger);
    assert(moved != NULL);
    assert((uint8_t*)moved >= (uint8_t*)mem_c);
    assert_bytes(moved, 80, 0x63);
    MEM_ALLOCATOR_FREE(allocator, moved);
  }

  ptrs[0] = MEM_ALLOCATOR_REALLOC(allocator, NULL, 8);
  assert(ptrs[0] != NULL);
  assert(mem_allocator_composite_is_valid_addr(allocator, ptrs[0]) == TRUE);
  MEM_ALLOCATOR_FREE(allocator, ptrs[0]);
  assert(mem_allocator_destroy(allocator) == RET_OK);
  assert(allocator->vt == NULL);
  assert(mem_allocator_alloc(allocator, 8, __FUNCTION__, __LINE__) == NULL);
  assert(mem_allocator_destroy(allocator) == RET_BAD_PARAMS);
  /* destroy 只清掉 vt，底层 buffer 还在，地址判断仍然成立 */
  assert(mem_allocator_composite_is_valid_addr(allocator, mem_c) == TRUE);
}

static void allocator_test_composite_alignment(void) {
  uintptr_t mem1[256];
  uintptr_t mem2[256];
  mem_allocator_composite_t composite;
  mem_allocator_t* allocator =
      mem_allocator_composite_init(&composite, mem1, sizeof(mem1), mem2, sizeof(mem2), NULL);

  assert(allocator != NULL);
  allocator_test_alignment_impl(allocator);
  mem_allocator_destroy(allocator);
}

void allocator_test() {
  allocator_test_simple_edges();
  allocator_test_pool_edges();
  allocator_test_composite_edges();
  allocator_test_composite1();
  allocator_test_basic();
  allocator_test_rand();
  allocator_test_simple();
  allocator_test_alignment();
  allocator_test_composite_alignment();
}
