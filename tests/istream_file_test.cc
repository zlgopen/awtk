#include "tkc/fs.h"
#include "tkc/mem.h"
#include "gtest/gtest.h"
#include "streams/file/istream_file.h"

TEST(IStreamFile, readline) {
  str_t str;
  tk_istream_t* is = tk_istream_file_create("tests/testdata/test_lines.txt");

  str_init(&str, 1024);
  ASSERT_EQ(tk_istream_read_line_str(is, &str), RET_OK);
  ASSERT_STREQ(str.str, "");

  ASSERT_EQ(tk_istream_read_line_str(is, &str), RET_OK);
  ASSERT_STREQ(str.str, "line2");

  ASSERT_EQ(tk_istream_read_line_str(is, &str), RET_OK);
  ASSERT_STREQ(str.str, "");

  ASSERT_EQ(tk_istream_read_line_str(is, &str), RET_OK);
  ASSERT_STREQ(str.str, "line4");

  ASSERT_EQ(tk_istream_read_line_str(is, &str), RET_OK);
  ASSERT_STREQ(str.str,
               "line5xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
               "xxxxxxxxxxxxxxxxxxx");

  ASSERT_NE(tk_istream_read_line_str(is, &str), RET_OK);

  str_reset(&str);
  tk_object_unref(TK_OBJECT(is));
}
