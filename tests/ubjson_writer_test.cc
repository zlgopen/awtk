#include "gtest/gtest.h"
#include "tkc/buffer.h"
#include "tkc/endian.h"
#include "ubjson/ubjson_writer.h"

TEST(UBJsonWriter, null) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_null(&ub), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_NULL);
  ASSERT_EQ(wb.cursor, 1u);
}

TEST(UBJsonWriter, noop) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_noop(&ub), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_NOOP);
  ASSERT_EQ(wb.cursor, 1u);
}

TEST(UBJsonWriter, true) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_true(&ub), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_TRUE);
  ASSERT_EQ(wb.cursor, 1u);
}

TEST(UBJsonWriter, false) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_false(&ub), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_FALSE);
  ASSERT_EQ(wb.cursor, 1u);
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
  ASSERT_EQ(wb.cursor, 2u);
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
  ASSERT_EQ(wb.cursor, 2u);
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
  ASSERT_EQ(wb.cursor, 2u);
}

TEST(UBJsonWriter, char) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_char(&ub, 'a'), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_CHAR);
  ASSERT_EQ(buff[1], 'a');
  ASSERT_EQ(wb.cursor, 2u);
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
  ASSERT_EQ(wb.cursor, 3u);
}

TEST(UBJsonWriter, uint16) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_uint16(&ub, 0x1122), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_UINT16);
  ASSERT_EQ(buff[1], 0x11);
  ASSERT_EQ(buff[2], 0x22);
  ASSERT_EQ(wb.cursor, 3u);
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
  ASSERT_EQ(wb.cursor, 5u);
}

TEST(UBJsonWriter, uint32) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_uint32(&ub, 0x11223344), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_UINT32);
  ASSERT_EQ(buff[1], 0x11);
  ASSERT_EQ(buff[2], 0x22);
  ASSERT_EQ(buff[3], 0x33);
  ASSERT_EQ(buff[4], 0x44);
  ASSERT_EQ(wb.cursor, 5u);
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
  ASSERT_EQ(wb.cursor, 9u);
}

TEST(UBJsonWriter, uint64) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_uint64(&ub, 0x1122334455667788), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_UINT64);
  ASSERT_EQ(buff[1], 0x11);
  ASSERT_EQ(buff[2], 0x22);
  ASSERT_EQ(buff[3], 0x33);
  ASSERT_EQ(buff[4], 0x44);
  ASSERT_EQ(buff[5], 0x55);
  ASSERT_EQ(buff[6], 0x66);
  ASSERT_EQ(buff[7], 0x77);
  ASSERT_EQ(buff[8], 0x88);
  ASSERT_EQ(wb.cursor, 9u);
}

TEST(UBJsonWriter, float) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_float32(&ub, 1.2), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_FLOAT32);
  ASSERT_EQ(wb.cursor, 5u);
}

TEST(UBJsonWriter, double) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_float64(&ub, 1.2), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_FLOAT64);
  ASSERT_EQ(wb.cursor, 9u);
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
  ASSERT_EQ(wb.cursor, 4u);
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
  ASSERT_EQ(wb.cursor, 4u);
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
  ASSERT_EQ(wb.cursor, 7u);
}

TEST(UBJsonWriter, kv_wstring) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_kv_wstr(&ub, "a", L"b"), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_INT8);
  ASSERT_EQ(buff[1], 0x1);
  ASSERT_EQ(buff[2], 'a');

  ASSERT_EQ(buff[3], UBJSON_MARKER_WSTRING);
  ASSERT_EQ(buff[4], UBJSON_MARKER_INT8);
  ASSERT_EQ(buff[5], 0x1);
  ASSERT_EQ(buff[6], 'b');
  ASSERT_EQ(wb.cursor, 7u);
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
  ASSERT_EQ(wb.cursor, 7u);
}

