#include "tkc/fscript.h"
#include "tkc/object_default.h"
#include "gtest/gtest.h"
#include "widgets/button.h"
#include "widgets/progress_bar.h"
#include "base/window.h"
#include "base/window_manager.h"

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

TEST(FScriptWidget, layout) {
  value_t v;
  wh_t w = 10;
  wh_t h = 10;
  widget_t* wm = window_manager();
  tk_object_t* obj = object_default_create();
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* widget = progress_bar_create(win, 0, 0, w, h);

  widget_set_self_layout_params(widget, "c", "m", NULL, NULL);
  tk_object_set_prop_pointer(obj, STR_PROP_SELF, widget);
  fscript_eval(obj, "widget_layout('self')", &v);
  ASSERT_EQ(widget->x, (wm->w - w) / 2);
  ASSERT_EQ(widget->y, (wm->h - h) / 2);
  ASSERT_EQ(widget->w, w);
  ASSERT_EQ(widget->h, h);

  ((window_base_t*)win)->need_relayout = FALSE;
  widget_set_self_layout_params(widget, "c:10", "m:10", NULL, NULL);
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
