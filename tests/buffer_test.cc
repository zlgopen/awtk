#include "tkc/buffer.h"
#include "gtest/gtest.h"
#include <string>

using std::string;

TEST(Buffer, demo1) {
  wbuffer_t wbuffer;
  wbuffer_init_extendable(&wbuffer);

  wbuffer_write_string(&wbuffer, "hello awtk");

  wbuffer_deinit(&wbuffer);
}

TEST(Buffer, demo2) {
  uint8_t buff[128];
  wbuffer_t wbuffer;
  rbuffer_t rbuffer;
  const char* str = NULL;
  wbuffer_init(&wbuffer, buff, sizeof(buff));

  wbuffer_write_string(&wbuffer, "hello awtk");

  rbuffer_init(&rbuffer, wbuffer.data, wbuffer.cursor);
  rbuffer_read_string(&rbuffer, &str);

  ASSERT_EQ(string(str), "hello awtk");
}

static void testWriteRead(wbuffer_t* wb) {
  uint8_t v8;
  uint16_t v16;
  uint32_t v32;
  rbuffer_t rbuffer;
  rbuffer_t* rb = &rbuffer;

  for (uint32_t i = 0; i < 10; i++) {
    ASSERT_EQ(wbuffer_write_uint8(wb, i), RET_OK);
    ASSERT_EQ(wb->cursor, 11 * i + 1);

    rbuffer_init(rb, wb->data, wb->cursor);
    rb->cursor = wb->cursor - 1;
    ASSERT_EQ(rbuffer_read_uint8(rb, &v8), RET_OK);
    ASSERT_EQ((uint32_t)v8, i);

    ASSERT_EQ(wbuffer_write_uint16(wb, i), RET_OK);
    ASSERT_EQ(wb->cursor, 11 * i + 3);

    rbuffer_init(rb, wb->data, wb->cursor);
    rb->cursor = wb->cursor - 2;
    ASSERT_EQ(rbuffer_read_uint16(rb, &v16), RET_OK);
    ASSERT_EQ((uint32_t)v16, i);

    ASSERT_EQ(wbuffer_write_uint32(wb, i), RET_OK);
    ASSERT_EQ(wb->cursor, 11 * i + 7);

    rbuffer_init(rb, wb->data, wb->cursor);
    rb->cursor = wb->cursor - 4;
    ASSERT_EQ(rbuffer_read_uint32(rb, &v32), RET_OK);
    ASSERT_EQ((uint32_t)v32, i);

    ASSERT_EQ(wbuffer_write_binary(wb, &i, sizeof(i)), RET_OK);
    ASSERT_EQ(wb->cursor, 11 * i + 11);

    rbuffer_init(rb, wb->data, wb->cursor);
    rb->cursor = wb->cursor - sizeof(i);
    ASSERT_EQ(rbuffer_read_binary(rb, &v32, sizeof(v32)), RET_OK);
    ASSERT_EQ((uint32_t)v32, i);
  }
}

TEST(Buffer, basic) {
  uint8_t data[128];
  wbuffer_t wbuffer;
  ASSERT_EQ(wbuffer_init(&wbuffer, data, sizeof(data)), &wbuffer);

  testWriteRead(&wbuffer);
}

TEST(Buffer, extendable) {
  wbuffer_t wbuffer;
  ASSERT_EQ(wbuffer_init_extendable(&wbuffer), &wbuffer);

  testWriteRead(&wbuffer);

  wbuffer_deinit(&wbuffer);
}

TEST(Buffer, WbufferSkipExtendable) {
  wbuffer_t wbuffer;
  ASSERT_EQ(wbuffer_init_extendable(&wbuffer), &wbuffer);

  ASSERT_EQ(wbuffer_skip(&wbuffer, 10), RET_OK);
  ASSERT_EQ(wbuffer.cursor, 10);

  ASSERT_EQ(wbuffer_skip(&wbuffer, -10), RET_OK);
  ASSERT_EQ(wbuffer.cursor, 0);

  ASSERT_EQ(wbuffer_skip(&wbuffer, -10), RET_BAD_PARAMS);
  ASSERT_EQ(wbuffer.cursor, 0);

  wbuffer_deinit(&wbuffer);
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
