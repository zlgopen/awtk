#include "scroll_label/hscroll_label.h"
#include "gtest/gtest.h"

TEST(HScrollLabel, basic) {
  widget_t* w = hscroll_label_create(NULL, 10, 20, 300, 30);

  ASSERT_EQ(widget_get_prop_bool(w, HSCROLL_LABEL_PROP_ONLY_FOCUS, TRUE), FALSE);
  ASSERT_EQ(widget_get_prop_bool(w, HSCROLL_LABEL_PROP_ONLY_FOCUS, FALSE), FALSE);
  ASSERT_EQ(widget_set_prop_bool(w, HSCROLL_LABEL_PROP_ONLY_FOCUS, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(w, HSCROLL_LABEL_PROP_ONLY_FOCUS, FALSE), TRUE);

  ASSERT_EQ(widget_get_prop_bool(w, HSCROLL_LABEL_PROP_YOYO, TRUE), FALSE);
  ASSERT_EQ(widget_get_prop_bool(w, HSCROLL_LABEL_PROP_YOYO, FALSE), FALSE);
  ASSERT_EQ(widget_set_prop_bool(w, HSCROLL_LABEL_PROP_YOYO, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(w, HSCROLL_LABEL_PROP_YOYO, FALSE), TRUE);

  ASSERT_EQ(widget_get_prop_bool(w, HSCROLL_LABEL_PROP_ELLIPSES, TRUE), FALSE);
  ASSERT_EQ(widget_get_prop_bool(w, HSCROLL_LABEL_PROP_ELLIPSES, FALSE), FALSE);
  ASSERT_EQ(widget_set_prop_bool(w, HSCROLL_LABEL_PROP_ELLIPSES, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(w, HSCROLL_LABEL_PROP_ELLIPSES, FALSE), TRUE);

  ASSERT_EQ(widget_get_prop_bool(w, HSCROLL_LABEL_PROP_LOOP, TRUE), FALSE);
  ASSERT_EQ(widget_get_prop_bool(w, HSCROLL_LABEL_PROP_LOOP, FALSE), FALSE);
  ASSERT_EQ(widget_set_prop_bool(w, HSCROLL_LABEL_PROP_LOOP, TRUE), RET_OK);
  ASSERT_EQ(widget_get_prop_bool(w, HSCROLL_LABEL_PROP_LOOP, FALSE), TRUE);

  ASSERT_EQ(widget_get_prop_int(w, HSCROLL_LABEL_PROP_DURATION, 0), 5000);
  ASSERT_EQ(widget_set_prop_int(w, HSCROLL_LABEL_PROP_DURATION, 8000), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, HSCROLL_LABEL_PROP_DURATION, 0), 8000);

  ASSERT_EQ(widget_get_prop_int(w, HSCROLL_LABEL_PROP_LULL, 0), 3000);
  ASSERT_EQ(widget_set_prop_int(w, HSCROLL_LABEL_PROP_LULL, 8000), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, HSCROLL_LABEL_PROP_LULL, 0), 8000);

  ASSERT_EQ(widget_get_prop_int(w, HSCROLL_LABEL_PROP_XOFFSET, 0), 0);
  ASSERT_EQ(widget_set_prop_int(w, HSCROLL_LABEL_PROP_XOFFSET, 10), RET_OK);
  ASSERT_EQ(widget_get_prop_int(w, HSCROLL_LABEL_PROP_XOFFSET, 0), 10);

  ASSERT_EQ(w, hscroll_label_cast(w));

  widget_destroy(w);
}
