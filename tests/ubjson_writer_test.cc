#include "gtest/gtest.h"
#include "tkc/buffer.h"
#include "ubjson/ubjson_writer.h"

TEST(UBJsonWriter, null) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_null(&ub), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_NULL);
  ASSERT_EQ(wb.cursor, 1);
}

TEST(UBJsonWriter, noop) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_noop(&ub), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_NOOP);
  ASSERT_EQ(wb.cursor, 1);
}

TEST(UBJsonWriter, true) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_true(&ub), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_TRUE);
  ASSERT_EQ(wb.cursor, 1);
}

TEST(UBJsonWriter, false) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_false(&ub), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_FALSE);
  ASSERT_EQ(wb.cursor, 1);
}

TEST(UBJsonWriter, array) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_array_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_array_end(&ub), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_ARRAY_BEGIN);
  ASSERT_EQ(buff[1], UBJSON_MARKER_ARRAY_END);
  ASSERT_EQ(wb.cursor, 2);
}

TEST(UBJsonWriter, object) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_OBJECT_BEGIN);
  ASSERT_EQ(buff[1], UBJSON_MARKER_OBJECT_END);
  ASSERT_EQ(wb.cursor, 2);
}

TEST(UBJsonWriter, int8) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_int8(&ub, 0x12), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_INT8);
  ASSERT_EQ(buff[1], 0x12);
  ASSERT_EQ(wb.cursor, 2);
}

TEST(UBJsonWriter, int16) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_int16(&ub, 0x1122), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_INT16);
  ASSERT_EQ(buff[1], 0x11);
  ASSERT_EQ(buff[2], 0x22);
  ASSERT_EQ(wb.cursor, 3);
}

TEST(UBJsonWriter, int32) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_int32(&ub, 0x11223344), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_INT32);
  ASSERT_EQ(buff[1], 0x11);
  ASSERT_EQ(buff[2], 0x22);
  ASSERT_EQ(buff[3], 0x33);
  ASSERT_EQ(buff[4], 0x44);
  ASSERT_EQ(wb.cursor, 5);
}

TEST(UBJsonWriter, int64) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_int64(&ub, 0x1122334455667788), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_INT64);
  ASSERT_EQ(buff[1], 0x11);
  ASSERT_EQ(buff[2], 0x22);
  ASSERT_EQ(buff[3], 0x33);
  ASSERT_EQ(buff[4], 0x44);
  ASSERT_EQ(buff[5], 0x55);
  ASSERT_EQ(buff[6], 0x66);
  ASSERT_EQ(buff[7], 0x77);
  ASSERT_EQ(buff[8], 0x88);
  ASSERT_EQ(wb.cursor, 9);
}

TEST(UBJsonWriter, float) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_float32(&ub, 1.2), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_FLOAT32);
  ASSERT_EQ(wb.cursor, 5);
}

TEST(UBJsonWriter, double) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_float64(&ub, 1.2), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_FLOAT64);
  ASSERT_EQ(wb.cursor, 9);
}

TEST(UBJsonWriter, string) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_str(&ub, "a"), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_STRING);
  ASSERT_EQ(buff[1], UBJSON_MARKER_INT8);
  ASSERT_EQ(buff[2], 0x1);
  ASSERT_EQ(buff[3], 'a');
  ASSERT_EQ(wb.cursor, 4);
}

TEST(UBJsonWriter, binary) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_str_len(&ub, "aasdf", 1), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_STRING);
  ASSERT_EQ(buff[1], UBJSON_MARKER_INT8);
  ASSERT_EQ(buff[2], 0x1);
  ASSERT_EQ(buff[3], 'a');
  ASSERT_EQ(wb.cursor, 4);
}

TEST(UBJsonWriter, kv_string) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_kv_str(&ub, "a", "b"), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_INT8);
  ASSERT_EQ(buff[1], 0x1);
  ASSERT_EQ(buff[2], 'a');

  ASSERT_EQ(buff[3], UBJSON_MARKER_STRING);
  ASSERT_EQ(buff[4], UBJSON_MARKER_INT8);
  ASSERT_EQ(buff[5], 0x1);
  ASSERT_EQ(buff[6], 'b');
  ASSERT_EQ(wb.cursor, 7);
}

TEST(UBJsonWriter, kv_string_len) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_kv_str_len(&ub, "a", "b1231", 1), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_INT8);
  ASSERT_EQ(buff[1], 0x1);
  ASSERT_EQ(buff[2], 'a');

  ASSERT_EQ(buff[3], UBJSON_MARKER_STRING);
  ASSERT_EQ(buff[4], UBJSON_MARKER_INT8);
  ASSERT_EQ(buff[5], 0x1);
  ASSERT_EQ(buff[6], 'b');
  ASSERT_EQ(wb.cursor, 7);
}
