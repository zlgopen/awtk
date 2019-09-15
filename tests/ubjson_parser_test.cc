#include "gtest/gtest.h"
#include "tkc/buffer.h"
#include "ubjson/ubjson_writer.h"
#include "ubjson/ubjson_parser.h"

TEST(UBJsonWriter, basic) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  object_t* obj = NULL;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "name"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "aaa"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "age"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 100), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  obj = ubjson_parse(wb.data, wb.cursor);
  ASSERT_STREQ(object_get_prop_str(obj, "name"), "aaa");
  ASSERT_EQ(object_get_prop_int(obj, "age", 0), 100);

  object_unref(obj);
}

TEST(UBJsonWriter, embedded) {
  uint8_t buff[256];
  wbuffer_t wb;
  ubjson_writer_t ub;
  object_t* obj = NULL;
  object_t* addr = NULL;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "name"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "aaa"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "age"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_int32(&ub, 100), RET_OK);

  ASSERT_EQ(ubjson_writer_write_str(&ub, "addr"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_begin(&ub), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "city"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "shenzhen"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "post"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_str(&ub, "518000"), RET_OK);
  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  ASSERT_EQ(ubjson_writer_write_object_end(&ub), RET_OK);

  obj = ubjson_parse(wb.data, wb.cursor);
  ASSERT_STREQ(object_get_prop_str(obj, "name"), "aaa");
  ASSERT_EQ(object_get_prop_int(obj, "age", 0), 100);
  addr = object_get_prop_object(obj, "addr");
  ASSERT_EQ(addr != NULL, true);

  ASSERT_STREQ(object_get_prop_str(addr, "city"), "shenzhen");
  ASSERT_STREQ(object_get_prop_str(addr, "post"), "518000");

  object_unref(obj);
}
