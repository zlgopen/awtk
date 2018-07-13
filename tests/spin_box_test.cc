#include "base/spin_box.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(SpinBox, clone) {
  value_t v1;
  widget_t* w1 = spin_box_create(NULL, 10, 20, 30, 40);

  value_set_int(&v1, 200);
  ASSERT_EQ(widget_set_prop(w1, WIDGET_PROP_MAX, &v1), RET_OK);

  value_set_int(&v1, 20);
  ASSERT_EQ(widget_set_prop(w1, WIDGET_PROP_MIN, &v1), RET_OK);

  widget_t* w2 = widget_clone(w1, NULL);
  ASSERT_EQ(widget_equal(w1, w2), TRUE);

  widget_destroy(w1);
  widget_destroy(w2);
}
