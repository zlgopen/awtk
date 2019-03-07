#include <stdlib.h>
#include "gtest/gtest.h"
#include "widgets/button.h"
#include "slide_view/slide_view.h"
#include <string>

using std::string;

static ret_t slide_view_on_change(void* ctx, event_t* e) {
  string& s = *(string*)ctx;

  if (e->type == EVT_VALUE_CHANGED) {
    s += "changed:";
  } else if (e->type == EVT_VALUE_WILL_CHANGE) {
    s += "will_changed:";
  }

  return RET_OK;
}

TEST(SlideView, basic) {
  string str;
  widget_t* w = slide_view_create(NULL, 0, 0, 400, 300);
  slide_view_t* slide_view = SLIDE_VIEW(w);
  widget_t* b1 = button_create(w, 0, 0, 400, 300);
  widget_t* b2 = button_create(w, 0, 0, 400, 300);
  widget_t* b3 = button_create(w, 0, 0, 400, 300);

  widget_set_name(b1, "b1");
  widget_set_name(b2, "b2");
  widget_set_name(b3, "b3");

  widget_on(w, EVT_VALUE_WILL_CHANGE, slide_view_on_change, &str);
  widget_on(w, EVT_VALUE_CHANGED, slide_view_on_change, &str);

  slide_view_set_active(w, 0);
  ASSERT_EQ(slide_view->active, 0);
  ASSERT_EQ(slide_view_get_prev(slide_view), WIDGET(NULL));
  ASSERT_EQ(slide_view_get_next(slide_view), b2);

  ASSERT_EQ(slide_view_activate_prev(slide_view), RET_FAIL);
  ASSERT_EQ(slide_view_activate_next(slide_view), RET_OK);
  ASSERT_EQ(slide_view->active, 1);
  ASSERT_EQ(str, "will_changed:changed:");

  ASSERT_EQ(slide_view_activate_next(slide_view), RET_OK);
  ASSERT_EQ(slide_view->active, 2);

  ASSERT_EQ(slide_view_activate_next(slide_view), RET_FAIL);
  ASSERT_EQ(slide_view->active, 2);

  ASSERT_EQ(slide_view_get_prev(slide_view), b2);
  ASSERT_EQ(slide_view_get_next(slide_view), WIDGET(NULL));

  ASSERT_EQ(slide_view_set_auto_play(w, 1000), RET_OK);
  ASSERT_EQ(slide_view->auto_play, 1000);
  slide_view_set_active(w, 0);
  ASSERT_EQ(slide_view_get_prev(slide_view), WIDGET(b3));

  slide_view_set_active(w, 2);
  ASSERT_EQ(slide_view_get_next(slide_view), WIDGET(b1));

  widget_destroy(w);
}

TEST(SlideView, props) {
  value_t v1;
  value_t v2;
  widget_t* w = slide_view_create(NULL, 0, 0, 400, 300);

  value_set_int(&v1, 321);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_XOFFSET, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_XOFFSET, &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 123);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_YOFFSET, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_YOFFSET, &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 2);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_VALUE, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_VALUE, &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 3000);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_AUTO_PLAY, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_AUTO_PLAY, &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_int(&v1), value_int(&v2));
  ASSERT_EQ(value_int(&v1), SLIDE_VIEW(w)->auto_play);

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_VERTICAL, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_VERTICAL, &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));
  ASSERT_EQ(value_bool(&v1), SLIDE_VIEW(w)->vertical);

  value_set_bool(&v1, TRUE);
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_LOOP, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_LOOP, &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_EQ(value_bool(&v1), value_bool(&v2));
  ASSERT_EQ(value_bool(&v1), SLIDE_VIEW(w)->loop);

  value_set_str(&v1, "overlap");
  ASSERT_EQ(widget_set_prop(w, WIDGET_PROP_ANIM_HINT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(w, WIDGET_PROP_ANIM_HINT, &v2), RET_OK);
  ASSERT_EQ(v1.type, v2.type);
  ASSERT_STREQ(value_str(&v1), value_str(&v2));
  ASSERT_STREQ(value_str(&v1), SLIDE_VIEW(w)->anim_hint);

  widget_destroy(w);
}

TEST(SlideView, cast) {
  widget_t* w = slide_view_create(NULL, 0, 0, 400, 300);

  ASSERT_EQ(w, slide_view_cast(w));

  widget_destroy(w);
}
