#include "scroll_view/scroll_view.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "base/layout.h"
#include "widgets/button.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(ScrollView, basic) {
  value_t v1;
  widget_t* widget = scroll_view_create(NULL, 0, 0, 200, 400);
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);

  ASSERT_EQ(scroll_view_set_virtual_w(widget, 400), RET_OK);
  ASSERT_EQ(scroll_view_set_virtual_h(widget, 800), RET_OK);

  ASSERT_EQ(widget_get_prop(widget, WIDGET_PROP_LAYOUT_W, &v1), RET_OK);
  ASSERT_EQ(value_int(&v1), 400);
  ASSERT_EQ(widget_get_prop(widget, WIDGET_PROP_LAYOUT_H, &v1), RET_OK);
  ASSERT_EQ(value_int(&v1), 800);

  value_set_int(&v1, 600);
  ASSERT_EQ(widget_set_prop(widget, WIDGET_PROP_VIRTUAL_W, &v1), RET_OK);
  ASSERT_EQ(scroll_view->virtual_w, value_int(&v1));

  value_set_int(&v1, 900);
  ASSERT_EQ(widget_set_prop(widget, WIDGET_PROP_VIRTUAL_H, &v1), RET_OK);
  ASSERT_EQ(scroll_view->virtual_h, value_int(&v1));

  value_set_int(&v1, 60);
  ASSERT_EQ(widget_set_prop(widget, WIDGET_PROP_XOFFSET, &v1), RET_OK);
  ASSERT_EQ(scroll_view->xoffset, value_int(&v1));

  value_set_int(&v1, 70);
  ASSERT_EQ(widget_set_prop(widget, WIDGET_PROP_YOFFSET, &v1), RET_OK);
  ASSERT_EQ(scroll_view->yoffset, value_int(&v1));

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop(widget, WIDGET_PROP_XSLIDABLE, &v1), RET_OK);
  ASSERT_EQ(scroll_view->xslidable, value_bool(&v1));

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop(widget, WIDGET_PROP_YSLIDABLE, &v1), RET_OK);
  ASSERT_EQ(scroll_view->yslidable, value_bool(&v1));

  widget_destroy(widget);
}

TEST(ScrollView, layout) {
  widget_t* widget = scroll_view_create(NULL, 0, 0, 200, 400);
  widget_t* button = button_create(widget, 0, 0, 0, 0);

  ASSERT_EQ(scroll_view_set_virtual_w(widget, 400), RET_OK);
  ASSERT_EQ(scroll_view_set_virtual_h(widget, 800), RET_OK);

  widget_set_self_layout_params(button, "right", "bottom", "10%", "10%");
  widget_layout(widget);

  ASSERT_EQ(button->x, 360);
  ASSERT_EQ(button->y, 720);
  ASSERT_EQ(button->w, 40);
  ASSERT_EQ(button->h, 80);

  widget_destroy(widget);
}

TEST(ScrollView, cast) {
  widget_t* w = scroll_view_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, scroll_view_cast(w));

  widget_destroy(w);
}
