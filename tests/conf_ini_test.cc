#include "gtest/gtest.h"
#include "conf_io/conf_ini.h"

TEST(Ini, group1) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_ini("[hello]");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  node = conf_node_find_child(doc->root, "hello");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "hello");

  str_init(&str, 100);
  conf_doc_save_ini(doc, &str);
  ASSERT_STREQ(str.str, "[hello]\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Ini, group2) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_ini("[hello]\n[ world ]");

  node = conf_node_find_child(doc->root, "hello");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "hello");

  node = conf_node_find_child(doc->root, "world");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "world");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  str_init(&str, 100);
  conf_doc_save_ini(doc, &str);
  ASSERT_STREQ(str.str, "[hello]\n[world]\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Ini, long_name) {
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_ini(
      "[helloxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx]\n[ "
      "worldxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx ]");

  node = conf_node_find_child(doc->root, "helloxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "helloxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

  node = conf_node_find_child(doc->root, "helloxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "helloxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

  conf_doc_destroy(doc);
}

TEST(Ini, key1) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
  conf_node_t* group = NULL;
  conf_doc_t* doc = conf_doc_load_ini("[hello]\nkey1=value1   \r\nkey2=value2");

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

  str_init(&str, 100);
  conf_doc_save_ini(doc, &str);
  ASSERT_STREQ(str.str, "[hello]\n  key1 = value1\n  key2 = value2\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Ini, comment) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
  conf_node_t* group = NULL;
  conf_doc_t* doc = conf_doc_load_ini("[hello]#comment\nkey1=value1#comment   \r\nkey2=\\#ff00ff");

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
  ASSERT_STREQ(value_str(&v), "#ff00ff");

  str_init(&str, 100);
  conf_doc_save_ini(doc, &str);
  ASSERT_STREQ(str.str, "[hello]\n  key1 = value1\n  key2 = \\#ff00ff\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Ini, long_key_value) {
  value_t v;
  conf_node_t* node = NULL;
  conf_node_t* group = NULL;
  conf_doc_t* doc = conf_doc_load_ini(
      "[hello]\n\tkey111111111111111111111111 = value111111111111111111111111   "
      "\r\nkey2222222222222222222=value222222222222");

  group = conf_node_find_child(doc->root, "hello");
  ASSERT_EQ(group != NULL, true);
  ASSERT_STREQ(conf_node_get_name(group), "hello");

  node = conf_node_find_child(group, "key111111111111111111111111");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "key111111111111111111111111");
  ASSERT_EQ(conf_node_get_value(node, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "value111111111111111111111111");

  node = conf_node_find_child(group, "key2222222222222222222");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "key2222222222222222222");
  ASSERT_EQ(conf_node_get_value(node, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "value222222222222");

  conf_doc_destroy(doc);
}

TEST(Ini, nogroup) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_ini("\nkey1=value1   \r\nkey2=value2");

  node = conf_node_find_child(doc->root, "key1");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "key1");
  ASSERT_EQ(conf_node_get_value(node, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "value1");

  node = conf_node_find_child(doc->root, "key2");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "key2");
  ASSERT_EQ(conf_node_get_value(node, &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "value2");

  str_init(&str, 100);
  conf_doc_save_ini(doc, &str);
  ASSERT_STREQ(str.str, "  key1 = value1\n  key2 = value2\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Ini, get_set_remove_exist) {
  value_t v;
  conf_doc_t* doc =
      conf_doc_load_ini("[hello]\nkey1=value1   \r\nkey2=value2\n[world]\nkey1=xxx\nkey2=yyyy\n");
  ASSERT_EQ(conf_doc_get(doc, "hello.key1", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "value1");

  ASSERT_EQ(conf_doc_get(doc, "hello.key2", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "value2");

  ASSERT_EQ(conf_doc_get(doc, "world.key1", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "xxx");

  ASSERT_EQ(conf_doc_get(doc, "world.key2", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "yyyy");

  ASSERT_EQ(conf_doc_get(doc, "world.key3", &v), RET_NOT_FOUND);
  value_set_str(&v, "foobar");
  ASSERT_EQ(conf_doc_set(doc, "world.key3", &v), RET_OK);
  ASSERT_EQ(conf_doc_get(doc, "world.key3", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "foobar");

  ASSERT_EQ(conf_doc_set(doc, "foobar.key4", &v), RET_OK);
  ASSERT_EQ(conf_doc_get(doc, "foobar.key4", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "foobar");

  ASSERT_EQ(conf_doc_exists(doc, "foobar.key4"), TRUE);
  ASSERT_EQ(conf_doc_remove(doc, "foobar.key4"), RET_OK);
  ASSERT_EQ(conf_doc_exists(doc, "foobar.key4"), FALSE);

  ASSERT_EQ(conf_doc_exists(doc, "world.key3"), TRUE);
  ASSERT_EQ(conf_doc_remove(doc, "world"), RET_OK);
  ASSERT_EQ(conf_doc_exists(doc, "world"), FALSE);
  ASSERT_EQ(conf_doc_exists(doc, "world.key3"), FALSE);

  conf_doc_destroy(doc);
}

TEST(Ini, get_set_remove_exist_by_index) {
  value_t v;
  conf_doc_t* doc =
      conf_doc_load_ini("[hello]\nkey1=value1   \r\nkey2=value2\n[world]\nkey1=xxx\nkey2=yyyy\n");

  ASSERT_EQ(conf_doc_get(doc, "hello.[0]", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "value1");

  value_set_str(&v, "test");
  ASSERT_EQ(conf_doc_set(doc, "hello.[0]", &v), RET_OK);
  ASSERT_EQ(conf_doc_get(doc, "hello.[0]", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "test");
  ASSERT_NE(conf_doc_set(doc, "hello.[4]", &v), RET_OK);

  ASSERT_EQ(conf_doc_exists(doc, "hello.[0]"), TRUE);
  ASSERT_EQ(conf_doc_remove(doc, "hello.[0]"), RET_OK);
  ASSERT_EQ(conf_doc_remove(doc, "hello.[0]"), RET_OK);
  ASSERT_EQ(conf_doc_exists(doc, "hello.[0]"), FALSE);

  conf_doc_destroy(doc);
}

TEST(Ini, deep_level) {
  value_t v;
  conf_doc_t* doc =
      conf_doc_load_ini("[hello]\nkey1=value1   \r\nkey2=value2\n[world]\nkey1=xxx\nkey2=yyyy\n");

  value_set_str(&v, "test");
  ASSERT_EQ(conf_doc_set(doc, "hello.normal.color", &v), RET_OK);
  ASSERT_EQ(conf_doc_get(doc, "hello.normal.color", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "test");

  conf_doc_destroy(doc);
}

TEST(Ini, file) {
  object_t* conf = conf_ini_load("file://./tests/testdata/test.ini", TRUE);

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

TEST(Ini, index) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_ini("[hello]\n[ world ]\n[awtk]\nname=aaa\n");

  ASSERT_EQ(conf_doc_get(doc, "hello.#index", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  ASSERT_EQ(conf_doc_get(doc, "world.#index", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(conf_doc_get(doc, "awtk.#index", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  conf_doc_destroy(doc);
}

TEST(Ini, last_first) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_ini("[hello]\n[ world ]\n[awtk]\nname=aaa\n");

  ASSERT_EQ(conf_doc_is_last(doc, "hello"), FALSE);
  ASSERT_EQ(conf_doc_is_last(doc, "world"), FALSE);
  ASSERT_EQ(conf_doc_is_last(doc, "awtk"), TRUE);

  ASSERT_EQ(conf_doc_is_first(doc, "hello"), TRUE);
  ASSERT_EQ(conf_doc_is_first(doc, "world"), FALSE);
  ASSERT_EQ(conf_doc_is_first(doc, "awtk"), FALSE);

  conf_doc_destroy(doc);
}

TEST(Ini, move_up) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_ini("[hello]\n[ world ]\n[awtk]\nname=aaa\n");

  ASSERT_EQ(conf_doc_move_up(doc, "hello"), RET_FAIL);
  ASSERT_EQ(conf_doc_is_first(doc, "hello"), TRUE);
  ASSERT_EQ(conf_doc_get(doc, "hello.#index", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  ASSERT_EQ(conf_doc_move_down(doc, "hello"), RET_OK);
  ASSERT_EQ(conf_doc_is_first(doc, "hello"), FALSE);
  ASSERT_EQ(conf_doc_is_first(doc, "world"), TRUE);
  ASSERT_EQ(conf_doc_get(doc, "hello.#index", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(conf_doc_move_down(doc, "hello"), RET_OK);
  ASSERT_EQ(conf_doc_is_last(doc, "hello"), TRUE);
  ASSERT_EQ(conf_doc_get(doc, "hello.#index", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  ASSERT_EQ(conf_doc_move_down(doc, "hello"), RET_FAIL);

  ASSERT_EQ(conf_doc_move_up(doc, "hello"), RET_OK);
  ASSERT_EQ(conf_doc_is_last(doc, "hello"), FALSE);
  ASSERT_EQ(conf_doc_get(doc, "hello.#index", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(conf_doc_move_up(doc, "hello"), RET_OK);
  ASSERT_EQ(conf_doc_is_last(doc, "hello"), FALSE);
  ASSERT_EQ(conf_doc_get(doc, "hello.#index", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  ASSERT_EQ(conf_doc_move_up(doc, "hello"), RET_FAIL);

  conf_doc_destroy(doc);
}

TEST(Ini, clear) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_ini("[hello]\n name=aaa\n");

  ASSERT_EQ(conf_doc_get(doc, "hello.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);
  ASSERT_EQ(conf_doc_clear(doc, "hello"), RET_OK);
  ASSERT_EQ(conf_doc_get(doc, "hello.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  conf_doc_destroy(doc);
}

TEST(Ini, readonly) {
  object_t* conf = conf_ini_load("file://./tests/testdata/test.ini", TRUE);

  ASSERT_EQ(object_set_prop_str(conf, "tom.name", "tom"), RET_OK);
  ASSERT_EQ(object_remove_prop(conf, "tom.name"), RET_OK);

  ASSERT_EQ(conf_obj_set_readonly(conf, TRUE), RET_OK);
  ASSERT_NE(object_set_prop_str(conf, "tom.name", "tom"), RET_OK);
  ASSERT_NE(object_remove_prop(conf, "tom.name"), RET_OK);

  ASSERT_EQ(conf_obj_set_readonly(conf, FALSE), RET_OK);
  ASSERT_EQ(object_set_prop_str(conf, "tom.name", "tom"), RET_OK);
  ASSERT_EQ(object_remove_prop(conf, "tom.name"), RET_OK);

  OBJECT_UNREF(conf);
}
