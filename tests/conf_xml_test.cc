
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
  conf_doc_t* doc = conf_doc_load_xml(
      "<hello name='abc'><world name='cde' value='123'/><foo name='oo' value='456'/></hello>");

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
  ASSERT_STREQ(str.str,
               "<hello name=\"abc\">\n  <world name=\"cde\" value=\"123\"/>\n  <foo name=\"oo\" "
               "value=\"456\"/>\n</hello>\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Xml, basic3) {
  value_t v;
  str_t str;
  conf_doc_t* doc = conf_doc_load_xml(
      "<hello name='abc'><world name='cde' value='123'/><foo name='oo' value='456'/></hello><awtk "
      "name='ttt'/>");

  ASSERT_EQ(conf_doc_get(doc, "awtk.name", &v), RET_OK);
  ASSERT_STREQ(value_str(&v), "ttt");

  str_init(&str, 100);
  conf_doc_save_xml(doc, &str);
  ASSERT_STREQ(str.str,
               "<hello name=\"abc\">\n  <world name=\"cde\" value=\"123\"/>\n  <foo name=\"oo\" "
               "value=\"456\"/>\n</hello>\n<awtk name=\"ttt\"/>\n");
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

TEST(Xml, basic5) {
  str_t str;
  conf_doc_t* doc = conf_doc_load_xml("<group_box><style/></group_box>");

  str_init(&str, 100);
  conf_doc_save_xml(doc, &str);
  ASSERT_STREQ(str.str, "<group_box>\n  <style/>\n</group_box>\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Xml, basic6) {
  str_t str;
  conf_doc_t* doc = conf_doc_load_xml("<group_box>1</group_box>");

  str_init(&str, 100);
  conf_doc_save_xml(doc, &str);
  ASSERT_STREQ(str.str, "<group_box>1</group_box>\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Xml, basic7) {
  str_t str;
  conf_doc_t* doc = conf_doc_load_xml("<group_box><child>abc</child></group_box>");

  str_init(&str, 100);
  conf_doc_save_xml(doc, &str);
  ASSERT_STREQ(str.str, "<group_box>\n  <child>abc</child>\n</group_box>\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Xml, cdata) {
  str_t str;
  conf_doc_t* doc = conf_doc_load_xml("<group_box><![CDATA[abc]]></group_box>");

  ASSERT_STREQ(conf_doc_get_str(doc, "group_box." CONF_XML_TEXT, NULL), "abc");
  ASSERT_EQ(conf_doc_set_str(doc, "group_box." CONF_XML_TEXT, "<![CDATA[abc]]>"), RET_OK);

  str_init(&str, 100);
  conf_doc_save_xml(doc, &str);
  ASSERT_STREQ(str.str, "<group_box><![CDATA[abc]]></group_box>\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Xml, text0) {
  str_t str;
  conf_doc_t* doc = conf_doc_load_xml("<group_box>123</group_box>");

  ASSERT_STREQ(conf_doc_get_str(doc, "group_box." CONF_XML_TEXT, NULL), "123");
  str_init(&str, 100);
  conf_doc_save_xml(doc, &str);
  ASSERT_STREQ(str.str, "<group_box>123</group_box>\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Xml, text1) {
  str_t str;
  conf_doc_t* doc = conf_doc_load_xml("<group_box>123&lt;abc</group_box>");

  ASSERT_STREQ(conf_doc_get_str(doc, "group_box." CONF_XML_TEXT, NULL), "123<abc");
  str_init(&str, 100);
  conf_doc_save_xml(doc, &str);
  ASSERT_STREQ(str.str, "<group_box>123&lt;abc</group_box>\n");
  str_reset(&str);
  conf_doc_destroy(doc);
}

TEST(Xml, buff) {

  wbuffer_t wb;
  tk_object_t* conf = conf_xml_create();
  ASSERT_NE(conf, (tk_object_t*)NULL);
  ASSERT_EQ(tk_object_set_prop_int(conf, "awtk.value", 123), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(conf, "awtk.value", 0), 123);

  ASSERT_EQ(conf_xml_save_to_buff(conf, &wb), RET_OK);
  TK_OBJECT_UNREF(conf);

  conf = conf_xml_load_from_buff(wb.data, wb.cursor, FALSE);
  ASSERT_EQ(tk_object_get_prop_int(conf, "awtk.value", 0), 123);
  TK_OBJECT_UNREF(conf);

  wbuffer_deinit(&wb);
}
