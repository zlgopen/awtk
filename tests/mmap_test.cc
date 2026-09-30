#include "tkc/fs.h"
#include "tkc/mmap.h"
#include "gtest/gtest.h"
#include <string.h>

/* 准备一个内容确定的文件 */
static void mmap_test_prepare(const char* filename, const char* content) {
  file_remove(filename);
  file_write(filename, content, strlen(content));
}

TEST(MMap, read) {
  const char* str = "test";
  const char* filename = "test.bin";

  mmap_test_prepare(filename, str);
  mmap_t* map = mmap_create(filename, FALSE, FALSE);
  ASSERT_TRUE(map != NULL);
  ASSERT_EQ(map->size, (uint32_t)strlen(str));
  ASSERT_EQ(memcmp(map->data, str, strlen(str)) == 0, TRUE);
  mmap_destroy(map);
  file_remove(filename);
}

TEST(MMap, write) {
  const char* filename = "test.bin";

  mmap_test_prepare(filename, "test");
  mmap_t* map = mmap_create(filename, TRUE, FALSE);
  ASSERT_TRUE(map != NULL);
  ASSERT_EQ(map->size, (uint32_t)4);
  ASSERT_EQ(memcmp(map->data, "test", 4) == 0, TRUE);
  memcpy(map->data, "1234", 4);
  ASSERT_EQ(memcmp(map->data, "1234", 4) == 0, TRUE);
  mmap_destroy(map);
  file_remove(filename);
}

TEST(MMap, file_not_exist) {
  const char* filename = "mmap_no_such_file.bin";
  mmap_t* map = NULL;

  file_remove(filename);
  map = mmap_create(filename, FALSE, FALSE);
  ASSERT_TRUE(map == NULL);
  if (map != NULL) {
    mmap_destroy(map);
  }
}

TEST(MMap, empty_file) {
  const char* filename = "mmap_empty.bin";
  mmap_t* map = NULL;

  file_remove(filename);
  file_write(filename, "", 0);
  map = mmap_create(filename, FALSE, FALSE);
  ASSERT_TRUE(map == NULL);
  if (map != NULL) {
    mmap_destroy(map);
  }
  file_remove(filename);
}

TEST(MMap, bad_param) {
  ASSERT_TRUE(mmap_create(NULL, FALSE, FALSE) == NULL);
  ASSERT_TRUE(mmap_destroy(NULL) == RET_BAD_PARAMS);
}

/* 同一文件映射两次，两边互相看得见 */
TEST(MMap, shared_view) {
  const char* filename = "mmap_shared.bin";
  mmap_t* a = NULL;
  mmap_t* b = NULL;
  char* pa = NULL;
  char* pb = NULL;

  mmap_test_prepare(filename, "abcd");

  a = mmap_create(filename, TRUE, TRUE);
  ASSERT_TRUE(a != NULL);
  b = mmap_create(filename, TRUE, TRUE);
  ASSERT_TRUE(b != NULL);

  pa = (char*)a->data;
  pb = (char*)b->data;

  /* 两次映射的虚拟地址可以不同，所以只比较内容 */
  pa[0] = 'X';
  ASSERT_EQ(memcmp(pb, "Xbcd", 4) == 0, TRUE);
  ASSERT_EQ(memcmp(pa, "Xbcd", 4) == 0, TRUE);

  mmap_destroy(a);
  mmap_destroy(b);
  file_remove(filename);
}

/* 同一文件映射两次，两边互不影响，也不回写文件 */
TEST(MMap, private_view) {
  const char* filename = "mmap_private.bin";
  mmap_t* a = NULL;
  mmap_t* b = NULL;
  mmap_t* r = NULL;
  char* pa = NULL;
  char* pb = NULL;

  mmap_test_prepare(filename, "abcd");

  a = mmap_create(filename, TRUE, FALSE);
  ASSERT_TRUE(a != NULL);
  b = mmap_create(filename, TRUE, FALSE);
  ASSERT_TRUE(b != NULL);

  pa = (char*)a->data;
  pb = (char*)b->data;

  pa[0] = 'X';
  ASSERT_EQ(memcmp(pa, "Xbcd", 4) == 0, TRUE);
  ASSERT_EQ(memcmp(pb, "abcd", 4) == 0, TRUE);

  mmap_destroy(a);
  mmap_destroy(b);

  r = mmap_create(filename, FALSE, FALSE);
  ASSERT_TRUE(r != NULL);
  ASSERT_EQ(memcmp(r->data, "abcd", 4) == 0, TRUE);
  mmap_destroy(r);
  file_remove(filename);
}

/* 只读 + 共享 */
TEST(MMap, read_shared_view) {
  const char* filename = "mmap_read_shared.bin";
  mmap_t* a = NULL;
  mmap_t* b = NULL;

  mmap_test_prepare(filename, "abcd");

  a = mmap_create(filename, FALSE, TRUE);
  ASSERT_TRUE(a != NULL);
  b = mmap_create(filename, FALSE, TRUE);
  ASSERT_TRUE(b != NULL);

  ASSERT_EQ(memcmp(a->data, "abcd", 4) == 0, TRUE);
  ASSERT_EQ(memcmp(b->data, "abcd", 4) == 0, TRUE);

  mmap_destroy(a);
  mmap_destroy(b);
  file_remove(filename);
}
