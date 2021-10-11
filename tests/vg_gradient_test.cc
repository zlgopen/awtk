
#include "base/vg_gradient.h"
#include "gtest/gtest.h"

TEST(VGGradient, radial) {
  vg_gradient_t* g = vg_gradient_create_radial(10, 20, 100, 30, 40, 200);
  ASSERT_EQ(g->gradient.type, GRADIENT_RADIAL);
  ASSERT_EQ(g->info.radial.x0, 10.0f);
  ASSERT_EQ(g->info.radial.y0, 20.0f);
  ASSERT_EQ(g->info.radial.r0, 100.0f);
  ASSERT_EQ(g->info.radial.x1, 30.0f);
  ASSERT_EQ(g->info.radial.y1, 40.0f);
  ASSERT_EQ(g->info.radial.r1, 200.0f);

  ASSERT_EQ(vg_gradient_add_stop(g, color_init(0, 0, 0, 0), 0), RET_OK);
  ASSERT_EQ(g->gradient.nr, 1u);
  ASSERT_EQ(vg_gradient_add_stop(g, color_init(1, 1, 1, 1), 0.5), RET_OK);
  ASSERT_EQ(g->gradient.nr, 2u);
  ASSERT_EQ(vg_gradient_add_stop(g, color_init(2, 2, 2, 2), 1), RET_OK);
  ASSERT_EQ(g->gradient.nr, 3u);

  ASSERT_EQ(vg_gradient_get_stop(g, 0)->offset, 0.0f);
  ASSERT_EQ(vg_gradient_get_stop(g, 0)->color.rgba.r, 0.0f);

  ASSERT_EQ(vg_gradient_get_stop(g, 1)->offset, 0.5f);
  ASSERT_EQ(vg_gradient_get_stop(g, 1)->color.rgba.r, 1);

  ASSERT_EQ(vg_gradient_get_stop(g, 2)->offset, 1.0f);
  ASSERT_EQ(vg_gradient_get_stop(g, 2)->color.rgba.r, 2);

  vg_gradient_destroy(g);
}

TEST(VGGradient, linear) {
  uint32_t i = 0;
  vg_gradient_t* g = vg_gradient_create_linear(10, 20, 30, 40);
  ASSERT_EQ(g->gradient.type, GRADIENT_LINEAR);
  ASSERT_EQ(g->info.linear.sx, 10.0f);
  ASSERT_EQ(g->info.linear.sy, 20.0f);
  ASSERT_EQ(g->info.linear.ex, 30.0f);
  ASSERT_EQ(g->info.linear.ey, 40.0f);

  for (i = 0; i < TK_GRADIENT_MAX_STOP_NR; i++) {
    ASSERT_EQ(vg_gradient_add_stop(g, color_init(0, 0, 0, 0), 0.1 * i), RET_OK);
    ASSERT_EQ(g->gradient.nr, i + 1u);
    ASSERT_EQ((uint32_t)(vg_gradient_get_stop(g, i)->offset * 10), i);
  }

  ASSERT_NE(vg_gradient_add_stop(g, color_init(0, 0, 0, 0), 0.1 * i), RET_OK);

  vg_gradient_destroy(g);
}

TEST(VGGradient, init_linear) {
  vg_gradient_t gg;
  vg_gradient_t* g = vg_gradient_init_linear(&gg, 10, 20, 30, 40);
  ASSERT_EQ(g->gradient.type, GRADIENT_LINEAR);
  ASSERT_EQ(g->info.linear.sx, 10.0f);
  ASSERT_EQ(g->info.linear.sy, 20.0f);
  ASSERT_EQ(g->info.linear.ex, 30.0f);
  ASSERT_EQ(g->info.linear.ey, 40.0f);
}

TEST(VGGradient, init_gradient) {
  vg_gradient_t gg;
  vg_gradient_t* g = vg_gradient_init_radial(&gg, 10, 20, 100, 30, 40, 200);
  ASSERT_EQ(g->gradient.type, GRADIENT_RADIAL);
  ASSERT_EQ(g->info.radial.x0, 10.0f);
  ASSERT_EQ(g->info.radial.y0, 20.0f);
  ASSERT_EQ(g->info.radial.r0, 100.0f);
  ASSERT_EQ(g->info.radial.x1, 30.0f);
  ASSERT_EQ(g->info.radial.y1, 40.0f);
  ASSERT_EQ(g->info.radial.r1, 200.0f);
}

TEST(VGGradient, init_with_gradient) {
  vg_gradient_t gg;
  gradient_t agradient;
  rect_t rect = {0, 0, 100, 100};
  gradient_init_from_str(&agradient, "linear-gradient(180deg, #6AB2FF, #0D81FF);");
  vg_gradient_t* g = vg_gradient_init_with_gradient(&gg, &rect, &agradient);
  ASSERT_EQ(g->gradient.type, GRADIENT_LINEAR);
  ASSERT_EQ(g->info.linear.sx, 0.0f);
  ASSERT_EQ(g->info.linear.sy, 0.0f);
  ASSERT_EQ(g->info.linear.ex, 0.0f);
  ASSERT_EQ(g->info.linear.ey, 100.0f);
}
