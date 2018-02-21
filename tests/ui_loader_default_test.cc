#include "gtest/gtest.h"
#include "base/dialog.h"
#include "ui_loader/ui_loader_default.h"
#include "ui_loader/ui_builder_writer.h"
#include "ui_loader/ui_builder_default.h"

TEST(UILoader, basic) {
  uint8_t data[1024];
  wbuffer_t wbuffer;
  widget_t* ok = NULL;
  widget_t* cancel = NULL;
  ui_loader_t* loader = default_ui_loader();
  ui_builder_t* builder = ui_builder_default();
  ui_builder_t* writer = ui_builder_writer(wbuffer_init(&wbuffer, data, sizeof(data)));

  ASSERT_EQ(ui_builder_on_widget_start(writer, WIDGET_DIALOG, 0, 0, 400, 300), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);

  ASSERT_EQ(ui_builder_on_widget_start(writer, WIDGET_BUTTON, 0, 0, 80, 30), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "text", "ok"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "name", "ok"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);

  ASSERT_EQ(ui_builder_on_widget_start(writer, WIDGET_LABEL, 128, 0, 80, 30), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "text", "cancel"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "name", "cancel"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);

  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);
  ASSERT_EQ(wbuffer.cursor, 110);

  ASSERT_EQ(ui_loader_load(loader, wbuffer.data, wbuffer.cursor, builder), RET_OK);
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
