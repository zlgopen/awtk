#include "gtest/gtest.h"
#include "tkc/mem_allocator_fixed_block.h"

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

  ASSERT_EQ(mem_allocator_destroy(allocator), RET_OK);

  (void)p2;
  (void)p3;
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
