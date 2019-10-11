#include "gtest/gtest.h"
#include "streams/mem/ostream_mem.h"
#include "streams/buffered/ostream_buffered.h"

TEST(OStreamBuffered, basic) {
  char buff[32];

  tk_ostream_t* mem = tk_ostream_mem_create((uint8_t*)buff, sizeof(buff), 0, FALSE);
  tk_ostream_t* os = tk_ostream_buffered_create(mem);

  memset(buff, 0x00, sizeof(buff));

  ASSERT_EQ(tk_ostream_write(os, (uint8_t*)"1234", 4), 4);
  ASSERT_STREQ(buff, "");
  ASSERT_EQ(tk_ostream_buffered_get_size(os), 4);
  ASSERT_EQ(tk_ostream_flush(os), RET_OK);
  ASSERT_STREQ(buff, "1234");

  ASSERT_EQ(tk_ostream_write(os, (uint8_t*)"abcd", 4), 4);
  ASSERT_EQ(tk_ostream_write(os, (uint8_t*)"abcd", 4), 4);
  ASSERT_STREQ(buff, "1234");

  ASSERT_EQ(tk_ostream_buffered_get_size(os), 8);
  ASSERT_EQ(tk_ostream_flush(os), RET_OK);
  ASSERT_STREQ(buff, "1234abcdabcd");

  object_unref(OBJECT(os));
  object_unref(OBJECT(mem));
}
