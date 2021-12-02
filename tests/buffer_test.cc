#include "tkc/buffer.h"
#include "tkc/object_rbuffer.h"
#include "tkc/object_wbuffer.h"
#include "gtest/gtest.h"
#include <string>

using std::string;

TEST(Buffer, demo1) {
  wbuffer_t wbuffer;
  wbuffer_init_extendable(&wbuffer);

  wbuffer_write_string(&wbuffer, "hello awtk");

  wbuffer_deinit(&wbuffer);
}

TEST(Buffer, rewind) {
  wbuffer_t wbuffer;
  const char* str = "hello awtk";
  wbuffer_init_extendable(&wbuffer);
  wbuffer_write_string(&wbuffer, str);
  ASSERT_EQ(wbuffer.cursor, strlen(str) + 1u);
  ASSERT_EQ(wbuffer_rewind(&wbuffer), RET_OK);
  ASSERT_EQ(wbuffer.cursor, 0u);

  wbuffer_deinit(&wbuffer);
}

TEST(Buffer, demo2) {
  uint8_t buff[128];
  wbuffer_t wbuffer;
  rbuffer_t rbuffer;
  const char* rstr = NULL;
  const char* wstr = "hello awtk";
  wbuffer_init(&wbuffer, buff, sizeof(buff));

  wbuffer_write_string(&wbuffer, wstr);

  rbuffer_init(&rbuffer, wbuffer.data, wbuffer.cursor);
  rbuffer_read_string(&rbuffer, &rstr);

  ASSERT_EQ(rbuffer.cursor, strlen(wstr) + 1u);
  ASSERT_EQ(rbuffer_rewind(&rbuffer), RET_OK);
  ASSERT_EQ(rbuffer.cursor, 0u);

  rbuffer_read_string(&rbuffer, &rstr);

  ASSERT_STREQ(rstr, wstr);
}

static void testWriteRead(wbuffer_t* wb) {
  uint8_t v8 = 0;
  uint16_t v16 = 0;
  uint32_t v32 = 0;
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

TEST(Buffer, basic1) {
  float f32 = 0;
  double f64 = 0;
  uint64_t v64 = 0;
  uint8_t data[128];
  wbuffer_t wbuffer;
  rbuffer_t rbuffer;
  rbuffer_t* rb = &rbuffer;

  ASSERT_EQ(wbuffer_init(&wbuffer, data, sizeof(data)), &wbuffer);
  ASSERT_EQ(wbuffer_write_uint64(&wbuffer, 0x112233445566), RET_OK);
  ASSERT_EQ(wbuffer.cursor, sizeof(uint64_t));
  ASSERT_EQ(rbuffer_init(rb, wbuffer.data, wbuffer.cursor) != NULL, true);
  ASSERT_EQ(rbuffer_read_uint64(rb, &v64), RET_OK);
  ASSERT_EQ(v64, (uint64_t)0x112233445566);
  ASSERT_EQ(rbuffer_has_more(rb), FALSE);

  ASSERT_EQ(wbuffer_init(&wbuffer, data, sizeof(data)), &wbuffer);
  ASSERT_EQ(wbuffer_write_float(&wbuffer, 100), RET_OK);
  ASSERT_EQ(wbuffer.cursor, sizeof(float));
  ASSERT_EQ(rbuffer_init(rb, wbuffer.data, wbuffer.cursor) != NULL, true);
  ASSERT_EQ(rbuffer_read_float(rb, &f32), RET_OK);
  ASSERT_EQ(f32, 100.0F);
  ASSERT_EQ(rbuffer_has_more(rb), FALSE);

  ASSERT_EQ(wbuffer_init(&wbuffer, data, sizeof(data)), &wbuffer);
  ASSERT_EQ(wbuffer_write_double(&wbuffer, 100), RET_OK);
  ASSERT_EQ(wbuffer.cursor, sizeof(double));
  ASSERT_EQ(rbuffer_init(rb, wbuffer.data, wbuffer.cursor) != NULL, true);
  ASSERT_EQ(rbuffer_read_double(rb, &f64), RET_OK);
  ASSERT_EQ(f64, 100.0F);
  ASSERT_EQ(rbuffer_has_more(rb), FALSE);
}

TEST(Buffer, basic2) {
  uint8_t data[128];
  wbuffer_t wbuffer;
  ASSERT_EQ(wbuffer_init(&wbuffer, data, sizeof(data)), &wbuffer);
}
uint64_t v64 = 0;
float f32 = 0;
double f64 = 0;

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
  ASSERT_EQ(wbuffer.cursor, 10u);

  ASSERT_EQ(wbuffer_skip(&wbuffer, -10), RET_OK);
  ASSERT_EQ(wbuffer.cursor, 0u);

  ASSERT_EQ(wbuffer_skip(&wbuffer, -10), RET_BAD_PARAMS);
  ASSERT_EQ(wbuffer.cursor, 0u);

  wbuffer_deinit(&wbuffer);
}

