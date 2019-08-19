#include <string>
#include "gtest/gtest.h"
#include "widgets/button.h"
#include "base/window.h"
#include "layouters/self_layouter_default.h"
#include "base/self_layouter_factory.h"

using std::string;

TEST(SelfLayoutDefault, basic) {
  rect_t r;
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b = button_create(w, 0, 0, 0, 0);
  const char* layout_params = "default(x=10,y=20,w=30,h=40)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x", 0), 10);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y", 0), 20);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w", 0), 30);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h", 0), 40);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_DEFAULT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_DEFAULT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w_attr", 0), W_ATTR_PIXEL);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h_attr", 0), H_ATTR_PIXEL);

  r = rect_init(0, 0, 400, 300);
  ASSERT_EQ(self_layouter_layout(layouter, b, &r), RET_OK);

  ASSERT_EQ(b->x, 10);
  ASSERT_EQ(b->y, 20);
  ASSERT_EQ(b->w, 30);
  ASSERT_EQ(b->h, 40);
  ASSERT_EQ(string(self_layouter_to_string(layouter)), string(layout_params));

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutDefault, undef) {
  rect_t r;
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b = button_create(w, 0, 0, 0, 0);
  self_layouter_t* layouter = self_layouter_create("default()");

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x", 0), 0);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y", 0), 0);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w", 0), 0);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h", 0), 0);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_UNDEF);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_UNDEF);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w_attr", 0), W_ATTR_UNDEF);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h_attr", 0), H_ATTR_UNDEF);

  r = rect_init(0, 0, 400, 300);
  widget_move_resize(b, 10, 20, 30, 40);
  ASSERT_EQ(self_layouter_layout(layouter, b, &r), RET_OK);

  ASSERT_EQ(b->x, 10);
  ASSERT_EQ(b->y, 20);
  ASSERT_EQ(b->w, 30);
  ASSERT_EQ(b->h, 40);
  ASSERT_EQ(string(self_layouter_to_string(layouter)), string("default()"));

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutDefault, minuswh) {
  rect_t r;
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b = button_create(w, 0, 0, 0, 0);
  const char* layout_params = "default(x=10,y=20,w=-30,h=-40)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x", 0), 10);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y", 0), 20);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w", 0), -30);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h", 0), -40);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_DEFAULT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_DEFAULT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w_attr", 0), W_ATTR_PIXEL);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h_attr", 0), H_ATTR_PIXEL);

  r = rect_init(0, 0, 400, 300);
  ASSERT_EQ(self_layouter_layout(layouter, b, &r), RET_OK);
  ASSERT_EQ(string(self_layouter_to_string(layouter)), string(layout_params));

  ASSERT_EQ(b->x, 10);
  ASSERT_EQ(b->y, 20);
  ASSERT_EQ(b->w, 400 - 30);
  ASSERT_EQ(b->h, 300 - 40);

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutDefault, percent) {
  rect_t r;
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b = button_create(w, 0, 0, 0, 0);
  const char* layout_params = "default(x=10%,y=20%,w=30%,h=40%)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x", 0), 10);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y", 0), 20);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w", 0), 30);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h", 0), 40);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w_attr", 0), W_ATTR_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h_attr", 0), H_ATTR_PERCENT);

  r = rect_init(0, 0, 400, 300);
  ASSERT_EQ(self_layouter_layout(layouter, b, &r), RET_OK);
  ASSERT_EQ(string(self_layouter_to_string(layouter)), string(layout_params));

  ASSERT_EQ(b->x, 40);
  ASSERT_EQ(b->y, 60);
  ASSERT_EQ(b->w, 120);
  ASSERT_EQ(b->h, 120);

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutDefault, center_middle) {
  rect_t r;
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b = button_create(w, 0, 0, 0, 0);
  self_layouter_t* layouter = self_layouter_create("default(x=c, y=m, w=30%, h=40%)");

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x", 0), 0);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y", 0), 0);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w", 0), 30);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h", 0), 40);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_CENTER);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_MIDDLE);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w_attr", 0), W_ATTR_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h_attr", 0), H_ATTR_PERCENT);

  r = rect_init(0, 0, 400, 300);
  ASSERT_EQ(self_layouter_layout(layouter, b, &r), RET_OK);

  ASSERT_EQ(b->x, 140);
  ASSERT_EQ(b->y, 90);
  ASSERT_EQ(b->w, 120);
  ASSERT_EQ(b->h, 120);
  ASSERT_EQ(string(self_layouter_to_string(layouter)), string("default(x=c:0,y=m:0,w=30%,h=40%)"));

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutDefault, center_middle1020) {
  rect_t r;
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b = button_create(w, 0, 0, 0, 0);
  const char* layout_params = "default(x=c:10,y=m:20,w=30%,h=40%)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x", 0), 10);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y", 0), 20);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w", 0), 30);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h", 0), 40);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_CENTER);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_MIDDLE);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w_attr", 0), W_ATTR_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h_attr", 0), H_ATTR_PERCENT);

  r = rect_init(0, 0, 400, 300);
  ASSERT_EQ(self_layouter_layout(layouter, b, &r), RET_OK);

  ASSERT_EQ(b->x, 140 + 10);
  ASSERT_EQ(b->y, 90 + 20);
  ASSERT_EQ(b->w, 120);
  ASSERT_EQ(b->h, 120);
  ASSERT_EQ(string(self_layouter_to_string(layouter)), string(layout_params));

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutDefault, center_middle1020minus) {
  rect_t r;
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b = button_create(w, 0, 0, 0, 0);
  const char* layout_params = "default(x=c:-10,y=m:-20,w=30%,h=40%)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x", 0), -10);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y", 0), -20);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w", 0), 30);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h", 0), 40);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_CENTER);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_MIDDLE);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w_attr", 0), W_ATTR_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h_attr", 0), H_ATTR_PERCENT);

  r = rect_init(0, 0, 400, 300);
  ASSERT_EQ(self_layouter_layout(layouter, b, &r), RET_OK);

  ASSERT_EQ(b->x, 140 - 10);
  ASSERT_EQ(b->y, 90 - 20);
  ASSERT_EQ(b->w, 120);
  ASSERT_EQ(b->h, 120);
  ASSERT_EQ(string(self_layouter_to_string(layouter)), string(layout_params));

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutDefault, right_bottom) {
  rect_t r;
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b = button_create(w, 0, 0, 0, 0);
  const char* layout_params = "default(x=r,y=b,w=30%,h=40%)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x", 0), 0);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y", 0), 0);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w", 0), 30);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h", 0), 40);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_RIGHT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_BOTTOM);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w_attr", 0), W_ATTR_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h_attr", 0), H_ATTR_PERCENT);

  r = rect_init(0, 0, 400, 300);
  ASSERT_EQ(self_layouter_layout(layouter, b, &r), RET_OK);

  ASSERT_EQ(b->x, 400 - 120);
  ASSERT_EQ(b->y, 300 - 120);
  ASSERT_EQ(b->w, 120);
  ASSERT_EQ(b->h, 120);
  ASSERT_EQ(string(self_layouter_to_string(layouter)), string("default(x=r:0,y=b:0,w=30%,h=40%)"));

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutDefault, right_bottom1020) {
  rect_t r;
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b = button_create(w, 0, 0, 0, 0);
  const char* layout_params = "default(x=r:10,y=b:20,w=30%,h=40%)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x", 0), 10);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y", 0), 20);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w", 0), 30);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h", 0), 40);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_RIGHT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_BOTTOM);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w_attr", 0), W_ATTR_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h_attr", 0), H_ATTR_PERCENT);

  r = rect_init(0, 0, 400, 300);
  ASSERT_EQ(self_layouter_layout(layouter, b, &r), RET_OK);

  ASSERT_EQ(b->x, 400 - 120 - 10);
  ASSERT_EQ(b->y, 300 - 120 - 20);
  ASSERT_EQ(b->w, 120);
  ASSERT_EQ(b->h, 120);

  ASSERT_EQ(string(self_layouter_to_string(layouter)), string(layout_params));

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutDefault, right_bottom1020_percent) {
  rect_t r;
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b = button_create(w, 0, 0, 0, 0);
  const char* layout_params = "default(x=r:10%,y=b:20%,w=30%,h=40%)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x", 0), 10);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y", 0), 20);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w", 0), 30);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h", 0), 40);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_RIGHT_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_BOTTOM_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w_attr", 0), W_ATTR_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h_attr", 0), H_ATTR_PERCENT);

  r = rect_init(0, 0, 400, 300);
  ASSERT_EQ(self_layouter_layout(layouter, b, &r), RET_OK);

  ASSERT_EQ(b->x, 400 - 120 - 400 * 10 / 100);
  ASSERT_EQ(b->y, 300 - 120 - 300 * 20 / 100);
  ASSERT_EQ(b->w, 120);
  ASSERT_EQ(b->h, 120);

  ASSERT_EQ(string(self_layouter_to_string(layouter)), string(layout_params));

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutDefault, center_middle1020_percent) {
  rect_t r;
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b = button_create(w, 0, 0, 0, 0);
  const char* layout_params = "default(x=c:10%,y=m:20%,w=30%,h=40%)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x", 0), 10);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y", 0), 20);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w", 0), 30);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h", 0), 40);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_CENTER_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_MIDDLE_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w_attr", 0), W_ATTR_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h_attr", 0), H_ATTR_PERCENT);

  r = rect_init(0, 0, 400, 300);
  ASSERT_EQ(self_layouter_layout(layouter, b, &r), RET_OK);

  ASSERT_EQ(b->x, 140 + 400 * 10 / 100);
  ASSERT_EQ(b->y, 90 + 300 * 20 / 100);
  ASSERT_EQ(b->w, 120);
  ASSERT_EQ(b->h, 120);
  ASSERT_EQ(string(self_layouter_to_string(layouter)), string(layout_params));

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutDefault, center_middle1020minus_percent) {
  rect_t r;
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b = button_create(w, 0, 0, 0, 0);
  const char* layout_params = "default(x=c:-10%,y=m:-20%,w=30%,h=40%)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x", 0), -10);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y", 0), -20);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w", 0), 30);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h", 0), 40);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_CENTER_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_MIDDLE_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w_attr", 0), W_ATTR_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h_attr", 0), H_ATTR_PERCENT);

  r = rect_init(0, 0, 400, 300);
  ASSERT_EQ(self_layouter_layout(layouter, b, &r), RET_OK);

  ASSERT_EQ(b->x, 140 - 400 * 10 / 100);
  ASSERT_EQ(b->y, 90 - 300 * 20 / 100);
  ASSERT_EQ(b->w, 120);
  ASSERT_EQ(b->h, 120);
  ASSERT_EQ(string(self_layouter_to_string(layouter)), string(layout_params));

  self_layouter_destroy(layouter);
  widget_destroy(w);
}
