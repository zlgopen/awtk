#include "gtest/gtest.h"
#include "tkc/named_value.h"
#include "conf_io/conf_json.h"

TEST(ConfJson, arr) {
  value_t v;
  str_t str;
  const char* data = " [[1,2,3], \"abc\"] ";
  conf_doc_t* doc = conf_doc_load_json(data, -1);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  str_reset(&str);

  ASSERT_EQ(conf_doc_get(doc, "[1]", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "abc");

  ASSERT_EQ(conf_doc_get(doc, "[0].[0]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(conf_doc_get(doc, "[0].[1]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  ASSERT_EQ(conf_doc_get(doc, "[0].[2]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(ConfJson, arr_comment) {
  value_t v;
  str_t str;
  const char* data =
      " /*comment*/[/*comment*/[/*comment*/1/*comment*/,2/*comment*/,/*comment*/3]/*comment*/, "
      "/*comment*/\"abc\"/*comment*/] ";
  conf_doc_t* doc = conf_doc_load_json(data, -1);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  str_reset(&str);

  ASSERT_EQ(conf_doc_get(doc, "[1]", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "abc");

  ASSERT_EQ(conf_doc_get(doc, "[0].[0]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(conf_doc_get(doc, "[0].[1]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  ASSERT_EQ(conf_doc_get(doc, "[0].[2]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(ConfJson, arr1) {
  value_t v;
  str_t str;
  const char* data =
      "{\
    \"y\": 220,\
    \"wires\": [\
            [   \
                \"f3233c29.cc714\"\
            ],  \
            [], \
            [], \
            [   \
                \"983b6406.96f448\"\
            ]   \
        ]    \
   }";

  conf_doc_t* doc = conf_doc_load_json(data, -1);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  str_reset(&str);

  ASSERT_EQ(conf_doc_get(doc, "wires.[0].[0]", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "f3233c29.cc714");

  ASSERT_EQ(conf_doc_get(doc, "wires.[3].[0]", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "983b6406.96f448");

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  log_debug("%s\n", str.str);
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(ConfJson, basic1) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
  const char* data = " {\"tom\" : { \"name\" : \"tom\", \"age\" : 100  }  } ";

  conf_doc_t* doc = conf_doc_load_json(data, -1);

  node = conf_node_find_child(doc->root, "tom");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "tom");

  ASSERT_EQ(conf_doc_get(doc, "tom.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "tom");

  ASSERT_EQ(conf_doc_get(doc, "tom.age", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 100);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);

  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(ConfJson, basic1_comment) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
  const char* data =
      " /*comment*/{//comment\r\"tom\"//comment\n : { //comment\r\n \"name\"//comment\r\n : "
      "//comment\r\n\"tom\"//comment\r\n, \"age\"//comment\r : 100 //comment\n } /*comment*/ "
      "/*comment*/} ";

  conf_doc_t* doc = conf_doc_load_json(data, -1);

  node = conf_node_find_child(doc->root, "tom");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "tom");

  ASSERT_EQ(conf_doc_get(doc, "tom.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "tom");

  ASSERT_EQ(conf_doc_get(doc, "tom.age", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 100);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);

  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(ConfJson, basic21) {
  value_t v;
  str_t str;
  const char* data = " {\"name\" : \"aaa\\\\n\"";

  conf_doc_t* doc = conf_doc_load_json(data, -1);

  ASSERT_EQ(conf_doc_get(doc, "name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "aaa\\n");

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  ASSERT_STREQ(str.str, "{\n    \"name\" : \"aaa\\\\n\"\n}");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Json, clear) {
  value_t v;
  const char* data = " {\"hello\" : { \"name\" : \"tom\", \"age\" : 100  }  } ";
  conf_doc_t* doc = conf_doc_load_json(data, -1);

  ASSERT_EQ(conf_doc_get(doc, "hello.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  ASSERT_EQ(conf_doc_clear(doc, "hello"), RET_OK);
  ASSERT_EQ(conf_doc_get(doc, "hello.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  conf_doc_destroy(doc);
}

TEST(Json, clear_array) {
  value_t v;
  const char* data = " {\"hello\" : [{\"a\":1},{\"b\":2}]  } ";
  conf_doc_t* doc = conf_doc_load_json(data, -1);

  ASSERT_EQ(conf_doc_get(doc, "hello.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  ASSERT_EQ(conf_doc_clear(doc, "hello"), RET_OK);
  ASSERT_EQ(conf_doc_get(doc, "hello.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  conf_doc_destroy(doc);
}

TEST(ConfJson, name) {
  value_t v;
  const char* data = " {\"tom\" : { \"name\" : \"tom\", \"age\" : 100  }  } ";

  conf_doc_t* doc = conf_doc_load_json(data, -1);

  ASSERT_EQ(conf_doc_get(doc, "tom.name.#name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "name");

  ASSERT_EQ(conf_doc_get(doc, "tom.[0].#name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "name");

  ASSERT_EQ(conf_doc_get(doc, "tom.age.#name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "age");

  ASSERT_EQ(conf_doc_get(doc, "tom.[1].#name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "age");

  ASSERT_EQ(conf_doc_get(doc, "tom.#name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "tom");

  conf_doc_destroy(doc);
}

TEST(ConfJson, size1) {
  value_t v;
  const char* data = " {\"tom\" : { \"name\" : \"tom\", \"age\" : 100  }  } ";

  conf_doc_t* doc = conf_doc_load_json(data, -1);

  ASSERT_EQ(conf_doc_get(doc, "tom.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  ASSERT_EQ(conf_doc_get(doc, "tom.name.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  conf_doc_destroy(doc);
}

TEST(ConfJson, size2) {
  value_t v;
  const char* data = " {\"data\" : [1,2,3,4,5]  } ";

  conf_doc_t* doc = conf_doc_load_json(data, -1);

  ASSERT_EQ(conf_doc_get(doc, "data.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 5);

  conf_doc_destroy(doc);
}

TEST(ConfJson, size3) {
  value_t v;
  const char* data = " {\"data\" : [{\"name\":\"tom\"},{\"name\":\"jack\"}]  } ";

  conf_doc_t* doc = conf_doc_load_json(data, -1);

  ASSERT_EQ(conf_doc_get(doc, "data.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  conf_doc_destroy(doc);
}

TEST(ConfJson, size4) {
  value_t v;
  const char* data = " {\"data\" : [{\"name\":\"tom\"},{\"name\":\"jack\"}]  } ";

  conf_doc_t* doc = conf_doc_load_json(data, -1);

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  conf_doc_destroy(doc);
}

TEST(ConfJson, escape) {
  value_t v;
  str_t str;
  const char* data = " {\"name\" : \"\\\"tom\\\"\"}";

  conf_doc_t* doc = conf_doc_load_json(data, -1);

  ASSERT_EQ(conf_doc_get(doc, "name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "\"tom\"");

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  ASSERT_STREQ(str.str, "{\n    \"name\" : \"\\\"tom\\\"\"\n}");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(ConfJson, basic2) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
  const char* data =
      " {\"tom\" : { \"name\" : \"tom\", \"age\" : 100, \"weight\":58.5, \"gender\" : true, "
      "\"dead\": false }  } ";

  conf_doc_t* doc = conf_doc_load_json(data, -1);

  node = conf_node_find_child(doc->root, "tom");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "tom");

  ASSERT_EQ(conf_doc_get(doc, "tom.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "tom");

  ASSERT_EQ(conf_doc_get(doc, "tom.age", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 100);

  ASSERT_EQ(conf_doc_get(doc, "tom.weight", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 58);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Json, file) {
  tk_object_t* conf = conf_json_load("file://./tests/testdata/test.json", TRUE);

  ASSERT_EQ(conf_obj_save(conf), RET_OK);
  ASSERT_EQ(tk_object_set_prop_str(conf, "tom.name", "tom"), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(conf, "tom.age", 100), RET_OK);
  ASSERT_EQ(tk_object_set_prop_float(conf, "tom.weight", 60.5), RET_OK);

  ASSERT_STREQ(tk_object_get_prop_str(conf, "tom.name"), "tom");
  ASSERT_EQ(tk_object_get_prop_int(conf, "tom.age", 0), 100);
  ASSERT_EQ(tk_object_get_prop_float(conf, "tom.weight", 0), 60.5);

  ASSERT_EQ(conf_obj_save(conf), RET_OK);

  ASSERT_STREQ(tk_object_get_prop_str(conf, "group.key"), "value");
  ASSERT_EQ(tk_object_remove_prop(conf, "group.key"), RET_OK);
  ASSERT_EQ(tk_object_get_prop_str(conf, "group.key"), (char*)NULL);

  TK_OBJECT_UNREF(conf);
}

TEST(Json, exec) {
  tk_object_t* conf = conf_json_load("file://./tests/testdata/test.json", TRUE);

  ASSERT_EQ(tk_object_can_exec(conf, TK_OBJECT_CMD_MOVE_UP, "group.key"), FALSE);
  ASSERT_EQ(tk_object_can_exec(conf, TK_OBJECT_CMD_MOVE_DOWN, "group.key"), TRUE);
  ASSERT_EQ(tk_object_can_exec(conf, TK_OBJECT_CMD_CLEAR, "group"), TRUE);
  ASSERT_EQ(tk_object_can_exec(conf, TK_OBJECT_CMD_REMOVE, "group.key"), TRUE);
  ASSERT_EQ(tk_object_can_exec(conf, TK_OBJECT_CMD_SAVE, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(conf, TK_OBJECT_CMD_RELOAD, NULL), TRUE);

  ASSERT_EQ(tk_object_exec(conf, TK_OBJECT_CMD_MOVE_DOWN, "group.key"), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(conf, "group.key.#index", 0), 1);

  ASSERT_EQ(tk_object_exec(conf, TK_OBJECT_CMD_MOVE_UP, "group.key"), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(conf, "group.key.#index", 0), 0);

  ASSERT_STREQ(tk_object_get_prop_str(conf, "group.key"), "value");
  ASSERT_EQ(tk_object_exec(conf, TK_OBJECT_CMD_REMOVE, "group.key"), RET_OK);
  ASSERT_EQ(tk_object_get_prop_str(conf, "group.key"), (char*)NULL);

  ASSERT_EQ(tk_object_exec(conf, TK_OBJECT_CMD_CLEAR, "group"), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(conf, "group.#size", -1), 0);

  ASSERT_EQ(tk_object_exec(conf, TK_OBJECT_CMD_RELOAD, NULL), RET_OK);
  ASSERT_EQ(tk_object_exec(conf, TK_OBJECT_CMD_SAVE, NULL), RET_OK);

  TK_OBJECT_UNREF(conf);
}

TEST(Json, load1) {
  tk_object_t* conf = conf_json_load(NULL, FALSE);
  ASSERT_EQ(conf, (tk_object_t*)NULL);

  conf = conf_json_load(NULL, TRUE);
  ASSERT_NE(conf, (tk_object_t*)NULL);

  TK_OBJECT_UNREF(conf);
}

TEST(Json, create) {
  tk_object_t* conf = conf_json_create();
  ASSERT_NE(conf, (tk_object_t*)NULL);
  ASSERT_EQ(tk_object_set_prop_int(conf, "value", 123), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(conf, "value", 0), 123);
  TK_OBJECT_UNREF(conf);
}

#include "tkc/data_reader_mem.h"
#include "tkc/data_writer_wbuffer.h"

TEST(Json, save_as) {
  wbuffer_t wb;
  char url[MAX_PATH + 1];
  tk_object_t* conf = conf_json_create();
  ASSERT_NE(conf, (tk_object_t*)NULL);
  ASSERT_EQ(tk_object_set_prop_int(conf, "value", 123), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(conf, "value", 0), 123);
  wbuffer_init_extendable(&wb);
  data_writer_wbuffer_build_url(&wb, url);

  ASSERT_EQ(conf_json_save_as(conf, url), RET_OK);
  TK_OBJECT_UNREF(conf);

  data_reader_mem_build_url(wb.data, wb.cursor, url);
  conf = conf_json_load(url, FALSE);
  ASSERT_NE(conf, (tk_object_t*)NULL);

  ASSERT_EQ(tk_object_get_prop_int(conf, "value", 0), 123);
  wbuffer_deinit(&wb);
  TK_OBJECT_UNREF(conf);
}

TEST(ConfJson, find) {
  value_t v;
  const char* data = "{\"tom\": {\"name\":{\"first\":\"bill\", \"last\":\"tom\"}, \"age\":100}} ";
  conf_doc_t* doc = conf_doc_load_json(data, -1);
  conf_node_t* tom = conf_doc_find_node(doc, doc->root, "tom", FALSE);
  ASSERT_EQ(tom != NULL, true);
  conf_node_t* name = conf_doc_find_node(doc, tom, "name", FALSE);
  ASSERT_EQ(name != NULL, true);

  ASSERT_EQ(conf_doc_get_ex(doc, tom, "age", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 100);

  ASSERT_EQ(conf_doc_get_ex(doc, tom, "name.first", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "bill");

  ASSERT_EQ(conf_doc_get_ex(doc, tom, "name.last", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "tom");

  ASSERT_EQ(conf_doc_get_ex(doc, name, "first", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "bill");

  ASSERT_EQ(conf_doc_get_ex(doc, name, "last", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "tom");

  conf_doc_destroy(doc);
}

TEST(ConfJson, null) {
  value_t v;
  const char* data = " {\"tom\" : { \"name\" : null, \"age\" : 100  }  } ";
  conf_doc_t* doc = conf_doc_load_json(data, -1);
  ASSERT_EQ(conf_doc_get(doc, "tom.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), NULL);

  conf_doc_destroy(doc);
}

TEST(ConfJson, null_in_array) {
  value_t v;
  const char* data = " [null, null] ";
  conf_doc_t* doc = conf_doc_load_json(data, -1);

  value_set_str(&v, "123");

  ASSERT_EQ(conf_doc_get(doc, "[0]", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), NULL);

  ASSERT_EQ(conf_doc_get(doc, "[1]", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), NULL);

  conf_doc_destroy(doc);
}

TEST(ConfJson, number) {
  value_t v;
  str_t str;
  const char* data = " [0,1000,-1000,12147483647,-12147483647] ";
  conf_doc_t* doc = conf_doc_load_json(data, -1);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  str_reset(&str);

  ASSERT_EQ(conf_doc_get(doc, "[0]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  ASSERT_EQ(conf_doc_get(doc, "[1]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1000);

  ASSERT_EQ(conf_doc_get(doc, "[2]", &v), RET_OK);
  ASSERT_EQ(value_int(&v), -1000);

  ASSERT_EQ(conf_doc_get(doc, "[3]", &v), RET_OK);
  ASSERT_EQ(value_int64(&v), 12147483647);

  ASSERT_EQ(conf_doc_get(doc, "[4]", &v), RET_OK);
  ASSERT_EQ(value_int64(&v), -12147483647);

  conf_doc_destroy(doc);
}

static ret_t on_prop(void* ctx, const void* data) {
  char buff[64];
  str_t* str = (str_t*)ctx;
  named_value_t* nv = (named_value_t*)data;

  str_append_more(str, nv->name, "=", value_str_ex(&(nv->value), buff, sizeof(buff)), "\n", NULL);

  return RET_OK;
}

TEST(Json, foreach) {
  str_t str;
  tk_object_t* conf = conf_json_create();
  ASSERT_NE(conf, (tk_object_t*)NULL);
  ASSERT_EQ(tk_object_set_prop_str(conf, "name", "awtk"), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(conf, "value", 123), RET_OK);

  str_init(&str, 100);
  ASSERT_EQ(tk_object_foreach_prop(conf, on_prop, &str), RET_OK);
  ASSERT_STREQ(str.str, "name=awtk\nvalue=123\n");

  str_reset(&str);
  TK_OBJECT_UNREF(conf);
}

TEST(Json, foreach1) {
  str_t str;
  tk_object_t* conf = conf_json_create();
  ASSERT_NE(conf, (tk_object_t*)NULL);
  ASSERT_EQ(tk_object_set_prop_str(conf, "name", "awtk"), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(conf, "value", 123), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(conf, "detail.age", 123), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(conf, "detail.salary", 1000), RET_OK);

  str_init(&str, 100);
  ASSERT_EQ(tk_object_foreach_prop(conf, on_prop, &str), RET_OK);
  ASSERT_EQ(strstr(str.str, "name=awtk\nvalue=123\n") != NULL, TRUE);

  str_reset(&str);
  TK_OBJECT_UNREF(conf);
}

TEST(Json, subobject1) {
  str_t str;
  tk_object_t* conf = conf_json_create();
  ASSERT_NE(conf, (tk_object_t*)NULL);
  ASSERT_EQ(tk_object_set_prop_str(conf, "name", "awtk"), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(conf, "value", 123), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(conf, "detail.age", 123), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(conf, "detail.salary", 1000), RET_OK);
  ASSERT_EQ(tk_object_set_prop_str(conf, "detail.desc", "hello"), RET_OK);
  tk_object_t* detail = conf_obj_create_sub_object(conf, "detail");

  str_init(&str, 100);
  ASSERT_EQ(tk_object_foreach_prop(detail, on_prop, &str), RET_OK);
  ASSERT_STREQ(str.str, "age=123\nsalary=1000\ndesc=hello\n");

  ASSERT_EQ(tk_object_get_prop_int(detail, "age", 0), 123);
  ASSERT_EQ(tk_object_get_prop_int(detail, "salary", 0), 1000);
  ASSERT_STREQ(tk_object_get_prop_str(detail, "desc"), "hello");

  ASSERT_EQ(tk_object_set_prop_int(detail, "age", 1000), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(detail, "age", 0), 1000);

  ASSERT_EQ(tk_object_remove_prop(detail, "age"), RET_OK);

  str_reset(&str);

  ASSERT_EQ(tk_object_foreach_prop(detail, on_prop, &str), RET_OK);
  ASSERT_STREQ(str.str, "salary=1000\ndesc=hello\n");

  str_reset(&str);
  TK_OBJECT_UNREF(detail);
  TK_OBJECT_UNREF(conf);
}

TEST(ConfJson, dup0) {
  str_t str;
  const char* data = "[0,1,2]";
  conf_doc_t* doc = conf_doc_load_json(data, -1);
  conf_node_t* node = conf_node_find_child(doc->root, "0");
  ASSERT_EQ(node != NULL, true);
  conf_node_t* new_node = conf_doc_dup_node(doc, node, NULL);
  ASSERT_EQ(new_node->node_type, node->node_type);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  ASSERT_STREQ(str.str, "[\n    0,\n    0,\n    1,\n    2\n]");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(ConfJson, dup1) {
  str_t str;
  const char* data = "{\"age\":123}";
  conf_doc_t* doc = conf_doc_load_json(data, -1);
  conf_node_t* node = conf_node_find_child(doc->root, "age");
  conf_node_t* weight = NULL;
  ASSERT_EQ(node != NULL, true);
  weight = conf_doc_dup_node(doc, node, "weight");
  ASSERT_EQ(weight->node_type, node->node_type);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  ASSERT_STREQ(str.str, "{\n    \"age\" : 123,\n    \"weight\" : 123\n}");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(ConfJson, dup2) {
  value_t v;
  str_t str;
  const char* data = " {\"tom\" : { \"name\" : null, \"age\" : 100  }  } ";
  conf_doc_t* doc = conf_doc_load_json(data, -1);
  conf_node_t* tom = conf_node_find_child(doc->root, "tom");
  ASSERT_EQ(tom != NULL, true);
  conf_node_t* jim = conf_doc_dup_node(doc, tom, "jim");
  ASSERT_EQ(jim->node_type, tom->node_type);

  ASSERT_EQ(conf_doc_get(doc, "jim.age", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 100);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  ASSERT_STREQ(str.str,
               "{\n    \"tom\" : {\n        \"name\" : \"\",\n        \"age\" : 100\n    },\n    "
               "\"jim\" : {\n        \"name\" : \"\",\n        \"age\" : 100\n    }\n}");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(ConfJson, dup3) {
  str_t str;
  const char* data = " {\"tom\" : [1,2,3,4] } ";
  conf_doc_t* doc = conf_doc_load_json(data, -1);
  conf_node_t* tom = conf_node_find_child(doc->root, "tom");
  ASSERT_EQ(tom != NULL, true);
  conf_node_t* jim = conf_doc_dup_node(doc, tom, "jim");
  ASSERT_EQ(jim->node_type, tom->node_type);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  ASSERT_STREQ(str.str,
               "{\n    \"tom\" : [\n        1,\n        2,\n        3,\n        4\n    ],\n    "
               "\"jim\" : [\n        1,\n        2,\n        3,\n        4\n    ]\n}");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(ConfJson, set_prop) {
  value_t v;
  str_t str;
  const char* data = " {\"tom\" : { \"name\" : null, \"age\" : 100  }  } ";
  conf_doc_t* doc = conf_doc_load_json(data, -1);
  conf_node_t* tom = conf_node_find_child(doc->root, "tom");
  ASSERT_EQ(tom != NULL, true);

  value_set_str(&v, "tom");
  ASSERT_EQ(conf_doc_set_node_prop(doc, tom, "name", &v), RET_OK);

  value_set_str(&v, "awtk");
  ASSERT_EQ(conf_doc_set_node_prop(doc, tom, "product", &v), RET_OK);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  ASSERT_STREQ(str.str,
               "{\n    \"tom\" : {\n        \"name\" : \"tom\",\n        \"age\" : 100,\n        "
               "\"product\" : \"awtk\"\n    }\n}");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Json, append_array) {
  value_t v;
  str_t str;
  const char* data = " {\"hello\" : [1,2,3,4]  } ";
  conf_doc_t* doc = conf_doc_load_json(data, -1);

  ASSERT_EQ(conf_doc_get(doc, "hello.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 4);
  ASSERT_EQ(conf_doc_set_int(doc, "hello.[-1]", 5), RET_OK);

  str_init(&str, 100);
  conf_doc_save_json(doc, &str);
  ASSERT_STREQ(
      str.str,
      "{\n    \"hello\" : [\n        1,\n        2,\n        3,\n        4,\n        5\n    ]\n}");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Json, subobject2) {
  tk_object_t* conf = conf_json_create();
  ASSERT_NE(conf, (tk_object_t*)NULL);
  ASSERT_EQ(tk_object_set_prop_str(conf, "awtk.name", "awtk"), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(conf, "awtk.value", 123), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(conf, "awtk.detail.age", 123), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(conf, "awtk.detail.salary", 1000), RET_OK);
  ASSERT_EQ(tk_object_set_prop_str(conf, "awtk.detail.desc", "hello"), RET_OK);
  tk_object_t* awtk = conf_obj_create_sub_object(conf, "awtk");
  ASSERT_NE(awtk, (tk_object_t*)NULL);

  tk_object_t* detail = conf_obj_create_sub_object(awtk, "detail");
  ASSERT_NE(detail, (tk_object_t*)NULL);
  ASSERT_EQ(tk_object_get_prop_int(detail, "age", 0), 123);

  TK_OBJECT_UNREF(detail);
  TK_OBJECT_UNREF(awtk);
  TK_OBJECT_UNREF(conf);
}

TEST(Json, buff) {
  wbuffer_t wb;
  tk_object_t* conf = conf_json_create();
  ASSERT_NE(conf, (tk_object_t*)NULL);
  ASSERT_EQ(tk_object_set_prop_int(conf, "awtk.value", 123), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(conf, "awtk.value", 0), 123);

  ASSERT_EQ(conf_json_save_to_buff(conf, &wb), RET_OK);
  TK_OBJECT_UNREF(conf);

  conf = conf_json_load_from_buff(wb.data, wb.cursor, FALSE);
  ASSERT_EQ(tk_object_get_prop_int(conf, "awtk.value", 0), 123);
  TK_OBJECT_UNREF(conf);

  wbuffer_deinit(&wb);
}

TEST(Json, uint32) {
  str_t str;
  tk_object_t* conf = conf_json_create();
  ASSERT_NE(conf, (tk_object_t*)NULL);
  ASSERT_EQ(tk_object_set_prop_str(conf, "name", "awtk"), RET_OK);
  ASSERT_EQ(tk_object_set_prop_uint32(conf, "value", 0xffffffff), RET_OK);

  str_init(&str, 100);
  ASSERT_EQ(tk_object_foreach_prop(conf, on_prop, &str), RET_OK);
  ASSERT_STREQ(str.str, "name=awtk\nvalue=4294967295\n");

  str_reset(&str);
  TK_OBJECT_UNREF(conf);
}

TEST(ConfJson, save_json_ex) {
  value_t v;
  str_t str;
  conf_doc_t* doc = conf_doc_create(100);

  str_init(&str, 0);
  ASSERT_EQ(conf_doc_set(doc, "[0].label", value_set_str(&v, "jim")), RET_OK);
  ASSERT_EQ(conf_doc_set(doc, "[1].label", value_set_str(&v, "jim1")), RET_OK);
  ASSERT_EQ(conf_doc_set(doc, "[2].label", value_set_str(&v, "jim2")), RET_OK);

  ASSERT_EQ(conf_doc_set(doc, "[0].nodes.[0].name", value_set_str(&v, "timer")), RET_OK);
  ASSERT_EQ(conf_doc_set(doc, "[0].nodes.[0].last_out", value_set_int(&v, 1111)), RET_OK);
  ASSERT_EQ(conf_doc_set(doc, "[0].nodes.[1].last_out", value_set_int(&v, 1111)), RET_OK);

  ASSERT_EQ(conf_doc_save_json_ex(doc, &str, 0), RET_OK);
  log_info("\n%s\n\n", str.str);
  ASSERT_STREQ(str.str,
               "[{\"label\":\"jim\",\"nodes\":[{\"name\":\"timer\",\"last_out\":1111},{\"last_"
               "out\":1111}]},{\"label\":\"jim1\"},{\"label\":\"jim2\"}]");

  ASSERT_EQ(conf_doc_save_json_ex(doc, &str, 2), RET_OK);
  log_info("\n%s\n\n", str.str);
  ASSERT_STREQ(str.str,
               "[\n  {\n    \"label\" : \"jim\",\n    \"nodes\" : [\n      {\n        \"name\" : "
               "\"timer\",\n        \"last_out\" : 1111\n      },\n      {\n        \"last_out\" : "
               "1111\n      }\n    ]\n  },\n  {\n    \"label\" : \"jim1\"\n  },\n  {\n    "
               "\"label\" : \"jim2\"\n  }\n]");

  conf_doc_destroy(doc);
  str_reset(&str);
}

TEST(ConfJson, comment) {
  const char* data =
      "\
{\
    \"log_message\": {\
        \"a\":1,\"b\":2,\
        \"enable\": true, /*是否启用告警信息*/\
        \"fields_count\": 4, /*告警信息中带的字段个数: 级别 | 时间 | 设备 | 信息*/\
        \"fields_seperator\": \"|\", /*字段之间的分隔符*/\
        \"max_rows\": 1000 /*告警信息最大行数*/\
    }\
}";
  conf_doc_t* doc = conf_doc_load_json(data, -1);
  ASSERT_EQ(conf_doc_get_int(doc, "log_message.a", 0), 1);
  ASSERT_EQ(conf_doc_get_int(doc, "log_message.b", 0), 2);
  ASSERT_EQ(conf_doc_get_int(doc, "log_message.fields_count", 0), 4);
  ASSERT_EQ(conf_doc_get_int(doc, "log_message.max_rows", 0), 1000);

  conf_doc_destroy(doc);
}

TEST(Json, format) {
  wbuffer_t wb;
  tk_object_t* conf = conf_json_create();
  ASSERT_NE(conf, (tk_object_t*)NULL);
  ASSERT_EQ(tk_object_set_prop_str_with_format(conf, "age", "%d", 123), RET_OK);

  ASSERT_EQ(conf_json_save_to_buff(conf, &wb), RET_OK);
  ASSERT_STREQ((char*)(wb.data), "{\n    \"age\" : \"123\"\n}");
  TK_OBJECT_UNREF(conf);


  wbuffer_deinit(&wb);
}
