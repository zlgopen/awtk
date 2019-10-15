#include "gtest/gtest.h"

#include "streams/mem/iostream_mem.h"
#include "streams/noisy/ostream_noisy.h"
#include "streams/noisy/iostream_noisy.h"

TEST(IOStreamNoisy, basic) {
  uint8_t buff[62];
  uint8_t rbuff[16];
  const char* str = "12345";
  uint32_t size = strlen(str) + 1;
  tk_iostream_t* mem = tk_iostream_mem_create(buff, sizeof(buff), buff, sizeof(buff), FALSE);
  tk_iostream_t* io = tk_iostream_noisy_create(mem);
  tk_ostream_t* os = tk_iostream_get_ostream(io);
  tk_istream_t* is = tk_iostream_get_istream(io);

  object_set_prop_int(OBJECT(os), TK_OSTREAM_NOISY_PROP_ERROR_LEVEL, 0);
  ASSERT_EQ(tk_ostream_write(os, (uint8_t*)str, size), size);
  ASSERT_EQ(tk_istream_read(is, rbuff, sizeof(rbuff)), size);
  ASSERT_STREQ((char*)rbuff, str);

  OBJECT_UNREF(io);
  OBJECT_UNREF(mem);
}

