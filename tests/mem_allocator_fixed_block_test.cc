#include "gtest/gtest.h"
#include "tkc/mem_allocator_fixed_block.h"
#include "assert_catcher.hpp"

TEST(MemAllocatorFixedBlock, base) {
  int* user = NULL;
  mem_allocator_t* allocator = mem_allocator_fixed_block_create(sizeof(int), 0);
  ASSERT_EQ(allocator != NULL, TRUE);

  user = (int*)MEM_ALLOCATOR_ALLOC(allocator, mem_allocator_fixed_block_size(allocator));
  ASSERT_EQ(user != NULL, TRUE);
  *user = 1;
  ASSERT_EQ(*user, 1);

  MEM_ALLOCATOR_FREE(allocator, user);

  ASSERT_EQ(mem_allocator_destroy(allocator), RET_OK);
}

TEST(MemAllocatorFixedBlock, multiple_alloc) {
  int *p1 = NULL, *p2 = NULL, *p3 = NULL;
  mem_allocator_t* allocator = mem_allocator_fixed_block_create(sizeof(int), 2);
  ASSERT_EQ(allocator != NULL, TRUE);

  // 测试初始容量
  p1 = (int*)MEM_ALLOCATOR_ALLOC(allocator, mem_allocator_fixed_block_size(allocator));
  p2 = (int*)MEM_ALLOCATOR_ALLOC(allocator, mem_allocator_fixed_block_size(allocator));
  ASSERT_EQ(p1 != NULL && p2 != NULL, TRUE);

  *p1 = 1;
  *p2 = 2;

  // 测试扩容
  p3 = (int*)MEM_ALLOCATOR_ALLOC(allocator, mem_allocator_fixed_block_size(allocator));
  ASSERT_EQ(p3 != NULL, TRUE);

  *p3 = 3;

  mem_allocator_dump(allocator);

  ASSERT_EQ(*p1, 1);
  ASSERT_EQ(*p2, 2);
  ASSERT_EQ(*p3, 3);

  MEM_ALLOCATOR_FREE(allocator, p2);

  MEM_ALLOCATOR_FREE(allocator, p1);
  MEM_ALLOCATOR_FREE(allocator, p3);

  ASSERT_EQ(mem_allocator_destroy(allocator), RET_OK);
}

TEST(MemAllocatorFixedBlock, reuse) {
  int *p1 = NULL, *p2 = NULL;
  mem_allocator_t* allocator = mem_allocator_fixed_block_create(sizeof(int), 2);

  p1 = (int*)MEM_ALLOCATOR_ALLOC(allocator, mem_allocator_fixed_block_size(allocator));
  mem_allocator_free(allocator, p1);

  // 测试内存重用
  p2 = (int*)MEM_ALLOCATOR_ALLOC(allocator, mem_allocator_fixed_block_size(allocator));
  ASSERT_EQ(p1, p2);  // 指针应该相同

  ASSERT_EQ(mem_allocator_destroy(allocator), RET_OK);
}

TEST(MemAllocatorFixedBlock, invalid_free) {
  int dummy = 0;
  int *p1 = NULL, *p2 = NULL, *p3 = NULL;
  mem_allocator_t* allocator = mem_allocator_fixed_block_create(sizeof(int), 2);

  // 测试无效指针释放
  ASSERT_EQ(
      MEM_ALLOCATOR_REALLOC(allocator, &dummy, mem_allocator_fixed_block_size(allocator)) == NULL,
      TRUE);

  p1 = (int*)MEM_ALLOCATOR_ALLOC(allocator, mem_allocator_fixed_block_size(allocator));
  p2 = (int*)MEM_ALLOCATOR_ALLOC(allocator, mem_allocator_fixed_block_size(allocator));

  p3 = (int*)MEM_ALLOCATOR_ALLOC(allocator, mem_allocator_fixed_block_size(allocator));

  // 测试无效指针释放
  ASSERT_EQ(
      MEM_ALLOCATOR_REALLOC(allocator, &dummy, mem_allocator_fixed_block_size(allocator)) == NULL,
      TRUE);
  ASSERT_EQ(MEM_ALLOCATOR_REALLOC(allocator, (uint8_t*)(p1) + 1,
                                  mem_allocator_fixed_block_size(allocator)) == NULL,
            TRUE);

  // 测试重复释放
  mem_allocator_free(allocator, p2);
  ASSERT_CATCH(mem_allocator_free(allocator, p2));

  (void)p3;

  ASSERT_EQ(mem_allocator_destroy(allocator), RET_OK);
}

TEST(MemAllocatorFixedBlock, realloc) {
  /* size = 16 为了测试快速地址对齐检查 */
  mem_allocator_t* allocator = mem_allocator_fixed_block_create(16, 2);

  void* p1 = MEM_ALLOCATOR_ALLOC(allocator, mem_allocator_fixed_block_size(allocator));
  void* p2 = MEM_ALLOCATOR_ALLOC(allocator, mem_allocator_fixed_block_size(allocator));
  p2 = MEM_ALLOCATOR_REALLOC(allocator, p2, mem_allocator_fixed_block_size(allocator));
  ASSERT_EQ(MEM_ALLOCATOR_REALLOC(allocator, (uint8_t*)(p1) + 1,
                                  mem_allocator_fixed_block_size(allocator)) == NULL,
            TRUE);

  ASSERT_EQ(mem_allocator_destroy(allocator), RET_OK);
}

