#include "gtest/gtest.h"
#include "base/dialog.h"
#include "ui_loader/ui_loader_xml.h"
#include "ui_loader/ui_builder_writer.h"
#include "ui_loader/ui_builder_default.h"

TEST(UILoaderXML, basic) {
  widget_t* ok = NULL;
  widget_t* cancel = NULL;
  ui_loader_t* loader = xml_ui_loader();
  ui_builder_t* builder = ui_builder_default();
  const char* str =
      "<dialog x=\"0\" y=\"0\" w=\"400\" h=\"300\">\
                     <button x=\"0\" y=\"0\" w=\"80\" h=\"30\" name=\"ok\" text=\"ok\" />\
                     <label x=\"128\" y=\"0\" w=\"80\" h=\"30\" name=\"cancel\" text=\"cancel\" />\
                     </dialog>";

  ASSERT_EQ(ui_loader_load(loader, (const uint8_t*)str, strlen(str), builder), RET_OK);

  ASSERT_EQ(builder->root->type == WIDGET_DIALOG, TRUE);
  ASSERT_EQ(widget_count_children(builder->root), 1);

  ok = widget_lookup(builder->root, "ok");
  cancel = widget_lookup(builder->root, "cancel");

  ASSERT_EQ(ok != NULL, true);
  ASSERT_EQ(ok->type == WIDGET_BUTTON, true);
  ASSERT_EQ(cancel != NULL, true);
  ASSERT_EQ(cancel->type == WIDGET_LABEL, true);

  ASSERT_EQ(widget_get_child(DIALOG(builder->root)->client, 0)->type == WIDGET_BUTTON, true);
  ASSERT_EQ(widget_get_child(DIALOG(builder->root)->client, 1)->type == WIDGET_LABEL, true);

  widget_destroy(builder->root);
}
