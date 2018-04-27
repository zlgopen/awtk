#include "base/fs.h"
#include "base/mem.h"
#include "gtest/gtest.h"

TEST(Fs, basic) {
  char buff[128];
  uint32_t size = 0;
  const char* str = "hello world";
  const char* filename = "test.bin";

  fs_write_file(filename, str, strlen(str));
  char* ret = (char*)fs_read_file(filename, &size);
  ASSERT_EQ(fs_read_file_part(filename, buff, sizeof(buff), 0), strlen(str));
  ASSERT_EQ(strcmp(ret, str), 0);
  ASSERT_EQ(size, strlen(str));
  fs_unlink(filename);
  MEM_FREE(ret);
}

