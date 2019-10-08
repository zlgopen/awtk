#include "gtest/gtest.h"
#include "streams/istream_mem.h"
#include "streams/istream_buffered.h"

TEST(IStreamBuffered, basic) {
  char c = 0;
  uint32_t i = 0;
  const char* str =
      "1234567890qwertyutyuioyupisdfaasdfasdfjaserwqqerlqo20341qasdfasdfas34  q234134132";
  tk_istream_t* mem = tk_istream_mem_create((uint8_t*)str, sizeof(str), 0, FALSE);
  tk_istream_t* is = tk_istream_buffered_create(mem, 32);

  for (i = 0; i < sizeof(str); i++) {
    ASSERT_EQ(tk_istream_read(is, (uint8_t*)&c, 1), 1);
    ASSERT_EQ(c == str[i], true);
  }

  object_unref(OBJECT(is));
  object_unref(OBJECT(mem));
}
