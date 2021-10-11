
#include "base/gradient.h"
#include "gtest/gtest.h"

TEST(Gradient, binary) {
  gradient_t a;
  gradient_t b;
  wbuffer_t wb;
  color_t red = color_init(0xff, 0, 0, 0xff);
  color_t blue = color_init(0, 0xff, 0, 0xff);
  color_t green = color_init(0, 0, 0xff, 0xff);

  ASSERT_EQ(gradient_init(&a) != NULL, true);
  ASSERT_EQ(gradient_set_type(&a, GRADIENT_LINEAR), RET_OK);
  ASSERT_EQ(gradient_set_linear_degree(&a, 90), RET_OK);

  ASSERT_EQ(gradient_add_stop(&a, red, 0), RET_OK);
  ASSERT_EQ(gradient_add_stop(&a, blue, 0.5), RET_OK);
  ASSERT_EQ(gradient_add_stop(&a, green, 1), RET_OK);
  ASSERT_EQ(a.nr, 3u);
  ASSERT_EQ(a.degree, 90u);
  ASSERT_EQ(a.type, GRADIENT_LINEAR);

  wbuffer_init_extendable(&wb);
  ASSERT_EQ(gradient_to_binary(&a, &wb), RET_OK);
  ASSERT_EQ(gradient_init_from_binary(&b, wb.data, wb.cursor) != NULL, true);

  ASSERT_EQ(memcmp(&a, &b, sizeof(a)) == 0, true);
  wbuffer_deinit(&wb);
}

TEST(Gradient, str1) {
  gradient_t a;
  color_t c;
  gradient_init_from_str(&a, "linear-gradient(#FF0000, #0000FF)");
  ASSERT_EQ(a.type, GRADIENT_LINEAR);
  ASSERT_EQ(a.degree, 0u);
  ASSERT_EQ(a.nr, 2u);

  ASSERT_EQ(a.stops[0].offset, 0.0f);
  ASSERT_EQ(a.stops[0].color.rgba.r, 0xff);
  ASSERT_EQ(a.stops[1].offset, 1.0f);
  ASSERT_EQ(a.stops[1].color.rgba.b, 0xff);

  c = gradient_get_color(&a, 0);
  ASSERT_EQ(c.rgba.r, 0xff);
  ASSERT_EQ(c.rgba.g, 0);
  ASSERT_EQ(c.rgba.b, 0);
  ASSERT_EQ(c.rgba.a, 0xff);

  c = gradient_get_color(&a, 0.5);
  ASSERT_EQ(c.rgba.r, 0x7f);
  ASSERT_EQ(c.rgba.g, 0);
  ASSERT_EQ(c.rgba.b, 0x7f);
  ASSERT_EQ(c.rgba.a, 0xff);

  c = gradient_get_color(&a, 1);
  ASSERT_EQ(c.rgba.r, 0);
  ASSERT_EQ(c.rgba.g, 0);
  ASSERT_EQ(c.rgba.b, 0xff);
  ASSERT_EQ(c.rgba.a, 0xff);

  str_t str;
  str_init(&str, 0);
  ASSERT_EQ(gradient_to_str(&a, &str), RET_OK);
  ASSERT_STREQ(str.str, "linear-gradient(0deg, #FF0000 0%, #0000FF 100%)");
  str_reset(&str);
}

TEST(Gradient, str2) {
  gradient_t a;
  gradient_init_from_str(&a, "linear-gradient(#FF0000 0%, #00FF00 50%, #0000FF 100%)");
  ASSERT_EQ(a.type, GRADIENT_LINEAR);
  ASSERT_EQ(a.degree, 0u);
  ASSERT_EQ(a.nr, 3u);

  ASSERT_EQ(a.stops[0].offset, 0.0f);
  ASSERT_EQ(a.stops[0].color.rgba.r, 0xff);
  ASSERT_EQ(a.stops[1].offset, 0.5f);
  ASSERT_EQ(a.stops[1].color.rgba.g, 0xff);
  ASSERT_EQ(a.stops[2].offset, 1.0f);
  ASSERT_EQ(a.stops[2].color.rgba.b, 0xff);

  str_t str;
  str_init(&str, 0);
  ASSERT_EQ(gradient_to_str(&a, &str), RET_OK);
  ASSERT_STREQ(str.str, "linear-gradient(0deg, #FF0000 0%, #00FF00 50%, #0000FF 100%)");
  str_reset(&str);
}

