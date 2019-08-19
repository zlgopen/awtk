#include "base/dialog.h"
#include "gtest/gtest.h"
#include "ui_loader/ui_xml_writer.h"
#include "ui_loader/ui_loader_default.h"
#include "ui_loader/ui_builder_default.h"

#include <string>

using std::string;

#define INIT_DESC(tt, xx, yy, ww, hh) \
  desc.layout.x = xx;                 \
  desc.layout.y = yy;                 \
  desc.layout.w = ww;                 \
  desc.layout.h = hh;                 \
  strncpy(desc.type, tt, TK_NAME_LEN);

TEST(UIXmlWriter, basic1) {
  str_t str;
  widget_desc_t desc;
  ui_xml_writer_t ui_xml_writer;
  ui_builder_t* writer = ui_xml_writer_init(&ui_xml_writer, str_init(&str, 1024));

  memset(&desc, 0x00, sizeof(desc));
  INIT_DESC("button", 0, 0, 80, 30);
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "text", "ok"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "name", "ok"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);

  const char* result =
      "<button x=\"0\" y=\"0\" w=\"80\" h=\"30\" text=\"ok\" name=\"ok\">\n</button>\n";
  ASSERT_EQ(string(str.str), string(result));

  str_reset(&str);
}

TEST(UIXmlWriter, basic2) {
  str_t str;
  widget_desc_t desc;
  ui_xml_writer_t ui_xml_writer;
  ui_builder_t* writer = ui_xml_writer_init(&ui_xml_writer, str_init(&str, 1024));

  memset(&desc, 0x00, sizeof(desc));
  INIT_DESC("dialog_client", 0, 0, 200, 300);
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);

  INIT_DESC("button", 0, 0, 80, 30);
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "text", "ok"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "name", "ok"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);

  INIT_DESC("label", 128, 0, 80, 30);
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "text", "cancel"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "name", "cancel"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);

  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);

  const char* result =
      "<dialog_client x=\"0\" y=\"0\" w=\"200\" h=\"300\">\n <button x=\"0\" y=\"0\" w=\"80\" "
      "h=\"30\" text=\"ok\" name=\"ok\">\n </button>\n <label x=\"128\" y=\"0\" w=\"80\" h=\"30\" "
      "text=\"cancel\" name=\"cancel\">\n </label>\n</dialog_client>\n";
  ASSERT_EQ(string(str.str), string(result));

  str_reset(&str);
}
