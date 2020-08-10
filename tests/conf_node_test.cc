#include "gtest/gtest.h"
#include "conf_io/conf_json.h"
#include "conf_io/conf_node.h"

TEST(ConfNode, basic) {
  conf_doc_t* doc = conf_doc_create(100);
  conf_node_t* node = NULL;
  doc->root = conf_doc_create_node(doc, "root");

  conf_doc_append_child(doc, doc->root, conf_doc_create_node(doc, "1"));
  conf_doc_append_child(doc, doc->root, conf_doc_create_node(doc, "2"));
  conf_doc_append_child(doc, doc->root, conf_doc_create_node(doc, "3"));
  conf_doc_append_child(doc, doc->root, conf_doc_create_node(doc, "4"));

  node = conf_node_find_child(doc->root, "1");
  ASSERT_STREQ(conf_node_get_name(node), "1");
  node = conf_node_find_child(doc->root, "2");
  ASSERT_STREQ(conf_node_get_name(node), "2");
  node = conf_node_find_child(doc->root, "3");
  ASSERT_STREQ(conf_node_get_name(node), "3");
  node = conf_node_find_child(doc->root, "4");
  ASSERT_STREQ(conf_node_get_name(node), "4");

  ASSERT_EQ(conf_doc_remove_child_by_name(doc, doc->root, "2"), RET_OK);
  ASSERT_NE(conf_doc_remove_child_by_name(doc, doc->root, "2"), RET_OK);
  ASSERT_EQ(conf_doc_remove_child_by_name(doc, doc->root, "1"), RET_OK);
  ASSERT_NE(conf_doc_remove_child_by_name(doc, doc->root, "1"), RET_OK);
  ASSERT_EQ(conf_doc_remove_child_by_name(doc, doc->root, "4"), RET_OK);
  ASSERT_NE(conf_doc_remove_child_by_name(doc, doc->root, "4"), RET_OK);

  node = conf_node_find_child(doc->root, "3");
  ASSERT_EQ(conf_doc_remove_child(doc, doc->root, node), RET_OK);
  ASSERT_NE(conf_doc_remove_child_by_name(doc, doc->root, "3"), RET_OK);

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

  ASSERT_EQ(conf_doc_save_json(doc, &str), RET_OK);
  ASSERT_STREQ(str.str, "{\n    \"names\" : [\n        \"jim\",\n        \"tom\",\n        \"anny\"\n    ]\n}");

  conf_doc_destroy(doc);
}
