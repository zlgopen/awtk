#include "gtest/gtest.h"
#include "conf_io/conf_yaml.h"

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
  conf_doc_t* doc = conf_doc_load_yaml("person:\r\n  name: jim\n  age:100");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  ASSERT_STREQ(conf_doc_get_str(doc, "person.name", ""), "jim");
  ASSERT_EQ(conf_doc_get_int(doc, "person.age", 0), 100);
  
  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "person:\n  name: jim\n  age: 100\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, basic4) {
  value_t v;
  conf_doc_t* doc = conf_doc_load_yaml("jim:\r\n  age:100\ntom:\n  age:99\n");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  ASSERT_EQ(conf_doc_get_int(doc, "jim.age", 0), 100);
  ASSERT_EQ(conf_doc_get_int(doc, "tom.age", 0), 99);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "jim:\n  age: 100\ntom:\n  age: 99\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, basic5) {
  value_t v;
  conf_doc_t* doc = conf_doc_load_yaml("jim:\r\n  age:100\n  weight: 60");

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);
  
  ASSERT_EQ(conf_doc_get(doc, "jim.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);
  
  ASSERT_EQ(conf_doc_get_int(doc, "jim.age", 0), 100);
  ASSERT_EQ(conf_doc_get_int(doc, "jim.weight", 0), 60);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "jim:\n  age: 100\n  weight: 60\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, basic6) {
  value_t v;
  conf_doc_t* doc = conf_doc_load_yaml("jim:\r\n  age:100\n  weight: 60\ntom:\n  age:99\n  weight:70");

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
  ASSERT_STREQ(str.str, "jim:\n  age: 100\n  weight: 60\ntom:\n  age: 99\n  weight: 70\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, basic7) {
  value_t v;
  const char* data = "plan_request_params:\n\
  planning_attempts: 1\n\
  planning_pipeline: ompl\n\
  max_velocity_scaling_factor: 1.0\n\
  max_acceleration_scaling_factor: 0.5 ";

  conf_doc_t* doc = conf_doc_load_yaml(data);

  ASSERT_EQ(conf_doc_get(doc, "#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);
  
  ASSERT_EQ(conf_doc_get(doc, "plan_request_params.#size", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 4);

  ASSERT_EQ(conf_doc_get_int(doc, "plan_request_params.planning_attempts", 0), 1);
  ASSERT_STREQ(conf_doc_get_str(doc, "plan_request_params.planning_pipeline", ""), "ompl");
  ASSERT_FLOAT_EQ(conf_doc_get_float(doc, "plan_request_params.max_velocity_scaling_factor", 0), 1.0f);
  ASSERT_FLOAT_EQ(conf_doc_get_float(doc, "plan_request_params.max_acceleration_scaling_factor", 0), 0.5f);

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "plan_request_params:\n  planning_attempts: 1\n  planning_pipeline: ompl\n  max_velocity_scaling_factor: 1.0\n  max_acceleration_scaling_factor: 0.5\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Yaml, list1) {
  value_t v;
  const char* data = "planning_pipelines:\n\
  pipeline_names:\n\
    - ompl\n";

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
  ASSERT_STREQ(str.str, "planning_pipelines:\n  pipeline_names:\n    - ompl\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Yaml, list2) {
  value_t v;
  const char* data = "planning_pipelines:\n\
  pipeline_names:\n\
    - ompl\n\
    - kdl\n\
    - ikfast\n";

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
  ASSERT_STREQ(str.str, "planning_pipelines:\n  pipeline_names:\n    - ompl\n    - kdl\n    - ikfast\n");
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
  const char* data = "planning_pipelines:#comment\n\
  pipeline_names:#comment\n\
    - ompl#comment\n\
    #comment\n\
    - kdl#comment\n\
    - ikfast #comment\n";

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
  ASSERT_STREQ(str.str, "planning_pipelines:\n  pipeline_names:\n    - ompl\n    - kdl\n    - ikfast\n");
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
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "hello");

  str_t str;
  str_init(&str, 100);
  conf_doc_save_yaml(doc, &str);
  ASSERT_STREQ(str.str, "hello:\n");
  str_reset(&str);

  conf_doc_destroy(doc);
}

TEST(Yaml, error3) {
  value_t v;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_yaml("hel#lo");

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
  ASSERT_STREQ(str.str, "name: hello\\nworld\n");
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
  ASSERT_STREQ(str.str, "name: \\#hello\n");
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
  ASSERT_STREQ(str.str, "name: \\#hello\ntom:\n  age: 100\n  weight: 60.000000\n  male: 1\n");
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
  ASSERT_STREQ(tk_object_get_prop_str(conf, "plan_request_params.planning_attempts"), "1");
  ASSERT_STREQ(tk_object_get_prop_str(conf, "plan_request_params.max_velocity_scaling_factor"), "1.0");
  ASSERT_STREQ(tk_object_get_prop_str(conf, "plan_request_params.max_acceleration_scaling_factor"), "1.0");

  ASSERT_STREQ(tk_object_get_prop_str(conf, "planning_pipelines.pipeline_names.[0]"), "ompl");
  ASSERT_STREQ(tk_object_get_prop_str(conf, "planning_pipelines.pipeline_names.[1]"), "kdl");


  TK_OBJECT_UNREF(conf);
}
