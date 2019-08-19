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
