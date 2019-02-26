#include <stdlib.h>
#include "gtest/gtest.h"
#include "widgets/button.h"
#include "slide_menu/slide_menu.h"
#include <string>

using std::string;

static ret_t slide_menu_on_change(void* ctx, event_t* e) {
  string& s = *(string*)ctx;

  if (e->type == EVT_VALUE_CHANGED) {
    s += "changed:";
  } else if (e->type == EVT_VALUE_WILL_CHANGE) {
    s += "will_changed:";
  }

  return RET_OK;
}

TEST(SlideMenu, basic) {
  string str;
  widget_t* w = slide_menu_create(NULL, 0, 0, 400, 300);
  slide_menu_t* slide_menu = SLIDE_MENU(w);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  widget_t* b3 = button_create(w, 0, 0, 0, 0);

  widget_set_name(b1, "b1");
  widget_set_name(b2, "b2");
  widget_set_name(b3, "b3");

  widget_on(w, EVT_VALUE_WILL_CHANGE, slide_menu_on_change, &str);
  widget_on(w, EVT_VALUE_CHANGED, slide_menu_on_change, &str);

  slide_menu_set_value(w, 0);
  str = "";
  slide_menu_set_value(w, 1);
  ASSERT_EQ(slide_menu->value, 1);
  ASSERT_EQ(str, "will_changed:changed:");

  widget_destroy(w);
}

TEST(SlideMenu, fix_index) {
  widget_t* w = slide_menu_create(NULL, 0, 0, 400, 300);
  button_create(w, 0, 0, 0, 0);
  button_create(w, 0, 0, 0, 0);
  button_create(w, 0, 0, 0, 0);

  ASSERT_EQ(slide_menu_fix_index(w, 0), 0);
  ASSERT_EQ(slide_menu_fix_index(w, 1), 1);
  ASSERT_EQ(slide_menu_fix_index(w, 2), 2);
  ASSERT_EQ(slide_menu_fix_index(w, 3), 0);
  ASSERT_EQ(slide_menu_fix_index(w, 30), 0);
  ASSERT_EQ(slide_menu_fix_index(w, -1), 2);
  ASSERT_EQ(slide_menu_fix_index(w, -2), 1);
  ASSERT_EQ(slide_menu_fix_index(w, -3), 0);
  ASSERT_EQ(slide_menu_fix_index(w, -4), 2);

  widget_destroy(w);
}

TEST(SlideMenu, props) {
  value_t v1;
  value_t v2;
  widget_t* w = slide_menu_create(NULL, 0, 0, 400, 300);

  value_set_int(&v1, 321);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_XOFFSET, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_XOFFSET, &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 123);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_YOFFSET, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_YOFFSET, &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(0, value_int(&v2));

  value_set_int(&v1, 2);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_VALUE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_VALUE, &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_float(&v1, 0.6f);
  ASSERT_EQ(widget_set_prop(w, SLIDE_MENU_PROP_MIN_SCALE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, SLIDE_MENU_PROP_MIN_SCALE, &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_float(&v1), value_float(&v2));

  value_set_str(&v1, "bottom");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_ALIGN_V, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_ALIGN_V, &v2), RET_OK);
  ASSERT_EQ(ALIGN_V_BOTTOM, value_int(&v2));

  widget_destroy(w);
}

TEST(SlideMenu, cast) {
  widget_t* w = slide_menu_create(NULL, 0, 0, 400, 300);

  ASSERT_EQ(w, slide_menu_cast(w));

  widget_destroy(w);
}
