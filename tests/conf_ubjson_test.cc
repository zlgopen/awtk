#include "gtest/gtest.h"
#include "tkc/buffer.h"
#include "conf_io/conf_ini.h"
#include "conf_io/conf_ubjson.h"
#include "ubjson/ubjson_parser.h"

TEST(ConfUBJson, basic1) {
  conf_node_t* node = NULL;
  wbuffer_t wb;
  uint8_t buff[256];
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  conf_doc_t* idoc = conf_doc_load_ini("[hello]");
  ASSERT_EQ(conf_doc_save_ubjson(idoc, &ub), RET_OK);
  conf_doc_t* doc = conf_doc_load_ubjson(wb.data, wb.cursor);

  node = conf_node_find_child(doc->root, "hello");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "hello");
  conf_doc_destroy(idoc);
  conf_doc_destroy(doc);
}

TEST(ConfUBJson, basic2) {
  value_t v;
  wbuffer_t wb;
  uint8_t buff[256];
  ubjson_writer_t ub;
  conf_node_t* node = NULL;
  conf_node_t* group = NULL;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  conf_doc_t* idoc = conf_doc_load_ini("[hello]\nkey1=value1\nkey2=value2\n");
  ASSERT_EQ(conf_doc_save_ubjson(idoc, &ub), RET_OK);
  conf_doc_t* doc = conf_doc_load_ubjson(wb.data, wb.cursor);

  group = conf_node_find_child(doc->root, "hello");
  ASSERT_EQ(group != NULL, true);
  ASSERT_STREQ(conf_node_get_name(group), "hello");

  node = conf_node_find_child(group, "key1");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "key1");
  ASSERT_EQ(conf_node_get_value(node, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "value1");

  node = conf_node_find_child(group, "key2");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "key2");
  ASSERT_EQ(conf_node_get_value(node, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "value2");

  conf_doc_destroy(doc);
  conf_doc_destroy(idoc);
}

static conf_doc_t* create_doc(void) {
  value_t v;
  uint32_t i = 0;
  conf_doc_t* doc = conf_doc_create(100);
  conf_node_t* node = NULL;
  conf_node_t* obj = NULL;
  conf_node_t* arr = NULL;
  doc->root = conf_doc_create_node(doc, "root");

  obj = conf_doc_create_node(doc, "tom");
  conf_doc_append_child(doc, doc->root, obj);

  node = conf_doc_create_node(doc, "name");
  value_set_str(&v, "tom");
  conf_node_set_value(node, &v);
  conf_doc_append_child(doc, obj, node);

  node = conf_doc_create_node(doc, "age");
  value_set_int(&v, 28);
  conf_node_set_value(node, &v);
  conf_doc_append_child(doc, obj, node);

  arr = conf_doc_create_node(doc, "data");
  arr->node_type = CONF_NODE_ARRAY;
  conf_doc_append_child(doc, obj, arr);

  for (i = 0; i < 10; i++) {
    node = conf_doc_create_node(doc, NULL);
    value_set_int(&v, i);
    conf_node_set_value(node, &v);
    conf_doc_append_child(doc, arr, node);
  }

  return doc;
}

TEST(ConfUBJson, create_doc) {
  value_t v;
  wbuffer_t wb;
  uint8_t buff[1024];
  ubjson_writer_t ub;
  wbuffer_init(&wb, buff, sizeof(buff));
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  conf_doc_t* idoc = create_doc();
  ASSERT_EQ(conf_doc_save_ubjson(idoc, &ub), RET_OK);
  conf_doc_t* doc = conf_doc_load_ubjson(wb.data, wb.cursor);
  ubjson_dump(wb.data, wb.cursor);

  ASSERT_EQ(conf_doc_get(doc, "tom.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "tom");

  ASSERT_EQ(conf_doc_get(doc, "tom.age", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 28);

  ASSERT_EQ(conf_doc_get(doc, "tom.data.[2]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  conf_doc_destroy(doc);
  conf_doc_destroy(idoc);
}

TEST(UbJson, file) {
  object_t* conf = conf_ubjson_load("file://./tests/testdata/test.ubjson", TRUE);

  ASSERT_EQ(object_set_prop_str(conf, "tom.name", "tom"), RET_OK);
  ASSERT_EQ(object_set_prop_int(conf, "tom.age", 100), RET_OK);
  ASSERT_EQ(object_set_prop_float(conf, "tom.weight", 60.5), RET_OK);

  ASSERT_STREQ(object_get_prop_str(conf, "tom.name"), "tom");
  ASSERT_EQ(object_get_prop_int(conf, "tom.age", 0), 100);
  ASSERT_EQ(object_get_prop_float(conf, "tom.weight", 0), 60.5);

  ASSERT_EQ(conf_obj_save(conf), RET_OK);

  ASSERT_EQ(object_set_prop_str(conf, "group.key", "value"), RET_OK);
  ASSERT_STREQ(object_get_prop_str(conf, "group.key"), "value");
  ASSERT_EQ(object_remove_prop(conf, "group.key"), RET_OK);
  ASSERT_EQ(object_get_prop_str(conf, "group.key"), (char*)NULL);

  OBJECT_UNREF(conf);
}
