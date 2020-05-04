#include "tkc/fs.h"
#include "tkc/mem.h"
#include "gtest/gtest.h"

TEST(Fs, basic) {
  ASSERT_EQ(fs_test(os_fs()), RET_OK);
}

TEST(Fs, read_part) {
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

TEST(Fs, eof) {
  char buff[128];
  const char* str = "hello world";
  const char* filename = "test.bin";

  file_write(filename, str, strlen(str));

  fs_file_t* f = fs_open_file(os_fs(), filename, "r");
  memset(buff, 0x00, sizeof(buff));
  fs_file_read(f, buff, sizeof(buff));
  ASSERT_STREQ(buff, str);
  ASSERT_EQ(fs_file_eof(f), TRUE);
  fs_file_close(f);

  file_remove(filename);
}
