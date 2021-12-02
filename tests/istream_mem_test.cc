#include "streams/mem/istream_mem.h"
#include "gtest/gtest.h"

TEST(IStreamMem, basic) {
  uint8_t buff[4];
  const char* str = "111122223333";
  tk_istream_t* is = tk_istream_mem_create((uint8_t*)str, strlen(str), 0, FALSE);

  ASSERT_EQ(tk_object_get_prop_bool(TK_OBJECT(is), TK_STREAM_PROP_IS_EOS, TRUE), FALSE);

  ASSERT_EQ(tk_istream_read(is, buff, 4), 4);
  ASSERT_EQ(memcmp(buff, "1111", 4), 0);

  ASSERT_EQ(tk_istream_read(is, buff, 4), 4);
  ASSERT_EQ(memcmp(buff, "2222", 4), 0);

  ASSERT_EQ(tk_istream_read(is, buff, 4), 4);
  ASSERT_EQ(memcmp(buff, "3333", 4), 0);

  ASSERT_EQ(tk_istream_read(is, buff, 4), 0);
  ASSERT_EQ(tk_object_get_prop_bool(TK_OBJECT(is), TK_STREAM_PROP_IS_EOS, FALSE), TRUE);

  tk_object_unref(TK_OBJECT(is));
}

TEST(IStreamMem, packet_size) {
  uint8_t buff[12];
  const char* str = "111122223333";
  tk_istream_t* is = tk_istream_mem_create((uint8_t*)str, strlen(str), 4, FALSE);

  ASSERT_EQ(tk_istream_read(is, buff, 12), 4);
  ASSERT_EQ(memcmp(buff, "1111", 4), 0);

  ASSERT_EQ(tk_istream_read(is, buff, 10), 4);
  ASSERT_EQ(memcmp(buff, "2222", 4), 0);

  ASSERT_EQ(tk_istream_read(is, buff, 8), 4);
  ASSERT_EQ(memcmp(buff, "3333", 4), 0);

  ASSERT_EQ(tk_istream_read(is, buff, 4), 0);

  ASSERT_EQ(tk_istream_seek(is, 0), RET_OK);
  ASSERT_EQ(tk_istream_read(is, buff, 12), 4);
  ASSERT_EQ(memcmp(buff, "1111", 4), 0);

  tk_object_unref(TK_OBJECT(is));
}

TEST(IStreamMem, read_len) {
  uint8_t buff[12];
  const char* str = "111122223333";
  tk_istream_t* is = tk_istream_mem_create((uint8_t*)str, strlen(str), 4, FALSE);

  ASSERT_EQ(tk_istream_read_len(is, buff, 12, 3000), 12);
  ASSERT_EQ(memcmp(buff, str, 12), 0);

  tk_object_unref(TK_OBJECT(is));
}

TEST(IStreamMem, seek) {
  uint8_t buff[12];
  const char* str = "111122223333";
  tk_istream_t* is = tk_istream_mem_create((uint8_t*)str, strlen(str), 4, FALSE);

  ASSERT_EQ(tk_istream_eos(is), FALSE);
  ASSERT_EQ(tk_istream_read_len(is, buff, 12, 3000), 12);
  ASSERT_EQ(memcmp(buff, str, 12), 0);
  ASSERT_EQ(tk_istream_eos(is), TRUE);
  ASSERT_EQ(tk_istream_tell(is), 12);

  ASSERT_EQ(tk_istream_seek(is, 0), RET_OK);
  ASSERT_EQ(tk_istream_tell(is), 0);

  ASSERT_EQ(tk_istream_read_len(is, buff, 12, 3000), 12);
  ASSERT_EQ(memcmp(buff, str, 12), 0);
  ASSERT_EQ(tk_istream_eos(is), TRUE);
  ASSERT_EQ(tk_istream_tell(is), 12);

  tk_object_unref(TK_OBJECT(is));
}

TEST(IStreamMem, read_line) {
  str_t str;
  const char* content = "1111\n2222\r\n\r\n3333\n";
  tk_istream_t* is = tk_istream_mem_create((uint8_t*)content, strlen(content), 4, FALSE);

  str_init(&str, 1024);
  ASSERT_EQ(tk_istream_read_line_str(is, &str), RET_OK);
  ASSERT_STREQ(str.str, "1111");

  ASSERT_EQ(tk_istream_read_line_str(is, &str), RET_OK);
  ASSERT_STREQ(str.str, "2222");

  ASSERT_EQ(tk_istream_read_line_str(is, &str), RET_OK);
  ASSERT_STREQ(str.str, "");

  ASSERT_EQ(tk_istream_read_line_str(is, &str), RET_OK);
  ASSERT_STREQ(str.str, "3333");

  ASSERT_NE(tk_istream_read_line_str(is, &str), RET_OK);

  str_reset(&str);
  tk_object_unref(TK_OBJECT(is));
}
