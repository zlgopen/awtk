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
      ASSERT_EQ(log->index, 1);
    } else {
      ASSERT_EQ(log->index, 0);
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
