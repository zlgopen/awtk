#include "gtest/gtest.h"
#include "xml/xml_parser.h"

#include <string>
using std::string;

static string s_log;

static void xml_gen_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  uint32_t i = 0;
  s_log += "<" + string(tag);

  while (attrs[i] != NULL) {
    if (i == 0) {
      s_log += ' ';
    }
    s_log += attrs[i];
    s_log += string("=\"") + attrs[i + 1] + string("\" ");
    i += 2;
  }

  s_log += ">";

  return;
}

static void xml_gen_on_end(XmlBuilder* thiz, const char* tag) {
  (void)thiz;
  s_log += "</" + string(tag) + ">";

  return;
}

static void xml_gen_on_error(XmlBuilder* thiz, int line, int row, const char* message) {
  log_debug("%d:%d %s\n", line, row, message);
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
  memset(&b, 0x00, sizeof(XmlBuilder));

  b.on_start = xml_gen_on_start;
  b.on_end = xml_gen_on_end;
  b.on_text = xml_gen_on_text;
  b.on_error = xml_gen_on_error;
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

TEST(XmlParser, not_trim_text) {
  XmlBuilder b;
  XmlParser* p = xml_parser_create();
  xml_parser_set_builder(p, builder_init(b));
  xml_parser_set_trim_text(p, FALSE);

  test_str_ex(p, "<a> 123</a>", "<a> 123</a>");
  test_str_ex(p, "<a>123 </a>", "<a>123 </a>");
  test_str_ex(p, "<a> 123 </a>", "<a> 123 </a>");
  test_str_ex(p, "<a>\n 123 </a>", "<a>\n 123 </a>");
  test_str_ex(p, "<a>\n 123 \n</a>", "<a>\n 123 \n</a>");
  test_str_ex(p, "<a>\n <b>123</b> \n</a>", "<a>\n <b>123</b> \n</a>");

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

TEST(XmlParser, space1) {
  XmlBuilder b;
  XmlParser* p = xml_parser_create();
  xml_parser_set_builder(p, builder_init(b));

  test_str_ex(p, "<test a1=\" 1 \" ></test>", "<test a1=\" 1 \" ></test>");

  xml_parser_destroy(p);
}

TEST(XmlParser, space2) {
  XmlBuilder b;
  XmlParser* p = xml_parser_create();
  xml_parser_set_builder(p, builder_init(b));

  test_str_ex(p, "< test a1 =\" 1 \"></ test >", "<test a1=\" 1 \" ></test>");

  xml_parser_destroy(p);
}

/* 捕获 on_error 的 line/col/message，用于断言报错位置(独立于 s_log) */
static int s_err_line = 0;
static int s_err_col = 0;
static string s_err_msg;

static void xml_capture_on_error(XmlBuilder* thiz, int line, int col, const char* message) {
  (void)thiz;
  s_err_line = line;
  s_err_col = col;
  s_err_msg = message ? string(message) : "";
}

static XmlBuilder* capture_builder_init(XmlBuilder& b) {
  memset(&b, 0x00, sizeof(XmlBuilder));

  b.on_start = xml_gen_on_start;
  b.on_end = xml_gen_on_end;
  b.on_text = xml_gen_on_text;
  b.on_error = xml_capture_on_error;
  b.destroy = xml_gen_destroy;

  return &b;
}

static void parse_and_reset(XmlParser* p, const char* str) {
  s_err_line = 0;
  s_err_col = 0;
  s_err_msg.clear();
  s_log.clear();
  xml_parser_parse(p, str, strlen(str));
}

/* 第 2 行的重复属性：列号应指向重复的 h，而非其后的 '='
 * (修复前 last_line_start 指向换行符，列号偏 +1 报 56；修复后报 55) */
TEST(XmlParser, duplicate_attr_col_line2) {
  XmlBuilder b;
  XmlParser* p = xml_parser_create();
  xml_parser_set_builder(p, capture_builder_init(b));

  /* 第 2 行前导两个空格，第二个 h 位于第 2 行第 55 列 */
  const char* xml =
      "<window name=\"page0\" v-model=\"page0_view_model\">\n"
      "  <button name=\"button\" x=\"146\" y=\"82\" w=\"100\" h=\"36\" h=\"37\" text=\"Button\" "
      "style.normal.icon=\"waef\"/>\n"
      "</window>";

  parse_and_reset(p, xml);

  ASSERT_EQ(s_err_line, 2);
  ASSERT_EQ(s_err_col, 55);
  ASSERT_NE(s_err_msg.find("duplicate attribute"), string::npos);
  ASSERT_NE(s_err_msg.find("'h'"), string::npos);

  xml_parser_destroy(p);
}

/* 第 1 行的重复属性：列号也应正确(第 1 行 last_line_start=0 本就正确，
 * 此用例作回归保护，确保 +1 修复未影响第 1 行) */
TEST(XmlParser, duplicate_attr_col_line1) {
  XmlBuilder b;
  XmlParser* p = xml_parser_create();
  xml_parser_set_builder(p, capture_builder_init(b));

  /* <a h="1" h="2"/> 中第二个 h 位于第 1 行第 10 列 */
  const char* xml = "<a h=\"1\" h=\"2\"/>";

  parse_and_reset(p, xml);

  ASSERT_EQ(s_err_line, 1);
  ASSERT_EQ(s_err_col, 10);
  ASSERT_NE(s_err_msg.find("duplicate attribute"), string::npos);
  ASSERT_NE(s_err_msg.find("'h'"), string::npos);

  xml_parser_destroy(p);
}
