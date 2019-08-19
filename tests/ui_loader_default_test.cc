#include "base/dialog.h"
#include "ui_loader/ui_builder_default.h"
#include "ui_loader/ui_binary_writer.h"
#include "ui_loader/ui_loader_default.h"
#include "gtest/gtest.h"

#define INIT_DESC(tt, xx, yy, ww, hh) \
  desc.layout.x = xx;                 \
  desc.layout.y = yy;                 \
  desc.layout.w = ww;                 \
  desc.layout.h = hh;                 \
  strncpy(desc.type, tt, TK_NAME_LEN);

TEST(UILoader, basic) {
  uint8_t data[1024];
  wbuffer_t wbuffer;
  widget_t* ok = NULL;
  widget_t* cancel = NULL;
  widget_desc_t desc;
  ui_binary_writer_t ui_binary_writer;
  ui_loader_t* loader = default_ui_loader();
  ui_builder_t* builder = ui_builder_default("");
  ui_builder_t* writer =
      ui_binary_writer_init(&ui_binary_writer, wbuffer_init(&wbuffer, data, sizeof(data)));

  memset(&desc, 0x00, sizeof(desc));
  INIT_DESC("dialog", 0, 0, 400, 300);
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);

  INIT_DESC("dialog_title", 0, 0, 200, 300);
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);

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

  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);

  // ASSERT_EQ(wbuffer.cursor, 170);

  ASSERT_EQ(ui_loader_load(loader, wbuffer.data, wbuffer.cursor, builder), RET_OK);
  ASSERT_EQ(tk_str_eq(widget_get_type(builder->root), WIDGET_TYPE_DIALOG), TRUE);
  ASSERT_EQ(widget_count_children(builder->root), 2);

  ok = widget_lookup(builder->root, "ok", TRUE);
  cancel = widget_lookup(builder->root, "cancel", TRUE);

  ASSERT_EQ(ok != NULL, true);
  ASSERT_EQ(tk_str_eq(widget_get_type(ok), WIDGET_TYPE_BUTTON), true);
  ASSERT_EQ(cancel != NULL, true);
  ASSERT_EQ(tk_str_eq(widget_get_type(cancel), WIDGET_TYPE_LABEL), true);

  ASSERT_EQ(tk_str_eq(widget_get_type(widget_get_child(DIALOG(builder->root)->client, 0)),
                      WIDGET_TYPE_BUTTON),
            true);
  ASSERT_EQ(tk_str_eq(widget_get_type(widget_get_child(DIALOG(builder->root)->client, 1)),
                      WIDGET_TYPE_LABEL),
            true);
  ASSERT_EQ(strcmp(widget_get_child(DIALOG(builder->root)->client, 0)->name, "ok"), 0);
  ASSERT_EQ(strcmp(widget_get_child(DIALOG(builder->root)->client, 1)->name, "cancel"), 0);

  widget_destroy(builder->root);
}

TEST(UILoader, ext) {
  uint8_t data[1024];
  wbuffer_t wbuffer;
  widget_t* ok = NULL;
  widget_t* cancel = NULL;
  widget_desc_t desc;
  ui_binary_writer_t ui_binary_writer;
  ui_loader_t* loader = default_ui_loader();
  ui_builder_t* builder = ui_builder_default("");
  ui_builder_t* writer =
      ui_binary_writer_init(&ui_binary_writer, wbuffer_init(&wbuffer, data, sizeof(data)));

  memset(&desc, 0x00, sizeof(desc));
  INIT_DESC("group_box", 0, 0, 100, 200);
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);

  INIT_DESC("button", 10, 20, 30, 40);
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "text", "ok"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "name", "ok"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);

  INIT_DESC("label", 1, 2, 30, 40);
  ASSERT_EQ(ui_builder_on_widget_start(writer, &desc), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "text", "cancel"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop(writer, "name", "cancel"), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_prop_end(writer), RET_OK);
  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);

  ASSERT_EQ(ui_builder_on_widget_end(writer), RET_OK);
  ASSERT_EQ(wbuffer.cursor, 194);

  ASSERT_EQ(ui_loader_load(loader, wbuffer.data, wbuffer.cursor, builder), RET_OK);
  ASSERT_EQ(tk_str_eq(widget_get_type(builder->root), WIDGET_TYPE_GROUP_BOX), TRUE);
  ASSERT_EQ(widget_count_children(builder->root), 2);

  ok = widget_lookup(builder->root, "ok", TRUE);
  cancel = widget_lookup(builder->root, "cancel", TRUE);

  ASSERT_EQ(ok != NULL, true);
  ASSERT_EQ(tk_str_eq(widget_get_type(ok), WIDGET_TYPE_BUTTON), true);
  ASSERT_EQ(cancel != NULL, true);
  ASSERT_EQ(tk_str_eq(widget_get_type(cancel), WIDGET_TYPE_LABEL), true);

  ASSERT_EQ(ok->x, 10);
  ASSERT_EQ(ok->y, 20);
  ASSERT_EQ(ok->w, 30);
  ASSERT_EQ(ok->h, 40);

  ASSERT_EQ(cancel->x, 1);
  ASSERT_EQ(cancel->y, 2);
  ASSERT_EQ(cancel->w, 30);
  ASSERT_EQ(cancel->h, 40);

  widget_destroy(builder->root);
}
