#include "awtk.h"
#include "gauge/gauge_pointer.h"
#include "gauge/gauge.h"
#include "gtest/gtest.h"

TEST(GuagePointer, cast) {
  widget_t* w = gauge_pointer_create(NULL, 0, 0, 100, 100);

  EXPECT_EQ(w, gauge_pointer_cast(w));

  widget_destroy(w);
  idle_dispatch();
}

TEST(GuagePointer, set_angle) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* gauge_pointer = gauge_pointer_create(w, 0, 0, 200, 200);

  EXPECT_EQ(gauge_pointer, gauge_pointer_cast(gauge_pointer));

  gauge_pointer_t* gauge_pointer_tmp = GAUGE_POINTER(gauge_pointer);

  EXPECT_TRUE(gauge_pointer_tmp != NULL);

  EXPECT_FALSE(gauge_pointer_set_angle(NULL, 10) == RET_OK);

  EXPECT_TRUE(gauge_pointer_set_angle(gauge_pointer, 10) == RET_OK);
  EXPECT_TRUE(gauge_pointer_tmp->angle == 10);

  EXPECT_TRUE(gauge_pointer_set_angle(gauge_pointer, -10) == RET_OK);

  EXPECT_TRUE(gauge_pointer_set_angle(gauge_pointer, 370) == RET_OK);

  EXPECT_TRUE(gauge_pointer_set_angle(gauge_pointer, 10.5) == RET_OK);
  EXPECT_TRUE(gauge_pointer_tmp->angle == 10.5);

  widget_destroy(w);
  idle_dispatch();
}

TEST(GuagePointer, set_image) {
#define IMAGE_NAME "test"

  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* gauge_pointer = gauge_pointer_create(w, 0, 0, 200, 200);

  EXPECT_EQ(gauge_pointer, gauge_pointer_cast(gauge_pointer));

  gauge_pointer_t* gauge_pointer_tmp = GAUGE_POINTER(gauge_pointer);

  EXPECT_TRUE(gauge_pointer_tmp != NULL);

  EXPECT_FALSE(gauge_pointer_set_image(NULL, IMAGE_NAME) == RET_OK);

  EXPECT_FALSE(gauge_pointer_set_image(gauge_pointer, NULL) == RET_OK);

  EXPECT_TRUE(gauge_pointer_set_image(gauge_pointer, IMAGE_NAME) == RET_OK);
  EXPECT_STREQ(gauge_pointer_tmp->image, IMAGE_NAME);

  widget_destroy(w);
  idle_dispatch();
}

TEST(GuagePointer, set_anchor) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* gauge_pointer = gauge_pointer_create(w, 0, 0, 200, 200);

  EXPECT_EQ(gauge_pointer, gauge_pointer_cast(gauge_pointer));

  gauge_pointer_t* gauge_pointer_tmp = GAUGE_POINTER(gauge_pointer);

  EXPECT_TRUE(gauge_pointer_tmp != NULL);

  EXPECT_FALSE(gauge_pointer_set_anchor_for_str(NULL, "100px", TRUE) == RET_OK);

  EXPECT_TRUE(gauge_pointer_set_anchor_for_str(gauge_pointer, "100px", TRUE) == RET_OK);
  EXPECT_TRUE(tk_atoi(gauge_pointer_tmp->anchor_x) == 100);

  EXPECT_TRUE(gauge_pointer_set_anchor_for_str(gauge_pointer, "100PX", TRUE) == RET_OK);
  EXPECT_TRUE(tk_atoi(gauge_pointer_tmp->anchor_x) == 100);

  EXPECT_TRUE(gauge_pointer_set_anchor_for_str(gauge_pointer, "300px", TRUE) == RET_OK);
  EXPECT_TRUE(tk_atoi(gauge_pointer_tmp->anchor_x) == 300);

  EXPECT_TRUE(gauge_pointer_set_anchor_for_str(gauge_pointer, "100px", FALSE) == RET_OK);
  EXPECT_TRUE(tk_atoi(gauge_pointer_tmp->anchor_y) == 100);

  EXPECT_TRUE(gauge_pointer_set_anchor_for_str(gauge_pointer, "100PX", FALSE) == RET_OK);
  EXPECT_TRUE(tk_atoi(gauge_pointer_tmp->anchor_y) == 100);

  EXPECT_TRUE(gauge_pointer_set_anchor_for_str(gauge_pointer, "300px", FALSE) == RET_OK);
  EXPECT_TRUE(tk_atoi(gauge_pointer_tmp->anchor_y) == 300);

  widget_destroy(w);
  idle_dispatch();
}
