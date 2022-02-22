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
  ASSERT_STREQ(str.str, "name=awtk\nvalue=123\n");

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
