#include "base/dialog.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>
#include "base/window.h"

TEST(Dialog, basic) {
  value_t v1;
  value_t v2;
  widget_t* w1 = NULL;
  widget_t* w = dialog_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_is_dialog(w), TRUE);
  value_set_wstr(&v1, L"dialog");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_TEXT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_TEXT, &v2), RET_OK);
  ASSERT_EQ(wcscmp(v1.value.wstr, v2.value.wstr), 0);

  value_set_str(&v1, "center_scale");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_ANIM_HINT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_ANIM_HINT, &v2), RET_OK);
  ASSERT_EQ(strcmp(value_str(&v2), "center_scale"), 0);

  value_set_str(&v1, "default(alpha=10)");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_HIGHLIGHT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_HIGHLIGHT, &v2), RET_OK);
  ASSERT_STREQ(DIALOG(w)->highlight, value_str(&v1));
  ASSERT_STREQ(DIALOG(w)->highlight, value_str(&v2));

  w1 = widget_clone(w, NULL);
  ASSERT_EQ(widget_equal(w, w1), TRUE);

  widget_destroy(w);
  widget_destroy(w1);
}

TEST(Dialog, cast) {
  widget_t* w = dialog_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, dialog_cast(w));
  ASSERT_EQ(w, window_base_cast(w));

  widget_destroy(w);
}

TEST(Dialog, copy) {
  widget_t* w = dialog_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_prop_bool(w, WIDGET_PROP_STRONGLY_FOCUS, TRUE), RET_OK);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_DESIGN_W, 400), RET_OK);
  ASSERT_EQ(widget_set_prop_int(w, WIDGET_PROP_DESIGN_H, 300), RET_OK);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_THEME, "hello"), RET_OK);
  ASSERT_EQ(widget_set_prop_str(w, WIDGET_PROP_ANIM_HINT, "popup"), RET_OK);
  widget_t* w1 = widget_clone(w, NULL);

  ASSERT_EQ(widget_get_prop_bool(w1, WIDGET_PROP_STRONGLY_FOCUS, FALSE), TRUE);
  ASSERT_EQ(widget_get_prop_int(w1, WIDGET_PROP_DESIGN_W, 0), 400);
  ASSERT_EQ(widget_get_prop_int(w1, WIDGET_PROP_DESIGN_H, 0), 300);
  ASSERT_STREQ(widget_get_prop_str(w1, WIDGET_PROP_THEME, NULL), "hello");
  ASSERT_STREQ(widget_get_prop_str(w1, WIDGET_PROP_ANIM_HINT, NULL), "popup");

  widget_destroy(w);
  widget_destroy(w1);
}
