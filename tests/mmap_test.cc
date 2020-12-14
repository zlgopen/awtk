#include "tkc/fs.h"
#include "tkc/mmap.h"
#include "gtest/gtest.h"

TEST(MMap, read) {
  const char* str = "test";
  const char* filename = "test.bin";
  file_write(filename, str, strlen(str));
  mmap_t* map = mmap_create(filename, FALSE, FALSE);
  ASSERT_EQ(map->size, strlen(str));
  ASSERT_EQ(memcmp(map->data, str, strlen(str)) == 0, TRUE);
  mmap_destroy(map);
  file_remove(filename);
}

TEST(MMap, write) {
  const char* str = "test";
  const char* filename = "test.bin";
  file_write(filename, str, strlen(str));
  mmap_t* map = mmap_create(filename, TRUE, FALSE);
  ASSERT_EQ(map->size, strlen(str));
  ASSERT_EQ(memcmp(map->data, str, strlen(str)) == 0, TRUE);
  memcpy(map->data, "1234", 4);

  str = "1234";
  ASSERT_EQ(memcmp(map->data, str, strlen(str)) == 0, TRUE);
  mmap_destroy(map);

  file_remove(filename);
}
