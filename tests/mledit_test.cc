#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>
#include "mledit/mledit.h"
#include "widgets/window.h"

TEST(MLEdit, int) {
  value_t v1;
  value_t v2;
  widget_t* b = mledit_create(NULL, 10, 20, 30, 40);

  value_set_int(&v1, 20);
  ASSERT_EQ(widget_set_prop(b, MLEDIT_PROP_MAX_LINES, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, MLEDIT_PROP_MAX_LINES, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_bool(&v1, true);
  ASSERT_EQ(widget_set_prop(b, MLEDIT_PROP_WRAP_WORD, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, MLEDIT_PROP_WRAP_WORD, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  value_set_bool(&v1, false);
  ASSERT_EQ(widget_set_prop(b, MLEDIT_PROP_WRAP_WORD, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(b, MLEDIT_PROP_WRAP_WORD, &v2), RET_OK);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));

  widget_destroy(b);
}
