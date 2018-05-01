#include "base/dialog.h"
#include "ui_loader/ui_builder_default.h"
#include "ui_loader/ui_builder_writer.h"
#include "ui_loader/ui_loader_default.h"
#include "gtest/gtest.h"

#define INIT_DESC(tt, xx, yy, ww, hh) \
  desc.version = 1;                   \
  desc.layout.x = xx;                 \
  desc.layout.y = yy;                 \
  desc.layout.w = ww;                 \
  desc.layout.h = hh;                 \
  desc.type = tt

TEST(UILoader, basic) {
  uint8_t data[1024];
  wbuffer_t wbuffer;
  widget_t* ok = NULL;
  widget_t* cancel = NULL;
  widget_desc_t desc;
  ui_loader_t* loader = default_ui_loader();
  ui_builder_t* builder = ui_builder_default();
  ui_builder_t* writer = ui_builder_writer(wbuffer_init(&wbuffer, data, sizeof(data)));

  memset(&desc, 0x00, sizeof(desc));
  INIT_DESC(WIDGET_DIALOG, 0, 0, 400, 300);
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);

  INIT_DESC(WIDGET_BUTTON, 0, 0, 80, 30);
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "text", "ok"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "name", "ok"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);

  INIT_DESC(WIDGET_LABEL, 128, 0, 80, 30);
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "text", "cancel"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "name", "cancel"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);

  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);
  ASSERT_EQ(wbuffer.cursor, 122);

  ASSERT_EQ(ui_loader_load(loader, wbuffer.data, wbuffer.cursor, builder), RET_OK);
  ASSERT_EQ(builder->root->type == WIDGET_DIALOG, TRUE);
  ASSERT_EQ(widget_count_children(builder->root), 2);

  ok = widget_lookup(builder->root, "ok", TRUE);
  cancel = widget_lookup(builder->root, "cancel", TRUE);

  ASSERT_EQ(ok != NULL, true);
  ASSERT_EQ(ok->type == WIDGET_BUTTON, true);
  ASSERT_EQ(cancel != NULL, true);
  ASSERT_EQ(cancel->type == WIDGET_LABEL, true);

  ASSERT_EQ(widget_get_child(DIALOG(builder->root)->client, 0)->type == WIDGET_BUTTON, true);
  ASSERT_EQ(widget_get_child(DIALOG(builder->root)->client, 1)->type == WIDGET_LABEL, true);
  ASSERT_EQ(strcmp(widget_get_child(DIALOG(builder->root)->client, 0)->name.str, "ok"), 0);
  ASSERT_EQ(strcmp(widget_get_child(DIALOG(builder->root)->client, 1)->name.str, "cancel"), 0);

  widget_destroy(builder->root);
}

TEST(UILoader, ext) {
  uint8_t data[1024];
  wbuffer_t wbuffer;
  widget_t* ok = NULL;
  widget_t* cancel = NULL;
  widget_desc_t desc;
  ui_loader_t* loader = default_ui_loader();
  ui_builder_t* builder = ui_builder_default();
  ui_builder_t* writer = ui_builder_writer(wbuffer_init(&wbuffer, data, sizeof(data)));

  memset(&desc, 0x00, sizeof(desc));
  INIT_DESC(WIDGET_GROUP_BOX, 0, 0, 100, 200);
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);

  INIT_DESC(WIDGET_BUTTON, 10, 20, 30, 40);
  desc.layout.x_attr = X_ATTR_PERCENT;
  desc.layout.y_attr = Y_ATTR_PERCENT;
  desc.layout.w_attr = W_ATTR_PERCENT;
  desc.layout.h_attr = H_ATTR_PERCENT;
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "text", "ok"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "name", "ok"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);

  INIT_DESC(WIDGET_LABEL, 1, 2, 30, 40);
  desc.layout.x_attr = X_ATTR_CENTER;
  desc.layout.y_attr = Y_ATTR_MIDDLE;
  desc.layout.w_attr = W_ATTR_PIXEL;
  desc.layout.h_attr = H_ATTR_PIXEL;
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "text", "cancel"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "name", "cancel"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);

  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);
  ASSERT_EQ(wbuffer.cursor, 122);

  ASSERT_EQ(ui_loader_load(loader, wbuffer.data, wbuffer.cursor, builder), RET_OK);
  ASSERT_EQ(builder->root->type == WIDGET_GROUP_BOX, TRUE);
  ASSERT_EQ(widget_count_children(builder->root), 2);

  ok = widget_lookup(builder->root, "ok", TRUE);
  cancel = widget_lookup(builder->root, "cancel", TRUE);

  ASSERT_EQ(ok != NULL, true);
  ASSERT_EQ(ok->type == WIDGET_BUTTON, true);
  ASSERT_EQ(cancel != NULL, true);
  ASSERT_EQ(cancel->type == WIDGET_LABEL, true);

  ASSERT_EQ(ok->x, 10);
  ASSERT_EQ(ok->y, 40);
  ASSERT_EQ(ok->w, 30);
  ASSERT_EQ(ok->h, 80);

  ASSERT_EQ(cancel->w, 30);
  ASSERT_EQ(cancel->h, 40);
  ASSERT_EQ(cancel->x, 36);
  ASSERT_EQ(cancel->y, 82);

  widget_destroy(builder->root);
}
