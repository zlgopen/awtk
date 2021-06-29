
#include "base/vg_gradient.h"
#include "gtest/gtest.h"

TEST(VGGradient, radial) {
  vg_gradient_t* g = vg_gradient_create_radial(10, 20, 30, 40);
  ASSERT_EQ(g->type, VG_GRADIENT_RADIAL);

  ASSERT_EQ(vg_gradient_add_stop(g, color_init(0, 0, 0, 0), 0), RET_OK);
  ASSERT_EQ(g->nr, 1);
  ASSERT_EQ(vg_gradient_add_stop(g, color_init(1, 1, 1, 1), 0.5), RET_OK);
  ASSERT_EQ(g->nr, 2);
  ASSERT_EQ(vg_gradient_add_stop(g, color_init(2, 2, 2, 2), 1), RET_OK);
  ASSERT_EQ(g->nr, 3);

  ASSERT_EQ(vg_gradient_get_stop(g, 0)->offset, 0);
  ASSERT_EQ(vg_gradient_get_stop(g, 0)->color.rgba.r, 0);
  
  ASSERT_EQ(vg_gradient_get_stop(g, 1)->offset, 0.5);
  ASSERT_EQ(vg_gradient_get_stop(g, 1)->color.rgba.r, 1);
  
  ASSERT_EQ(vg_gradient_get_stop(g, 2)->offset, 1);
  ASSERT_EQ(vg_gradient_get_stop(g, 2)->color.rgba.r, 2);

  vg_gradient_destroy(g);
}

TEST(VGGradient, linear) {
  uint32_t i = 0;
  vg_gradient_t* g = vg_gradient_create_linear(10, 20, 30, 40);
  ASSERT_EQ(g->type, VG_GRADIENT_LINEAR);

  for(i = 0; i < TK_GRADIENT_MAX_STOP_NR; i++) {
    ASSERT_EQ(vg_gradient_add_stop(g, color_init(0, 0, 0, 0), 0.1 * i), RET_OK);
    ASSERT_EQ(g->nr, i+1);
    ASSERT_EQ((int)(vg_gradient_get_stop(g, i)->offset * 10), i );
  }
  
  ASSERT_NE(vg_gradient_add_stop(g, color_init(0, 0, 0, 0), 0.1 * i), RET_OK);

  vg_gradient_destroy(g);
}

TEST(VGGradient, none) {
  vg_gradient_t g;
  vg_gradient_init(&g, VG_GRADIENT_NONE, 0, 0, 0, 0);
  ASSERT_EQ(g.type, VG_GRADIENT_NONE);
  ASSERT_EQ(g.nr, 0);

}