TEST(Gradient, str3) {
  gradient_t a;
  gradient_init_from_str(&a, "linear-gradient(0deg, #FF0000 0%, #00FF00 50%, #0000FF 100%)");
  ASSERT_EQ(a.type, GRADIENT_LINEAR);
  ASSERT_EQ(a.degree, 0u);
  ASSERT_EQ(a.nr, 3u);

  ASSERT_EQ(a.stops[0].offset, 0.0f);
  ASSERT_EQ(a.stops[0].color.rgba.r, 0xff);
  ASSERT_EQ(a.stops[0].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[1].offset, 0.5f);
  ASSERT_EQ(a.stops[1].color.rgba.g, 0xff);
  ASSERT_EQ(a.stops[1].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[2].offset, 1.0f);
  ASSERT_EQ(a.stops[2].color.rgba.b, 0xff);
  ASSERT_EQ(a.stops[2].color.rgba.a, 0xff);

  str_t str;
  str_init(&str, 0);
  ASSERT_EQ(gradient_to_str(&a, &str), RET_OK);
  ASSERT_STREQ(str.str, "linear-gradient(0deg, #FF0000 0%, #00FF00 50%, #0000FF 100%)");
  str_reset(&str);
}

TEST(Gradient, str4) {
  gradient_t a;
  gradient_init_from_str(&a, "linear-gradient(to top, #FF0000 0%, #00FF00 50%, #0000FF 100%)");
  ASSERT_EQ(a.type, GRADIENT_LINEAR);
  ASSERT_EQ(a.degree, 0u);
  ASSERT_EQ(a.nr, 3u);

  ASSERT_EQ(a.stops[0].offset, 0.0f);
  ASSERT_EQ(a.stops[0].color.rgba.r, 0xff);
  ASSERT_EQ(a.stops[0].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[1].offset, 0.5f);
  ASSERT_EQ(a.stops[1].color.rgba.g, 0xff);
  ASSERT_EQ(a.stops[1].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[2].offset, 1.0f);
  ASSERT_EQ(a.stops[2].color.rgba.b, 0xff);
  ASSERT_EQ(a.stops[2].color.rgba.a, 0xff);

  str_t str;
  str_init(&str, 0);
  ASSERT_EQ(gradient_to_str(&a, &str), RET_OK);
  ASSERT_STREQ(str.str, "linear-gradient(0deg, #FF0000 0%, #00FF00 50%, #0000FF 100%)");
  str_reset(&str);
}

TEST(Gradient, str5) {
  gradient_t a;
  gradient_init_from_str(&a, "linear-gradient(180deg, #FF0000 0%, #00FF00 50%, #0000FF 100%)");
  ASSERT_EQ(a.type, GRADIENT_LINEAR);
  ASSERT_EQ(a.degree, 180u);
  ASSERT_EQ(a.nr, 3u);

  ASSERT_EQ(a.stops[0].offset, 0.0f);
  ASSERT_EQ(a.stops[0].color.rgba.r, 0xff);
  ASSERT_EQ(a.stops[0].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[1].offset, 0.5f);
  ASSERT_EQ(a.stops[1].color.rgba.g, 0xff);
  ASSERT_EQ(a.stops[1].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[2].offset, 1.0f);
  ASSERT_EQ(a.stops[2].color.rgba.b, 0xff);
  ASSERT_EQ(a.stops[2].color.rgba.a, 0xff);

  str_t str;
  str_init(&str, 0);
  ASSERT_EQ(gradient_to_str(&a, &str), RET_OK);
  ASSERT_STREQ(str.str, "linear-gradient(180deg, #FF0000 0%, #00FF00 50%, #0000FF 100%)");
  str_reset(&str);
}

TEST(Gradient, str6) {
  gradient_t a;
  gradient_init_from_str(&a,
                         "linear-gradient(to right top, #FF0000 0%, #00FF00 50%, #0000FF 100%)");
  ASSERT_EQ(a.type, GRADIENT_LINEAR);
  ASSERT_EQ(a.degree, 45u);
  ASSERT_EQ(a.nr, 3u);

  ASSERT_EQ(a.stops[0].offset, 0.0f);
  ASSERT_EQ(a.stops[0].color.rgba.r, 0xff);
  ASSERT_EQ(a.stops[0].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[1].offset, 0.5f);
  ASSERT_EQ(a.stops[1].color.rgba.g, 0xff);
  ASSERT_EQ(a.stops[1].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[2].offset, 1.0f);
  ASSERT_EQ(a.stops[2].color.rgba.b, 0xff);
  ASSERT_EQ(a.stops[2].color.rgba.a, 0xff);

  str_t str;
  str_init(&str, 0);
  ASSERT_EQ(gradient_to_str(&a, &str), RET_OK);
  ASSERT_STREQ(str.str, "linear-gradient(45deg, #FF0000 0%, #00FF00 50%, #0000FF 100%)");
  str_reset(&str);
}

