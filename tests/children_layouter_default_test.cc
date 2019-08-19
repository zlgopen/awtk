#include <string>
#include "gtest/gtest.h"
#include "widgets/button.h"
#include "base/window.h"
#include "layouters/children_layouter_default.h"
#include "base/children_layouter_factory.h"

using std::string;

TEST(ChildrenLayoutDefault, basic) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  const char* layout_params = "default(c=2,r=1,x=3,y=4,s=14)";
  children_layouter_t* layouter = children_layouter_create(layout_params);

  ASSERT_EQ(children_layouter_get_param_int(layouter, "r", 0), 1);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "c", 0), 2);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "x", 0), 3);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "y", 0), 4);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "s", 0), 14);

  widget_move_resize(w, 0, 0, 400, 300);
  ASSERT_EQ(children_layouter_layout(layouter, w), RET_OK);
  ASSERT_EQ(string(children_layouter_to_string(layouter)), string(layout_params));

  ASSERT_EQ(b1->x, 3);
  ASSERT_EQ(b1->y, 4);
  ASSERT_EQ(b1->w, 190);
  ASSERT_EQ(b1->h, 292);

  ASSERT_EQ(b2->x, 3 + 190 + 14);
  ASSERT_EQ(b2->y, 4);
  ASSERT_EQ(b2->w, 190);
  ASSERT_EQ(b2->h, 292);

  children_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(ChildrenLayoutDefault, wh) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  const char* layout_params = "default(w=190,h=292,x=3,y=4,s=14)";
  children_layouter_t* layouter = children_layouter_create(layout_params);

  ASSERT_EQ(children_layouter_get_param_int(layouter, "w", 0), 190);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "h", 0), 292);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "x", 0), 3);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "y", 0), 4);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "s", 0), 14);

  widget_move_resize(w, 0, 0, 400, 300);
  ASSERT_EQ(children_layouter_layout(layouter, w), RET_OK);
  ASSERT_EQ(string(children_layouter_to_string(layouter)), string(layout_params));

  ASSERT_EQ(b1->x, 3);
  ASSERT_EQ(b1->y, 4);
  ASSERT_EQ(b1->w, 190);
  ASSERT_EQ(b1->h, 292);

  ASSERT_EQ(b2->x, 3 + 190 + 14);
  ASSERT_EQ(b2->y, 4);
  ASSERT_EQ(b2->w, 190);
  ASSERT_EQ(b2->h, 292);

  children_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(ChildrenLayoutDefault, hbox_left) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  children_layouter_t* layouter = children_layouter_create("default(r=1, c=0)");

  ASSERT_EQ(children_layouter_get_param_int(layouter, "r", 0), 1);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "c", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "x", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "y", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "s", 0), 0);

  widget_move_resize(w, 0, 0, 400, 300);
  widget_move_resize(b1, 0, 0, 40, 0);
  widget_move_resize(b2, 0, 0, 80, 0);
  ASSERT_EQ(children_layouter_layout(layouter, w), RET_OK);

  ASSERT_EQ(b1->x, 0);
  ASSERT_EQ(b1->y, 0);
  ASSERT_EQ(b1->w, 40);
  ASSERT_EQ(b1->h, 300);

  ASSERT_EQ(b2->x, 40);
  ASSERT_EQ(b2->y, 0);
  ASSERT_EQ(b2->w, 80);
  ASSERT_EQ(b2->h, 300);

  children_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(ChildrenLayoutDefault, hbox_right) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  children_layouter_t* layouter = children_layouter_create("default(r=1, c=0, align_h=right)");

  ASSERT_EQ(children_layouter_get_param_int(layouter, "r", 0), 1);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "c", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "x", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "y", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "s", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "a", 0), (int)ALIGN_H_RIGHT);

  widget_move_resize(w, 0, 0, 400, 300);
  widget_move_resize(b1, 0, 0, 40, 0);
  widget_move_resize(b2, 0, 0, 80, 0);
  ASSERT_EQ(children_layouter_layout(layouter, w), RET_OK);

  ASSERT_EQ(b1->x, 280);
  ASSERT_EQ(b1->y, 0);
  ASSERT_EQ(b1->w, 40);
  ASSERT_EQ(b1->h, 300);

  ASSERT_EQ(b2->x, 320);
  ASSERT_EQ(b2->y, 0);
  ASSERT_EQ(b2->w, 80);
  ASSERT_EQ(b2->h, 300);

  children_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(ChildrenLayoutDefault, hbox_center) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  children_layouter_t* layouter = children_layouter_create("default(r=1, c=0, align_h=center)");

  ASSERT_EQ(children_layouter_get_param_int(layouter, "r", 0), 1);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "c", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "x", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "y", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "s", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "a", 0), (int)ALIGN_H_CENTER);

  widget_move_resize(w, 0, 0, 400, 300);
  widget_move_resize(b1, 0, 0, 40, 0);
  widget_move_resize(b2, 0, 0, 80, 0);
  ASSERT_EQ(children_layouter_layout(layouter, w), RET_OK);

  ASSERT_EQ(b1->x, 140);
  ASSERT_EQ(b1->y, 0);
  ASSERT_EQ(b1->w, 40);
  ASSERT_EQ(b1->h, 300);

  ASSERT_EQ(b2->x, 180);
  ASSERT_EQ(b2->y, 0);
  ASSERT_EQ(b2->w, 80);
  ASSERT_EQ(b2->h, 300);

  children_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(ChildrenLayoutDefault, disable0) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  children_layouter_t* layouter = children_layouter_create("default(r=1, c=0, keep_disable=true)");

  ASSERT_EQ(children_layouter_get_param_int(layouter, "r", 0), 1);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "c", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "x", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "y", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "s", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "keep_disable", 1), 1);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "keep_invisible", 1), 0);

  widget_move_resize(w, 0, 0, 400, 300);
  widget_move_resize(b1, 0, 0, 40, 0);
  widget_move_resize(b2, 0, 0, 80, 0);
  widget_set_enable(b2, FALSE);
  ASSERT_EQ(children_layouter_layout(layouter, w), RET_OK);

  ASSERT_EQ(b1->x, 0);
  ASSERT_EQ(b1->y, 0);
  ASSERT_EQ(b1->w, 40);
  ASSERT_EQ(b1->h, 300);

  ASSERT_EQ(b2->x, 40);
  ASSERT_EQ(b2->y, 0);
  ASSERT_EQ(b2->w, 80);
  ASSERT_EQ(b2->h, 300);

  children_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(ChildrenLayoutDefault, disable1) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  children_layouter_t* layouter = children_layouter_create("default(r=1, c=0, keep_disable=false)");

  ASSERT_EQ(children_layouter_get_param_int(layouter, "r", 0), 1);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "c", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "x", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "y", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "s", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "keep_disable", 1), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "keep_invisible", 1), 0);

  widget_move_resize(w, 0, 0, 400, 300);
  widget_move_resize(b1, 0, 0, 40, 0);
  widget_move_resize(b2, 0, 0, 80, 0);
  widget_set_enable(b2, FALSE);
  ASSERT_EQ(children_layouter_layout(layouter, w), RET_OK);

  ASSERT_EQ(b1->x, 0);
  ASSERT_EQ(b1->y, 0);
  ASSERT_EQ(b1->w, 40);
  ASSERT_EQ(b1->h, 300);

  ASSERT_EQ(b2->x, 0);
  ASSERT_EQ(b2->y, 0);
  ASSERT_EQ(b2->w, 80);
  ASSERT_EQ(b2->h, 0);

  children_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(ChildrenLayoutDefault, invisible0) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  children_layouter_t* layouter =
      children_layouter_create("default(r=1, c=0, keep_invisible=true)");

  ASSERT_EQ(children_layouter_get_param_int(layouter, "r", 0), 1);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "c", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "x", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "y", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "s", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "keep_invisible", 1), 1);

  widget_move_resize(w, 0, 0, 400, 300);
  widget_move_resize(b1, 0, 0, 40, 0);
  widget_move_resize(b2, 0, 0, 80, 0);
  widget_set_visible(b2, FALSE, FALSE);
  ASSERT_EQ(children_layouter_layout(layouter, w), RET_OK);

  ASSERT_EQ(b1->x, 0);
  ASSERT_EQ(b1->y, 0);
  ASSERT_EQ(b1->w, 40);
  ASSERT_EQ(b1->h, 300);

  ASSERT_EQ(b2->x, 40);
  ASSERT_EQ(b2->y, 0);
  ASSERT_EQ(b2->w, 80);
  ASSERT_EQ(b2->h, 300);

  children_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(ChildrenLayoutDefault, invisible1) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  children_layouter_t* layouter =
      children_layouter_create("default(r=1, c=0, keep_invisible=false)");

  ASSERT_EQ(children_layouter_get_param_int(layouter, "r", 0), 1);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "c", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "x", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "y", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "s", 0), 0);
  ASSERT_EQ(children_layouter_get_param_int(layouter, "keep_invisible", 1), 0);

  widget_move_resize(w, 0, 0, 400, 300);
  widget_move_resize(b1, 0, 0, 40, 0);
  widget_move_resize(b2, 0, 0, 80, 0);
  widget_set_visible(b2, FALSE, FALSE);
  ASSERT_EQ(children_layouter_layout(layouter, w), RET_OK);

  ASSERT_EQ(b1->x, 0);
  ASSERT_EQ(b1->y, 0);
  ASSERT_EQ(b1->w, 40);
  ASSERT_EQ(b1->h, 300);

  ASSERT_EQ(b2->x, 0);
  ASSERT_EQ(b2->y, 0);
  ASSERT_EQ(b2->w, 80);
  ASSERT_EQ(b2->h, 0);

  children_layouter_destroy(layouter);
  widget_destroy(w);
}

