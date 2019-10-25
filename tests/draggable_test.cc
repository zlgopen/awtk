#include "widgets/view.h"
#include "features/draggable.h"
#include "gtest/gtest.h"

TEST(Draggable, basic) {
  widget_t* w = view_create(NULL, 10, 20, 30, 40);
  widget_t* d = draggable_create(w, 0, 0, 0, 0);
  draggable_t* draggable = DRAGGABLE(d);

  ASSERT_EQ(widget_get_prop_bool(d, DRAGGABLE_PROP_VERTICAL_ONLY, FALSE), FALSE);
  ASSERT_EQ(widget_get_prop_bool(d, DRAGGABLE_PROP_HORIZONTAL_ONLY, FALSE), FALSE);
  ASSERT_EQ(widget_get_prop_bool(d, DRAGGABLE_PROP_DRAG_WINDOW, FALSE), FALSE);
  ASSERT_EQ(widget_get_prop_int(d, DRAGGABLE_PROP_LEFT, 0), DRAGGABLE_UNSPECIFIED_NUM);
  ASSERT_EQ(widget_get_prop_int(d, DRAGGABLE_PROP_TOP, 0), DRAGGABLE_UNSPECIFIED_NUM);
  ASSERT_EQ(widget_get_prop_int(d, DRAGGABLE_PROP_RIGHT, 0), DRAGGABLE_UNSPECIFIED_NUM);
  ASSERT_EQ(widget_get_prop_int(d, DRAGGABLE_PROP_BOTTOM, 0), DRAGGABLE_UNSPECIFIED_NUM);

  ASSERT_EQ(widget_set_prop_bool(d, DRAGGABLE_PROP_VERTICAL_ONLY, TRUE), RET_OK);
  ASSERT_EQ(widget_set_prop_bool(d, DRAGGABLE_PROP_HORIZONTAL_ONLY, TRUE), RET_OK);
  ASSERT_EQ(widget_set_prop_bool(d, DRAGGABLE_PROP_DRAG_WINDOW, TRUE), RET_OK);

  ASSERT_EQ(widget_set_prop_int(d, DRAGGABLE_PROP_LEFT, 1), RET_OK);
  ASSERT_EQ(widget_set_prop_int(d, DRAGGABLE_PROP_TOP, 2), RET_OK);
  ASSERT_EQ(widget_set_prop_int(d, DRAGGABLE_PROP_RIGHT, 3), RET_OK);
  ASSERT_EQ(widget_set_prop_int(d, DRAGGABLE_PROP_BOTTOM, 4), RET_OK);

  ASSERT_EQ(widget_get_prop_bool(d, DRAGGABLE_PROP_VERTICAL_ONLY, FALSE), TRUE);
  ASSERT_EQ(widget_get_prop_bool(d, DRAGGABLE_PROP_HORIZONTAL_ONLY, FALSE), TRUE);
  ASSERT_EQ(widget_get_prop_bool(d, DRAGGABLE_PROP_DRAG_WINDOW, FALSE), TRUE);

  ASSERT_EQ(widget_get_prop_int(d, DRAGGABLE_PROP_LEFT, 0), 1);
  ASSERT_EQ(widget_get_prop_int(d, DRAGGABLE_PROP_TOP, 0), 2);
  ASSERT_EQ(widget_get_prop_int(d, DRAGGABLE_PROP_RIGHT, 0), 3);
  ASSERT_EQ(widget_get_prop_int(d, DRAGGABLE_PROP_BOTTOM, 9), 4);

  ASSERT_EQ(widget_get_prop_bool(d, DRAGGABLE_PROP_VERTICAL_ONLY, FALSE), draggable->vertical_only);
  ASSERT_EQ(widget_get_prop_bool(d, DRAGGABLE_PROP_HORIZONTAL_ONLY, FALSE),
            draggable->horizontal_only);
  ASSERT_EQ(widget_get_prop_bool(d, DRAGGABLE_PROP_DRAG_WINDOW, FALSE), draggable->drag_window);

  ASSERT_EQ(widget_get_prop_int(d, DRAGGABLE_PROP_LEFT, 0), draggable->left);
  ASSERT_EQ(widget_get_prop_int(d, DRAGGABLE_PROP_TOP, 0), draggable->top);
  ASSERT_EQ(widget_get_prop_int(d, DRAGGABLE_PROP_RIGHT, 0), draggable->right);
  ASSERT_EQ(widget_get_prop_int(d, DRAGGABLE_PROP_BOTTOM, 0), draggable->bottom);

  widget_destroy(w);
}
