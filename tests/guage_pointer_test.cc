#include "awtk.h"
#include "guage/guage_pointer.h"
#include "guage/guage.h"
#include "gtest/gtest.h"

TEST(GuagePointer, cast) {
  widget_t* w = guage_pointer_create(NULL, 0, 0, 100, 100);

  EXPECT_EQ(w, guage_pointer_cast(w));

  widget_destroy(w);
}

TEST(GuagePointer, set_angle) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* guage_pointer = guage_pointer_create(w, 0, 0, 200, 200);

  EXPECT_EQ(guage_pointer, guage_pointer_cast(guage_pointer));

  guage_pointer_t* guage_pointer_tmp = GUAGE_POINTER(guage_pointer);

  EXPECT_TRUE(guage_pointer_tmp != NULL);

  EXPECT_FALSE(guage_pointer_set_angle(NULL, 10) == RET_OK);

  EXPECT_TRUE(guage_pointer_set_angle(guage_pointer, 10) == RET_OK);
  EXPECT_TRUE(guage_pointer_tmp->angle == 10);

  EXPECT_TRUE(guage_pointer_set_angle(guage_pointer, -10) == RET_OK);

  EXPECT_TRUE(guage_pointer_set_angle(guage_pointer, 370) == RET_OK);

  widget_destroy(w);
}

TEST(GuagePointer, set_image) {
#define IMAGE_NAME "test"

  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* guage_pointer = guage_pointer_create(w, 0, 0, 200, 200);

  EXPECT_EQ(guage_pointer, guage_pointer_cast(guage_pointer));

  guage_pointer_t* guage_pointer_tmp = GUAGE_POINTER(guage_pointer);

  EXPECT_TRUE(guage_pointer_tmp != NULL);

  EXPECT_FALSE(guage_pointer_set_image(NULL, IMAGE_NAME) == RET_OK);

  EXPECT_FALSE(guage_pointer_set_image(guage_pointer, NULL) == RET_OK);

  EXPECT_TRUE(guage_pointer_set_image(guage_pointer, IMAGE_NAME) == RET_OK);
  EXPECT_STREQ(guage_pointer_tmp->image, IMAGE_NAME);

  widget_destroy(w);
}

TEST(GuagePointer, set_anchor) {
  widget_t* w = window_create(NULL, 0, 0, 0, 0);
  widget_t* guage_pointer = guage_pointer_create(w, 0, 0, 200, 200);

  EXPECT_EQ(guage_pointer, guage_pointer_cast(guage_pointer));

  guage_pointer_t* guage_pointer_tmp = GUAGE_POINTER(guage_pointer);

  EXPECT_TRUE(guage_pointer_tmp != NULL);

  EXPECT_FALSE(guage_pointer_set_anchor_for_str(NULL, "100px", TRUE) == RET_OK);

  EXPECT_TRUE(guage_pointer_set_anchor_for_str(guage_pointer, "100px", TRUE) == RET_OK);
  EXPECT_TRUE(tk_atoi(guage_pointer_tmp->anchor_x) == 100);

  EXPECT_TRUE(guage_pointer_set_anchor_for_str(guage_pointer, "100PX", TRUE) == RET_OK);
  EXPECT_TRUE(tk_atoi(guage_pointer_tmp->anchor_x) == 100);

  EXPECT_TRUE(guage_pointer_set_anchor_for_str(guage_pointer, "300px", TRUE) == RET_OK);
  EXPECT_TRUE(tk_atoi(guage_pointer_tmp->anchor_x) == 300);

  EXPECT_TRUE(guage_pointer_set_anchor_for_str(guage_pointer, "100px", FALSE) == RET_OK);
  EXPECT_TRUE(tk_atoi(guage_pointer_tmp->anchor_y) == 100);

  EXPECT_TRUE(guage_pointer_set_anchor_for_str(guage_pointer, "100PX", FALSE) == RET_OK);
  EXPECT_TRUE(tk_atoi(guage_pointer_tmp->anchor_y) == 100);

  EXPECT_TRUE(guage_pointer_set_anchor_for_str(guage_pointer, "300px", FALSE) == RET_OK);
  EXPECT_TRUE(tk_atoi(guage_pointer_tmp->anchor_y) == 300);

  widget_destroy(w);
}
