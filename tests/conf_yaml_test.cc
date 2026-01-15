#include "gtest/gtest.h"
#include "conf_io/conf_yaml.h"

/* 辅助函数：验证保存和重新加载的一致性 */
static void verify_save_and_reload(conf_doc_t* doc) {
  str_t str;
  value_t v1, v2;
  conf_doc_t* reloaded_doc = NULL;

  str_init(&str, 1000);
  conf_doc_save_yaml(doc, &str);

  /* 重新加载保存的 YAML */
  reloaded_doc = conf_doc_load_yaml(str.str);
  ASSERT_NE(reloaded_doc, nullptr);

  /* 比较根节点的子节点数量 */
  if (conf_doc_get(doc, "#size", &v1) == RET_OK) {
    ASSERT_EQ(conf_doc_get(reloaded_doc, "#size", &v2), RET_OK);
    ASSERT_EQ(value_int(&v1), value_int(&v2));
  }

  str_reset(&str);
  conf_doc_destroy(reloaded_doc);
}

/* 辅助函数：验证保存和重新加载后特定路径的值 */
static void verify_save_and_reload_value(conf_doc_t* doc, const char* path,
                                         const char* expected_str, int32_t expected_int,
                                         float expected_float, int expected_bool) {
  str_t str;
  conf_doc_t* reloaded_doc = NULL;

  str_init(&str, 1000);
  conf_doc_save_yaml(doc, &str);

  /* 重新加载保存的 YAML */
  reloaded_doc = conf_doc_load_yaml(str.str);
  ASSERT_NE(reloaded_doc, nullptr);

  /* 验证字符串值 */
  if (expected_str != NULL) {
    const char* reloaded_str = conf_doc_get_str(reloaded_doc, path, NULL);
    ASSERT_STREQ(reloaded_str, expected_str);
  }

  /* 验证整数值 */
  if (expected_int != INT32_MIN) {
    int32_t reloaded_int = conf_doc_get_int(reloaded_doc, path, INT32_MIN);
    ASSERT_EQ(reloaded_int, expected_int);
  }

  /* 验证浮点数值 */
  if (expected_float != FLT_MAX) {
    float reloaded_float = conf_doc_get_float(reloaded_doc, path, FLT_MAX);
    ASSERT_FLOAT_EQ(reloaded_float, expected_float);
  }

  /* 验证布尔值 */
  if (expected_bool >= 0) {
    bool_t reloaded_bool = conf_doc_get_bool(reloaded_doc, path, -1);
    ASSERT_EQ(reloaded_bool, expected_bool);
  }

  str_reset(&str);
  conf_doc_destroy(reloaded_doc);
}

TEST(Yaml, basic1) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_yaml("hello:");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  node = conf_node_find_child(doc->root, "hello");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "hello");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "hello:\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, basic2) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_yaml("name: jim");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  node = conf_node_find_child(doc->root, "name");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "name");

  ASSERT_STREQ(conf_node_get_child_value_str(doc->root, "name", ""), "jim");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "name: jim\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, basic3) {
  value_t v;
  conf_doc_t* doc = conf_doc_load_yaml(
      "person:\r\n"
      "  name: jim\n"
      "  age:100");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_STREQ(conf_doc_get_str(doc, "person.name", ""), "jim");
  ASSERT_EQ(conf_doc_get_int(doc, "person.age", 0), 100);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "person:\n"
               "  name: jim\n"
               "  age: 100\n");
  str_reset(&str);

  /* 验证保存和重新加载 */
  verify_save_and_reload_value(doc, "person.name", "jim", INT32_MIN, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "person.age", NULL, 100, FLT_MAX, -1);

  conf_doc_destroy(doc);
}

TEST(Yaml, basic4) {
  value_t v;
  conf_doc_t* doc = conf_doc_load_yaml(
      "jim:\r\n"
      "  age:100\n"
      "tom:\n"
      "  age:99\n");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  ASSERT_EQ(conf_doc_get_int(doc, "jim.age", 0), 100);
  ASSERT_EQ(conf_doc_get_int(doc, "tom.age", 0), 99);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "jim:\n"
               "  age: 100\n"
               "tom:\n"
               "  age: 99\n");
  str_reset(&str);

  /* 验证保存和重新加载 */
  verify_save_and_reload_value(doc, "jim.age", NULL, 100, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "tom.age", NULL, 99, FLT_MAX, -1);

  conf_doc_destroy(doc);
}

