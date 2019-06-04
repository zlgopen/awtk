#include "gtest/gtest.h"
#include "xml/xml_parser.h"

#include <string>
using std::string;

static string s_log;

static void xml_gen_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  s_log += "<" + string(tag) + ">";

  return;
}

static void xml_gen_on_end(XmlBuilder* thiz, const char* tag) {
  (void)thiz;
  s_log += "</" + string(tag) + ">";

  return;
}

static void xml_gen_on_text(XmlBuilder* thiz, const char* text, size_t length) {
  char str[1024];
  assert(length < sizeof(str));

  memset(str, 0x00, sizeof(str));
  strncpy(str, text, length);
  s_log += str;

  return;
}

static void xml_gen_destroy(XmlBuilder* thiz) {
  (void)thiz;
  return;
}

static XmlBuilder* builder_init(XmlBuilder& b) {
  b.on_start = xml_gen_on_start;
  b.on_end = xml_gen_on_end;
  b.on_text = xml_gen_on_text;
  b.destroy = xml_gen_destroy;

  return &(b);
}

static void test_str(XmlParser* p, const char* str) {
  s_log = "";
  xml_parser_parse(p, str, strlen(str));
  ASSERT_EQ(s_log, str);
}

static void test_str_ex(XmlParser* p, const char* str, const char* expected) {
  s_log = "";
  xml_parser_parse(p, str, strlen(str));
  ASSERT_EQ(s_log, expected);
}

TEST(XmlParser, cdata) {
  XmlBuilder b;
  XmlParser* p = xml_parser_create();
  xml_parser_set_builder(p, builder_init(b));

  test_str_ex(p, "<a><![CDATA[123]]></a>", "<a>123</a>");
  test_str_ex(p, "<a><![CDATA[<abc>]]></a>", "<a><abc></a>");
  test_str_ex(p, "<a>123<![CDATA[<abc>]]></a>", "<a>123<abc></a>");
  test_str_ex(p, "<a>123<![CDATA[<abc>]]></a>", "<a>123<abc></a>");
  test_str_ex(p, "<a>123<![CDATA[<abc>]]>1</a>", "<a>123<abc>1</a>");
  test_str_ex(p, "<a>123<![CDATA[<abc>]]>123</a>", "<a>123<abc>123</a>");
  test_str_ex(p, "<a>   <![CDATA[123]]>  </a>", "<a>123</a>");
  test_str_ex(p, "<a>  \n <![CDATA[123]]>  </a>", "<a>123</a>");
  test_str_ex(p, "<a>  \n <![CDATA[123]]> \n </a>", "<a>123</a>");

  xml_parser_destroy(p);
}

TEST(XmlParser, basic) {
  XmlBuilder b;
  XmlParser* p = xml_parser_create();
  xml_parser_set_builder(p, builder_init(b));

  test_str(p, "<a></a>");
  test_str_ex(p, "<a>   </a>", "<a></a>");
  test_str_ex(p, "<a> \n  </a>", "<a></a>");
  test_str_ex(p, "<a> \n123\n  </a>", "<a>123</a>");
  test_str(p, "<a>test</a>");
  test_str(p, "<a><b>test</b></a>");
  test_str(p, "<a><b>test</b><c>test1</c></a>");

  xml_parser_destroy(p);
}
