#include "tkc/utils.h"
#include "streams/statistics/iostream_stats.h"
#include "streams/statistics/istream_stats.h"
#include "streams/statistics/ostream_stats.h"
#include "streams/mem/iostream_mem.h"
#include "gtest/gtest.h"

static void test_write(tk_iostream_t* ios, uint8_t* buff) {
  tk_ostream_t* is = tk_iostream_get_ostream(ios);

  ASSERT_EQ(tk_ostream_write(is, (uint8_t*)"aaaa", 4), 4);
  ASSERT_STREQ((char *)buff, "aaaa");
  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(ios), TK_OSTREAM_STATS_PROP_WRITE, -1), 4);

  ASSERT_EQ(tk_ostream_write(is, (uint8_t*)"2222", 4), 4);
  ASSERT_STREQ((char *)buff, "aaaa2222");
  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(ios), TK_OSTREAM_STATS_PROP_WRITE, -1), 8);
  
  ASSERT_EQ(tk_ostream_write(is, (uint8_t*)"3333", 4), 4);
  ASSERT_STREQ((char *)buff, "aaaa22223333");
  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(ios), TK_OSTREAM_STATS_PROP_WRITE, -1), 12);

  ASSERT_EQ(tk_ostream_seek(is, 0), RET_OK);
  ASSERT_EQ(tk_ostream_write(is, (uint8_t*)"1111", 4), 4);
  ASSERT_STREQ((char *)buff, "111122223333");

  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(ios), TK_OSTREAM_STATS_PROP_WRITE, -1), 16);
  ASSERT_EQ(tk_object_exec(TK_OBJECT(ios), TK_OSTREAM_STATS_CMD_RESET, NULL), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(ios), TK_OSTREAM_STATS_PROP_WRITE, -1), 0);
}

static void test_read(tk_iostream_t* ios, const char* str) {
  uint8_t buff[5];
  tk_istream_t* is = tk_iostream_get_istream(ios);

  memset(buff, 0x00, sizeof(buff));
  ASSERT_EQ(tk_istream_read(is, buff, 4), 4);
  ASSERT_STREQ((char *)buff, "1111");
  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(ios), TK_ISTREAM_STATS_PROP_READ, -1), 4);

  ASSERT_EQ(tk_istream_read(is, buff, 4), 4);
  ASSERT_STREQ((char *)buff, "2222");
  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(ios), TK_ISTREAM_STATS_PROP_READ, -1), 8);

  ASSERT_EQ(tk_istream_read(is, buff, 4), 4);
  ASSERT_STREQ((char *)buff, "3333");
  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(ios), TK_ISTREAM_STATS_PROP_READ, -1), 12);

  ASSERT_EQ(tk_istream_read(is, buff, 4), 0);
  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(ios), TK_ISTREAM_STATS_PROP_READ, -1), 12);

  ASSERT_EQ(tk_istream_seek(is, 0), RET_OK);
  ASSERT_EQ(tk_istream_read(is, buff, 4), 4);
  ASSERT_STREQ((char *)buff, "1111");
  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(ios), TK_ISTREAM_STATS_PROP_READ, -1), 16);

  ASSERT_EQ(tk_istream_seek(is, 12), RET_OK);
  ASSERT_EQ(tk_istream_read(is, buff, 4), 0);
  ASSERT_EQ(tk_object_get_prop_bool(TK_OBJECT(is), TK_STREAM_PROP_IS_EOS, FALSE), TRUE);

  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(ios), TK_ISTREAM_STATS_PROP_READ, -1), 16);
  ASSERT_EQ(tk_object_exec(TK_OBJECT(ios), TK_ISTREAM_STATS_CMD_RESET, NULL), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(ios), TK_ISTREAM_STATS_PROP_READ, -1), 0);
  tk_object_unref(TK_OBJECT(ios));
}

TEST(StreamStats, basic) {
  uint8_t buff[1024];
  const char* str = "111122223333";
  tk_iostream_t* mem_iostream = tk_iostream_mem_create((void *)str, tk_strlen(str), buff, sizeof(buff), FALSE);
  tk_iostream_t* stats_iostream = tk_iostream_stats_create(mem_iostream);

  memset(buff, 0, sizeof(buff));
  test_write(stats_iostream, buff);
  test_read(stats_iostream, str);
}