TEST(ChildrenLayoutDefault, self) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* b0 = button_create(w, 0, 0, 0, 0);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  widget_t* b3 = button_create(w, 0, 0, 0, 0);
  widget_t* b4 = button_create(w, 0, 0, 0, 0);
  children_layouter_t* layouter = children_layouter_create("default(r=2, c=2)");

  widget_set_floating(b0, TRUE);
  widget_set_self_layout(b0, "default(x=10,y=20,w=20,h=30");
  widget_set_self_layout(b1, "default(x=c,y=m,w=20,h=30");
  widget_set_self_layout(b2, "default(x=r,y=b,w=50%,h=50%");
  widget_set_self_layout(b3, "default(x=10,y=20,w=30,h=40");
  widget_set_self_layout(b4, "default(x=0,y=0,w=-20,h=-30");
  widget_move_resize(w, 0, 0, 400, 400);
  ASSERT_EQ(children_layouter_layout(layouter, w), RET_OK);

  ASSERT_EQ(b1->x, 90);
  ASSERT_EQ(b1->y, 85);
  ASSERT_EQ(b1->w, 20);
  ASSERT_EQ(b1->h, 30);

  ASSERT_EQ(b2->x, 300);
  ASSERT_EQ(b2->y, 100);
  ASSERT_EQ(b2->w, 100);
  ASSERT_EQ(b2->h, 100);

  ASSERT_EQ(b3->x, 10);
  ASSERT_EQ(b3->y, 220);
  ASSERT_EQ(b3->w, 30);
  ASSERT_EQ(b3->h, 40);

  ASSERT_EQ(b4->x, 200);
  ASSERT_EQ(b4->y, 200);
  ASSERT_EQ(b4->w, 180);
  ASSERT_EQ(b4->h, 170);

  children_layouter_destroy(layouter);
  widget_destroy(w);
}
