#include "gtest/gtest.h"
#include "conf_io/conf_json.h"

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
  conf_node_t* node = NULL;
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
  conf_node_t* node = NULL;
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
  str_t str;
  conf_node_t* node = NULL;
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
  str_t str;
  conf_node_t* node = NULL;
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
  str_t str;
  conf_node_t* node = NULL;
  const char* data = " {\"data\" : [1,2,3,4,5]  } ";

  conf_doc_t* doc = conf_doc_load_json(data, -1);

  ASSERT_EQ(conf_doc_get(doc, "data.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 5);

  conf_doc_destroy(doc);
}

TEST(ConfJson, size3) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
  const char* data = " {\"data\" : [{\"name\":\"tom\"},{\"name\":\"jack\"}]  } ";

  conf_doc_t* doc = conf_doc_load_json(data, -1);

  ASSERT_EQ(conf_doc_get(doc, "data.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  conf_doc_destroy(doc);
}

TEST(ConfJson, size4) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
  const char* data = " {\"data\" : [{\"name\":\"tom\"},{\"name\":\"jack\"}]  } ";

  conf_doc_t* doc = conf_doc_load_json(data, -1);

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  conf_doc_destroy(doc);
}

TEST(ConfJson, escape) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
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
  object_t* conf = conf_json_load("file://./tests/testdata/test.json", TRUE);

  ASSERT_EQ(conf_obj_save(conf), RET_OK);
  ASSERT_EQ(object_set_prop_str(conf, "tom.name", "tom"), RET_OK);
  ASSERT_EQ(object_set_prop_int(conf, "tom.age", 100), RET_OK);
  ASSERT_EQ(object_set_prop_float(conf, "tom.weight", 60.5), RET_OK);

  ASSERT_STREQ(object_get_prop_str(conf, "tom.name"), "tom");
  ASSERT_EQ(object_get_prop_int(conf, "tom.age", 0), 100);
  ASSERT_EQ(object_get_prop_float(conf, "tom.weight", 0), 60.5);

  ASSERT_EQ(conf_obj_save(conf), RET_OK);

  ASSERT_STREQ(object_get_prop_str(conf, "group.key"), "value");
  ASSERT_EQ(object_remove_prop(conf, "group.key"), RET_OK);
  ASSERT_EQ(object_get_prop_str(conf, "group.key"), (char*)NULL);

  OBJECT_UNREF(conf);
}

TEST(Json, exec) {
  value_t v;
  object_t* conf = conf_json_load("file://./tests/testdata/test.json", TRUE);

  ASSERT_EQ(object_can_exec(conf, OBJECT_CMD_MOVE_UP, "group.key"), FALSE);
  ASSERT_EQ(object_can_exec(conf, OBJECT_CMD_MOVE_DOWN, "group.key"), TRUE);
  ASSERT_EQ(object_can_exec(conf, OBJECT_CMD_CLEAR, "group"), TRUE);
  ASSERT_EQ(object_can_exec(conf, OBJECT_CMD_REMOVE, "group.key"), TRUE);
  ASSERT_EQ(object_can_exec(conf, OBJECT_CMD_SAVE, NULL), TRUE);
  ASSERT_EQ(object_can_exec(conf, OBJECT_CMD_RELOAD, NULL), TRUE);

  ASSERT_EQ(object_exec(conf, OBJECT_CMD_MOVE_DOWN, "group.key"), RET_OK);
  ASSERT_EQ(object_get_prop_int(conf, "group.key.#index", 0), 1);

  ASSERT_EQ(object_exec(conf, OBJECT_CMD_MOVE_UP, "group.key"), RET_OK);
  ASSERT_EQ(object_get_prop_int(conf, "group.key.#index", 0), 0);

  ASSERT_STREQ(object_get_prop_str(conf, "group.key"), "value");
  ASSERT_EQ(object_exec(conf, OBJECT_CMD_REMOVE, "group.key"), RET_OK);
  ASSERT_EQ(object_get_prop_str(conf, "group.key"), (char*)NULL);

  ASSERT_EQ(object_exec(conf, OBJECT_CMD_CLEAR, "group"), RET_OK);
  ASSERT_EQ(object_get_prop_int(conf, "group.#size", -1), 0);

  ASSERT_EQ(object_exec(conf, OBJECT_CMD_RELOAD, NULL), RET_OK);
  ASSERT_EQ(object_exec(conf, OBJECT_CMD_SAVE, NULL), RET_OK);

  OBJECT_UNREF(conf);
}
