#include "gtest/gtest.h"
#include "rich_text/rich_text_node.h"
#include <string>

using std::string;

TEST(RichTextNode, text) {
  rich_text_font_t font;
  const char* str = "hello";
  font.name = (char*)"default";
  font.size = 12;

  rich_text_node_t* text = rich_text_text_create(&font, str);

  ASSERT_EQ(string(text->u.text.font.name), string(font.name));
  ASSERT_EQ(text->u.text.font.size, font.size);
  ASSERT_EQ(text->type, RICH_TEXT_TEXT);

  rich_text_node_destroy(text);
}

TEST(RichTextNode, image) {
  rich_text_node_t* image = rich_text_image_create("earth", 12, 24, 0);

  ASSERT_EQ(image->type, RICH_TEXT_IMAGE);
  ASSERT_EQ(string(image->u.image.name), string("earth"));
  ASSERT_EQ(image->u.image.w, 12);
  ASSERT_EQ(image->u.image.h, 24);

  rich_text_node_destroy(image);
}

TEST(RichTextNode, basic) {
  rich_text_font_t font;
  font.size = 12;
  font.name = (char*)"default";
  rich_text_node_t* node = rich_text_node_append(NULL, rich_text_text_create(&font, "good"));

  ASSERT_EQ(rich_text_node_count(node), 1);
  node = rich_text_node_append(node, rich_text_text_create(&font, "good"));
  ASSERT_EQ(rich_text_node_count(node), 2);
  node = rich_text_node_append(node, rich_text_image_create("earth", 12, 24, 0));
  ASSERT_EQ(rich_text_node_count(node), 3);

  rich_text_node_destroy(node);
}
