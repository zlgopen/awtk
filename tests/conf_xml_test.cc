
#include "gtest/gtest.h"
#include "conf_io/conf_xml.h"

TEST(Xml, basic0) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_xml("<hello name='abc'/>");

  ASSERT_EQ(conf_doc_get(doc, "hello.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "abc");

  node = conf_node_find_child(doc->root, "hello");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "hello");

  str_init(&str, 100);
  conf_doc_save_xml(doc, &str);
  ASSERT_STREQ(str.str, "<hello name=\"abc\"/>\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Xml, basic1) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_xml("<hello name='abc'><world name='cde' value='123'/></hello>");

  ASSERT_EQ(conf_doc_get(doc, "hello.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "abc");
  
  ASSERT_EQ(conf_doc_get(doc, "hello.world.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "cde");
  
  ASSERT_EQ(conf_doc_get(doc, "hello.world.value", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "123");

  node = conf_node_find_child(doc->root, "hello");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "hello");

  str_init(&str, 100);
  conf_doc_save_xml(doc, &str);
  ASSERT_STREQ(str.str, "<hello name=\"abc\">\n  <world name=\"cde\" value=\"123\"/>\n</hello>\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Xml, basic2) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_xml("<hello name='abc'><world name='cde' value='123'/><foo name='oo' value='456'/></hello>");

  ASSERT_EQ(conf_doc_get(doc, "hello.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "abc");
  
  ASSERT_EQ(conf_doc_get(doc, "hello.world.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "cde");
  
  ASSERT_EQ(conf_doc_get(doc, "hello.world.value", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "123");
  
  ASSERT_EQ(conf_doc_get(doc, "hello.foo.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "oo");
  
  ASSERT_EQ(conf_doc_get(doc, "hello.foo.value", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "456");

  node = conf_node_find_child(doc->root, "hello");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "hello");

  str_init(&str, 100);
  conf_doc_save_xml(doc, &str);
  ASSERT_STREQ(str.str, "<hello name=\"abc\">\n  <world name=\"cde\" value=\"123\"/>\n  <foo name=\"oo\" value=\"456\"/>\n</hello>\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Xml, basic3) {
  value_t v;
  str_t str;
  conf_doc_t* doc = conf_doc_load_xml("<hello name='abc'><world name='cde' value='123'/><foo name='oo' value='456'/></hello><awtk name='ttt'/>");

  ASSERT_EQ(conf_doc_get(doc, "awtk.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "ttt");

  str_init(&str, 100);
  conf_doc_save_xml(doc, &str);
  ASSERT_STREQ(str.str, "<hello name=\"abc\">\n  <world name=\"cde\" value=\"123\"/>\n  <foo name=\"oo\" value=\"456\"/>\n</hello>\n<awtk name=\"ttt\"/>\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Xml, basic4) {
  value_t v;
  str_t str;
  conf_node_t* node = NULL;
  conf_doc_t* doc = conf_doc_load_xml("<hello name='&lt;&gt;&amp;&quot;'/>");

  ASSERT_EQ(conf_doc_get(doc, "hello.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "<>&\"");

  node = conf_node_find_child(doc->root, "hello");
  ASSERT_EQ(node != NULL, true);
  ASSERT_STREQ(conf_node_get_name(node), "hello");

  str_init(&str, 100);
  conf_doc_save_xml(doc, &str);
  ASSERT_STREQ(str.str, "<hello name=\"&lt;&gt;&amp;&quot;\"/>\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}
