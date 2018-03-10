#include "base/layout.h"
#include "gtest/gtest.h"

TEST(Layuout, basic) {
  rect_t r;
  widget_layout_t layout;
  ASSERT_EQ(widget_layout_parse(&layout, "1", "2", "3", "4"), &layout);
  ASSERT_EQ(layout.x, 1);
  ASSERT_EQ(layout.y, 2);
  ASSERT_EQ(layout.w, 3);
  ASSERT_EQ(layout.h, 4);
  ASSERT_EQ(layout.x_attr, X_ATTR_DEFAULT);
  ASSERT_EQ(layout.y_attr, Y_ATTR_DEFAULT);
  ASSERT_EQ(layout.w_attr, W_ATTR_PIXEL);
  ASSERT_EQ(layout.h_attr, H_ATTR_PIXEL);

  widget_layout_calc(&layout, &r, 100, 200);
  ASSERT_EQ(r.x, 1);
  ASSERT_EQ(r.y, 2);
  ASSERT_EQ(r.w, 3);
  ASSERT_EQ(r.h, 4);
}

TEST(Layuout, percent) {
  rect_t r;
  widget_layout_t layout;
  ASSERT_EQ(widget_layout_parse(&layout, "1%", "2%", "3%", "4%"), &layout);
  ASSERT_EQ(layout.x, 1);
  ASSERT_EQ(layout.y, 2);
  ASSERT_EQ(layout.w, 3);
  ASSERT_EQ(layout.h, 4);
  ASSERT_EQ(layout.x_attr, X_ATTR_PERCENT);
  ASSERT_EQ(layout.y_attr, Y_ATTR_PERCENT);
  ASSERT_EQ(layout.w_attr, W_ATTR_PERCENT);
  ASSERT_EQ(layout.h_attr, H_ATTR_PERCENT);

  widget_layout_calc(&layout, &r, 100, 200);
  ASSERT_EQ(r.x, 1);
  ASSERT_EQ(r.y, 4);
  ASSERT_EQ(r.w, 3);
  ASSERT_EQ(r.h, 8);
}

TEST(Layuout, center_middle) {
  rect_t r;
  widget_layout_t layout;
  ASSERT_EQ(widget_layout_parse(&layout, "center", "middle", "30%", "40%"), &layout);
  ASSERT_EQ(layout.x, 0);
  ASSERT_EQ(layout.y, 0);
  ASSERT_EQ(layout.x_attr, X_ATTR_CENTER);
  ASSERT_EQ(layout.y_attr, Y_ATTR_MIDDLE);

  widget_layout_calc(&layout, &r, 100, 200);
  ASSERT_EQ(r.x, 35);
  ASSERT_EQ(r.y, 60);
  ASSERT_EQ(r.w, 30);
  ASSERT_EQ(r.h, 80);
}

TEST(Layuout, center_middle1) {
  rect_t r;
  widget_layout_t layout;
  ASSERT_EQ(widget_layout_parse(&layout, "center:1", "middle:2", "30%", "40%"), &layout);
  ASSERT_EQ(layout.x, 1);
  ASSERT_EQ(layout.y, 2);
  ASSERT_EQ(layout.x_attr, X_ATTR_CENTER);
  ASSERT_EQ(layout.y_attr, Y_ATTR_MIDDLE);

  widget_layout_calc(&layout, &r, 100, 200);
  ASSERT_EQ(r.x, 36);
  ASSERT_EQ(r.y, 62);
  ASSERT_EQ(r.w, 30);
  ASSERT_EQ(r.h, 80);
}

TEST(Layuout, right_bottom) {
  rect_t r;
  widget_layout_t layout;
  ASSERT_EQ(widget_layout_parse(&layout, "right", "bottom", "30%", "40%"), &layout);
  ASSERT_EQ(layout.x, 0);
  ASSERT_EQ(layout.y, 0);
  ASSERT_EQ(layout.x_attr, X_ATTR_RIGHT);
  ASSERT_EQ(layout.y_attr, Y_ATTR_BOTTOM);

  widget_layout_calc(&layout, &r, 100, 200);
  ASSERT_EQ(r.x, 70);
  ASSERT_EQ(r.y, 120);
  ASSERT_EQ(r.w, 30);
  ASSERT_EQ(r.h, 80);
}

TEST(Layuout, right_bottom1) {
  rect_t r;
  widget_layout_t layout;
  ASSERT_EQ(widget_layout_parse(&layout, "right:1", "bottom:2", "30%", "40%"), &layout);
  ASSERT_EQ(layout.x, 1);
  ASSERT_EQ(layout.y, 2);
  ASSERT_EQ(layout.x_attr, X_ATTR_RIGHT);
  ASSERT_EQ(layout.y_attr, Y_ATTR_BOTTOM);

  widget_layout_calc(&layout, &r, 100, 200);
  ASSERT_EQ(r.x, 69);
  ASSERT_EQ(r.y, 118);
  ASSERT_EQ(r.w, 30);
  ASSERT_EQ(r.h, 80);
}
