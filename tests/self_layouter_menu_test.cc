#include <string>
#include "gtest/gtest.h"
#include "widgets/popup.h"
#include "layouters/self_layouter_menu.h"
#include "base/self_layouter_factory.h"

using std::string;

TEST(SelfLayoutMenu, point) {
  rect_t r;
  widget_t* w = popup_create(NULL, 0, 0, 30, 40);
  const char* layout_params = "menu()";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "p", 0), (int)'p');
  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_DEFAULT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_DEFAULT);

  r = rect_init(0, 0, 400, 300);

  point_t pressed = {20, 30};
  rect_t trigger_r = rect_init(0, 0, 0, 0);

  widget_layout_self_set_trigger(layouter, pressed, trigger_r);
  ASSERT_EQ(widget_layout_self_menu_with_rect(layouter, w, &r), RET_OK);

  ASSERT_EQ(w->x, 20);
  ASSERT_EQ(w->y, 30);
  ASSERT_EQ(w->w, 30);
  ASSERT_EQ(w->h, 40);

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutMenu, left_top) {
  rect_t r;
  widget_t* w = popup_create(NULL, 0, 0, 30, 40);
  const char* layout_params = "menu(position=left)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "p", 0), (int)'l');
  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_DEFAULT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_DEFAULT);

  r = rect_init(0, 0, 400, 300);

  point_t pressed = {0, 0};
  rect_t trigger_r = rect_init(360, 100, 40, 50);

  widget_layout_self_set_trigger(layouter, pressed, trigger_r);
  ASSERT_EQ(widget_layout_self_menu_with_rect(layouter, w, &r), RET_OK);

  ASSERT_EQ(w->x, 330);
  ASSERT_EQ(w->y, 100);
  ASSERT_EQ(w->w, 30);
  ASSERT_EQ(w->h, 40);

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutMenu, right_top) {
  rect_t r;
  widget_t* w = popup_create(NULL, 0, 0, 30, 40);
  const char* layout_params = "menu(position=right,w=50%,h=50%)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "p", 0), (int)'r');
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_DEFAULT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "w_attr", 0), W_ATTR_PERCENT);
  ASSERT_EQ(self_layouter_get_param_int(layouter, "h_attr", 0), W_ATTR_PERCENT);

  r = rect_init(0, 0, 400, 300);

  point_t pressed = {0, 0};
  rect_t trigger_r = rect_init(0, 100, 40, 50);

  widget_layout_self_set_trigger(layouter, pressed, trigger_r);
  ASSERT_EQ(widget_layout_self_menu_with_rect(layouter, w, &r), RET_OK);

  ASSERT_EQ(w->x, 40);
  ASSERT_EQ(w->y, 100);
  ASSERT_EQ(w->w, 200);
  ASSERT_EQ(w->h, 150);

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutMenu, left_middle) {
  rect_t r;
  widget_t* w = popup_create(NULL, 0, 0, 30, 40);
  const char* layout_params = "menu(position=left,y=middle)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "p", 0), (int)'l');
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_MIDDLE);

  r = rect_init(0, 0, 400, 300);

  point_t pressed = {0, 0};
  rect_t trigger_r = rect_init(360, 100, 40, 50);

  widget_layout_self_set_trigger(layouter, pressed, trigger_r);
  ASSERT_EQ(widget_layout_self_menu_with_rect(layouter, w, &r), RET_OK);

  ASSERT_EQ(w->x, 330);
  ASSERT_EQ(w->y, 105);
  ASSERT_EQ(w->w, 30);
  ASSERT_EQ(w->h, 40);

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutMenu, left_bottom) {
  rect_t r;
  widget_t* w = popup_create(NULL, 0, 0, 30, 40);
  const char* layout_params = "menu(position=left,y=bottom)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "p", 0), (int)'l');
  ASSERT_EQ(self_layouter_get_param_int(layouter, "y_attr", 0), Y_ATTR_BOTTOM);

  r = rect_init(0, 0, 400, 300);

  point_t pressed = {0, 0};
  rect_t trigger_r = rect_init(360, 100, 40, 50);

  widget_layout_self_set_trigger(layouter, pressed, trigger_r);
  ASSERT_EQ(widget_layout_self_menu_with_rect(layouter, w, &r), RET_OK);

  ASSERT_EQ(w->x, 330);
  ASSERT_EQ(w->y, 110);
  ASSERT_EQ(w->w, 30);
  ASSERT_EQ(w->h, 40);

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutMenu, up_left) {
  rect_t r;
  widget_t* w = popup_create(NULL, 0, 0, 30, 40);
  const char* layout_params = "menu(position=up)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "p", 0), (int)'u');
  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_DEFAULT);

  r = rect_init(0, 0, 400, 300);

  point_t pressed = {0, 0};
  rect_t trigger_r = rect_init(60, 200, 40, 50);

  widget_layout_self_set_trigger(layouter, pressed, trigger_r);
  ASSERT_EQ(widget_layout_self_menu_with_rect(layouter, w, &r), RET_OK);

  ASSERT_EQ(w->x, 60);
  ASSERT_EQ(w->y, 160);
  ASSERT_EQ(w->w, 30);
  ASSERT_EQ(w->h, 40);

  self_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(SelfLayoutMenu, down_left) {
  rect_t r;
  widget_t* w = popup_create(NULL, 0, 0, 30, 40);
  const char* layout_params = "menu(position=down)";
  self_layouter_t* layouter = self_layouter_create(layout_params);

  ASSERT_EQ(self_layouter_get_param_int(layouter, "p", 0), (int)'d');
  ASSERT_EQ(self_layouter_get_param_int(layouter, "x_attr", 0), X_ATTR_DEFAULT);

  r = rect_init(0, 0, 400, 300);

  point_t pressed = {0, 0};
  rect_t trigger_r = rect_init(60, 0, 40, 50);

  widget_layout_self_set_trigger(layouter, pressed, trigger_r);
  ASSERT_EQ(widget_layout_self_menu_with_rect(layouter, w, &r), RET_OK);

  ASSERT_EQ(w->x, 60);
  ASSERT_EQ(w->y, 50);
  ASSERT_EQ(w->w, 30);
  ASSERT_EQ(w->h, 40);

  self_layouter_destroy(layouter);
  widget_destroy(w);
}