TEST(UBJsonWriter, kv_wstring_len) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_kv_wstr_len(&ub, "a", L"b1231", 1), RET_OK);

  ASSERT_EQ(buff[0], UBJSON_MARKER_INT8);
  ASSERT_EQ(buff[1], 0x1);
  ASSERT_EQ(buff[2], 'a');

  ASSERT_EQ(buff[3], UBJSON_MARKER_WSTRING);
  ASSERT_EQ(buff[4], UBJSON_MARKER_INT8);
  ASSERT_EQ(buff[5], 0x1);
  ASSERT_EQ(buff[6], 'b');
  ASSERT_EQ(wb.cursor, 7u);
}

TEST(UBJsonWriter, optimized_array_uint8) {
  uint8_t buff[256];
  uint8_t data[120];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  for (int i = 0; i < ARRAY_SIZE(data); i++) {
    data[i] = i;
  }

  ASSERT_EQ(ubjson_writer_write_array_uint8(&ub, data, ARRAY_SIZE(data)), RET_OK);

  ASSERT_EQ(wb.cursor, 6 + sizeof(data));
  ASSERT_EQ(buff[0], UBJSON_MARKER_ARRAY_BEGIN);
  ASSERT_EQ(buff[1], UBJSON_MARKER_CONTAINER_TYPE);
  ASSERT_EQ(buff[2], UBJSON_MARKER_UINT8);
  ASSERT_EQ(buff[3], UBJSON_MARKER_CONTAINER_COUNT);
  ASSERT_EQ(buff[4], UBJSON_MARKER_INT8);
  ASSERT_EQ(buff[5], ARRAY_SIZE(data));
  ASSERT_EQ(memcmp(buff + 6, data, sizeof(data)), 0);
}

TEST(UBJsonWriter, optimized_array_int32) {
  uint8_t buff[256 + 120 * sizeof(int32_t)];
  int32_t data[120];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  for (int i = 0; i < ARRAY_SIZE(data); i++) {
    data[i] = i;
  }

  ASSERT_EQ(ubjson_writer_write_array_int32(&ub, data, ARRAY_SIZE(data)), RET_OK);

  ASSERT_EQ(wb.cursor, 6 + sizeof(data));
  ASSERT_EQ(buff[0], UBJSON_MARKER_ARRAY_BEGIN);
  ASSERT_EQ(buff[1], UBJSON_MARKER_CONTAINER_TYPE);
  ASSERT_EQ(buff[2], UBJSON_MARKER_INT32);
  ASSERT_EQ(buff[3], UBJSON_MARKER_CONTAINER_COUNT);
  ASSERT_EQ(buff[4], UBJSON_MARKER_INT8);
  ASSERT_EQ(buff[5], ARRAY_SIZE(data));

  int32_t* p = (int32_t*)(buff + 6);
  for (int i = 0; i < ARRAY_SIZE(data); i++) {
    int32_t v = int32_from_big_endian(p[i]);
    ASSERT_EQ(v, data[i]);
  }
}

TEST(UBJsonWriter, optimized_array_float64) {
  uint8_t buff[256 + 120 * sizeof(double)];
  double data[120];
  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  for (int i = 0; i < ARRAY_SIZE(data); i++) {
    data[i] = i * 1.11;
  }

  ASSERT_EQ(ubjson_writer_write_array_float64(&ub, data, ARRAY_SIZE(data)), RET_OK);

  ASSERT_EQ(wb.cursor, 6 + sizeof(data));
  ASSERT_EQ(buff[0], UBJSON_MARKER_ARRAY_BEGIN);
  ASSERT_EQ(buff[1], UBJSON_MARKER_CONTAINER_TYPE);
  ASSERT_EQ(buff[2], UBJSON_MARKER_FLOAT64);
  ASSERT_EQ(buff[3], UBJSON_MARKER_CONTAINER_COUNT);
  ASSERT_EQ(buff[4], UBJSON_MARKER_INT8);
  ASSERT_EQ(buff[5], ARRAY_SIZE(data));

  double* p = (double*)(buff + 6);
  for (int i = 0; i < ARRAY_SIZE(data); i++) {
    double v = double_from_big_endian(p[i]);
    ASSERT_EQ(v, data[i]);
  }
}