TEST(Buffer, WbufferSkip) {
  uint8_t data[128];
  wbuffer_t wbuffer;
  ASSERT_EQ(wbuffer_init(&wbuffer, data, sizeof(data)), &wbuffer);

  ASSERT_EQ(wbuffer_skip(&wbuffer, 10), RET_OK);
  ASSERT_EQ(wbuffer.cursor, 10u);

  ASSERT_EQ(wbuffer_skip(&wbuffer, -10), RET_OK);
  ASSERT_EQ(wbuffer.cursor, 0u);

  ASSERT_EQ(wbuffer_skip(&wbuffer, -10), RET_BAD_PARAMS);
  ASSERT_EQ(wbuffer.cursor, 0u);
}

TEST(Buffer, RbufferSkip) {
  uint8_t data[128];
  rbuffer_t rbuffer;
  ASSERT_EQ(rbuffer_init(&rbuffer, data, sizeof(data)), &rbuffer);

  ASSERT_EQ(rbuffer_skip(&rbuffer, 10), RET_OK);
  ASSERT_EQ(rbuffer.cursor, 10u);

  ASSERT_EQ(rbuffer_skip(&rbuffer, -10), RET_OK);
  ASSERT_EQ(rbuffer.cursor, 0u);

  ASSERT_EQ(rbuffer_skip(&rbuffer, -10), RET_BAD_PARAMS);
  ASSERT_EQ(rbuffer.cursor, 0u);
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

TEST(Buffer, obj) {
  const char* str = NULL;
  tk_object_t* wobj = object_wbuffer_create_extendable();
  wbuffer_t* wbuffer = OBJECT_WBUFFER(wobj)->wbuffer;

  wbuffer_write_string(wbuffer, "hello");
  wbuffer_write_string(wbuffer, " world");
  tk_object_t* robj = object_rbuffer_create(wbuffer->data, wbuffer->cursor);
  rbuffer_t* rbuffer = OBJECT_RBUFFER(robj)->rbuffer;

  ASSERT_EQ(tk_object_get_prop_int(wobj, "cursor", 0), 13);
  ASSERT_EQ(tk_object_get_prop_int(wobj, "capacity", 0), 14);
  ASSERT_EQ(tk_object_get_prop_pointer(wobj, "data"), (void*)(wbuffer->data));

  rbuffer_read_string(rbuffer, &str);
  ASSERT_STREQ(str, "hello");

  ASSERT_EQ(tk_object_get_prop_int(robj, "cursor", 0), 6);
  ASSERT_EQ(tk_object_get_prop_int(robj, "capacity", 0), 13);
  ASSERT_EQ(tk_object_get_prop_pointer(robj, "data"), (void*)(rbuffer->data));

  TK_OBJECT_UNREF(wobj);
  TK_OBJECT_UNREF(robj);
}
