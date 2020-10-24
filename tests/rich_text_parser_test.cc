#include "gtest/gtest.h"
#include "rich_text/rich_text_parser.h"

#include <string>

using std::string;

TEST(RichTextParser, basic) {
  const char* str = "text";
  rich_text_node_t* node = rich_text_parse(str, strlen(str), NULL, TK_DEFAULT_FONT_SIZE,
                                           color_init(0, 0, 0, 0xff), ALIGN_V_BOTTOM);

  ASSERT_EQ(node->type, RICH_TEXT_TEXT);
  ASSERT_EQ(rich_text_node_count(node), 1);
  ASSERT_EQ(wcscmp(node->u.text.text, L"text"), 0);

  rich_text_node_destroy(node);
}

TEST(RichTextParser, font) {
  const char* str = "<font color=\"#123456\" size=\"12\" name=\"test\">text</font>";
  rich_text_node_t* node = rich_text_parse(str, strlen(str), NULL, TK_DEFAULT_FONT_SIZE,
                                           color_init(0, 0, 0, 0xff), ALIGN_V_BOTTOM);

  ASSERT_EQ(node->type, RICH_TEXT_TEXT);
  ASSERT_EQ(rich_text_node_count(node), 1);
  ASSERT_EQ(wcscmp(node->u.text.text, L"text"), 0);
  ASSERT_EQ(node->u.text.font.size, 12);
  ASSERT_EQ(node->u.text.font.color.rgba.r, 0x12);
  ASSERT_EQ(node->u.text.font.color.rgba.g, 0x34);
  ASSERT_EQ(node->u.text.font.color.rgba.b, 0x56);
  ASSERT_EQ(node->u.text.font.color.rgba.a, 0xff);
  ASSERT_EQ(node->u.text.font.name, string("test"));

  rich_text_node_destroy(node);
}

TEST(RichTextParser, b) {
  const char* str = "<b>text</b>";
  rich_text_node_t* node = rich_text_parse(str, strlen(str), NULL, TK_DEFAULT_FONT_SIZE,
                                           color_init(0, 0, 0, 0xff), ALIGN_V_BOTTOM);

  ASSERT_EQ(node->type, RICH_TEXT_TEXT);
  ASSERT_EQ(rich_text_node_count(node), 1);
  ASSERT_EQ(wcscmp(node->u.text.text, L"text"), 0);
  ASSERT_EQ(node->u.text.font.bold, TRUE);

  rich_text_node_destroy(node);
}

TEST(RichTextParser, i) {
  const char* str = "<i>text</i>";
  rich_text_node_t* node = rich_text_parse(str, strlen(str), NULL, TK_DEFAULT_FONT_SIZE,
                                           color_init(0, 0, 0, 0xff), ALIGN_V_BOTTOM);

  ASSERT_EQ(node->type, RICH_TEXT_TEXT);
  ASSERT_EQ(rich_text_node_count(node), 1);
  ASSERT_EQ(wcscmp(node->u.text.text, L"text"), 0);
  ASSERT_EQ(node->u.text.font.italic, TRUE);

  rich_text_node_destroy(node);
}

TEST(RichTextParser, u) {
  const char* str = "<u>text</u>";
  rich_text_node_t* node = rich_text_parse(str, strlen(str), NULL, TK_DEFAULT_FONT_SIZE,
                                           color_init(0, 0, 0, 0xff), ALIGN_V_BOTTOM);

  ASSERT_EQ(node->type, RICH_TEXT_TEXT);
  ASSERT_EQ(rich_text_node_count(node), 1);
  ASSERT_EQ(wcscmp(node->u.text.text, L"text"), 0);
  ASSERT_EQ(node->u.text.font.underline, TRUE);

  rich_text_node_destroy(node);
}

TEST(RichTextParser, image) {
  const char* str = "<image name=\"earth\" w=\"12\" h=\"34\"/>text";
  rich_text_node_t* node = rich_text_parse(str, strlen(str), NULL, TK_DEFAULT_FONT_SIZE,
                                           color_init(0, 0, 0, 0xff), ALIGN_V_BOTTOM);

  ASSERT_EQ(node->type, RICH_TEXT_IMAGE);
  ASSERT_EQ(node->u.image.name, string("earth"));
  ASSERT_EQ(node->u.image.w, 12);
  ASSERT_EQ(node->u.image.h, 34);

  ASSERT_EQ(rich_text_node_count(node), 2);

  ASSERT_EQ(wcscmp(node->next->u.text.text, L"text"), 0);

  rich_text_node_destroy(node);
}

