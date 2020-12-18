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

TEST(Fs, read_line1) {
  char buff[128];
  const char* str = "1\r22\r\n333\n4444";
  const char* filename = "test.bin";

  file_write(filename, str, strlen(str));

  fs_file_t* f = fs_open_file(os_fs(), filename, "rb");

  ASSERT_EQ(fs_file_read_line(f, buff, sizeof(buff) - 1), 1);
  ASSERT_STREQ(buff, "1");
  ASSERT_EQ(fs_file_eof(f), FALSE);

  ASSERT_EQ(fs_file_read_line(f, buff, sizeof(buff) - 1), 2);
  ASSERT_STREQ(buff, "22");
  ASSERT_EQ(fs_file_eof(f), FALSE);

  ASSERT_EQ(fs_file_read_line(f, buff, sizeof(buff) - 1), 3);
  ASSERT_STREQ(buff, "333");
  ASSERT_EQ(fs_file_eof(f), FALSE);

  ASSERT_EQ(fs_file_read_line(f, buff, 3), 3);
  ASSERT_STREQ(buff, "444");
  ASSERT_EQ(fs_file_eof(f), FALSE);

  ASSERT_EQ(fs_file_read_line(f, buff, 3), 1);
  ASSERT_STREQ(buff, "4");

  ASSERT_EQ(fs_file_read_line(f, buff, 3), 0);
  ASSERT_EQ(fs_file_eof(f), TRUE);

  fs_file_close(f);

  file_remove(filename);
}

TEST(Fs, read_line2) {
  char buff[256];
  const char* str =
      "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"
      "11111111111111111111111111111111111";
  const char* filename = "test.bin";

  file_write(filename, str, strlen(str));

  fs_file_t* f = fs_open_file(os_fs(), filename, "rb");

  ASSERT_EQ(fs_file_read_line(f, buff, sizeof(buff) - 1), 127);
  ASSERT_EQ(fs_file_eof(f), FALSE);

  ASSERT_EQ(fs_file_read_line(f, buff, 3), 0);
  ASSERT_EQ(fs_file_eof(f), TRUE);

  fs_file_close(f);

  file_remove(filename);
}

TEST(Fs, read_line3) {
  char buff[256];
  const char* str =
      "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"
      "111111111111111111111111111111111111\r\n";
  const char* filename = "test.bin";

  file_write(filename, str, strlen(str));

  fs_file_t* f = fs_open_file(os_fs(), filename, "rb");

  ASSERT_EQ(fs_file_read_line(f, buff, sizeof(buff) - 1), 128);
  ASSERT_EQ(fs_file_eof(f), FALSE);

  ASSERT_EQ(fs_file_read_line(f, buff, 3), 0);
  ASSERT_EQ(fs_file_eof(f), TRUE);

  fs_file_close(f);

  file_remove(filename);
}

TEST(Fs, read_line4) {
  char buff[256];
  const char* str =
      "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"
      "11111111111111111111111111111111111\r\nabc";
  const char* filename = "test.bin";

  file_write(filename, str, strlen(str));

  fs_file_t* f = fs_open_file(os_fs(), filename, "rb");

  ASSERT_EQ(fs_file_read_line(f, buff, sizeof(buff) - 1), 127);
  ASSERT_EQ(fs_file_read_line(f, buff, sizeof(buff) - 1), 3);
  ASSERT_EQ(fs_file_eof(f), FALSE);

  ASSERT_EQ(fs_file_read_line(f, buff, 3), 0);
  ASSERT_EQ(fs_file_eof(f), TRUE);

  fs_file_close(f);

  file_remove(filename);
}

TEST(Fs, create_dir_r) {
  char cwd[MAX_PATH + 1];
  ASSERT_EQ(fs_get_cwd(os_fs(), cwd), RET_OK);

  ASSERT_EQ(fs_create_dir_r(os_fs(), "a/b/c/d"), RET_OK);
  ASSERT_EQ(fs_dir_exist(os_fs(), "a/b/c/d"), TRUE);
  ASSERT_EQ(file_write("a/b/c/d/test.txt", "hello", 5), RET_OK);
  ASSERT_EQ(fs_change_dir(os_fs(), "a/b/c/d"), RET_OK);
  ASSERT_EQ(fs_change_dir(os_fs(), cwd), RET_OK);

  ASSERT_EQ(fs_remove_dir_r(os_fs(), "a"), RET_OK);
  ASSERT_EQ(fs_dir_exist(os_fs(), "a"), FALSE);
}
