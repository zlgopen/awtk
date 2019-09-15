#include "gtest/gtest.h"
#include "tkc/buffer.h"
#include "ubjson/ubjson_writer.h"
#include "ubjson/ubjson_reader.h"

#define PREPARE_TEST()                                                         \
  uint8_t buff[256];                                                           \
  value_t v;                                                                   \
  wbuffer_t wb;                                                                \
  rbuffer_t rb;                                                                \
  ubjson_writer_t ub;                                                          \
  ubjson_reader_t ur;                                                          \
  wbuffer_init(&wb, buff, sizeof(buff));                                       \
  rbuffer_init(&rb, buff, sizeof(buff));                                       \
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb); \
  ubjson_reader_init(&ur, (ubjson_read_callback_t)rbuffer_read_binary, &rb);

TEST(UBJsonReader, null) {
  PREPARE_TEST();

  ASSERT_EQ(ubjson_writer_write_null(&ub), RET_OK);

  rb.capacity = wb.cursor;
  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_POINTER, true);
  ASSERT_EQ(value_pointer(&v) == NULL, true);
  ubjson_reader_reset(&ur);
}

TEST(UBJsonReader, noop) {
  PREPARE_TEST();

  ASSERT_EQ(ubjson_writer_write_noop(&ub), RET_OK);

  rb.capacity = wb.cursor;
  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_POINTER, true);
  ASSERT_EQ(value_pointer(&v) == NULL, true);
  ubjson_reader_reset(&ur);
}

TEST(UBJsonReader, true) {
  PREPARE_TEST();

  ASSERT_EQ(ubjson_writer_write_true(&ub), RET_OK);

  rb.capacity = wb.cursor;
  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_BOOL, true);
  ASSERT_EQ(value_bool(&v), TRUE);
  ubjson_reader_reset(&ur);
}

TEST(UBJsonReader, false) {
  PREPARE_TEST();

  ASSERT_EQ(ubjson_writer_write_false(&ub), RET_OK);

  rb.capacity = wb.cursor;
  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_BOOL, true);
  ASSERT_EQ(value_bool(&v), FALSE);
  ubjson_reader_reset(&ur);
}

TEST(UBJsonReader, array) {
  PREPARE_TEST();

  ASSERT_EQ(ubjson_writer_write_array_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_array_end(&ub), RET_OK);

  rb.capacity = wb.cursor;
  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_TOKEN, true);
  ASSERT_EQ(value_token(&v), UBJSON_MARKER_ARRAY_BEGIN);

  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_TOKEN, true);
  ASSERT_EQ(value_token(&v), UBJSON_MARKER_ARRAY_END);

  ubjson_reader_reset(&ur);
}

TEST(UBJsonReader, object) {
  PREPARE_TEST();

  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  rb.capacity = wb.cursor;
  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_TOKEN, true);
  ASSERT_EQ(value_token(&v), UBJSON_MARKER_OBJECT_BEGIN);

  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_TOKEN, true);
  ASSERT_EQ(value_token(&v), UBJSON_MARKER_OBJECT_END);

  ubjson_reader_reset(&ur);
}

TEST(UBJsonReader, int8) {
  PREPARE_TEST();

  ASSERT_EQ(ubjson_writer_write_int8(&ub, 0x12), RET_OK);

  rb.capacity = wb.cursor;
  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_INT8, true);
  ASSERT_EQ(value_int(&v), 0x12);

  ubjson_reader_reset(&ur);
}

TEST(UBJsonReader, int16) {
  PREPARE_TEST();

  ASSERT_EQ(ubjson_writer_write_int16(&ub, 0x1122), RET_OK);

  rb.capacity = wb.cursor;
  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_INT16, true);
  ASSERT_EQ(value_int(&v), 0x1122);

  ubjson_reader_reset(&ur);
}

TEST(UBJsonReader, int32) {
  PREPARE_TEST();

  ASSERT_EQ(ubjson_writer_write_int32(&ub, 0x11223344), RET_OK);

  rb.capacity = wb.cursor;
  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_INT32, true);
  ASSERT_EQ(value_int(&v), 0x11223344);

  ubjson_reader_reset(&ur);
}

TEST(UBJsonReader, int64) {
  PREPARE_TEST();

  ASSERT_EQ(ubjson_writer_write_int64(&ub, 0x1122334455667788), RET_OK);

  rb.capacity = wb.cursor;
  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_INT64, true);
  ASSERT_EQ(value_int64(&v), 0x1122334455667788);

  ubjson_reader_reset(&ur);
}

TEST(UBJsonReader, float) {
  PREPARE_TEST();

  ASSERT_EQ(ubjson_writer_write_float32(&ub, 12), RET_OK);

  rb.capacity = wb.cursor;
  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_FLOAT32, true);
  ASSERT_EQ(value_float(&v), 12);

  ubjson_reader_reset(&ur);
}

TEST(UBJsonReader, double) {
  PREPARE_TEST();

  ASSERT_EQ(ubjson_writer_write_float64(&ub, 12), RET_OK);

  rb.capacity = wb.cursor;
  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_DOUBLE, true);
  ASSERT_EQ(value_double(&v), 12);

  ubjson_reader_reset(&ur);
}

TEST(UBJsonReader, string) {
  PREPARE_TEST();

  ASSERT_EQ(ubjson_writer_write_str(&ub, "a"), RET_OK);

  rb.capacity = wb.cursor;
  ASSERT_EQ(ubjson_reader_read(&ur, &v), RET_OK);
  ASSERT_EQ(v.type == VALUE_TYPE_STRING, true);
  ASSERT_STREQ(value_str(&v), "a");

  ubjson_reader_reset(&ur);
}