TEST(Gradient, str7) {
  gradient_t a;
  gradient_init_from_str(&a, "linear-gradient(180deg, red 0%, green 50%, blue 100%)");
  ASSERT_EQ(a.type, GRADIENT_LINEAR);
  ASSERT_EQ(a.degree, 180u);
  ASSERT_EQ(a.nr, 3u);

  ASSERT_EQ(a.stops[0].offset, 0.0f);
  ASSERT_EQ(a.stops[0].color.rgba.r, 0xff);
  ASSERT_EQ(a.stops[0].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[1].offset, 0.5f);
  ASSERT_EQ(a.stops[1].color.rgba.g, 0x80);
  ASSERT_EQ(a.stops[1].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[2].offset, 1.0f);
  ASSERT_EQ(a.stops[2].color.rgba.b, 0xff);
  ASSERT_EQ(a.stops[2].color.rgba.a, 0xff);
}

TEST(Gradient, str8) {
  gradient_t a;
  gradient_init_from_str(&a, "linear-gradient(180deg, red, green, blue)");
  ASSERT_EQ(a.type, GRADIENT_LINEAR);
  ASSERT_EQ(a.degree, 180u);
  ASSERT_EQ(a.nr, 3u);

  ASSERT_EQ(a.stops[0].offset, 0.0f);
  ASSERT_EQ(a.stops[0].color.rgba.r, 0xff);
  ASSERT_EQ(a.stops[0].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[1].offset, 0.5f);
  ASSERT_EQ(a.stops[1].color.rgba.g, 0x80);
  ASSERT_EQ(a.stops[1].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[2].offset, 1.0f);
  ASSERT_EQ(a.stops[2].color.rgba.b, 0xff);
  ASSERT_EQ(a.stops[2].color.rgba.a, 0xff);
}

TEST(Gradient, str9) {
  gradient_t a;
  gradient_init_from_str(
      &a, "linear-gradient(180deg, rgb(255,0,0) 0%, rgb(0,255,0) 50%, rgb(0, 0, 255) 100%)");
  ASSERT_EQ(a.type, GRADIENT_LINEAR);
  ASSERT_EQ(a.degree, 180u);
  ASSERT_EQ(a.nr, 3u);

  ASSERT_EQ(a.stops[0].offset, 0.0f);
  ASSERT_EQ(a.stops[0].color.rgba.r, 0xff);
  ASSERT_EQ(a.stops[0].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[1].offset, 0.5f);
  ASSERT_EQ(a.stops[1].color.rgba.g, 0xff);
  ASSERT_EQ(a.stops[1].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[2].offset, 1.0f);
  ASSERT_EQ(a.stops[2].color.rgba.b, 0xff);
  ASSERT_EQ(a.stops[2].color.rgba.a, 0xff);
}

TEST(Gradient, str10) {
  gradient_t a;
  gradient_init_from_str(&a, "linear-gradient(180deg, rgb(255,0,0), rgb(0,255,0), rgb(0,0,255))");
  ASSERT_EQ(a.type, GRADIENT_LINEAR);
  ASSERT_EQ(a.degree, 180u);
  ASSERT_EQ(a.nr, 3u);

  ASSERT_EQ(a.stops[0].offset, 0.0f);
  ASSERT_EQ(a.stops[0].color.rgba.r, 0xff);
  ASSERT_EQ(a.stops[0].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[1].offset, 0.5f);
  ASSERT_EQ(a.stops[1].color.rgba.g, 0xff);
  ASSERT_EQ(a.stops[1].color.rgba.a, 0xff);
  ASSERT_EQ(a.stops[2].offset, 1.0f);
  ASSERT_EQ(a.stops[2].color.rgba.b, 0xff);
  ASSERT_EQ(a.stops[2].color.rgba.a, 0xff);
}
