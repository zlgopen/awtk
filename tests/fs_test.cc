#include "tkc/fs.h"
#include "tkc/mem.h"
#include "gtest/gtest.h"

TEST(Fs, basic) {
  char buff[128];
  uint32_t size = 0;
  const char* str = "hello world";
  const char* filename = "test.bin";

  file_write(filename, str, strlen(str));
  char* ret = (char*)file_read(filename, &size);
  ASSERT_EQ(file_read_part(filename, buff, sizeof(buff), 0), strlen(str));
  ASSERT_EQ(strcmp(ret, str), 0);
  ASSERT_EQ(size, strlen(str));
  file_remove(filename);
  TKMEM_FREE(ret);
}
