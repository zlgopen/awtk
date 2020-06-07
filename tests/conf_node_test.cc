#include "gtest/gtest.h"
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
