#include "base/buffer.h"
#include "gtest/gtest.h"
#include <string>

using std::string;

TEST(Buffer, basic) {
  uint8_t v8;
  uint16_t v16;
  uint32_t v32;
  uint8_t data[128];
  wbuffer_t wbuffer;
  rbuffer_t rbuffer;
  ASSERT_EQ(wbuffer_init(&wbuffer, data, sizeof(data)), &wbuffer);
  ASSERT_EQ(rbuffer_init(&rbuffer, data, sizeof(data)), &rbuffer);

  for (uint32_t i = 0; i < 10; i++) {
    ASSERT_EQ(wbuffer_write_uint8(&wbuffer, i), RET_OK);
    ASSERT_EQ(wbuffer.cursor, 11 * i + 1);
    ASSERT_EQ(rbuffer_read_uint8(&rbuffer, &v8), RET_OK);
    ASSERT_EQ((uint32_t)v8, i);

    ASSERT_EQ(wbuffer_write_uint16(&wbuffer, i), RET_OK);
    ASSERT_EQ(wbuffer.cursor, 11 * i + 3);
    ASSERT_EQ(rbuffer_read_uint16(&rbuffer, &v16), RET_OK);
    ASSERT_EQ((uint32_t)v16, i);

    ASSERT_EQ(wbuffer_write_uint32(&wbuffer, i), RET_OK);
    ASSERT_EQ(wbuffer.cursor, 11 * i + 7);
    ASSERT_EQ(rbuffer_read_uint32(&rbuffer, &v32), RET_OK);
    ASSERT_EQ((uint32_t)v32, i);

    ASSERT_EQ(wbuffer_write_binary(&wbuffer, &i, sizeof(i)), RET_OK);
    ASSERT_EQ(wbuffer.cursor, 11 * i + 11);
    ASSERT_EQ(rbuffer_read_binary(&rbuffer, &v32, sizeof(v32)), RET_OK);
    ASSERT_EQ((uint32_t)v32, i);
  }
}

TEST(Buffer, WbufferSkip) {
  uint8_t data[128];
  wbuffer_t wbuffer;
  ASSERT_EQ(wbuffer_init(&wbuffer, data, sizeof(data)), &wbuffer);

  ASSERT_EQ(wbuffer_skip(&wbuffer, 10), RET_OK);
  ASSERT_EQ(wbuffer.cursor, 10);

  ASSERT_EQ(wbuffer_skip(&wbuffer, -10), RET_OK);
  ASSERT_EQ(wbuffer.cursor, 0);

  ASSERT_EQ(wbuffer_skip(&wbuffer, -10), RET_BAD_PARAMS);
  ASSERT_EQ(wbuffer.cursor, 0);
}

TEST(Buffer, RbufferSkip) {
  uint8_t data[128];
  rbuffer_t rbuffer;
  ASSERT_EQ(rbuffer_init(&rbuffer, data, sizeof(data)), &rbuffer);

  ASSERT_EQ(rbuffer_skip(&rbuffer, 10), RET_OK);
  ASSERT_EQ(rbuffer.cursor, 10);

  ASSERT_EQ(rbuffer_skip(&rbuffer, -10), RET_OK);
  ASSERT_EQ(rbuffer.cursor, 0);

  ASSERT_EQ(rbuffer_skip(&rbuffer, -10), RET_BAD_PARAMS);
  ASSERT_EQ(rbuffer.cursor, 0);
}

TEST(Buffer, string) {
  const char* str = NULL;
  const char* cstr = "hello world";
  uint8_t data[128];
  wbuffer_t wbuffer;
  rbuffer_t rbuffer;
  ASSERT_EQ(wbuffer_init(&wbuffer, data, sizeof(data)), &wbuffer);
  ASSERT_EQ(rbuffer_init(&rbuffer, data, sizeof(data)), &rbuffer);

  ASSERT_EQ(wbuffer_write_string(&wbuffer, cstr), RET_OK);
  ASSERT_EQ(rbuffer_read_string(&rbuffer, &str), RET_OK);
  ASSERT_EQ(string(str), string(cstr));
}
