#include <string>
#include "tkc/fs.h"
#include "tkc/utils.h"
#include "gtest/gtest.h"
#include "tkc/zip_file.h"

TEST(ZipFile, basic) {
  const char* dst = "temp";
  const char* filename = "tests/testdata/data.zip";

  ASSERT_EQ(zip_file_extract(filename, dst, NULL), RET_OK);
  ASSERT_EQ(file_exist("temp/data/timer.json"), TRUE);
  ASSERT_EQ(file_exist("temp/data/fscript1.json"), TRUE);
  ASSERT_EQ(file_exist("temp/data/multi.json"), TRUE);
  ASSERT_EQ(file_get_size("temp/data/multi.json"), 2132);
  ASSERT_EQ(file_get_size("temp/data/timer.json"), 877);
  fs_remove_dir_r(os_fs(), dst);
}
