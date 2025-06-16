#include "gtest/gtest.h"
#include "layouters/self_layouter_flex.h"
#include "base/self_layouter_factory.h"
#include "base/layout.h"
#include "base/window.h"
#include "widgets/view.h"
#include "widgets/button.h"

TEST(SelfLayouterFlex, height) {
  const char* layout_params = "flex(h=flex)";
  const rect_t rect = {0, 0, 400, 300};
  widget_t* w = window_create(NULL, 0, 0, 800, 480);
  widget_t* view = view_create(w, rect.x, rect.y, rect.w, rect.h);
  widget_t* button1 = button_create(view, 0, 0, 0, 0);
  widget_t* button2 = button_create(view, 0, 0, 0, 0);
  widget_t* button3 = button_create(view, 0, 0, 0, 0);

  widget_set_self_layout(view, layout_params);
  ASSERT_EQ(view->self_layout != NULL, TRUE);

  ASSERT_EQ(self_layouter_get_param_int(view->self_layout, "x_attr", 0), X_ATTR_UNDEF);
  ASSERT_EQ(self_layouter_get_param_int(view->self_layout, "y_attr", 0), X_ATTR_UNDEF);
  ASSERT_EQ(self_layouter_get_param_int(view->self_layout, "w_attr", 0), W_ATTR_UNDEF);
  ASSERT_EQ(self_layouter_get_param_int(view->self_layout, "h_attr", 0), H_ATTR_FLEX);

  widget_set_self_layout_params(button1, "c", "0", "50%", "30");
  widget_set_self_layout_params(button2, "c", "50", "50%", "40");
  widget_set_self_layout_params(button3, "c", "100", "50%", "50");

  ASSERT_EQ(widget_layout(view), RET_OK);

  ASSERT_EQ(view->x, rect.x);
  ASSERT_EQ(view->y, rect.y);
  ASSERT_EQ(view->w, rect.w);
  ASSERT_EQ(view->h, button3->y + button3->h);
  ASSERT_STREQ(self_layouter_to_string(view->self_layout), layout_params);

  widget_destroy(w);
}

TEST(SelfLayouterFlex, width) {
  const char* layout_params = "flex(w=flex)";
  const rect_t rect = {0, 0, 400, 300};
  widget_t* w = window_create(NULL, 0, 0, 800, 480);
  widget_t* view = view_create(w, rect.x, rect.y, rect.w, rect.h);
  widget_t* button1 = button_create(view, 0, 0, 0, 0);
  widget_t* button2 = button_create(view, 0, 0, 0, 0);
  widget_t* button3 = button_create(view, 0, 0, 0, 0);

  widget_set_self_layout(view, layout_params);
  ASSERT_EQ(view->self_layout != NULL, TRUE);

  ASSERT_EQ(self_layouter_get_param_int(view->self_layout, "x_attr", 0), X_ATTR_UNDEF);
  ASSERT_EQ(self_layouter_get_param_int(view->self_layout, "y_attr", 0), X_ATTR_UNDEF);
  ASSERT_EQ(self_layouter_get_param_int(view->self_layout, "w_attr", 0), W_ATTR_FLEX);
  ASSERT_EQ(self_layouter_get_param_int(view->self_layout, "h_attr", 0), H_ATTR_UNDEF);

  widget_set_self_layout_params(button1, "0", "m", "30", "50%");
  widget_set_self_layout_params(button2, "50", "m", "40", "50%");
  widget_set_self_layout_params(button3, "100", "m", "50", "50%");

  ASSERT_EQ(widget_layout(view), RET_OK);

  ASSERT_EQ(view->x, rect.x);
  ASSERT_EQ(view->y, rect.y);
  ASSERT_EQ(view->w, button3->x + button3->w);
  ASSERT_EQ(view->h, rect.h);
  ASSERT_STREQ(self_layouter_to_string(view->self_layout), layout_params);

  widget_destroy(w);
}

TEST(SelfLayouterFlex, width_and_height) {
  const char* layout_params = "flex(w=flex,h=flex)";
  const rect_t rect = {0, 0, 400, 300};
  widget_t* w = window_create(NULL, 0, 0, 800, 480);
  widget_t* view = view_create(w, rect.x, rect.y, rect.w, rect.h);
  widget_t* button1 = button_create(view, 0, 0, 0, 0);
  widget_t* button2 = button_create(view, 0, 0, 0, 0);
  widget_t* button3 = button_create(view, 0, 0, 0, 0);

  widget_set_self_layout(view, layout_params);
  ASSERT_EQ(view->self_layout != NULL, TRUE);

  ASSERT_EQ(self_layouter_get_param_int(view->self_layout, "x_attr", 0), X_ATTR_UNDEF);
  ASSERT_EQ(self_layouter_get_param_int(view->self_layout, "y_attr", 0), X_ATTR_UNDEF);
  ASSERT_EQ(self_layouter_get_param_int(view->self_layout, "w_attr", 0), W_ATTR_FLEX);
  ASSERT_EQ(self_layouter_get_param_int(view->self_layout, "h_attr", 0), H_ATTR_FLEX);

  widget_set_self_layout_params(button1, "0", "0", "30", "30");
  widget_set_self_layout_params(button2, "50", "0", "40", "40");
  widget_set_self_layout_params(button3, "100", "0", "50", "50");

  ASSERT_EQ(widget_layout(view), RET_OK);

  ASSERT_EQ(view->x, rect.x);
  ASSERT_EQ(view->y, rect.y);
  ASSERT_EQ(view->w, button3->x + button3->w);
  ASSERT_EQ(view->h, button3->y + button3->h);
  ASSERT_STREQ(self_layouter_to_string(view->self_layout), layout_params);

  widget_destroy(w);
}
