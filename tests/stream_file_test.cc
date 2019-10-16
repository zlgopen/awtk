#include "streams/file/ostream_file.h"
#include "streams/file/istream_file.h"
#include "gtest/gtest.h"

static void test_write(const char* filename) {
  tk_ostream_t* is = tk_ostream_file_create(filename);

  ASSERT_EQ(tk_ostream_write(is, (uint8_t*)"aaaa", 4), 4);
  ASSERT_EQ(tk_ostream_write(is, (uint8_t*)"2222", 4), 4);
  ASSERT_EQ(tk_ostream_write(is, (uint8_t*)"3333", 4), 4);

  ASSERT_EQ(tk_ostream_seek(is, 0), RET_OK);
  ASSERT_EQ(tk_ostream_write(is, (uint8_t*)"1111", 4), 4);

  object_unref(OBJECT(is));
}

static void test_read(const char* filename) {
  uint8_t buff[4];
  tk_istream_t* is = tk_istream_file_create(filename);

  ASSERT_EQ(tk_istream_read(is, buff, 4), 4);
  ASSERT_EQ(memcmp(buff, "1111", 4), 0);

  ASSERT_EQ(tk_istream_read(is, buff, 4), 4);
  ASSERT_EQ(memcmp(buff, "2222", 4), 0);

  ASSERT_EQ(tk_istream_read(is, buff, 4), 4);
  ASSERT_EQ(memcmp(buff, "3333", 4), 0);

  ASSERT_EQ(tk_istream_read(is, buff, 4), 0);

  ASSERT_EQ(tk_istream_seek(is, 0), RET_OK);
  ASSERT_EQ(tk_istream_read(is, buff, 4), 4);
  ASSERT_EQ(memcmp(buff, "1111", 4), 0);

  ASSERT_EQ(tk_istream_seek(is, 256), RET_OK);
  ASSERT_EQ(tk_istream_read(is, buff, 4), 0);
  ASSERT_EQ(object_get_prop_bool(OBJECT(is), TK_STREAM_PROP_IS_EOS, FALSE), TRUE);

  object_unref(OBJECT(is));
}

TEST(StreamFile, basic) {
  const char* filename = "./tests/testdata/istream.txt";

  test_write(filename);
  test_read(filename);
}
