#include "gtest/gtest.h"
#include "tkc/buffer.h"
#include "tkc/utils.h"
#include "ubjson/ubjson_writer.h"
#include "conf_io/conf_ini.h"
#include "conf_io/conf_json.h"
#include "conf_io/conf_ubjson.h"
#include "conf_io/conf_node.h"

#include "tkc/object_default.h"
#include "tkc/object_array.h"

TEST(ConfNode, basic) {
  value_t v;
  conf_doc_t* doc = conf_doc_create(100);
  conf_node_t* node = NULL;
  doc->root = conf_doc_create_node(doc, "root");

  conf_doc_append_child(doc, doc->root, conf_doc_create_node(doc, "1"));
  conf_doc_append_child(doc, doc->root, conf_doc_create_node(doc, "2"));
  conf_doc_append_child(doc, doc->root, conf_doc_create_node(doc, "3"));
  conf_doc_append_child(doc, doc->root, conf_doc_create_node(doc, "4"));

  node = conf_node_find_child(doc->root, "1");
  ASSERT_STREQ(conf_node_get_name(node), "1");
  value_set_int(&v, 1);
  ASSERT_EQ(conf_node_set_value(node, &v), RET_OK);
  value_set_int(&v, 0);
  ASSERT_EQ(conf_node_get_child_value(doc->root, "1", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);
  ASSERT_EQ(conf_node_get_child_value_by_index(doc->root, 0, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);
  ASSERT_EQ(conf_node_get_child_value_int32(doc->root, "1", 0), 1);
  ASSERT_EQ(conf_node_get_child_value_int32(doc->root, "abcd", 0), 0);

  node = conf_node_find_child(doc->root, "2");
  ASSERT_STREQ(conf_node_get_name(node), "2");
  value_set_int(&v, 2);
  ASSERT_EQ(conf_node_set_value(node, &v), RET_OK);
  value_set_int(&v, 0);
  ASSERT_EQ(conf_node_get_child_value(doc->root, "2", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  ASSERT_EQ(conf_node_get_child_value_by_index(doc->root, 1, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  node = conf_node_find_child(doc->root, "3");
  ASSERT_STREQ(conf_node_get_name(node), "3");
  value_set_int(&v, 3);
  ASSERT_EQ(conf_node_set_value(node, &v), RET_OK);
  value_set_int(&v, 0);
  ASSERT_EQ(conf_node_get_child_value(doc->root, "3", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);
  ASSERT_EQ(conf_node_get_child_value_by_index(doc->root, 2, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);

  node = conf_node_find_child(doc->root, "4");
  ASSERT_STREQ(conf_node_get_name(node), "4");
  value_set_int(&v, 4);
  ASSERT_EQ(conf_node_set_value(node, &v), RET_OK);
  ASSERT_EQ(conf_node_get_value_int32(node, 0), 4);
  value_set_int(&v, 0);
  ASSERT_EQ(conf_node_get_child_value(doc->root, "4", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 4);
  ASSERT_EQ(conf_node_get_child_value_float(doc->root, "4", 0), 4.0f);
  ASSERT_EQ(conf_node_get_child_value_double(doc->root, "4", 0), 4.0f);
  ASSERT_EQ(conf_node_get_child_value_by_index(doc->root, 3, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 4);

  ASSERT_EQ(conf_node_get_child_value(doc->root, "100", &v), RET_NOT_FOUND);

  ASSERT_EQ(conf_doc_remove_child_by_name(doc, doc->root, "2"), RET_OK);
  ASSERT_NE(conf_doc_remove_child_by_name(doc, doc->root, "2"), RET_OK);
  ASSERT_EQ(conf_doc_remove_child_by_name(doc, doc->root, "1"), RET_OK);
  ASSERT_NE(conf_doc_remove_child_by_name(doc, doc->root, "1"), RET_OK);
  ASSERT_EQ(conf_doc_remove_child_by_name(doc, doc->root, "4"), RET_OK);
  ASSERT_NE(conf_doc_remove_child_by_name(doc, doc->root, "4"), RET_OK);

  node = conf_node_find_child(doc->root, "3");
  ASSERT_EQ(conf_doc_remove_child(doc, doc->root, node), RET_OK);
  ASSERT_NE(conf_doc_remove_child_by_name(doc, doc->root, "3"), RET_OK);

  conf_doc_append_child(doc, doc->root, conf_doc_create_node(doc, "4"));
  node = conf_node_find_child(doc->root, "4");
  value_set_bool(&v, TRUE);
  ASSERT_EQ(conf_node_set_value(node, &v), RET_OK);
  ASSERT_EQ(conf_node_get_value_bool(node, FALSE), TRUE);
  ASSERT_EQ(conf_node_get_child_value_bool(doc->root, "4", FALSE), TRUE);
  ASSERT_EQ(conf_node_get_child_value_bool(doc->root, "abcd", FALSE), FALSE);

  value_set_str(&v, "abc");
  ASSERT_EQ(conf_node_set_value(node, &v), RET_OK);
  ASSERT_STREQ(conf_node_get_child_value_str(doc->root, "4", NULL), "abc");
  ASSERT_STREQ(conf_node_get_child_value_str(doc->root, "abcd", "123"), "123");

  value_t v1;
  value_set_str(&v, "abc");
  ASSERT_EQ(conf_node_set_value(node, &v), RET_OK);
  ASSERT_EQ(conf_node_get_value(node, &v1), RET_OK);
  ASSERT_EQ(conf_node_set_value(node, &v1), RET_OK);
  ASSERT_EQ(conf_node_get_value(node, &v1), RET_OK);
  ASSERT_STREQ(value_str(&v), "abc");
  ASSERT_STREQ(conf_node_get_value_str(node, NULL), "abc");

  value_set_wstr(&v, L"abc");
  ASSERT_EQ(conf_node_set_value(node, &v), RET_OK);
  ASSERT_EQ(conf_node_get_value(node, &v1), RET_OK);
  ASSERT_EQ(conf_node_set_value(node, &v1), RET_OK);
  ASSERT_EQ(conf_node_get_value(node, &v1), RET_OK);
  ASSERT_EQ(wcscmp(value_wstr(&v), L"abc"), 0);

  value_set_str(&v, "aaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbcccccc");
  ASSERT_EQ(conf_node_set_value(node, &v), RET_OK);
  ASSERT_EQ(conf_node_get_value(node, &v1), RET_OK);
  ASSERT_EQ(conf_node_set_value(node, &v1), RET_OK);
  ASSERT_EQ(conf_node_get_value(node, &v1), RET_OK);
  ASSERT_STREQ(value_str(&v), "aaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbcccccc");

  value_set_wstr(&v, L"aaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbcccccc");
  ASSERT_EQ(conf_node_set_value(node, &v), RET_OK);
  ASSERT_EQ(conf_node_get_value(node, &v1), RET_OK);
  ASSERT_EQ(conf_node_set_value(node, &v1), RET_OK);
  ASSERT_EQ(conf_node_get_value(node, &v1), RET_OK);
  ASSERT_EQ(wcscmp(value_wstr(&v), L"aaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbcccccc"), 0);

  conf_doc_destroy(doc);
}

TEST(ConfNode, set_get) {
  value_t v;
  conf_doc_t* doc = conf_doc_create(100);

  ASSERT_EQ(conf_doc_set(doc, "person.[0].name", value_set_str(&v, "jim")), RET_OK);
  ASSERT_EQ(conf_doc_set(doc, "person.[1].name", value_set_str(&v, "tom")), RET_OK);
  ASSERT_EQ(conf_doc_set(doc, "person.[2].name", value_set_str(&v, "mike")), RET_OK);

  ASSERT_EQ(conf_doc_get(doc, "person.[0].name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "jim");

  ASSERT_EQ(conf_doc_get(doc, "person.[1].name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "tom");

  ASSERT_EQ(conf_doc_get(doc, "person.[2].name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "mike");

  ASSERT_NE(conf_doc_set(doc, "person.[20].name", value_set_str(&v, "anny")), RET_OK);

  conf_doc_destroy(doc);
}

TEST(ConfNode, set_get_str) {
  value_t v;
  str_t str;
  conf_doc_t* doc = conf_doc_create(100);

  str_init(&str, 0);
  ASSERT_EQ(conf_doc_set(doc, "names.[0]", value_set_str(&v, "jim")), RET_OK);
  ASSERT_EQ(conf_doc_set(doc, "names.[1]", value_set_str(&v, "tom")), RET_OK);
  ASSERT_EQ(conf_doc_set(doc, "names.[2]", value_set_str(&v, "anny")), RET_OK);

  conf_node_t* node;
  node = conf_doc_find_node(doc, doc->root, "names.[0]", FALSE);
  ASSERT_TRUE(node != NULL);
  ASSERT_EQ(node->node_type, CONF_NODE_SIMPLE);

  ASSERT_EQ(conf_doc_save_json(doc, &str), RET_OK);
  ASSERT_STREQ(
      str.str,
      "{\n    \"names\" : [\n        \"jim\",\n        \"tom\",\n        \"anny\"\n    ]\n}");

  conf_doc_destroy(doc);
  str_reset(&str);
}

TEST(ConfNode, set_get_wstr) {
  value_t v;
  conf_doc_t* doc = conf_doc_create(100);

  ASSERT_EQ(conf_doc_set(doc, "names.[0]", value_set_wstr(&v, L"jim")), RET_OK);
  ASSERT_EQ(conf_doc_set(doc, "names.[1]", value_set_wstr(&v, L"tom")), RET_OK);
  ASSERT_EQ(conf_doc_set(doc, "names.[2]", value_set_wstr(&v, L"anny")), RET_OK);

  ASSERT_EQ(conf_doc_get(doc, "names.[0]", &v), RET_OK);
  ASSERT_EQ(wcscmp(value_wstr(&v), L"jim"), 0);
  ASSERT_EQ(conf_doc_get(doc, "names.[1]", &v), RET_OK);
  ASSERT_EQ(wcscmp(value_wstr(&v), L"tom"), 0);
  ASSERT_EQ(conf_doc_get(doc, "names.[2]", &v), RET_OK);
  ASSERT_EQ(wcscmp(value_wstr(&v), L"anny"), 0);

  str_t str;
  str_init(&str, 0);
  ASSERT_EQ(conf_doc_save_json(doc, &str), RET_OK);
  ASSERT_STREQ(
      str.str,
      "{\n    \"names\" : [\n        \"jim\",\n        \"tom\",\n        \"anny\"\n    ]\n}");

  str_reset(&str);

  str_init(&str, 0);
  ASSERT_EQ(conf_doc_save_ini(doc, &str), RET_OK);
  ASSERT_STREQ(str.str, "[names]\n  [0] = jim\n  [1] = tom\n  [2] = anny\n");
  str_reset(&str);

  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init_extendable(&wb);
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);
  conf_doc_save_ubjson(doc, &ub);

  conf_doc_destroy(doc);
  doc = conf_doc_load_ubjson(wb.data, wb.cursor);

  ASSERT_EQ(conf_doc_get(doc, "names.[0]", &v), RET_OK);
  ASSERT_EQ(wcscmp(value_wstr(&v), L"jim"), 0);
  ASSERT_EQ(conf_doc_get(doc, "names.[1]", &v), RET_OK);
  ASSERT_EQ(wcscmp(value_wstr(&v), L"tom"), 0);
  ASSERT_EQ(conf_doc_get(doc, "names.[2]", &v), RET_OK);
  ASSERT_EQ(wcscmp(value_wstr(&v), L"anny"), 0);

  conf_doc_destroy(doc);
  wbuffer_deinit(&wb);
}

TEST(ConfNode, binary) {
  value_t v;
  conf_doc_t* doc = conf_doc_create(100);

  ASSERT_EQ(conf_doc_set(doc, "names.[0]", value_set_binary_data(&v, (void*)"jim", 3)), RET_OK);
  ASSERT_EQ(conf_doc_set(doc, "names.[1]", value_set_binary_data(&v, (void*)"tom", 3)), RET_OK);

  ASSERT_EQ(conf_doc_get(doc, "names.[0]", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_BINARY);

  ASSERT_EQ(conf_doc_get(doc, "names.[1]", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_BINARY);
  ASSERT_EQ(memcmp(v.value.binary_data.data, "tom", v.value.binary_data.size), 0);
  ASSERT_EQ(memcmp(v.value.binary_data.data, "tom", v.value.binary_data.size), 0);

  conf_node_t* node;
  node = conf_doc_find_node(doc, doc->root, "names.[0]", FALSE);
  ASSERT_TRUE(node != NULL);
  ASSERT_EQ(node->node_type, CONF_NODE_ARRAY_UINT8);
  ASSERT_EQ(node->value_type, CONF_NODE_VALUE_BINARY);

  node = conf_doc_find_node(doc, doc->root, "names.[1]", FALSE);
  ASSERT_TRUE(node != NULL);
  ASSERT_EQ(node->node_type, CONF_NODE_ARRAY_UINT8);
  ASSERT_EQ(node->value_type, CONF_NODE_VALUE_BINARY);

  ASSERT_EQ(conf_doc_set(doc, "names.[0]", value_set_int32(&v, 1)), RET_OK);
  conf_doc_get(doc, "names.[0]", &v);
  ASSERT_EQ(v.type, VALUE_TYPE_INT32);
  ASSERT_EQ(value_int(&v), 1);

  conf_doc_destroy(doc);
}

static tk_object_t* conf_node_test_object_create(void) {
  tk_object_t* ret = object_default_create();
  tk_object_set_prop_str(ret, "name", "test1");
  tk_object_set_prop_uint32(ret, "id", 123);
  tk_object_set_prop_str(ret, "desc", "aaa");

  tk_object_t* obj = object_default_create();
  tk_object_set_prop_str(obj, "name", "test2");
  tk_object_set_prop_uint32(obj, "id", 567);
  tk_object_set_prop_str(obj, "desc", "bbb");

  tk_object_set_prop_object(ret, "obj", obj);
  tk_object_unref(obj);

  return ret;
}

TEST(ConfNode, set_get_object) {
  value_t v;
  conf_doc_t* doc = conf_doc_create(100);
  tk_object_t* obj = conf_node_test_object_create();
  str_t str;
  str_init(&str, 64);

  conf_doc_use_extend_type(doc, TRUE);

  ASSERT_EQ(conf_doc_set(doc, "obj", value_set_object(&v, obj)), RET_OK);

  ASSERT_EQ(conf_doc_save_json(doc, &str), RET_OK);
  ASSERT_STREQ(str.str,
               "{\n"
               "    \"obj\" : {\n"
               "        \"desc\" : \"aaa\",\n"
               "        \"id\" : 123,\n"
               "        \"name\" : \"test1\",\n"
               "        \"obj\" : {\n"
               "            \"desc\" : \"bbb\",\n"
               "            \"id\" : 567,\n"
               "            \"name\" : \"test2\"\n"
               "        }\n"
               "    }\n"
               "}");
  str_clear(&str);

  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init_extendable(&wb);
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);
  conf_doc_save_ubjson(doc, &ub);

  conf_doc_destroy(doc);
  doc = conf_doc_load_ubjson(wb.data, wb.cursor);

  ASSERT_EQ(conf_doc_get(doc, "obj.desc", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "aaa");
  ASSERT_EQ(conf_doc_get(doc, "obj.id", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 123);
  ASSERT_EQ(conf_doc_get(doc, "obj.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "test1");

  ASSERT_EQ(conf_doc_get(doc, "obj.obj.desc", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "bbb");
  ASSERT_EQ(conf_doc_get(doc, "obj.obj.id", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 567);
  ASSERT_EQ(conf_doc_get(doc, "obj.obj.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "test2");

  conf_doc_destroy(doc);
  wbuffer_deinit(&wb);

  str_reset(&str);
  TK_OBJECT_UNREF(obj);
}

static tk_object_t* conf_node_test_object_array_create(void) {
  value_t v;
  tk_object_t* ret = object_array_create();
  object_array_push(ret, value_set_str(&v, "aaa"));
  object_array_push(ret, value_set_uint32(&v, 123));
  object_array_push(ret, value_set_str(&v, "test1"));

  tk_object_t* obj = object_array_create();
  object_array_push(obj, value_set_str(&v, "bbb"));
  object_array_push(obj, value_set_uint32(&v, 567));
  object_array_push(obj, value_set_str(&v, "test2"));

  object_array_push(ret, value_set_object(&v, obj));
  tk_object_unref(obj);

  return ret;
}

TEST(ConfNode, set_get_object_array) {
  value_t v;
  conf_doc_t* doc = conf_doc_create(100);
  tk_object_t* obj = conf_node_test_object_array_create();
  str_t str;
  str_init(&str, 64);

  conf_doc_use_extend_type(doc, TRUE);

  ASSERT_EQ(conf_doc_set(doc, "obj", value_set_object(&v, obj)), RET_OK);

  ASSERT_EQ(conf_doc_save_json(doc, &str), RET_OK);
  ASSERT_STREQ(str.str,
               "{\n"
               "    \"obj\" : [\n"
               "        \"aaa\",\n"
               "        123,\n"
               "        \"test1\",\n"
               "        [\n"
               "            \"bbb\",\n"
               "            567,\n"
               "            \"test2\"\n"
               "        ]\n"
               "    ]\n"
               "}");
  str_clear(&str);

  wbuffer_t wb;
  ubjson_writer_t ub;
  wbuffer_init_extendable(&wb);
  ubjson_writer_init(&ub, (ubjson_write_callback_t)wbuffer_write_binary, &wb);
  conf_doc_save_ubjson(doc, &ub);

  conf_doc_destroy(doc);
  doc = conf_doc_load_ubjson(wb.data, wb.cursor);

  ASSERT_EQ(conf_doc_get(doc, "obj.[0]", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "aaa");
  ASSERT_EQ(conf_doc_get(doc, "obj.[1]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 123);
  ASSERT_EQ(conf_doc_get(doc, "obj.[2]", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "test1");

  ASSERT_EQ(conf_doc_get(doc, "obj.[3].[0]", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "bbb");
  ASSERT_EQ(conf_doc_get(doc, "obj.[3].[1]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 567);
  ASSERT_EQ(conf_doc_get(doc, "obj.[3].[2]", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "test2");

  conf_doc_destroy(doc);
  wbuffer_deinit(&wb);

  str_reset(&str);
  TK_OBJECT_UNREF(obj);
}

static ret_t conf_doc_on_check_data(void* ctx, const char* path, value_t* v) {
  value_t vv;
  ENSURE(tk_object_get_prop(TK_OBJECT(ctx), path, &vv) == RET_OK);
  ENSURE(value_equal(&vv, v));
  tk_object_remove_prop(TK_OBJECT(ctx), path);
  log_info("    %s=%s\n", path, value_str(v));
  return RET_OK;
}

static ret_t conf_doc_on_copy_data(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  ENSURE(conf_doc_set((conf_doc_t*)ctx, nv->name, &nv->value) == RET_OK);
  return RET_OK;
}

TEST(ConfNode, foreach) {
  conf_doc_t* doc = conf_doc_create(100);

  tk_object_t* data = object_default_create();
  tk_object_set_prop_str(data, "server.ip", "192.168.8.101");
  tk_object_set_prop_str(data, "server.port", "8383");
  tk_object_set_prop_str(data, "server.stat.up", "true");
  tk_object_set_prop_str(data, "server.stat.linkup", "false");
  tk_object_set_prop_str(data, "server.dns.[0]", "8.8.8.1");
  tk_object_set_prop_str(data, "server.dns.[1]", "4.4.4.1");

  tk_object_foreach_prop(data, conf_doc_on_copy_data, doc);

  conf_doc_foreach(doc, conf_doc_on_check_data, data);
  ENSURE(tk_object_get_prop_int(data, TK_OBJECT_PROP_SIZE, -1) == 0);

  conf_doc_destroy(doc);
  TK_OBJECT_UNREF(data);
}

static ret_t on_foreach_path(void* ctx, const char* path, value_t* v) {
  str_t* str = (str_t*)ctx;
  char buf[128];
  tk_snprintf(buf, sizeof(buf), "%s=%s;", path, value_str(v));
  str_append(str, buf);
  return RET_OK;
}

TEST(ConfNode, foreach_path) {
  conf_doc_t* doc = conf_doc_create(100);
  str_t *str = str_create(1024);
  tk_object_t* data = object_default_create();

  tk_object_set_prop_str(data, "client.ip", "192.168.8.101");
  tk_object_set_prop_str(data, "client.port", "8383");
  tk_object_set_prop_str(data, "server.stat.up", "true");
  tk_object_set_prop_str(data, "server.stat.linkup", "false");
  tk_object_set_prop_str(data, "server.dns.[0]", "8.8.8.1");
  tk_object_set_prop_str(data, "server.dns.[1]", "4.4.4.1");

  tk_object_foreach_prop(data, conf_doc_on_copy_data, doc);

  conf_doc_foreach_path(doc, "client", on_foreach_path, str);
  log_info("  %s\n", str->str);
  ASSERT_STREQ(str->str, "ip=192.168.8.101;port=8383;");
  str_reset(str);

  conf_doc_foreach_path(doc, "server.stat", on_foreach_path, str);
  log_info("  %s\n", str->str);
  ASSERT_STREQ(str->str, "linkup=false;up=true;");
  str_reset(str);

  conf_doc_foreach_path(doc, "server.dns", on_foreach_path, str);
  log_info("  %s\n", str->str);
  ASSERT_STREQ(str->str, "[0]=8.8.8.1;[1]=4.4.4.1;");
  str_reset(str);

  conf_doc_foreach_path(doc, "server", on_foreach_path, str);
  log_info("  %s\n", str->str);
  ASSERT_STREQ(str->str, "dns.[0]=8.8.8.1;dns.[1]=4.4.4.1;stat.linkup=false;stat.up=true;");
  str_reset(str);

  conf_doc_destroy(doc);
  TK_OBJECT_UNREF(data);
  str_destroy(str);

}
