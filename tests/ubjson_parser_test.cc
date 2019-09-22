#include "gtest/gtest.h"
#include "tkc/buffer.h"
#include "tkc/object_default.h"
#include "ubjson/ubjson_writer.h"
#include "ubjson/ubjson_parser.h"

TEST(UBJsonParser, basic) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  object_t* obj = NULL;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "name"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "aaa"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "age"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 100), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  obj = object_from_ubjson(wb.data, wb.cursor);
  ASSERT_STREQ(object_get_prop_str(obj, "name"), "aaa");
  ASSERT_EQ(object_get_prop_int(obj, "age", 0), 100);

  object_unref(obj);
}

TEST(UBJsonParser, object) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  object_t* obj = object_default_create();
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  object_set_prop_str(obj, "name", "aaa");
  object_set_prop_int(obj, "age", 100);

  ASSERT_EQ(ubjson_writer_write_object(&ub, obj), RET_OK);
  object_unref(obj);

  obj = object_from_ubjson(wb.data, wb.cursor);
  ASSERT_STREQ(object_get_prop_str(obj, "name"), "aaa");
  ASSERT_EQ(object_get_prop_int(obj, "age", 0), 100);

  object_unref(obj);
}

TEST(UBJsonParser, value) {
  uint8_t buff[256];
  wbuffer_t wb;
  value_t v;
  ubjson_writer_t ub;
  object_t* obj = object_default_create();
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  object_set_prop_str(obj, "name", "aaa");
  object_set_prop_int(obj, "age", 100);

  value_set_object(&v, obj);
  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_kv_value(&ub, "person", &v), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);
  object_unref(obj);
  value_reset(&v);

  obj = object_from_ubjson(wb.data, wb.cursor);
  ASSERT_STREQ(object_get_prop_str_by_path(obj, "person.name"), "aaa");
  ASSERT_EQ(object_get_prop_int_by_path(obj, "person.age", 0), 100);

  object_unref(obj);
}

TEST(UBJsonParser, array_basic) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  object_t* obj = NULL;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_array_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 100), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 200), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 300), RET_OK);
  ASSERT_EQ(ubjson_writer_write_array_end(&ub), RET_OK);

  obj = object_from_ubjson(wb.data, wb.cursor);
  ASSERT_EQ(object_get_prop_int(obj, "0", 0), 100);
  ASSERT_EQ(object_get_prop_int(obj, "1", 0), 200);
  ASSERT_EQ(object_get_prop_int(obj, "2", 0), 300);
  ASSERT_EQ(object_get_prop_int(obj, "size", 0), 3);

  object_unref(obj);
}

TEST(UBJsonParser, embedded) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  object_t* obj = NULL;
  object_t* addr = NULL;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "name"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "aaa"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "age"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 100), RET_OK);

  ASSERT_EQ(ubjson_writer_write_key(&ub, "addr"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "city"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "shenzhen"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "post"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "518000"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  obj = object_from_ubjson(wb.data, wb.cursor);
  ASSERT_STREQ(object_get_prop_str(obj, "name"), "aaa");
  ASSERT_EQ(object_get_prop_int(obj, "age", 0), 100);
  addr = object_get_prop_object(obj, "addr");
  ASSERT_EQ(addr != NULL, true);

  ASSERT_STREQ(object_get_prop_str(addr, "city"), "shenzhen");
  ASSERT_STREQ(object_get_prop_str(addr, "post"), "518000");

  object_unref(obj);
}

TEST(UBJsonParser, array_embedded) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  object_t* obj = NULL;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "values"), RET_OK);

  ASSERT_EQ(ubjson_writer_write_array_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 100), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 200), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 300), RET_OK);
  ASSERT_EQ(ubjson_writer_write_array_end(&ub), RET_OK);

  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  obj = object_from_ubjson(wb.data, wb.cursor);
  ASSERT_EQ(object_get_prop_int_by_path(obj, "values.0", 0), 100);
  ASSERT_EQ(object_get_prop_int_by_path(obj, "values.1", 0), 200);
  ASSERT_EQ(object_get_prop_int_by_path(obj, "values.2", 0), 300);
  ASSERT_EQ(object_get_prop_int_by_path(obj, "values.size", 0), 3);

  object_unref(obj);
}

TEST(UBJsonParser, array_embedded_obj) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  object_t* obj = NULL;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "version"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 1234), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "person"), RET_OK);

  ASSERT_EQ(ubjson_writer_write_array_begin(&ub), RET_OK);

  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "name"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "aaa"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "age"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 100), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "name"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "aaa"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "age"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 200), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "name"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "aaa"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "age"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 300), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  ASSERT_EQ(ubjson_writer_write_array_end(&ub), RET_OK);

  ASSERT_EQ(ubjson_writer_write_key(&ub, "date"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 1234), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  obj = object_from_ubjson(wb.data, wb.cursor);
  ASSERT_EQ(object_get_prop_int_by_path(obj, "person.0.age", 0), 100);
  ASSERT_EQ(object_get_prop_int_by_path(obj, "person.1.age", 0), 200);
  ASSERT_EQ(object_get_prop_int_by_path(obj, "person.2.age", 0), 300);
  ASSERT_EQ(object_get_prop_int_by_path(obj, "person.size", 0), 3);

  ubjson_dump(wb.data, wb.cursor);

  object_unref(obj);
}