TEST(RichTextParser, bu) {
  const char* str = "<b><u>text</u></b>";
  rich_text_node_t* node = rich_text_parse(str, strlen(str), NULL, TK_DEFAULT_FONT_SIZE,
                                           color_init(0, 0, 0, 0xff), ALIGN_V_BOTTOM);

  ASSERT_EQ(node->type, RICH_TEXT_TEXT);
  ASSERT_EQ(rich_text_node_count(node), 1);
  ASSERT_EQ(wcscmp(node->u.text.text, L"text"), 0);
  ASSERT_EQ(node->u.text.font.bold, TRUE);
  ASSERT_EQ(node->u.text.font.underline, TRUE);

  rich_text_node_destroy(node);
}

TEST(RichTextParser, bui) {
  const char* str = "<b><u><i>text</i></u></b>";
  rich_text_node_t* node = rich_text_parse(str, strlen(str), NULL, TK_DEFAULT_FONT_SIZE,
                                           color_init(0, 0, 0, 0xff), ALIGN_V_BOTTOM);

  ASSERT_EQ(node->type, RICH_TEXT_TEXT);
  ASSERT_EQ(rich_text_node_count(node), 1);
  ASSERT_EQ(wcscmp(node->u.text.text, L"text"), 0);
  ASSERT_EQ(node->u.text.font.bold, TRUE);
  ASSERT_EQ(node->u.text.font.italic, TRUE);
  ASSERT_EQ(node->u.text.font.underline, TRUE);

  rich_text_node_destroy(node);
}

TEST(RichTextParser, bui1) {
  const char* str = "<b>bold<u>underline<i>italic</i></u></b>";
  rich_text_node_t* node = rich_text_parse(str, strlen(str), NULL, TK_DEFAULT_FONT_SIZE,
                                           color_init(0, 0, 0, 0xff), ALIGN_V_BOTTOM);

  ASSERT_EQ(node->type, RICH_TEXT_TEXT);
  ASSERT_EQ(node->u.text.font.bold, TRUE);
  ASSERT_EQ(node->u.text.font.italic, FALSE);
  ASSERT_EQ(node->u.text.font.underline, FALSE);
  ASSERT_EQ(rich_text_node_count(node), 3);

  ASSERT_EQ(node->next->u.text.font.bold, TRUE);
  ASSERT_EQ(node->next->u.text.font.italic, FALSE);
  ASSERT_EQ(node->next->u.text.font.underline, TRUE);

  ASSERT_EQ(node->next->next->u.text.font.bold, TRUE);
  ASSERT_EQ(node->next->next->u.text.font.italic, TRUE);
  ASSERT_EQ(node->next->next->u.text.font.underline, TRUE);

  rich_text_node_destroy(node);
}

TEST(RichTextParser, bui2) {
  const char* str = "<b><u><i>italic</i>underline</u>bold</b>";
  rich_text_node_t* node = rich_text_parse(str, strlen(str), NULL, TK_DEFAULT_FONT_SIZE,
                                           color_init(0, 0, 0, 0xff), ALIGN_V_BOTTOM);

  ASSERT_EQ(node->type, RICH_TEXT_TEXT);
  ASSERT_EQ(node->u.text.font.bold, TRUE);
  ASSERT_EQ(node->u.text.font.italic, TRUE);
  ASSERT_EQ(node->u.text.font.underline, TRUE);
  ASSERT_EQ(rich_text_node_count(node), 3);

  ASSERT_EQ(node->next->u.text.font.bold, TRUE);
  ASSERT_EQ(node->next->u.text.font.underline, TRUE);
  ASSERT_EQ(node->next->u.text.font.italic, FALSE);

  ASSERT_EQ(node->next->next->u.text.font.bold, TRUE);
  ASSERT_EQ(node->next->next->u.text.font.italic, FALSE);
  ASSERT_EQ(node->next->next->u.text.font.underline, FALSE);

  rich_text_node_destroy(node);
}
