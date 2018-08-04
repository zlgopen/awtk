#include "gtest/gtest.h"
#include "rich_text/rich_text_render_node.h"
#include <string>

using std::string;

TEST(RichTextRenderNode, basic) {
  rich_text_font_t font;
  font.size = 12;
  font.name = (char*)"default";
  rich_text_node_t* node = rich_text_text_create(&font, "good");
  rich_text_render_node_t* render_node =
      rich_text_render_node_append(NULL, rich_text_render_node_create(node));
  ASSERT_EQ(rich_text_render_node_count(render_node), 1);

  render_node = rich_text_render_node_append(render_node, rich_text_render_node_create(node));
  ASSERT_EQ(rich_text_render_node_count(render_node), 2);

  render_node = rich_text_render_node_append(render_node, rich_text_render_node_create(node));
  ASSERT_EQ(rich_text_render_node_count(render_node), 3);

  rich_text_node_destroy(node);
  rich_text_render_node_destroy(render_node);
}
