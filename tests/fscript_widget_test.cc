#include "gtest/gtest.h"

#include "tkc/fscript.h"
#include "tkc/object_default.h"
#include "base/window.h"
#include "base/window_manager.h"
#include "base/object_widget.h"

#include "widgets/view.h"
#include "widgets/edit.h"
#include "widgets/button.h"
#include "widgets/progress_bar.h"

TEST(FScriptWidget, basic) {
  value_t v;
  tk_object_t* obj = object_default_create();
  widget_t* w = progress_bar_create(NULL, 0, 0, 100, 20);

  widget_set_value_int(w, 20);
  tk_object_set_prop_pointer(obj, STR_PROP_SELF, w);
  fscript_eval(obj, "widget_add_value('self', 20)", &v);
  ASSERT_EQ(widget_get_value_int(w), 40);
  value_reset(&v);

  fscript_eval(obj, "widget_set('self', 'value', 20)", &v);
  ASSERT_EQ(widget_get_value_int(w), 20);
  value_reset(&v);

  fscript_eval(obj, "widget_get('self', 'value')", &v);
  ASSERT_EQ(value_int(&v), 20);
  value_reset(&v);

  widget_unref(w);
  TK_OBJECT_UNREF(obj);
}

TEST(FScriptWidget, unload_image) {
  value_t v;
  tk_object_t* obj = object_default_create();
  widget_t* w = progress_bar_create(NULL, 0, 0, 100, 20);

  tk_object_set_prop_pointer(obj, STR_PROP_SELF, w);
  fscript_eval(obj, "widget_unload_image('check')", &v);
  value_reset(&v);

  widget_unref(w);
  TK_OBJECT_UNREF(obj);
}

TEST(FScriptWidget, layout) {
  value_t v;
  wh_t w = 10;
  wh_t h = 10;
  widget_t* wm = window_manager();
  tk_object_t* obj = object_default_create();
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* widget = progress_bar_create(win, 0, 0, w, h);

  widget_set_self_layout(widget, "default(x=c,y=m)");
  tk_object_set_prop_pointer(obj, STR_PROP_SELF, widget);
  fscript_eval(obj, "widget_layout('self')", &v);
  ASSERT_EQ(widget->x, (wm->w - w) / 2);
  ASSERT_EQ(widget->y, (wm->h - h) / 2);
  ASSERT_EQ(widget->w, w);
  ASSERT_EQ(widget->h, h);

  ((window_base_t*)win)->need_relayout = FALSE;
  widget_set_self_layout(widget, "default(x=c:10,y=m:10)");
  tk_object_set_prop_pointer(obj, STR_PROP_SELF, widget);
  fscript_eval(obj, "widget_request_relayout('self')", &v);
  ASSERT_EQ(widget->x, (wm->w - w) / 2);
  ASSERT_EQ(widget->y, (wm->h - h) / 2);
  ASSERT_EQ(widget->w, w);
  ASSERT_EQ(widget->h, h);
  ASSERT_EQ(((window_base_t*)win)->need_relayout, TRUE);

  widget_close_window(win);
  TK_OBJECT_UNREF(obj);
}

TEST(FScriptWidget, ulen) {
  value_t v;
  widget_t* w = edit_create(NULL, 0, 0, 100, 20);
  tk_object_t* obj = object_widget_create(w);

  widget_set_text(w, L"中文");
  fscript_eval(obj, "ulen(text)", &v);
  ASSERT_EQ(value_int(&v), 2);
  value_reset(&v);

  widget_set_text(w, L"abc");
  fscript_eval(obj, "ulen(text)", &v);
  ASSERT_EQ(value_int(&v), 3);
  value_reset(&v);

  widget_destroy(w);
  TK_OBJECT_UNREF(obj);
}

TEST(FScriptWidget, foreach) {
  value_t v;
  tk_object_t* obj = object_default_create();
  widget_t* w = view_create(NULL, 0, 0, 320, 240);
  widget_t* children[3] = {NULL};
  tk_object_set_prop_pointer(obj, STR_PROP_SELF, w);

  children[0] = edit_create(NULL, 0, 0, 100, 20);
  widget_add_child(w, children[0]);

  children[1] = button_create(NULL, 0, 20, 90, 30);
  widget_add_child(w, children[1]);

  children[2] = view_create(NULL, 0, 50, 100, 100);
  widget_add_child(w, children[2]);

  fscript_eval(obj, "widget_count_children('self')", &v);
  ASSERT_EQ(value_int32(&v), 3);
  value_reset(&v);

  for (size_t i = 0; i < ARRAY_SIZE(children); i++) {
    char script[64] = {0};
    tk_snprintf(script, ARRAY_SIZE(script) - 1, "widget_get_child('self', %d)", i);
    fscript_eval(obj, script, &v);
    ASSERT_EQ(tk_object_get_prop_pointer(value_object(&v), OBJECT_WIDGET_PROP_NATIVE_WIDGET),
              children[i]);
    value_reset(&v);
  }

  widget_unref(w);
  TK_OBJECT_UNREF(obj);
}
