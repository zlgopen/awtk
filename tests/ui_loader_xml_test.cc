#include "base/dialog.h"
#include "ui_loader/ui_builder_default.h"
#include "ui_loader/ui_builder_writer.h"
#include "ui_loader/ui_loader_xml.h"
#include "gtest/gtest.h"

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
  ASSERT_EQ(widget_count_children(builder->root), 2);

  ok = widget_lookup(builder->root, "ok", TRUE);
  cancel = widget_lookup(builder->root, "cancel", TRUE);

  ASSERT_EQ(ok != NULL, true);
  ASSERT_EQ(ok->type == WIDGET_BUTTON, true);
  ASSERT_EQ(cancel != NULL, true);
  ASSERT_EQ(cancel->type == WIDGET_LABEL, true);

  ASSERT_EQ(widget_get_child(DIALOG(builder->root)->client, 0)->type == WIDGET_BUTTON, true);
  ASSERT_EQ(widget_get_child(DIALOG(builder->root)->client, 1)->type == WIDGET_LABEL, true);
  ASSERT_EQ(strcmp(widget_get_child(DIALOG(builder->root)->client, 0)->name, "ok"), 0);
  ASSERT_EQ(strcmp(widget_get_child(DIALOG(builder->root)->client, 1)->name, "cancel"), 0);

  widget_destroy(builder->root);
}

TEST(UILoaderXML, attr) {
  widget_t* b1 = NULL;
  widget_t* b2 = NULL;
  widget_t* b3 = NULL;
  widget_t* b4 = NULL;
  ui_loader_t* loader = xml_ui_loader();
  ui_builder_t* builder = ui_builder_default();
  const char* str =
      "<dialog margin=\"0\" x=\"0\" y=\"0\" w=\"400\" h=\"300\">\
       <button name=\"b1\" x=\"10\" y=\"10\" w=\"80\" h=\"20\" />\
       <button name=\"b2\" x=\"10%\" y=\"10%\" w=\"80%\" h=\"20%\" />\
       <button name=\"b3\" x=\"center\" y=\"middle\" w=\"80\" h=\"20\" />\
       <button name=\"b4\" x=\"right\" y=\"bottom\" w=\"80\" h=\"20\" />\
  </dialog>";

  ASSERT_EQ(ui_loader_load(loader, (const uint8_t*)str, strlen(str), builder), RET_OK);

  ASSERT_EQ(builder->root != NULL, true);
  b1 = widget_lookup(builder->root, "b1", TRUE);
  ASSERT_EQ(b1 != NULL, true);
  ASSERT_EQ(b1->x, 10);
  ASSERT_EQ(b1->y, 10);
  ASSERT_EQ(b1->w, 80);
  ASSERT_EQ(b1->h, 20);
  wh_t parent_w = b1->parent->w;
  wh_t parent_h = b1->parent->h;

  b2 = widget_lookup(builder->root, "b2", TRUE);
  ASSERT_EQ(b2 != NULL, true);
  ASSERT_EQ(b2->x, 40);
  ASSERT_EQ(b2->y, parent_h * 10 / 100);
  ASSERT_EQ(b2->w, 320);
  ASSERT_EQ(b2->h, parent_h * 20 / 100);

  b3 = widget_lookup(builder->root, "b3", TRUE);
  ASSERT_EQ(b3 != NULL, true);
  ASSERT_EQ(b3->x, 160);
  ASSERT_EQ(b3->y, (parent_h - b3->h) / 2);
  ASSERT_EQ(b3->w, 80);
  ASSERT_EQ(b3->h, 20);

  b4 = widget_lookup(builder->root, "b4", TRUE);
  ASSERT_EQ(b4 != NULL, true);
  ASSERT_EQ(b4->x, parent_w - b4->w);
  ASSERT_EQ(b4->y, parent_h - b4->h);
  ASSERT_EQ(b4->w, 80);
  ASSERT_EQ(b4->h, 20);

  widget_destroy(builder->root);
}
