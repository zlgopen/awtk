#include "widgets/dragger.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(Dragger, basic) {
  value_t v1;
  value_t v2;
  widget_t* b = dragger_create(NULL, 0, 0, 30, 30);
  dragger_t* dragger = DRAGGER(b);

  ASSERT_EQ(dragger_set_range(b, 0, 100, 100, 200), RET_OK);
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_X_MIN, &v2), RET_OK);
  ASSERT_EQ(0, value_int(&v2));
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_Y_MIN, &v2), RET_OK);
  ASSERT_EQ(100, value_int(&v2));

  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_X_MAX, &v2), RET_OK);
  ASSERT_EQ(100, value_int(&v2));
  ASSERT_EQ(widget_get_prop(b, WIDGET_PROP_Y_MAX, &v2), RET_OK);
  ASSERT_EQ(200, value_int(&v2));

  value_set_int(&v1, 1);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_X_MIN, &v1), RET_OK);
  ASSERT_EQ(dragger->x_min, 1);

  value_set_int(&v1, 2);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_X_MAX, &v1), RET_OK);
  ASSERT_EQ(dragger->x_max, 2);

  value_set_int(&v1, 3);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_Y_MIN, &v1), RET_OK);
  ASSERT_EQ(dragger->y_min, 3);

  value_set_int(&v1, 4);
  ASSERT_EQ(widget_set_prop(b, WIDGET_PROP_Y_MAX, &v1), RET_OK);
  ASSERT_EQ(dragger->y_max, 4);

  widget_destroy(b);
}

TEST(Dragger, dragger_h) {
  pointer_event_t e;
  widget_t* b = dragger_create(NULL, 0, 0, 30, 30);

  ASSERT_EQ(dragger_set_range(b, 0, 0, 100, 0), RET_OK);

  e.x = 0;
  e.y = 0;
  e.e = event_init(EVT_POINTER_DOWN, b);
  widget_dispatch(b, &(e.e));
  ASSERT_EQ(b->x, 0);
  ASSERT_EQ(b->y, 0);

  e.x = 90;
  e.y = 90;
  e.e = event_init(EVT_POINTER_MOVE, b);
  widget_dispatch(b, &(e.e));
  ASSERT_EQ(b->x, 90);
  ASSERT_EQ(b->y, 0);

  e.x = 140;
  e.y = 90;
  e.e = event_init(EVT_POINTER_MOVE, b);
  widget_dispatch(b, &(e.e));
  ASSERT_EQ(b->x, 100);
  ASSERT_EQ(b->y, 0);

  e.x = 90;
  e.y = 90;
  e.e = event_init(EVT_POINTER_UP, b);
  widget_dispatch(b, &(e.e));
  ASSERT_EQ(b->x, 90);
  ASSERT_EQ(b->y, 0);

  widget_destroy(b);
}

TEST(Dragger, dragger_v) {
  pointer_event_t e;
  widget_t* b = dragger_create(NULL, 0, 0, 30, 30);

  ASSERT_EQ(dragger_set_range(b, 0, 0, 0, 100), RET_OK);

  e.x = 0;
  e.y = 0;
  e.e = event_init(EVT_POINTER_DOWN, b);
  widget_dispatch(b, &(e.e));
  ASSERT_EQ(b->x, 0);
  ASSERT_EQ(b->y, 0);

  e.x = 90;
  e.y = 90;
  e.e = event_init(EVT_POINTER_MOVE, b);
  widget_dispatch(b, &(e.e));
  ASSERT_EQ(b->x, 0);
  ASSERT_EQ(b->y, 90);

  e.x = 90;
  e.y = 160;
  e.e = event_init(EVT_POINTER_MOVE, b);
  widget_dispatch(b, &(e.e));
  ASSERT_EQ(b->x, 0);
  ASSERT_EQ(b->y, 100);

  e.x = 90;
  e.y = 90;
  e.e = event_init(EVT_POINTER_UP, b);
  widget_dispatch(b, &(e.e));
  ASSERT_EQ(b->x, 0);
  ASSERT_EQ(b->y, 90);

  widget_destroy(b);
}

TEST(Dragger, cast) {
  widget_t* b = dragger_create(NULL, 0, 0, 30, 30);

  ASSERT_EQ(b, dragger_cast(b));

  widget_destroy(b);
}
