#include "gtest/gtest.h"
#include "tkc/buffer.h"
#include "tkc/object_default.h"
#include "ubjson/ubjson_writer.h"
#include "ubjson/ubjson_parser.h"

TEST(UBJsonParser, basic) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  tk_object_t* obj = NULL;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "name"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "aaa"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_key(&ub, "age"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 100), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  obj = object_from_ubjson(wb.data, wb.cursor);
  ASSERT_STREQ(tk_object_get_prop_str(obj, "name"), "aaa");
  ASSERT_EQ(tk_object_get_prop_int(obj, "age", 0), 100);

  tk_object_unref(obj);
}

TEST(UBJsonParser, object) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  tk_object_t* obj = object_default_create();
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  tk_object_set_prop_str(obj, "name", "aaa");
  tk_object_set_prop_int(obj, "age", 100);

  ASSERT_EQ(ubjson_writer_write_object(&ub, obj), RET_OK);
  tk_object_unref(obj);

  obj = object_from_ubjson(wb.data, wb.cursor);
  ASSERT_STREQ(tk_object_get_prop_str(obj, "name"), "aaa");
  ASSERT_EQ(tk_object_get_prop_int(obj, "age", 0), 100);

  tk_object_unref(obj);
}

TEST(UBJsonParser, value) {
  uint8_t buff[256];
  wbuffer_t wb;
  value_t v;
  ubjson_writer_t ub;
  tk_object_t* obj = object_default_create();
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  tk_object_set_prop_str(obj, "name", "aaa");
  tk_object_set_prop_int(obj, "age", 100);

  value_set_object(&v, obj);
  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_kv_value(&ub, "person", &v), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);
  tk_object_unref(obj);
  value_reset(&v);

  obj = object_from_ubjson(wb.data, wb.cursor);
  ASSERT_STREQ(tk_object_get_prop_str_by_path(obj, "person.name"), "aaa");
  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "person.age", 0), 100);

  tk_object_unref(obj);
}

TEST(UBJsonParser, array_basic) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  tk_object_t* obj = NULL;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_array_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 100), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 200), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 300), RET_OK);
  ASSERT_EQ(ubjson_writer_write_array_end(&ub), RET_OK);

  obj = object_from_ubjson(wb.data, wb.cursor);
  ASSERT_EQ(tk_object_get_prop_int(obj, "0", 0), 100);
  ASSERT_EQ(tk_object_get_prop_int(obj, "1", 0), 200);
  ASSERT_EQ(tk_object_get_prop_int(obj, "2", 0), 300);
  ASSERT_EQ(tk_object_get_prop_int(obj, "size", 0), 3);

  tk_object_unref(obj);
}

TEST(UBJsonParser, embedded) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  tk_object_t* obj = NULL;
  tk_object_t* addr = NULL;
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
  ASSERT_STREQ(tk_object_get_prop_str(obj, "name"), "aaa");
  ASSERT_EQ(tk_object_get_prop_int(obj, "age", 0), 100);
  addr = tk_object_get_prop_object(obj, "addr");
  ASSERT_EQ(addr != NULL, true);

  ASSERT_STREQ(tk_object_get_prop_str(addr, "city"), "shenzhen");
  ASSERT_STREQ(tk_object_get_prop_str(addr, "post"), "518000");

  tk_object_unref(obj);
}

TEST(UBJsonParser, array_embedded) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  tk_object_t* obj = NULL;
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
  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "values.0", 0), 100);
  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "values.1", 0), 200);
  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "values.2", 0), 300);
  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "values.size", 0), 3);

  tk_object_unref(obj);
}

TEST(UBJsonParser, array_embedded_obj) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  tk_object_t* obj = NULL;
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
  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "person.0.age", 0), 100);
  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "person.1.age", 0), 200);
  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "person.2.age", 0), 300);
  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "person.size", 0), 3);

  ubjson_dump(wb.data, wb.cursor);

  tk_object_unref(obj);
}

TEST(UBJsonParser, ubjson_writer_write_kv_value) {
  uint8_t buff[256];
  wbuffer_t wb;
  value_t v;
  ubjson_writer_t ub;
  tk_object_t* obj = object_default_create();
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  value_set_float32(&v, 1.5);
  ASSERT_EQ(ubjson_writer_write_kv_value(&ub, "f32", &v), RET_OK);

  value_set_double(&v, 2.5);
  ASSERT_EQ(ubjson_writer_write_kv_value(&ub, "f64", &v), RET_OK);
  
  value_set_int8(&v, 3);
  ASSERT_EQ(ubjson_writer_write_kv_value(&ub, "int8", &v), RET_OK);
  
  value_set_int16(&v, 4);
  ASSERT_EQ(ubjson_writer_write_kv_value(&ub, "int16", &v), RET_OK);
  
  value_set_int32(&v, 5);
  ASSERT_EQ(ubjson_writer_write_kv_value(&ub, "int32", &v), RET_OK);
  
  value_set_int64(&v, 6);
  ASSERT_EQ(ubjson_writer_write_kv_value(&ub, "int64", &v), RET_OK);
  
  value_set_uint8(&v, 7);
  ASSERT_EQ(ubjson_writer_write_kv_value(&ub, "uint8", &v), RET_OK);
  
  value_set_uint16(&v, 8);
  ASSERT_EQ(ubjson_writer_write_kv_value(&ub, "uint16", &v), RET_OK);
  
  value_set_uint32(&v, 9);
  ASSERT_EQ(ubjson_writer_write_kv_value(&ub, "uint32", &v), RET_OK);
  
  value_set_uint64(&v, 10);
  ASSERT_EQ(ubjson_writer_write_kv_value(&ub, "uint64", &v), RET_OK);

  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  obj = object_from_ubjson(wb.data, wb.cursor);
  ASSERT_EQ(tk_object_get_prop_int8(obj, "int8", 0), 3);
  ASSERT_EQ(tk_object_get_prop_int16(obj, "int16", 0), 4);
  ASSERT_EQ(tk_object_get_prop_int32(obj, "int32", 0), 5);
  ASSERT_EQ(tk_object_get_prop_int64(obj, "int64", 0), 6);
  ASSERT_EQ(tk_object_get_prop_uint8(obj, "uint8", 0), 7);
  ASSERT_EQ(tk_object_get_prop_uint16(obj, "uint16", 0), 8);
  ASSERT_EQ(tk_object_get_prop_uint32(obj, "uint32", 0), 9);
  ASSERT_EQ(tk_object_get_prop_uint64(obj, "uint64", 0), 10);
  ASSERT_EQ(tk_object_get_prop_float(obj, "f32", 0), 1.5);
  ASSERT_EQ(tk_object_get_prop_double(obj, "f64", 0), 2.5);

  tk_object_unref(obj);
}
