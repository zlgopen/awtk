#include "tkc/easing.h"
#include "gtest/gtest.h"
#include "widgets/button.h"
#include "widget_animators/widget_animator_prop.h"

static float_t easing_linear(float_t k) {
  return k;
}


TEST(easing, basic) {
  ASSERT_EQ(easing_register("test1", easing_linear), EASING_FUNC_NR + 1);
  ASSERT_EQ(easing_register("test2", easing_linear), EASING_FUNC_NR + 2);

  uint32_t easing_type = easing_register("test3", easing_linear);
  widget_t* button = button_create(NULL, 0, 0, 100, 30);
  widget_animator_t* wa = widget_animator_prop_create(button, 1000, 0, (easing_type_t)easing_type, "x");

  ASSERT_EQ(widget_animator_prop_set_params(wa, 0, 300), RET_OK);

  widget_animator_destroy(wa);
  widget_destroy(button);
}
