#include "gtest/gtest.h"

#include "streams/mem/iostream_mem.h"
#include "streams/noisy/ostream_noisy.h"
#include "streams/noisy/iostream_noisy.h"

TEST(IOStreamNoisy, noerror) {
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
  ASSERT_EQ(tk_istream_read(is, rbuff, size), size);
  ASSERT_STREQ((char*)rbuff, str);

  OBJECT_UNREF(io);
  OBJECT_UNREF(mem);
}

TEST(IOStreamNoisy, all_error) {
  uint8_t buff[62];
  uint8_t rbuff[16];
  const char* str = "12345";
  uint32_t size = strlen(str) + 1;
  tk_iostream_t* mem = tk_iostream_mem_create(buff, sizeof(buff), buff, sizeof(buff), FALSE);
  tk_iostream_t* io = tk_iostream_noisy_create(mem);
  tk_ostream_t* os = tk_iostream_get_ostream(io);
  tk_istream_t* is = tk_iostream_get_istream(io);

  object_set_prop_int(OBJECT(os), TK_OSTREAM_NOISY_PROP_ERROR_LEVEL, 1);
  ASSERT_EQ(tk_ostream_write(os, (uint8_t*)str, size), size);
  ASSERT_EQ(tk_istream_read(is, rbuff, size), size);
  ASSERT_STRNE((char*)rbuff, str);

  OBJECT_UNREF(io);
  OBJECT_UNREF(mem);
}

static void test_n(tk_ostream_t* os, tk_istream_t* is, uint32_t level) {
  uint32_t i = 0;
  uint8_t rbuff[16];
  const char* str = "12345";
  uint32_t size = strlen(str) + 1;
  object_set_prop_int(OBJECT(os), TK_OSTREAM_NOISY_PROP_ERROR_LEVEL, level);

  for (i = 0; i < level; i++) {
    ASSERT_EQ(tk_ostream_write(os, (uint8_t*)str, size), size);
    ASSERT_EQ(tk_istream_read(is, rbuff, size), size);

    if (i % level == 0) {
      ASSERT_STRNE((char*)rbuff, str);
    } else {
      ASSERT_STREQ((char*)rbuff, str);
    }
  }
}

TEST(IOStreamNoisy, error_n) {
  uint32_t i = 0;
  uint8_t buff[1024];

  for (i = 2; i < 10; i++) {
    tk_iostream_t* mem = tk_iostream_mem_create(buff, sizeof(buff), buff, sizeof(buff), FALSE);
    tk_iostream_t* io = tk_iostream_noisy_create(mem);
    tk_ostream_t* os = tk_iostream_get_ostream(io);
    tk_istream_t* is = tk_iostream_get_istream(io);

    test_n(os, is, i);

    OBJECT_UNREF(io);
    OBJECT_UNREF(mem);
  }
}
