#include "tkc/rlog.h"
#include "tkc/mem.h"
#include "gtest/gtest.h"

TEST(RLog, basic) {
  rlog_t* log = rlog_create("tests/testdata/%d.log", 1024, 64);
  ASSERT_EQ(log != NULL, true);
  rlog_destroy(log);
}

TEST(RLog, basic1) {
  uint32_t i = 0;
  uint32_t size = 0;
  char* data = NULL;
  fs_remove_file(os_fs(), "tests/testdata/0.log");
  fs_remove_file(os_fs(), "tests/testdata/1.log");
  rlog_t* log = rlog_create("tests/testdata/%d.log", 1024, 64);
  ASSERT_EQ(log != NULL, true);
  for (i = 0; i < 1000; i++) {
    rlog_print(log, "hello wolrd %04d\n", i);
    if (i >= 60) {
      ASSERT_EQ(log->index, 1u);
    } else {
      ASSERT_EQ(log->index, 0u);
    }
  }
  rlog_destroy(log);
  data = (char*)file_read("tests/testdata/0.log", &size);
  ASSERT_EQ(memcmp(data, "hello wolrd 0900", strlen("hello wolrd 0900")), 0);
  TKMEM_FREE(data);

  data = (char*)file_read("tests/testdata/1.log", &size);
  ASSERT_EQ(memcmp(data, "hello wolrd 0960", strlen("hello wolrd 0960")), 0);
  TKMEM_FREE(data);
}

TEST(RLog, basic2) {
  uint32_t i = 0;
  uint32_t size = 0;
  char buff[1024 + 1];
  ret_t ret;
  fs_remove_file(os_fs(), "tests/testdata/0.log");
  fs_remove_file(os_fs(), "tests/testdata/1.log");
  rlog_t* log = rlog_create("tests/testdata/%d.log", 1024, 64);
  ASSERT_EQ(log != NULL, true);
  for (i = 0; i < 64; i++) {
    rlog_print(log, "hellowolrd %04d\n", i);
    ASSERT_EQ(log->index, 0u);
  }
  for (i = 0; i < 32; i++) {
    rlog_print(log, "hellowolrd %04d\n", i + 64);
    ASSERT_EQ(log->index, 1u);
  }
  rlog_size(log, &size);
  ASSERT_EQ(size, 16 * 96);

  memset(buff, 0, sizeof(buff));
  ret = rlog_read(log, 512, buff, sizeof(buff) - 1, &size);
  ASSERT_EQ(ret, RET_OK);
  ASSERT_EQ(size, 1024);
  ASSERT_EQ(tk_str_eq_with_len(buff, "helloworld 0032\n", 16), 0);

  ASSERT_EQ(rlog_clear(log), RET_OK);
  rlog_size(log, &size);
  ASSERT_EQ(size, 0);

  ret = rlog_print(log, "hellowolrd %04d\n", i + 64);
  ASSERT_EQ(ret, RET_OK);
  rlog_size(log, &size);
  ASSERT_EQ(size, 16);

  memset(buff, 0, sizeof(buff));
  ret = rlog_read(log, 0, buff, sizeof(buff) - 1, &size);
  ASSERT_EQ(ret, RET_OK);
  ASSERT_EQ(size, 16);
  ASSERT_EQ(tk_str_eq_with_len(buff, "helloworld 0096\n", 16), 0);

  rlog_destroy(log);
}
