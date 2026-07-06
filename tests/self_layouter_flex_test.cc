#include "gtest/gtest.h"
#include "layouters/self_layouter_flex.inc"
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

TEST(SelfLayouterFlex, fill) {
  rect_t rect = {0, 0, 410, 300};
  widget_t* w = window_create(NULL, 0, 0, 800, 480);
  widget_t* view = view_create(w, rect.x, rect.y, rect.w, rect.h);
  widget_t* button1 = button_create(view, 0, 0, 0, 0);
  widget_t* button2 = button_create(view, 0, 0, 0, 0);
  widget_t* button3 = button_create(view, 0, 0, 0, 0);
  widget_t* button4 = button_create(view, 0, 0, 0, 0);
  widget_t* button5 = button_create(view, 0, 0, 0, 0);

  /* button1/button2 非fill(可见), button3 非fill(不可见), button4/button5 按权重 fill */
  widget_set_self_layout_params(button1, "0", "0", "50", "40");
  widget_set_self_layout_params(button2, "0", "0", "60", "50");
  widget_set_self_layout_params(button3, "0", "0", "80", "60");
  widget_set_visible_only(button3, FALSE);
  widget_set_self_layout(button4, "flex(w=fill:1,h=fill:1)");
  widget_set_self_layout(button5, "flex(w=fill:3,h=fill:2)");

  ASSERT_EQ(widget_layout(view), RET_OK);

  /* fill 子按权重平分剩余空间(不可见 button3 不计入): w 1:3 -> 75/225, h 1:2 -> 70/140 */
  ASSERT_EQ(button4->w, 75);
  ASSERT_EQ(button5->w, 225);
  ASSERT_EQ(button4->h, 70);
  ASSERT_EQ(button5->h, 140);
  ASSERT_EQ(button5->w, 3 * button4->w);
  ASSERT_EQ(button5->h, 2 * button4->h);
  ASSERT_EQ(button4->w + button5->w, rect.w - button1->w - button2->w);
  ASSERT_EQ(button4->h + button5->h, rect.h - button1->h - button2->h);

  /* 父尺寸变化时,fill 子吸收全部增量,非 fill 兄弟不变 */
  rect.w = 510;
  widget_resize(view, rect.w, rect.h);
  ASSERT_EQ(widget_layout(view), RET_OK);
  ASSERT_EQ(button4->w, 100);
  ASSERT_EQ(button5->w, 300);
  ASSERT_EQ(button5->w, 3 * button4->w);
  ASSERT_EQ(button4->w + button5->w, rect.w - button1->w - button2->w);

  widget_destroy(w);
}

TEST(SelfLayouterFlex, fill_with_spacing) {
  const rect_t rect = {0, 0, 410, 300};
  widget_t* w = window_create(NULL, 0, 0, 800, 480);
  widget_t* view = view_create(w, rect.x, rect.y, rect.w, rect.h);
  widget_t* button1 = button_create(view, 0, 0, 0, 0);
  widget_t* button2 = button_create(view, 0, 0, 0, 0);

  widget_set_children_layout(view, "default(r=1,c=0,s=10,x=10)");
  widget_set_self_layout(button1, "flex(w=fill:1)");
  widget_set_self_layout(button2, "flex(w=fill)");

  ASSERT_EQ(widget_layout(view), RET_OK);

  ASSERT_EQ(button1->w, 190);
  ASSERT_EQ(button2->w, 190);
  /* fill 子平分剩余空间: (rect.w - 2*x_margin - spacing) / 2 = (410 - 20 - 10) / 2 = 190 */
  ASSERT_EQ(button1->w + button2->w, rect.w - 2 * 10 - 10);

  widget_destroy(w);
}

TEST(SelfLayouterFlex, fill_to_string) {
  widget_t* w = window_create(NULL, 0, 0, 800, 480);
  widget_t* view = view_create(w, 0, 0, 400, 300);

  widget_set_self_layout(view, "flex(w=fill:1.5)");
  ASSERT_EQ(self_layouter_get_param_int(view->self_layout, "w_attr", 0), W_ATTR_FILL);
  ASSERT_EQ(strstr(self_layouter_to_string(view->self_layout), "w=fill:1.5000") != NULL, TRUE);

  widget_set_self_layout(view, "flex(w=fill)");
  ASSERT_EQ(self_layouter_get_param_int(view->self_layout, "w_attr", 0), W_ATTR_FILL);
  ASSERT_EQ(strstr(self_layouter_to_string(view->self_layout), "w=fill:1.0000") != NULL, TRUE);

  widget_destroy(w);
}