TEST(MemAllocatorFixedBlock, large_block_many_allocations) {
  enum { kAllocCount = 24, kBlockSize = 40 };
  void* blocks[kAllocCount] = {0};
  mem_allocator_t* allocator = mem_allocator_fixed_block_create(kBlockSize, 5);
  ASSERT_EQ(allocator != NULL, TRUE);

  for (uint32_t i = 0; i < kAllocCount; i++) {
    blocks[i] = MEM_ALLOCATOR_ALLOC(allocator, mem_allocator_fixed_block_size(allocator));
    ASSERT_EQ(blocks[i] != NULL, TRUE);
    memset(blocks[i], (int)(i + 1), kBlockSize);
  }

  for (int32_t i = kAllocCount - 1; i >= 0; i--) {
    MEM_ALLOCATOR_FREE(allocator, blocks[i]);
  }

  ASSERT_EQ(mem_allocator_destroy(allocator), RET_OK);
}

TEST(MemAllocatorFixedBlock, block_alignment) {
  enum { kNr = 12 };
  uint32_t i = 0;
  uint32_t s = 0;
  void* blocks[kNr] = {0};
  const uint32_t sizes[] = {(uint32_t)sizeof(void*), 8, 12, 16, 24};
  const uint32_t nr = (uint32_t)(sizeof(sizes) / sizeof(sizes[0]));

  for (s = 0; s < nr; s++) {
    uint32_t size = sizes[s];
    /* 初始只有 4 块，分配 12 块会触发扩容，覆盖多个池 */
    mem_allocator_t* allocator = mem_allocator_fixed_block_create(size, 4);
    ASSERT_EQ(allocator != NULL, TRUE);

    for (i = 0; i < kNr; i++) {
      blocks[i] = MEM_ALLOCATOR_ALLOC(allocator, size);
      ASSERT_EQ(blocks[i] != NULL, TRUE);

      /* 对齐量 A >= sizeof(void*)，且 A 整除 stride，所以每块至少按指针大小对齐 */
      ASSERT_EQ(((uintptr_t)blocks[i]) % sizeof(void*), (uintptr_t)0);
      /* 块与块之间必须保持同一对齐（逐块对齐） */
      ASSERT_EQ(((uintptr_t)blocks[i]) % 8, ((uintptr_t)blocks[0]) % 8);
    }

    /* 平台最大自然对齐达到 16 时，16 字节的块必须 16 字节对齐 */
    if (size == 16 && sizeof(long double) >= 16) {
      for (i = 0; i < kNr; i++) {
        ASSERT_EQ(((uintptr_t)blocks[i]) % 16, (uintptr_t)0);
      }
    }

    for (i = 0; i < kNr; i++) {
      MEM_ALLOCATOR_FREE(allocator, blocks[i]);
      blocks[i] = NULL;
    }

    ASSERT_EQ(mem_allocator_destroy(allocator), RET_OK);
  }
}

TEST(MemAllocatorFixedBlock, block_no_overlap) {
  enum { kNr = 8, kBlockSize = 16 };
  uint32_t i = 0;
  uint32_t j = 0;
  void* blocks[kNr] = {0};
  mem_allocator_t* allocator = mem_allocator_fixed_block_create(kBlockSize, kNr);
  ASSERT_EQ(allocator != NULL, TRUE);

  /* 取空一个池，逐块写不同标记：对齐填充若没算进池大小，块之间会互相覆盖 */
  for (i = 0; i < kNr; i++) {
    blocks[i] = MEM_ALLOCATOR_ALLOC(allocator, kBlockSize);
    ASSERT_EQ(blocks[i] != NULL, TRUE);
    memset(blocks[i], (int)(i + 1), kBlockSize);
  }

  for (i = 0; i < kNr; i++) {
    uint8_t* p = (uint8_t*)blocks[i];
    for (j = 0; j < kBlockSize; j++) {
      ASSERT_EQ(p[j], (uint8_t)(i + 1));
    }
  }

  for (i = 0; i < kNr; i++) {
    MEM_ALLOCATOR_FREE(allocator, blocks[i]);
  }

  ASSERT_EQ(mem_allocator_destroy(allocator), RET_OK);
}

TEST(MemAllocatorFixedBlock, pool_overflow) {
  mem_allocator_t* allocator = NULL;

  /* 块数过多：池大小超出 uint32，应拒绝创建，而不是分配一个偏小的池 */
  ASSERT_CATCH(allocator = mem_allocator_fixed_block_create(sizeof(int), 0xFFFFFFFF));
  ASSERT_EQ(allocator, nullptr);

  /* 块尺寸大到取整对齐后回绕，同样应拒绝 */
  ASSERT_CATCH(allocator = mem_allocator_fixed_block_create(0xFFFFFFFF, 1));
  ASSERT_EQ(allocator, nullptr);

  /* 单个块就超过 uint32 可用范围 */
  ASSERT_CATCH(allocator = mem_allocator_fixed_block_create(0xFFFFFFF0, 1));
  ASSERT_EQ(allocator, nullptr);

  /* 正常尺寸不受影响 */
  allocator = mem_allocator_fixed_block_create(sizeof(int), 100);
  ASSERT_EQ(allocator != NULL, TRUE);
  ASSERT_EQ(mem_allocator_destroy(allocator), RET_OK);
}