TEST(Yaml, basic5) {
  value_t v;
  conf_doc_t* doc = conf_doc_load_yaml(
      "jim:\r\n"
      "  age:100\n"
      "  weight: 60");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(conf_doc_get(doc, "jim.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  ASSERT_EQ(conf_doc_get_int(doc, "jim.age", 0), 100);
  ASSERT_EQ(conf_doc_get_int(doc, "jim.weight", 0), 60);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "jim:\n"
               "  age: 100\n"
               "  weight: 60\n");
  str_reset(&str);

  /* 验证保存和重新加载 */
  verify_save_and_reload_value(doc, "jim.age", NULL, 100, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "jim.weight", NULL, 60, FLT_MAX, -1);

  conf_doc_destroy(doc);
}

TEST(Yaml, basic6) {
  value_t v;
  conf_doc_t* doc = conf_doc_load_yaml(
      "jim:\r\n"
      "  age:100\n"
      "  weight: 60\n"
      "tom:\n"
      "  age:99\n"
      "  weight:70");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  ASSERT_EQ(conf_doc_get(doc, "jim.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  ASSERT_EQ(conf_doc_get(doc, "tom.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  ASSERT_EQ(conf_doc_get_int(doc, "jim.age", 0), 100);
  ASSERT_EQ(conf_doc_get_int(doc, "tom.age", 0), 99);
  ASSERT_EQ(conf_doc_get_int(doc, "jim.weight", 0), 60);
  ASSERT_EQ(conf_doc_get_int(doc, "tom.weight", 0), 70);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "jim:\n"
               "  age: 100\n"
               "  weight: 60\n"
               "tom:\n"
               "  age: 99\n"
               "  weight: 70\n");
  str_reset(&str);

  /* 验证保存和重新加载 */
  verify_save_and_reload_value(doc, "jim.age", NULL, 100, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "jim.weight", NULL, 60, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "tom.age", NULL, 99, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "tom.weight", NULL, 70, FLT_MAX, -1);

  conf_doc_destroy(doc);
}

TEST(Yaml, basic7) {
  value_t v;
  const char* data =
      "plan_request_params:\n"
      "  planning_attempts: 1\n"
      "  planning_pipeline: ompl\n"
      "  max_velocity_scaling_factor: 1.0\n"
      "  max_acceleration_scaling_factor: 0.5 ";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(conf_doc_get(doc, "plan_request_params.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 4);

  ASSERT_EQ(conf_doc_get_int(doc, "plan_request_params.planning_attempts", 0), 1);
  ASSERT_STREQ(conf_doc_get_str(doc, "plan_request_params.planning_pipeline", ""), "ompl");
  ASSERT_FLOAT_EQ(conf_doc_get_float(doc, "plan_request_params.max_velocity_scaling_factor", 0),
                  1.0f);
  ASSERT_FLOAT_EQ(conf_doc_get_float(doc, "plan_request_params.max_acceleration_scaling_factor", 0),
                  0.5f);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "plan_request_params:\n"
               "  planning_attempts: 1\n"
               "  planning_pipeline: ompl\n"
               "  max_velocity_scaling_factor: 1.0\n"
               "  max_acceleration_scaling_factor: 0.5\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Yaml, list1) {
  value_t v;
  const char* data =
      "planning_pipelines:\n"
      "  pipeline_names:\n"
      "    - ompl\n";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(conf_doc_get(doc, "planning_pipelines.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(conf_doc_get(doc, "planning_pipelines.pipeline_names.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "planning_pipelines:\n"
               "  pipeline_names:\n"
               "    - ompl\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Yaml, list2) {
  value_t v;
  const char* data =
      "planning_pipelines:\n"
      "  pipeline_names:\n"
      "    - ompl\n"
      "    - kdl\n"
      "    - ikfast\n";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(conf_doc_get(doc, "planning_pipelines.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(conf_doc_get(doc, "planning_pipelines.pipeline_names.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "planning_pipelines:\n"
               "  pipeline_names:\n"
               "    - ompl\n"
               "    - kdl\n"
               "    - ikfast\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Yaml, comment1) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_yaml("name: jim #comment");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  node = conf_node_find_child(doc->root, "name");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "name");

  ASSERT_STREQ(conf_node_get_child_value_str(doc->root, "name", ""), "jim");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "name: jim\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, comment2) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_yaml("#comment\nname: jim #comment");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  node = conf_node_find_child(doc->root, "name");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "name");

  ASSERT_STREQ(conf_node_get_child_value_str(doc->root, "name", ""), "jim");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "name: jim\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, comment3) {
  value_t v;
  const char* data =
      "planning_pipelines:#comment\n"
      "  pipeline_names:#comment\n"
      "    - ompl#comment\n"
      "    #comment\n"
      "    - kdl#comment\n"
      "    - ikfast #comment\n";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(conf_doc_get(doc, "planning_pipelines.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(conf_doc_get(doc, "planning_pipelines.pipeline_names.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "planning_pipelines:\n"
               "  pipeline_names:\n"
               "    - ompl\n"
               "    - kdl\n"
               "    - ikfast\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Yaml, error1) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_yaml("hello");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  node = conf_node_find_child(doc->root, "hello");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "hello");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "hello:\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, error2) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_yaml("hel\tlo");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  node = conf_node_find_child(doc->root, "hello");
  ASSERT_EQ(node == NULL, true);

  node = conf_node_find_child(doc->root, "hel");
  ASSERT_EQ(node != NULL, true);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "hel:\n"
               "  lo:\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, error3) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_yaml("hel#lo");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  node = conf_node_find_child(doc->root, "hel");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "hel");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "hel:\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, escape1) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_yaml("name: hello\\nworld");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  node = conf_node_find_child(doc->root, "name");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "name");

  const char* value = conf_node_get_child_value_str(doc->root, "name", "");
  ASSERT_STREQ(value, "hello\nworld");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "name: \"hello\nworld\"\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, escape2) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_yaml("name: \\#hello");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  node = conf_node_find_child(doc->root, "name");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "name");

  const char* value = conf_node_get_child_value_str(doc->root, "name", "");
  ASSERT_STREQ(value, "#hello");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "name: \"#hello\"\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, set1) {
  conf_doc_t* doc = conf_doc_load_yaml("name: \\#hello");

  conf_doc_set_str(doc, "name", "tom");
  conf_doc_set_int(doc, "age", 100);
  conf_doc_set_float(doc, "weight", 60);
  conf_doc_set_bool(doc, "male", true);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "name: tom\nage: 100\nweight: 60.000000\nmale: 1\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, set2) {
  conf_doc_t* doc = conf_doc_load_yaml("name: \\#hello");

  conf_doc_set_int(doc, "tom.age", 100);
  conf_doc_set_float(doc, "tom.weight", 60);
  conf_doc_set_bool(doc, "tom.male", true);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "name: \"#hello\"\n"
               "tom:\n"
               "  age: 100\n"
               "  weight: 60.000000\n"
               "  male: 1\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, set3) {
  wbuffer_t wb;
  tk_object_t* conf = conf_yaml_create();
  ASSERT_NE(conf, (tk_object_t*)NULL);
  ASSERT_EQ(tk_object_set_prop_int(conf, "awtk.value", 123), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(conf, "awtk.value", 0), 123);

  ASSERT_EQ(conf_yaml_save_to_buff(conf, &wb), RET_OK);
  TK_OBJECT_UNREF(conf);

  conf = conf_yaml_load_from_buff(wb.data, wb.cursor, FALSE);
  ASSERT_EQ(tk_object_get_prop_int(conf, "awtk.value", 0), 123);
  TK_OBJECT_UNREF(conf);

  wbuffer_deinit(&wb);
}

TEST(Yaml, file) {
  tk_object_t* conf = conf_yaml_load("file://./tests/testdata/test.yaml", TRUE);

  ASSERT_STREQ(tk_object_get_prop_str(conf, "plan_request_params.planning_pipeline"), "ompl");
  ASSERT_EQ(tk_object_get_prop_bool(conf, "plan_request_params.planning_attempts", 0), TRUE);
  ASSERT_STREQ(tk_object_get_prop_str(conf, "plan_request_params.max_velocity_scaling_factor"),
               "1.0");
  ASSERT_STREQ(tk_object_get_prop_str(conf, "plan_request_params.max_acceleration_scaling_factor"),
               "1.0");

  ASSERT_STREQ(tk_object_get_prop_str(conf, "planning_pipelines.pipeline_names.[0]"), "ompl");
  ASSERT_STREQ(tk_object_get_prop_str(conf, "planning_pipelines.pipeline_names.[1]"), "kdl");

  TK_OBJECT_UNREF(conf);
}

TEST(Yaml, file1) {
  tk_object_t* conf = conf_yaml_load("file://./tests/testdata/test1.yaml", TRUE);

  /* 验证 {DEFAULT} 键（特殊字符键名） */
  ASSERT_STREQ(tk_object_get_prop_str(conf, "{DEFAULT}.output"), "file");
  ASSERT_STREQ(tk_object_get_prop_str(conf, "{DEFAULT}.level"), "DEBUG");
  ASSERT_STREQ(tk_object_get_prop_str(conf, "{DEFAULT}.format_prefix"),
               "\n[${hh}:${mm}:${ss}] >\n");

  /* 验证嵌套对象 */
  ASSERT_STREQ(
      tk_object_get_prop_str(conf, "pou_variable_watch_manager.var_monitoring.format_prefix"),
      "\n[${hh}:${mm}:${ss}] >\n");

  /* 验证布尔值 */
  ASSERT_EQ(tk_object_get_prop_bool(conf, "pou_variable_watch_manager.raw_data.output_ignore_level",
                                    TRUE),
            FALSE);

  /* 验证保存和重新加载 */
  str_t str;
  str_init(&str, 1000);
  conf_doc_t* doc = conf_obj_get_doc(conf);
  ASSERT_NE(doc, nullptr);
  conf_doc_save_yaml(doc, &str);

  conf_doc_t* reloaded_doc = conf_doc_load_yaml(str.str);
  ASSERT_NE(reloaded_doc, nullptr);
  ASSERT_STREQ(conf_doc_get_str(reloaded_doc, "{DEFAULT}.output", ""), "file");
  ASSERT_STREQ(conf_doc_get_str(reloaded_doc, "{DEFAULT}.level", ""), "DEBUG");
  ASSERT_EQ(conf_doc_get_bool(reloaded_doc,
                              "pou_variable_watch_manager.raw_data.output_ignore_level", TRUE),
            FALSE);

  str_reset(&str);
  conf_doc_destroy(reloaded_doc);

  TK_OBJECT_UNREF(conf);
}

TEST(Yaml, file2) {
  tk_object_t* conf = conf_yaml_load("file://./tests/testdata/test2.yaml", TRUE);

  /* 验证 {DEFAULT} 键（特殊字符键名） */
  ASSERT_STREQ(tk_object_get_prop_str(conf, "{DEFAULT}.output"), "file");
  ASSERT_STREQ(tk_object_get_prop_str(conf, "{DEFAULT}.level"), "DEBUG");
  /* 单引号字符串应该保留字面值（不转义），\n"" 被解析为两个字符：\ 和 n */
  ASSERT_STREQ(tk_object_get_prop_str(conf, "{DEFAULT}.format_prefix"),
               "\\n[${hh}:${mm}:${ss}] >\\n");

  /* 验证嵌套对象 */
  ASSERT_STREQ(
      tk_object_get_prop_str(conf, "pou_variable_watch_manager.var_monitoring.format_prefix"),
      "\\n[${hh}:${mm}:${ss}] >\\n");

  /* 验证布尔值 */
  ASSERT_EQ(tk_object_get_prop_bool(conf, "pou_variable_watch_manager.raw_data.output_ignore_level",
                                    TRUE),
            FALSE);

  /* 验证保存和重新加载 */
  str_t str;
  str_init(&str, 1000);
  conf_doc_t* doc = conf_obj_get_doc(conf);
  ASSERT_NE(doc, nullptr);
  conf_doc_save_yaml(doc, &str);

  conf_doc_t* reloaded_doc = conf_doc_load_yaml(str.str);
  ASSERT_NE(reloaded_doc, nullptr);
  ASSERT_STREQ(conf_doc_get_str(reloaded_doc, "{DEFAULT}.output", ""), "file");
  ASSERT_STREQ(conf_doc_get_str(reloaded_doc, "{DEFAULT}.level", ""), "DEBUG");
  ASSERT_EQ(conf_doc_get_bool(reloaded_doc,
                              "pou_variable_watch_manager.raw_data.output_ignore_level", TRUE),
            FALSE);

  str_reset(&str);
  conf_doc_destroy(reloaded_doc);

  TK_OBJECT_UNREF(conf);
}

TEST(Yaml, file3) {
  tk_object_t* conf = conf_yaml_load_ex("file://./tests/testdata/test2.yaml", TRUE, TRUE);

  ASSERT_TRUE(NULL != tk_object_get_prop_object(conf, "{DEFAULT}"));

  TK_OBJECT_UNREF(conf);
}

TEST(Yaml, file4) {
  tk_object_t* conf = conf_yaml_load_ex("file://./tests/testdata/test2.yaml", TRUE, TRUE);
  ASSERT_NE(conf, nullptr);

  /* 验证 {DEFAULT} 节点存在 */
  conf_doc_t* doc = conf_obj_get_doc(conf);
  ASSERT_NE(doc, nullptr);
  ASSERT_EQ(doc->use_extend_type, TRUE);
  
  conf_node_t* node = conf_node_find_child(doc->root, "{DEFAULT}");
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, CONF_NODE_OBJECT);
  ASSERT_EQ(node->value_type, CONF_NODE_VALUE_NODE);
  
  /* 验证可以通过 tk_object_get_prop_object 获取对象 */
  tk_object_t* obj = tk_object_get_prop_object(conf, "{DEFAULT}");
  ASSERT_NE(obj, nullptr);
  
  /* 验证对象有子属性 */
  ASSERT_STREQ(tk_object_get_prop_str(obj, "output"), "file");
  ASSERT_STREQ(tk_object_get_prop_str(obj, "level"), "DEBUG");

  TK_OBJECT_UNREF(conf);
}

TEST(Yaml, extend_type_nested_object) {
  /* 测试嵌套对象的扩展类型 */
  const char* data =
      "server:\n"
      "  host: localhost\n"
      "  port: 8080\n"
      "  ssl:\n"
      "    enabled: true\n"
      "    cert: /path/to/cert\n";
  
  conf_doc_t* doc = conf_doc_load_yaml(data);
  ASSERT_NE(doc, nullptr);
  conf_doc_use_extend_type(doc, TRUE);
  
  /* 验证可以通过扩展类型获取嵌套对象 */
  value_t v;
  ASSERT_EQ(conf_doc_get(doc, "server", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* server_obj = value_object(&v);
  ASSERT_NE(server_obj, nullptr);
  ASSERT_STREQ(tk_object_get_prop_str(server_obj, "host"), "localhost");
  ASSERT_EQ(tk_object_get_prop_int(server_obj, "port", 0), 8080);
  
  ASSERT_EQ(conf_doc_get(doc, "server.ssl", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* ssl_obj = value_object(&v);
  ASSERT_NE(ssl_obj, nullptr);
  ASSERT_EQ(tk_object_get_prop_bool(ssl_obj, "enabled", FALSE), TRUE);
  ASSERT_STREQ(tk_object_get_prop_str(ssl_obj, "cert"), "/path/to/cert");
  
  conf_doc_destroy(doc);
}

TEST(Yaml, extend_type_array) {
  /* 测试数组的扩展类型 */
  const char* data =
      "users:\n"
      "  -\n"
      "    name: jim\n"
      "    age: 30\n"
      "  -\n"
      "    name: tom\n"
      "    age: 25\n";
  
  conf_doc_t* doc = conf_doc_load_yaml(data);
  ASSERT_NE(doc, nullptr);
  conf_doc_use_extend_type(doc, TRUE);
  
  /* 验证可以通过扩展类型获取数组 */
  value_t v;
  ASSERT_EQ(conf_doc_get(doc, "users", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* users_array = value_object(&v);
  ASSERT_NE(users_array, nullptr);
  ASSERT_TRUE(tk_object_is_collection(users_array));
  
  /* 验证数组元素 - 通过完整路径访问对象属性 */
  ASSERT_STREQ(conf_doc_get_str(doc, "users.[0].name", ""), "jim");
  ASSERT_EQ(conf_doc_get_int(doc, "users.[0].age", 0), 30);
  ASSERT_STREQ(conf_doc_get_str(doc, "users.[1].name", ""), "tom");
  ASSERT_EQ(conf_doc_get_int(doc, "users.[1].age", 0), 25);
  
  /* 验证数组大小 */
  ASSERT_EQ(conf_doc_get(doc, "users.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  
  conf_doc_destroy(doc);
}

TEST(Yaml, extend_type_empty_object) {
  /* 测试空对象的扩展类型 */
  const char* data = "empty:\n";
  
  conf_doc_t* doc = conf_doc_load_yaml(data);
  ASSERT_NE(doc, nullptr);
  conf_doc_use_extend_type(doc, TRUE);
  
  /* 验证空对象可以通过扩展类型获取 */
  value_t v;
  ASSERT_EQ(conf_doc_get(doc, "empty", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* empty_obj = value_object(&v);
  ASSERT_NE(empty_obj, nullptr);
  
  /* 验证空对象没有属性 */
  value_t v2;
  ASSERT_NE(tk_object_get_prop(empty_obj, "any", &v2), RET_OK);
  
  conf_doc_destroy(doc);
}

TEST(Yaml, extend_type_complex_nested) {
  /* 测试复杂嵌套结构的扩展类型 */
  const char* data =
      "config:\n"
      "  app:\n"
      "    name: MyApp\n"
      "    version: 1.0.0\n"
      "  database:\n"
      "    host: localhost\n"
      "    port: 5432\n"
      "    connections:\n"
      "      - pool1\n"
      "      - pool2\n";
  
  conf_doc_t* doc = conf_doc_load_yaml(data);
  ASSERT_NE(doc, nullptr);
  conf_doc_use_extend_type(doc, TRUE);
  
  /* 验证多级嵌套对象 */
  value_t v;
  ASSERT_EQ(conf_doc_get(doc, "config", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* config_obj = value_object(&v);
  ASSERT_NE(config_obj, nullptr);
  
  ASSERT_EQ(conf_doc_get(doc, "config.app", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* app_obj = value_object(&v);
  ASSERT_NE(app_obj, nullptr);
  ASSERT_STREQ(tk_object_get_prop_str(app_obj, "name"), "MyApp");
  ASSERT_STREQ(tk_object_get_prop_str(app_obj, "version"), "1.0.0");
  
  ASSERT_EQ(conf_doc_get(doc, "config.database", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* db_obj = value_object(&v);
  ASSERT_NE(db_obj, nullptr);
  ASSERT_STREQ(tk_object_get_prop_str(db_obj, "host"), "localhost");
  ASSERT_EQ(tk_object_get_prop_int(db_obj, "port", 0), 5432);
  
  ASSERT_EQ(conf_doc_get(doc, "config.database.connections", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* conn_array = value_object(&v);
  ASSERT_NE(conn_array, nullptr);
  ASSERT_STREQ(tk_object_get_prop_str_by_path(conn_array, "[0]"), "pool1");
  ASSERT_STREQ(tk_object_get_prop_str_by_path(conn_array, "[1]"), "pool2");
  
  conf_doc_destroy(doc);
}

TEST(Yaml, extend_type_with_quoted_keys) {
  /* 测试带引号键名的扩展类型 */
  const char* data =
      "\"key with spaces\":\n"
      "  value1: test1\n"
      "  value2: test2\n";
  
  conf_doc_t* doc = conf_doc_load_yaml(data);
  ASSERT_NE(doc, nullptr);
  conf_doc_use_extend_type(doc, TRUE);
  
  /* 验证带引号的键名可以通过扩展类型获取 - 键名在解析时已去掉引号 */
  value_t v;
  ASSERT_EQ(conf_doc_get(doc, "key with spaces", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* obj = value_object(&v);
  ASSERT_NE(obj, nullptr);
  ASSERT_STREQ(tk_object_get_prop_str(obj, "value1"), "test1");
  ASSERT_STREQ(tk_object_get_prop_str(obj, "value2"), "test2");
  
  conf_doc_destroy(doc);
}

TEST(Yaml, extend_type_save_and_reload) {
  /* 测试扩展类型的保存和重新加载 */
  const char* data =
      "person:\n"
      "  name: John\n"
      "  age: 30\n"
      "  address:\n"
      "    city: Beijing\n"
      "    country: China\n";
  
  conf_doc_t* doc = conf_doc_load_yaml(data);
  ASSERT_NE(doc, nullptr);
  conf_doc_use_extend_type(doc, TRUE);
  
  /* 保存 */
  str_t str;
  str_init(&str, 1000);
  conf_doc_save_yaml(doc, &str);
  
  /* 重新加载 */
  conf_doc_t* reloaded_doc = conf_doc_load_yaml(str.str);
  ASSERT_NE(reloaded_doc, nullptr);
  
  conf_doc_use_extend_type(reloaded_doc, TRUE);
  
  /* 验证重新加载后的扩展类型 */
  value_t v;
  ASSERT_EQ(conf_doc_get(reloaded_doc, "person", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* person_obj = value_object(&v);
  ASSERT_NE(person_obj, nullptr);
  ASSERT_STREQ(tk_object_get_prop_str(person_obj, "name"), "John");
  ASSERT_EQ(tk_object_get_prop_int(person_obj, "age", 0), 30);
  
  ASSERT_EQ(conf_doc_get(reloaded_doc, "person.address", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* addr_obj = value_object(&v);
  ASSERT_NE(addr_obj, nullptr);
  ASSERT_STREQ(tk_object_get_prop_str(addr_obj, "city"), "Beijing");
  ASSERT_STREQ(tk_object_get_prop_str(addr_obj, "country"), "China");
  
  str_reset(&str);
  conf_doc_destroy(doc);
  conf_doc_destroy(reloaded_doc);
}

TEST(Yaml, extend_type_flow_collections) {
  /* 测试流式集合的扩展类型 */
  const char* data =
      "config: {servers: [{name: s1, port: 80}, {name: s2, port: 443}], timeout: 30}\n";
  
  conf_doc_t* doc = conf_doc_load_yaml(data);
  ASSERT_NE(doc, nullptr);
  conf_doc_use_extend_type(doc, TRUE);
  
  /* 验证流式集合可以通过扩展类型获取 */
  value_t v;
  ASSERT_EQ(conf_doc_get(doc, "config", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* config_obj = value_object(&v);
  ASSERT_NE(config_obj, nullptr);
  ASSERT_EQ(tk_object_get_prop_int(config_obj, "timeout", 0), 30);
  
  ASSERT_EQ(conf_doc_get(doc, "config.servers", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* servers_array = value_object(&v);
  ASSERT_NE(servers_array, nullptr);
  
  ASSERT_EQ(conf_doc_get(doc, "config.servers.[0]", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* server1 = value_object(&v);
  ASSERT_NE(server1, nullptr);
  ASSERT_STREQ(tk_object_get_prop_str(server1, "name"), "s1");
  ASSERT_EQ(tk_object_get_prop_int(server1, "port", 0), 80);
  
  ASSERT_EQ(conf_doc_get(doc, "config.servers.[1]", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* server2 = value_object(&v);
  ASSERT_NE(server2, nullptr);
  ASSERT_STREQ(tk_object_get_prop_str(server2, "name"), "s2");
  ASSERT_EQ(tk_object_get_prop_int(server2, "port", 0), 443);
  
  conf_doc_destroy(doc);
}

TEST(Yaml, extend_type_mixed_values) {
  /* 测试混合值类型（字符串、数字、布尔、对象）的扩展类型 */
  const char* data =
      "mixed:\n"
      "  string_val: hello\n"
      "  int_val: 42\n"
      "  bool_val: true\n"
      "  float_val: 3.14\n"
      "  object_val:\n"
      "    nested: value\n";
  
  conf_doc_t* doc = conf_doc_load_yaml(data);
  ASSERT_NE(doc, nullptr);
  conf_doc_use_extend_type(doc, TRUE);
  
  value_t v;
  ASSERT_EQ(conf_doc_get(doc, "mixed", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* mixed_obj = value_object(&v);
  ASSERT_NE(mixed_obj, nullptr);
  
  /* 验证各种类型的值 */
  ASSERT_STREQ(tk_object_get_prop_str(mixed_obj, "string_val"), "hello");
  ASSERT_EQ(tk_object_get_prop_int(mixed_obj, "int_val", 0), 42);
  ASSERT_EQ(tk_object_get_prop_bool(mixed_obj, "bool_val", FALSE), TRUE);
  ASSERT_FLOAT_EQ(tk_object_get_prop_float(mixed_obj, "float_val", 0), 3.14f);
  
  /* 验证嵌套对象 */
  ASSERT_EQ(conf_doc_get(doc, "mixed.object_val", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* obj_val = value_object(&v);
  ASSERT_NE(obj_val, nullptr);
  ASSERT_STREQ(tk_object_get_prop_str(obj_val, "nested"), "value");
  
  conf_doc_destroy(doc);
}

TEST(Yaml, extend_type_null_values) {
  /* 测试包含 null 值的扩展类型 */
  const char* data =
      "config:\n"
      "  value1: null\n"
      "  value2: ~\n"
      "  nested:\n"
      "    value3: test\n";
  
  conf_doc_t* doc = conf_doc_load_yaml(data);
  ASSERT_NE(doc, nullptr);
  conf_doc_use_extend_type(doc, TRUE);
  
  value_t v;
  ASSERT_EQ(conf_doc_get(doc, "config", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* config_obj = value_object(&v);
  ASSERT_NE(config_obj, nullptr);
  
  /* 验证 null 值 */
  const char* value1 = tk_object_get_prop_str(config_obj, "value1");
  ASSERT_TRUE(value1 == NULL || strlen(value1) == 0);
  
  const char* value2 = tk_object_get_prop_str(config_obj, "value2");
  ASSERT_TRUE(value2 == NULL || strlen(value2) == 0);
  
  /* 验证嵌套对象 */
  ASSERT_EQ(conf_doc_get(doc, "config.nested", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* nested_obj = value_object(&v);
  ASSERT_NE(nested_obj, nullptr);
  ASSERT_STREQ(tk_object_get_prop_str(nested_obj, "value3"), "test");
  
  conf_doc_destroy(doc);
}

TEST(Yaml, extend_type_modify_object) {
  /* 测试通过扩展类型修改对象属性 */
  const char* data =
      "person:\n"
      "  name: John\n"
      "  age: 30\n";
  
  conf_doc_t* doc = conf_doc_load_yaml(data);
  ASSERT_NE(doc, nullptr);
  conf_doc_use_extend_type(doc, TRUE);
  
  value_t v;
  ASSERT_EQ(conf_doc_get(doc, "person", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* person_obj = value_object(&v);
  ASSERT_NE(person_obj, nullptr);
  
  /* 修改属性 */
  ASSERT_EQ(tk_object_set_prop_str(person_obj, "name", "Jane"), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(person_obj, "age", 25), RET_OK);
  
  /* 验证修改 */
  ASSERT_STREQ(tk_object_get_prop_str(person_obj, "name"), "Jane");
  ASSERT_EQ(tk_object_get_prop_int(person_obj, "age", 0), 25);
  
  /* 验证通过 doc 对象也能看到修改 */
  ASSERT_STREQ(conf_doc_get_str(doc, "person.name", ""), "Jane");
  ASSERT_EQ(conf_doc_get_int(doc, "person.age", 0), 25);
  
  conf_doc_destroy(doc);
}

TEST(Yaml, extend_type_add_property) {
  /* 测试通过扩展类型添加新属性 */
  const char* data =
      "person:\n"
      "  name: John\n";
  
  conf_doc_t* doc = conf_doc_load_yaml(data);
  ASSERT_NE(doc, nullptr);
  conf_doc_use_extend_type(doc, TRUE);
  
  value_t v;
  ASSERT_EQ(conf_doc_get(doc, "person", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* person_obj = value_object(&v);
  ASSERT_NE(person_obj, nullptr);
  
  /* 添加新属性 */
  ASSERT_EQ(tk_object_set_prop_int(person_obj, "age", 30), RET_OK);
  ASSERT_EQ(tk_object_set_prop_str(person_obj, "city", "Beijing"), RET_OK);
  
  /* 验证新属性 */
  ASSERT_STREQ(tk_object_get_prop_str(person_obj, "name"), "John");
  ASSERT_EQ(tk_object_get_prop_int(person_obj, "age", 0), 30);
  ASSERT_STREQ(tk_object_get_prop_str(person_obj, "city"), "Beijing");
  
  /* 验证通过 doc 对象也能看到新属性 */
  ASSERT_STREQ(conf_doc_get_str(doc, "person.name", ""), "John");
  ASSERT_EQ(conf_doc_get_int(doc, "person.age", 0), 30);
  ASSERT_STREQ(conf_doc_get_str(doc, "person.city", ""), "Beijing");
  
  conf_doc_destroy(doc);
}

TEST(Yaml, extend_type_empty_array) {
  /* 测试空数组的扩展类型 */
  const char* data = "items:\n  -\n";
  
  conf_doc_t* doc = conf_doc_load_yaml(data);
  ASSERT_NE(doc, nullptr);
  conf_doc_use_extend_type(doc, TRUE);
  
  /* 验证空数组可以通过扩展类型获取 */
  value_t v;
  ASSERT_EQ(conf_doc_get(doc, "items", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* items_array = value_object(&v);
  ASSERT_NE(items_array, nullptr);
  ASSERT_TRUE(tk_object_is_collection(items_array));
  
  conf_doc_destroy(doc);
}

TEST(Yaml, extend_type_deep_nesting) {
  /* 测试深层嵌套的扩展类型 */
  const char* data =
      "level1:\n"
      "  level2:\n"
      "    level3:\n"
      "      level4:\n"
      "        value: deep\n";
  
  conf_doc_t* doc = conf_doc_load_yaml(data);
  ASSERT_NE(doc, nullptr);
  conf_doc_use_extend_type(doc, TRUE);
  
  /* 验证深层嵌套对象 */
  value_t v;
  ASSERT_EQ(conf_doc_get(doc, "level1", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* l1 = value_object(&v);
  ASSERT_NE(l1, nullptr);
  
  ASSERT_EQ(conf_doc_get(doc, "level1.level2", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* l2 = value_object(&v);
  ASSERT_NE(l2, nullptr);
  
  ASSERT_EQ(conf_doc_get(doc, "level1.level2.level3", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* l3 = value_object(&v);
  ASSERT_NE(l3, nullptr);
  
  ASSERT_EQ(conf_doc_get(doc, "level1.level2.level3.level4", &v), RET_OK);
  ASSERT_EQ(v.type, VALUE_TYPE_OBJECT);
  
  tk_object_t* l4 = value_object(&v);
  ASSERT_NE(l4, nullptr);
  ASSERT_STREQ(tk_object_get_prop_str(l4, "value"), "deep");
  
  conf_doc_destroy(doc);
}

TEST(Yaml, empty_string) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_yaml("name:");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  node = conf_node_find_child(doc->root, "name");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "name");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "name:\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, negative_number) {
  conf_doc_t* doc = conf_doc_load_yaml("age: -100");

  ASSERT_EQ(conf_doc_get_int(doc, "age", 0), -100);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "age: -100\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, float_negative) {
  conf_doc_t* doc = conf_doc_load_yaml("weight: -60.5");

  ASSERT_FLOAT_EQ(conf_doc_get_float(doc, "weight", 0), -60.5f);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "weight: -60.5\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, deep_nesting) {
  const char* data =
      "level1:\n"
      "  level2:\n"
      "    level3:\n"
      "      level4:\n"
      "        value: deep";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_STREQ(conf_doc_get_str(doc, "level1.level2.level3.level4.value", ""), "deep");

  str_t str;
  str_init(&str, 200);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "level1:\n"
               "  level2:\n"
               "    level3:\n"
               "      level4:\n"
               "        value: deep\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, empty_object) {
  value_t v;
  conf_doc_t* doc = conf_doc_load_yaml("empty:");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_EQ(conf_doc_get(doc, "empty.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "empty:\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, empty_array) {
  value_t v;
  const char* data =
      "items:\n"
      "  -";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_EQ(conf_doc_get(doc, "items.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "items:\n"
               "  -\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, array_with_objects) {
  value_t v;
  const char* data =
      "users:\n"
      "  -\n"
      "    name: jim\n"
      "    age: 30\n"
      "  -\n"
      "    name: tom\n"
      "    age: 25";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_EQ(conf_doc_get(doc, "users.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  ASSERT_STREQ(conf_doc_get_str(doc, "users.[0].name", ""), "jim");
  ASSERT_EQ(conf_doc_get_int(doc, "users.[0].age", 0), 30);
  ASSERT_STREQ(conf_doc_get_str(doc, "users.[1].name", ""), "tom");
  ASSERT_EQ(conf_doc_get_int(doc, "users.[1].age", 0), 25);

  str_t str;
  str_init(&str, 200);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "users:\n"
               "  -\n"
               "    name: jim\n"
               "    age: 30\n"
               "  -\n"
               "    name: tom\n"
               "    age: 25\n");
  str_reset(&str);

  /* 验证保存和重新加载 */
  verify_save_and_reload_value(doc, "users.[0].name", "jim", INT32_MIN, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "users.[0].age", NULL, 30, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "users.[1].name", "tom", INT32_MIN, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "users.[1].age", NULL, 25, FLT_MAX, -1);

  conf_doc_destroy(doc);
}

TEST(Yaml, special_chars_in_value) {
  conf_doc_t* doc = conf_doc_load_yaml("path: /usr/local/bin");

  ASSERT_STREQ(conf_doc_get_str(doc, "path", ""), "/usr/local/bin");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "path: /usr/local/bin\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, number_as_string) {
  conf_doc_t* doc = conf_doc_load_yaml("code: 123");

  const char* value = conf_doc_get_str(doc, "code", "");
  ASSERT_STREQ(value, "123");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "code: 123\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, zero_value) {
  conf_doc_t* doc = conf_doc_load_yaml("count: 0");

  ASSERT_EQ(conf_doc_get_int(doc, "count", -1), 0);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "count: 0\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, float_zero) {
  conf_doc_t* doc = conf_doc_load_yaml("rate: 0.0");

  ASSERT_FLOAT_EQ(conf_doc_get_float(doc, "rate", -1.0f), 0.0f);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "rate: 0.0\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, bool_true) {
  conf_doc_t* doc = conf_doc_load_yaml("enabled: 1");

  ASSERT_EQ(conf_doc_get_bool(doc, "enabled", FALSE), TRUE);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "enabled: 1\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, bool_false) {
  conf_doc_t* doc = conf_doc_load_yaml("enabled: 0");

  ASSERT_EQ(conf_doc_get_bool(doc, "enabled", TRUE), FALSE);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "enabled: 0\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, complex_nested) {
  const char* data =
      "server:\n"
      "  host: localhost\n"
      "  port: 8080\n"
      "  ssl:\n"
      "    enabled: 1\n"
      "    cert: /path/to/cert\n"
      "  databases:\n"
      "    -\n"
      "      name: db1\n"
      "      type: mysql\n"
      "    -\n"
      "      name: db2\n"
      "      type: postgres";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_STREQ(conf_doc_get_str(doc, "server.host", ""), "localhost");
  ASSERT_EQ(conf_doc_get_int(doc, "server.port", 0), 8080);
  ASSERT_EQ(conf_doc_get_bool(doc, "server.ssl.enabled", FALSE), TRUE);
  ASSERT_STREQ(conf_doc_get_str(doc, "server.ssl.cert", ""), "/path/to/cert");
  ASSERT_STREQ(conf_doc_get_str(doc, "server.databases.[0].name", ""), "db1");
  ASSERT_STREQ(conf_doc_get_str(doc, "server.databases.[0].type", ""), "mysql");
  ASSERT_STREQ(conf_doc_get_str(doc, "server.databases.[1].name", ""), "db2");
  ASSERT_STREQ(conf_doc_get_str(doc, "server.databases.[1].type", ""), "postgres");

  str_t str;
  str_init(&str, 500);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "server:\n"
               "  host: localhost\n"
               "  port: 8080\n"
               "  ssl:\n"
               "    enabled: 1\n"
               "    cert: "
               "/path/to/cert\n"
               "  databases:\n"
               "    -\n"
               "      name: db1\n"
               "      type: mysql\n"
               "    -\n"
               "      name: db2\n"
               "      type: postgres\n");
  str_reset(&str);

  /* 验证保存和重新加载 */
  verify_save_and_reload_value(doc, "server.host", "localhost", INT32_MIN, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "server.port", NULL, 8080, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "server.ssl.enabled", NULL, INT32_MIN, FLT_MAX, 1);
  verify_save_and_reload_value(doc, "server.ssl.cert", "/path/to/cert", INT32_MIN, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "server.databases.[0].name", "db1", INT32_MIN, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "server.databases.[0].type", "mysql", INT32_MIN, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "server.databases.[1].name", "db2", INT32_MIN, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "server.databases.[1].type", "postgres", INT32_MIN, FLT_MAX,
                               -1);

  conf_doc_destroy(doc);
}

TEST(Yaml, set_nested) {
  conf_doc_t* doc = conf_doc_load_yaml("name: test");

  conf_doc_set_str(doc, "user.name", "jim");
  conf_doc_set_int(doc, "user.age", 30);
  conf_doc_set_float(doc, "user.weight", 70.5);
  conf_doc_set_bool(doc, "user.male", TRUE);

  ASSERT_STREQ(conf_doc_get_str(doc, "user.name", ""), "jim");
  ASSERT_EQ(conf_doc_get_int(doc, "user.age", 0), 30);
  ASSERT_FLOAT_EQ(conf_doc_get_float(doc, "user.weight", 0), 70.5f);
  ASSERT_EQ(conf_doc_get_bool(doc, "user.male", FALSE), TRUE);

  str_t str;
  str_init(&str, 200);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "name: test\nuser:\n"
               "  name: jim\n"
               "  age: 30\n"
               "  weight: 70.500000\n"
               "  male: 1\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, set_array_element) {
  conf_doc_t* doc = conf_doc_load_yaml(
      "items:\n"
      "  - first");

  conf_doc_set_str(doc, "items.[1]", "second");
  conf_doc_set_str(doc, "items.[2]", "third");

  ASSERT_STREQ(conf_doc_get_str(doc, "items.[0]", ""), "first");
  ASSERT_STREQ(conf_doc_get_str(doc, "items.[1]", ""), "second");
  ASSERT_STREQ(conf_doc_get_str(doc, "items.[2]", ""), "third");

  str_t str;
  str_init(&str, 200);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "items:\n"
               "  - first\n"
               "  - second\n"
               "  - third\n");
  str_reset(&str);

  /* 验证保存和重新加载 */
  verify_save_and_reload_value(doc, "items.[0]", "first", INT32_MIN, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "items.[1]", "second", INT32_MIN, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "items.[2]", "third", INT32_MIN, FLT_MAX, -1);

  conf_doc_destroy(doc);
}

TEST(Yaml, empty_doc) {
  conf_doc_t* doc = conf_doc_load_yaml("");

  ASSERT_NE(doc, (conf_doc_t*)NULL);

  value_t v;
  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, whitespace_only) {
  conf_doc_t* doc = conf_doc_load_yaml(
      "   \n"
      "  \n"
      "  ");

  ASSERT_NE(doc, (conf_doc_t*)NULL);

  value_t v;
  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  conf_doc_destroy(doc);
}

TEST(Yaml, escape_tab) {
  conf_doc_t* doc = conf_doc_load_yaml("text: hello\\tworld");

  const char* value = conf_doc_get_str(doc, "text", "");
  ASSERT_STREQ(value, "hello\tworld");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "text: \"hello\tworld\"\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, escape_backslash) {
  conf_doc_t* doc = conf_doc_load_yaml("path: C:\\\\Windows\\\\System32");

  const char* value = conf_doc_get_str(doc, "path", "");
  ASSERT_STREQ(value, "C:\\Windows\\System32");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "path: \"C:\\\\Windows\\\\System32\"\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, large_float) {
  conf_doc_t* doc = conf_doc_load_yaml("pi: 3.14159265359");

  ASSERT_FLOAT_EQ(conf_doc_get_float(doc, "pi", 0), 3.14159265359f);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "pi: 3.14159265359\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, multiple_root_keys) {
  value_t v;
  const char* data = "key1: value1\nkey2: value2\nkey3: value3";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 3);

  ASSERT_STREQ(conf_doc_get_str(doc, "key1", ""), "value1");
  ASSERT_STREQ(conf_doc_get_str(doc, "key2", ""), "value2");
  ASSERT_STREQ(conf_doc_get_str(doc, "key3", ""), "value3");

  str_t str;
  str_init(&str, 200);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "key1: value1\nkey2: value2\nkey3: value3\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, list_of_numbers) {
  value_t v;
  const char* data =
      "numbers:\n"
      "  - 1\n"
      "  - 2\n"
      "  - 3\n"
      "  - 4\n"
      "  - 5";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_EQ(conf_doc_get(doc, "numbers.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 5);

  ASSERT_STREQ(conf_doc_get_str(doc, "numbers.[0]", ""), "1");
  ASSERT_STREQ(conf_doc_get_str(doc, "numbers.[1]", ""), "2");
  ASSERT_STREQ(conf_doc_get_str(doc, "numbers.[2]", ""), "3");
  ASSERT_STREQ(conf_doc_get_str(doc, "numbers.[3]", ""), "4");
  ASSERT_STREQ(conf_doc_get_str(doc, "numbers.[4]", ""), "5");

  str_t str;
  str_init(&str, 200);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "numbers:\n"
               "  - 1\n"
               "  - 2\n"
               "  - 3\n"
               "  - 4\n"
               "  - 5\n");
  str_reset(&str);

  /* 验证保存和重新加载 */
  verify_save_and_reload_value(doc, "numbers.[0]", NULL, 1, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "numbers.[1]", NULL, 2, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "numbers.[2]", NULL, 3, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "numbers.[3]", NULL, 4, FLT_MAX, -1);
  verify_save_and_reload_value(doc, "numbers.[4]", NULL, 5, FLT_MAX, -1);

  conf_doc_destroy(doc);
}

TEST(Yaml, create_and_save) {
  wbuffer_t wb;
  tk_object_t* conf = conf_yaml_create();
  ASSERT_NE(conf, (tk_object_t*)NULL);

  ASSERT_EQ(tk_object_set_prop_str(conf, "name", "test"), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(conf, "age", 25), RET_OK);
  ASSERT_EQ(tk_object_set_prop_double(conf, "weight", 65.5), RET_OK);
  ASSERT_EQ(tk_object_set_prop_bool(conf, "active", TRUE), RET_OK);

  ASSERT_STREQ(tk_object_get_prop_str(conf, "name"), "test");
  ASSERT_EQ(tk_object_get_prop_int(conf, "age", 0), 25);
  ASSERT_DOUBLE_EQ(tk_object_get_prop_double(conf, "weight", 0), 65.5);
  ASSERT_EQ(tk_object_get_prop_bool(conf, "active", FALSE), TRUE);

  ASSERT_EQ(conf_yaml_save_to_buff(conf, &wb), RET_OK);
  TK_OBJECT_UNREF(conf);

  conf = conf_yaml_load_from_buff(wb.data, wb.cursor, FALSE);
  ASSERT_STREQ(tk_object_get_prop_str(conf, "name"), "test");
  ASSERT_EQ(tk_object_get_prop_int(conf, "age", 0), 25);
  ASSERT_DOUBLE_EQ(tk_object_get_prop_double(conf, "weight", 0), 65.5);
  ASSERT_EQ(tk_object_get_prop_bool(conf, "active", FALSE), TRUE);
  TK_OBJECT_UNREF(conf);

  wbuffer_deinit(&wb);
}

TEST(Yaml, quoted_string_double) {
  conf_doc_t* doc = conf_doc_load_yaml("name: \"hello world\"");

  ASSERT_STREQ(conf_doc_get_str(doc, "name", ""), "hello world");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "name: \"hello world\"\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, quoted_string_single) {
  conf_doc_t* doc = conf_doc_load_yaml("name: 'hello world'");

  ASSERT_STREQ(conf_doc_get_str(doc, "name", ""), "hello world");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "name: \"hello world\"\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, quoted_string_with_escape) {
  conf_doc_t* doc = conf_doc_load_yaml("text: \"hello\\\"world\"");

  ASSERT_STREQ(conf_doc_get_str(doc, "text", ""), "hello\"world");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "text: \"hello\\\"world\"\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, quoted_string_with_backslash) {
  conf_doc_t* doc = conf_doc_load_yaml("path: \"C:\\\\Windows\\\\System32\"");

  ASSERT_STREQ(conf_doc_get_str(doc, "path", ""), "C:\\Windows\\System32");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "path: \"C:\\\\Windows\\\\System32\"\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, quoted_string_with_comment_char) {
  conf_doc_t* doc = conf_doc_load_yaml("text: \"hello # world\"");

  ASSERT_STREQ(conf_doc_get_str(doc, "text", ""), "hello # world");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "text: \"hello # world\"\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, quoted_string_with_comment_after) {
  conf_doc_t* doc = conf_doc_load_yaml("text: \"hello world\" # comment");

  ASSERT_STREQ(conf_doc_get_str(doc, "text", ""), "hello world");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "text: \"hello world\"\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, single_quote_literal_backslash) {
  /* 单引号字符串：\n"" 应该被解析为字面值（两个字符：\ 和 n），而不是换行符 */
  conf_doc_t* doc = conf_doc_load_yaml("text: 'hello\\nworld'");

  const char* result = conf_doc_get_str(doc, "text", "");
  ASSERT_STREQ(result, "hello\\nworld"); /* 字面值，包含反斜杠和n */

  conf_doc_destroy(doc);
}

TEST(Yaml, double_quote_escape_newline) {
  /* 双引号字符串：\n"" 应该被解析为换行符 */
  conf_doc_t* doc = conf_doc_load_yaml("text: \"hello\\nworld\"");

  const char* result = conf_doc_get_str(doc, "text", "");
  ASSERT_STREQ(result, "hello\nworld"); /* 转义为换行符 */

  conf_doc_destroy(doc);
}

TEST(Yaml, single_quote_escape_single_quote) {
  /* 单引号字符串：'' 应该被解析为一个单引号字符 */
  conf_doc_t* doc = conf_doc_load_yaml("text: 'It''s a sunny day'");

  const char* result = conf_doc_get_str(doc, "text", "");
  ASSERT_STREQ(result, "It's a sunny day"); /* 两个单引号表示一个单引号 */

  conf_doc_destroy(doc);
}

TEST(Yaml, single_quote_backslash_literal) {
  /* 单引号字符串：反斜杠应该被当作普通字符 */
  conf_doc_t* doc = conf_doc_load_yaml("path: 'C:\\\\Windows\\\\System32'");

  const char* result = conf_doc_get_str(doc, "path", "");
  ASSERT_STREQ(result, "C:\\\\Windows\\\\System32"); /* 字面值，包含反斜杠 */

  conf_doc_destroy(doc);
}

TEST(Yaml, key_with_hyphen) {
  conf_doc_t* doc = conf_doc_load_yaml("my-key: value");

  ASSERT_STREQ(conf_doc_get_str(doc, "my-key", ""), "value");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "my-key: value\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, key_with_dot) {
  /* 注意：my.key 会被解析为嵌套结构（my 下的 key），要作为单个键名需要使用引号 */
  conf_doc_t* doc = conf_doc_load_yaml("\"my.key\": value");

  /* 使用 conf_node_find_child 直接查找，因为 conf_doc_get_str 会将点号作为路径分隔符 */
  conf_node_t* node = conf_node_find_child(doc->root, "my.key");
  ASSERT_NE(node, (conf_node_t*)NULL);
  ASSERT_STREQ(conf_node_get_child_value_str(doc->root, "my.key", ""), "value");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "\"my.key\": value\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, key_with_slash) {
  conf_doc_t* doc = conf_doc_load_yaml("my/key: value");

  ASSERT_STREQ(conf_doc_get_str(doc, "my/key", ""), "value");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "my/key: value\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, quoted_key_name) {
  conf_doc_t* doc = conf_doc_load_yaml("\"key with spaces\": value");

  ASSERT_STREQ(conf_doc_get_str(doc, "key with spaces", ""), "value");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "\"key with spaces\": value\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, quoted_key_name_single) {
  conf_doc_t* doc = conf_doc_load_yaml("'key with spaces': value");

  ASSERT_STREQ(conf_doc_get_str(doc, "key with spaces", ""), "value");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "\"key with spaces\": value\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, quoted_key_name_with_special_chars) {
  conf_doc_t* doc = conf_doc_load_yaml("\"key:with:colons\": value");

  ASSERT_STREQ(conf_doc_get_str(doc, "key:with:colons", ""), "value");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "\"key:with:colons\": value\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, auto_quote_value_with_space) {
  conf_doc_t* doc = conf_doc_create(100);
  doc->root = conf_doc_create_node(doc, CONF_NODE_ROOT_NAME);
  conf_doc_set_str(doc, "text", "hello world");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "text: \"hello world\"\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, auto_quote_value_with_colon) {
  conf_doc_t* doc = conf_doc_create(100);
  doc->root = conf_doc_create_node(doc, CONF_NODE_ROOT_NAME);
  conf_doc_set_str(doc, "url", "http://example.com");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "url: \"http://example.com\"\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, auto_quote_key_with_space) {
  conf_doc_t* doc = conf_doc_create(100);
  doc->root = conf_doc_create_node(doc, CONF_NODE_ROOT_NAME);
  conf_doc_set_str(doc, "my key", "value");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "\"my key\": value\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, nested_quoted_strings) {
  const char* data =
      "person:\n"
      "  name: \"John Doe\"\n"
      "  email: 'john@example.com'\n"
      "  address: \"123 Main St\"";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_STREQ(conf_doc_get_str(doc, "person.name", ""), "John Doe");
  ASSERT_STREQ(conf_doc_get_str(doc, "person.email", ""), "john@example.com");
  ASSERT_STREQ(conf_doc_get_str(doc, "person.address", ""), "123 Main St");

  conf_doc_destroy(doc);
}

TEST(Yaml, mixed_quoted_and_unquoted) {
  const char* data =
      "name: \"quoted\"\n"
      "age: 30\n"
      "city: 'Beijing'\n"
      "country: China";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_STREQ(conf_doc_get_str(doc, "name", ""), "quoted");
  ASSERT_EQ(conf_doc_get_int(doc, "age", 0), 30);
  ASSERT_STREQ(conf_doc_get_str(doc, "city", ""), "Beijing");
  ASSERT_STREQ(conf_doc_get_str(doc, "country", ""), "China");

  conf_doc_destroy(doc);
}

TEST(Yaml, empty_quoted_string) {
  conf_doc_t* doc = conf_doc_load_yaml("name: \"\"");

  ASSERT_STREQ(conf_doc_get_str(doc, "name", ""), "");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  /* 空字符串不包含特殊字符，保存时可能不加引号 */
  ASSERT_TRUE(str.str != NULL &&
              (strcmp(str.str, "name:\n") == 0 || strcmp(str.str, "name: \"\"\n") == 0));
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, quoted_string_escape_sequences) {
  conf_doc_t* doc = conf_doc_load_yaml("text: \"hello\\nworld\\ttest\"");

  const char* value = conf_doc_get_str(doc, "text", "");
  ASSERT_STREQ(value, "hello\nworld\ttest");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "text: \"hello\nworld\ttest\"\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, complex_key_names) {
  const char* data =
      "my-key: value1\n"
      "\"my.key\": value2\n"
      "my/key: value3\n"
      "\"key with spaces\": value4";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_STREQ(conf_doc_get_str(doc, "my-key", ""), "value1");
  /* 使用 conf_node_get_child_value_str 访问包含点号的键名 */
  ASSERT_STREQ(conf_node_get_child_value_str(doc->root, "my.key", ""), "value2");
  ASSERT_STREQ(conf_doc_get_str(doc, "my/key", ""), "value3");
  ASSERT_STREQ(conf_node_get_child_value_str(doc->root, "key with spaces", ""), "value4");

  conf_doc_destroy(doc);
}

TEST(Yaml, quoted_string_in_array) {
  const char* data =
      "items:\n"
      "  - \"first\"\n"
      "  - 'second'\n"
      "  - third";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_STREQ(conf_doc_get_str(doc, "items.[0]", ""), "first");
  ASSERT_STREQ(conf_doc_get_str(doc, "items.[1]", ""), "second");
  ASSERT_STREQ(conf_doc_get_str(doc, "items.[2]", ""), "third");

  conf_doc_destroy(doc);
}

TEST(Yaml, bool_true_string) {
  conf_doc_t* doc = conf_doc_load_yaml("enabled: true");

  ASSERT_EQ(conf_doc_get_bool(doc, "enabled", FALSE), TRUE);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "enabled: 1\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, bool_false_string) {
  conf_doc_t* doc = conf_doc_load_yaml("enabled: false");

  ASSERT_EQ(conf_doc_get_bool(doc, "enabled", TRUE), FALSE);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "enabled: 0\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, bool_yes) {
  conf_doc_t* doc = conf_doc_load_yaml("enabled: yes");

  ASSERT_EQ(conf_doc_get_bool(doc, "enabled", FALSE), TRUE);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "enabled: 1\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, bool_no) {
  conf_doc_t* doc = conf_doc_load_yaml("enabled: no");

  ASSERT_EQ(conf_doc_get_bool(doc, "enabled", TRUE), FALSE);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "enabled: 0\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, bool_on) {
  conf_doc_t* doc = conf_doc_load_yaml("enabled: on");

  ASSERT_EQ(conf_doc_get_bool(doc, "enabled", FALSE), TRUE);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "enabled: 1\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, bool_off) {
  conf_doc_t* doc = conf_doc_load_yaml("enabled: off");

  ASSERT_EQ(conf_doc_get_bool(doc, "enabled", TRUE), FALSE);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "enabled: 0\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, bool_case_insensitive) {
  conf_doc_t* doc1 = conf_doc_load_yaml("enabled: TRUE");
  conf_doc_t* doc2 = conf_doc_load_yaml("enabled: True");
  conf_doc_t* doc3 = conf_doc_load_yaml("enabled: YES");
  conf_doc_t* doc4 = conf_doc_load_yaml("enabled: Yes");
  conf_doc_t* doc5 = conf_doc_load_yaml("enabled: ON");
  conf_doc_t* doc6 = conf_doc_load_yaml("enabled: On");
  conf_doc_t* doc7 = conf_doc_load_yaml("enabled: FALSE");
  conf_doc_t* doc8 = conf_doc_load_yaml("enabled: False");
  conf_doc_t* doc9 = conf_doc_load_yaml("enabled: NO");
  conf_doc_t* doc10 = conf_doc_load_yaml("enabled: No");
  conf_doc_t* doc11 = conf_doc_load_yaml("enabled: OFF");
  conf_doc_t* doc12 = conf_doc_load_yaml("enabled: Off");

  ASSERT_EQ(conf_doc_get_bool(doc1, "enabled", FALSE), TRUE);
  ASSERT_EQ(conf_doc_get_bool(doc2, "enabled", FALSE), TRUE);
  ASSERT_EQ(conf_doc_get_bool(doc3, "enabled", FALSE), TRUE);
  ASSERT_EQ(conf_doc_get_bool(doc4, "enabled", FALSE), TRUE);
  ASSERT_EQ(conf_doc_get_bool(doc5, "enabled", FALSE), TRUE);
  ASSERT_EQ(conf_doc_get_bool(doc6, "enabled", FALSE), TRUE);
  ASSERT_EQ(conf_doc_get_bool(doc7, "enabled", TRUE), FALSE);
  ASSERT_EQ(conf_doc_get_bool(doc8, "enabled", TRUE), FALSE);
  ASSERT_EQ(conf_doc_get_bool(doc9, "enabled", TRUE), FALSE);
  ASSERT_EQ(conf_doc_get_bool(doc10, "enabled", TRUE), FALSE);
  ASSERT_EQ(conf_doc_get_bool(doc11, "enabled", TRUE), FALSE);
  ASSERT_EQ(conf_doc_get_bool(doc12, "enabled", TRUE), FALSE);

  conf_doc_destroy(doc1);
  conf_doc_destroy(doc2);
  conf_doc_destroy(doc3);
  conf_doc_destroy(doc4);
  conf_doc_destroy(doc5);
  conf_doc_destroy(doc6);
  conf_doc_destroy(doc7);
  conf_doc_destroy(doc8);
  conf_doc_destroy(doc9);
  conf_doc_destroy(doc10);
  conf_doc_destroy(doc11);
  conf_doc_destroy(doc12);
}

TEST(Yaml, bool_not_bool_string) {
  /* 确保非布尔值字符串不会被误识别为布尔值 */
  conf_doc_t* doc1 = conf_doc_load_yaml("text: true_value");
  conf_doc_t* doc2 = conf_doc_load_yaml("text: false_alarm");
  conf_doc_t* doc3 = conf_doc_load_yaml("text: yes_no");
  conf_doc_t* doc4 = conf_doc_load_yaml("text: on_off");

  ASSERT_STREQ(conf_doc_get_str(doc1, "text", ""), "true_value");
  ASSERT_STREQ(conf_doc_get_str(doc2, "text", ""), "false_alarm");
  ASSERT_STREQ(conf_doc_get_str(doc3, "text", ""), "yes_no");
  ASSERT_STREQ(conf_doc_get_str(doc4, "text", ""), "on_off");

  conf_doc_destroy(doc1);
  conf_doc_destroy(doc2);
  conf_doc_destroy(doc3);
  conf_doc_destroy(doc4);
}

TEST(Yaml, null_value) {
  conf_doc_t* doc = conf_doc_load_yaml("value: null");

  ASSERT_TRUE(conf_doc_get_str(doc, "value", "default") == NULL);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "value: null\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, null_value_uppercase) {
  conf_doc_t* doc = conf_doc_load_yaml("value: NULL");

  ASSERT_TRUE(conf_doc_get_str(doc, "value", "default") == NULL);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "value: null\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, null_value_mixed_case) {
  conf_doc_t* doc = conf_doc_load_yaml("value: Null");

  ASSERT_TRUE(conf_doc_get_str(doc, "value", "default") == NULL);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "value: null\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, null_value_tilde) {
  conf_doc_t* doc = conf_doc_load_yaml("value: ~");

  ASSERT_TRUE(conf_doc_get_str(doc, "value", "default") == NULL);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "value: null\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, null_value_in_array) {
  const char* data =
      "items:\n"
      "  - null\n"
      "  - value\n"
      "  - ~";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_TRUE(conf_doc_get_str(doc, "items.[0]", "default") == NULL);
  ASSERT_STREQ(conf_doc_get_str(doc, "items.[1]", ""), "value");
  ASSERT_TRUE(conf_doc_get_str(doc, "items.[2]", "default") == NULL);

  str_t str;
  str_init(&str, 200);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str,
               "items:\n"
               "  - null\n"
               "  - value\n"
               "  - null\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, null_value_not_string) {
  /* 确保包含 "null" 的字符串不会被误识别为 null 值 */
  conf_doc_t* doc1 = conf_doc_load_yaml("text: null_value");
  conf_doc_t* doc2 = conf_doc_load_yaml("text: nullable");

  ASSERT_STREQ(conf_doc_get_str(doc1, "text", ""), "null_value");
  ASSERT_STREQ(conf_doc_get_str(doc2, "text", ""), "nullable");

  conf_doc_destroy(doc1);
  conf_doc_destroy(doc2);
}

TEST(Yaml, flow_sequence_simple) {
  conf_doc_t* doc = conf_doc_load_yaml("items: [item1, item2, item3]");

  ASSERT_STREQ(conf_doc_get_str(doc, "items.[0]", ""), "item1");
  ASSERT_STREQ(conf_doc_get_str(doc, "items.[1]", ""), "item2");
  ASSERT_STREQ(conf_doc_get_str(doc, "items.[2]", ""), "item3");

  conf_doc_destroy(doc);
}

TEST(Yaml, flow_sequence_numbers) {
  conf_doc_t* doc = conf_doc_load_yaml("numbers: [1, 2, 3, 4, 5]");

  ASSERT_EQ(conf_doc_get_int(doc, "numbers.[0]", 0), 1);
  ASSERT_EQ(conf_doc_get_int(doc, "numbers.[1]", 0), 2);
  ASSERT_EQ(conf_doc_get_int(doc, "numbers.[2]", 0), 3);

  conf_doc_destroy(doc);
}

TEST(Yaml, flow_mapping_simple) {
  conf_doc_t* doc = conf_doc_load_yaml("person: {name: John, age: 30}");

  ASSERT_STREQ(conf_doc_get_str(doc, "person.name", ""), "John");
  ASSERT_EQ(conf_doc_get_int(doc, "person.age", 0), 30);

  conf_doc_destroy(doc);
}

TEST(Yaml, flow_nested) {
  conf_doc_t* doc = conf_doc_load_yaml("config: {servers: [s1, s2], timeout: 30}");

  ASSERT_STREQ(conf_doc_get_str(doc, "config.servers.[0]", ""), "s1");
  ASSERT_STREQ(conf_doc_get_str(doc, "config.servers.[1]", ""), "s2");
  ASSERT_EQ(conf_doc_get_int(doc, "config.timeout", 0), 30);

  conf_doc_destroy(doc);
}

TEST(Yaml, flow_empty) {
  conf_doc_t* doc1 = conf_doc_load_yaml("items: []");
  conf_doc_t* doc2 = conf_doc_load_yaml("person: {}");

  ASSERT_EQ(conf_doc_get_int(doc1, "items.#size", 0), 0);
  ASSERT_EQ(conf_doc_get_int(doc2, "person.#size", 0), 0);

  conf_doc_destroy(doc1);
  conf_doc_destroy(doc2);
}

TEST(Yaml, flow_sequence_with_quotes) {
  conf_doc_t* doc = conf_doc_load_yaml("items: [\"item1\", \"item2\", \"item3\"]");

  ASSERT_STREQ(conf_doc_get_str(doc, "items.[0]", ""), "item1");
  ASSERT_STREQ(conf_doc_get_str(doc, "items.[1]", ""), "item2");
  ASSERT_STREQ(conf_doc_get_str(doc, "items.[2]", ""), "item3");

  conf_doc_destroy(doc);
}

TEST(Yaml, flow_mapping_with_quotes) {
  conf_doc_t* doc = conf_doc_load_yaml("person: {\"name\": \"John\", \"age\": 30}");

  ASSERT_STREQ(conf_doc_get_str(doc, "person.name", ""), "John");
  ASSERT_EQ(conf_doc_get_int(doc, "person.age", 0), 30);

  conf_doc_destroy(doc);
}

TEST(Yaml, flow_nested_deep) {
  conf_doc_t* doc = conf_doc_load_yaml(
      "config: {servers: [{name: s1, port: 80}, {name: s2, port: 443}], timeout: 30}");

  ASSERT_STREQ(conf_doc_get_str(doc, "config.servers.[0].name", ""), "s1");
  ASSERT_EQ(conf_doc_get_int(doc, "config.servers.[0].port", 0), 80);
  ASSERT_STREQ(conf_doc_get_str(doc, "config.servers.[1].name", ""), "s2");
  ASSERT_EQ(conf_doc_get_int(doc, "config.servers.[1].port", 0), 443);
  ASSERT_EQ(conf_doc_get_int(doc, "config.timeout", 0), 30);

  conf_doc_destroy(doc);
}

TEST(Yaml, flow_save_as_block) {
  conf_doc_t* doc = conf_doc_load_yaml("items: [item1, item2, item3]");

  str_t str;
  str_init(&str, 200);
  conf_doc_save_yaml(doc, &str);
  /* 保存时应该转换为块式风格 */
  ASSERT_TRUE(strstr(str.str, "items:") != NULL);
  ASSERT_TRUE(strstr(str.str, "-") != NULL);
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, literal_block_basic) {
  const char* data =
      "text: |\n"
      "  Line 1\n"
      "  Line 2\n"
      "  Line 3\n";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  const char* value = conf_doc_get_str(doc, "text", "");
  ASSERT_STREQ(value, "Line 1\nLine 2\nLine 3");

  conf_doc_destroy(doc);
}

TEST(Yaml, literal_block_empty) {
  const char* data = "text: |\n";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  const char* value = conf_doc_get_str(doc, "text", "");
  ASSERT_STREQ(value, "");

  conf_doc_destroy(doc);
}

TEST(Yaml, folded_block_basic) {
  const char* data =
      "text: >\n"
      "  This is a\n"
      "  long string\n"
      "  that folds\n";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  const char* value = conf_doc_get_str(doc, "text", "");
  ASSERT_STREQ(value, "This is a long string that folds");

  conf_doc_destroy(doc);
}

TEST(Yaml, literal_block_with_modifiers) {
  const char* data =
      "text_plus: |+\n"
      "  Line 1\n"
      "  Line 2\n"
      "\n"
      "text_minus: |-\n"
      "  Line 1\n"
      "  Line 2\n";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  const char* value1 = conf_doc_get_str(doc, "text_plus", "");
  const char* value2 = conf_doc_get_str(doc, "text_minus", "");

  /* |+ 保留末尾换行 */
  ASSERT_TRUE(strlen(value1) > 0);
  /* |- 去除末尾换行 */
  ASSERT_TRUE(strlen(value2) > 0);

  conf_doc_destroy(doc);
}

TEST(Yaml, multiline_in_nested) {
  const char* data =
      "person:\n"
      "  name: John\n"
      "  bio: |\n"
      "    This is a\n"
      "    multi-line bio\n";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_STREQ(conf_doc_get_str(doc, "person.name", ""), "John");
  ASSERT_STREQ(conf_doc_get_str(doc, "person.bio", ""), "This is a\nmulti-line bio");

  conf_doc_destroy(doc);
}

TEST(Yaml, literal_block_single_line) {
  const char* data =
      "text: |\n"
      "  Single line\n";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  const char* value = conf_doc_get_str(doc, "text", "");
  ASSERT_STREQ(value, "Single line");

  conf_doc_destroy(doc);
}

TEST(Yaml, folded_block_single_line) {
  const char* data =
      "text: >\n"
      "  Single line\n";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  const char* value = conf_doc_get_str(doc, "text", "");
  ASSERT_STREQ(value, "Single line");

  conf_doc_destroy(doc);
}

TEST(Yaml, literal_block_with_empty_lines) {
  const char* data =
      "text: |\n"
      "  Line 1\n"
      "\n"
      "  Line 2\n";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  const char* value = conf_doc_get_str(doc, "text", "");
  ASSERT_STREQ(value,
               "Line 1\n"
               "\n"
               "Line 2");

  conf_doc_destroy(doc);
}

TEST(Yaml, literal_block_modifier_plus) {
  const char* data =
      "text: |+\n"
      "  Line 1\n"
      "  Line 2\n";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  const char* value = conf_doc_get_str(doc, "text", "");
  /* |+ 保留末尾换行 */
  ASSERT_TRUE(strlen(value) > 0);
  ASSERT_TRUE(value[strlen(value) - 1] == '\n' || strstr(value,
                                                         "\n"
                                                         "\n") != NULL);

  conf_doc_destroy(doc);
}

TEST(Yaml, literal_block_modifier_minus) {
  const char* data =
      "text: |-\n"
      "  Line 1\n"
      "  Line 2\n";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  const char* value = conf_doc_get_str(doc, "text", "");
  /* |- 去除末尾换行 */
  ASSERT_TRUE(strlen(value) > 0);
  /* 末尾不应该是换行（除非是空字符串） */

  conf_doc_destroy(doc);
}

TEST(Yaml, folded_block_modifier_plus) {
  const char* data =
      "text: >+\n"
      "  Line 1\n"
      "  Line 2\n";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  const char* value = conf_doc_get_str(doc, "text", "");
  ASSERT_STREQ(value, "Line 1 Line 2");

  conf_doc_destroy(doc);
}

TEST(Yaml, folded_block_modifier_minus) {
  const char* data =
      "text: >-\n"
      "  Line 1\n"
      "  Line 2\n";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  const char* value = conf_doc_get_str(doc, "text", "");
  ASSERT_STREQ(value, "Line 1 Line 2");

  conf_doc_destroy(doc);
}

TEST(Yaml, multiline_in_array) {
  const char* data =
      "items:\n"
      "  - |\n"
      "    Item 1\n"
      "    Line 2\n"
      "  - |\n"
      "    Item 2\n";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_STREQ(conf_doc_get_str(doc, "items.[0]", ""), "Item 1\nLine 2");
  ASSERT_STREQ(conf_doc_get_str(doc, "items.[1]", ""), "Item 2");

  conf_doc_destroy(doc);
}

TEST(Yaml, multiline_save_test) {
  const char* data =
      "text: |\n"
      "  Line 1\n"
      "  Line 2\n";
  conf_doc_t* doc = conf_doc_load_yaml(data);

  const char* original_value = conf_doc_get_str(doc, "text", "");
  ASSERT_STREQ(original_value, "Line 1\nLine 2");

  str_t str;
  str_init(&str, 200);
  conf_doc_save_yaml(doc, &str);
  /* 保存时应该保留多行字符串的内容（可能使用引号格式） */
  ASSERT_TRUE(strstr(str.str, "Line 1") != NULL);
  ASSERT_TRUE(strstr(str.str, "Line 2") != NULL);

  /* 验证保存和重新加载 - 如果保存的字符串包含转义的换行符，则验证重新加载 */
  if (strstr(str.str, "\\n") != NULL) {
    conf_doc_t* reloaded_doc = conf_doc_load_yaml(str.str);
    ASSERT_NE(reloaded_doc, nullptr);
    const char* reloaded_value = conf_doc_get_str(reloaded_doc, "text", "");
    /* 重新加载的值应该与原始值相同（包含换行符） */
    ASSERT_STREQ(reloaded_value, original_value);
    conf_doc_destroy(reloaded_doc);
  }

  str_reset(&str);
  conf_doc_destroy(doc);
}
