#include "tkc/mem.h"
#include "tkc/utils.h"
#include "gtest/gtest.h"
#include "svg/svg_to_bsvg.h"

static const char* s_template =
    "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
    "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg t=\"1542187942584\" "
    "class=\"icon\" style=\"\" viewBox=\"0 0 1024 1024\" version=\"1.1\" "
    "xmlns=\"http://www.w3.org/2000/svg\" p-id=\"1907\" "
    "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"1024\" height=\"1024\"><defs><style "
    "type=\"text/css\"></style></defs>%s</svg>";

static char s_buff[1024];

TEST(SVGToBSVG, text) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const svg_shape_text_t* shape_text = NULL;
  const char* text = NULL;
  const char* content = "<text x=\"10\" y=\"10\" fill=\"black\">abc</text>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape_text = (const svg_shape_text_t*)bsvg_get_first_shape(&svg);
  shape = (const svg_shape_t*)shape_text;

  ASSERT_EQ(shape->type, SVG_SHAPE_TEXT);
  ASSERT_EQ(shape->fill_type, SVG_COLOR_SOLID);
  ASSERT_EQ(shape->stroke_type, SVG_COLOR_NULL);
  ASSERT_EQ(svg_shape_get_fill(shape)->solid.color, 0xff000000u);
  ASSERT_EQ(shape_text->x, 10.0f);
  ASSERT_EQ(shape_text->y, 10.0f);
  text = svg_shape_text_get_text(shape);
  ASSERT_STREQ(text, "abc");

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, path) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const char* content = "<path d=\"M409.6 281.6v460.8l128-128h179.2L409.6 281.6z\" p-id=\"1908\">";
  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);

  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);
  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);
  ASSERT_EQ(shape->type, SVG_SHAPE_PATH);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, rect) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const char* content =
      "<rect x=\"10\" y=\"10\" width=\"30\" height=\"40\" stroke=\"black\" fill=\"transparent\" "
      "stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->type, SVG_SHAPE_PATH);
  ASSERT_EQ(shape->fill_type, SVG_COLOR_NULL);
  ASSERT_EQ(shape->stroke_type, SVG_COLOR_SOLID);
  ASSERT_EQ(svg_shape_get_stroke(shape)->solid.color, 0xff000000u);
  ASSERT_EQ(shape->stroke_width, 5.0f);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, line) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const char* content =
      "<line x1=\"10\" y1=\"10\" x2=\"30\" y2=\"40\" stroke=\"black\" fill=\"red\" "
      "stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->type, SVG_SHAPE_PATH);
  ASSERT_EQ(shape->fill_type, SVG_COLOR_SOLID);
  ASSERT_EQ(shape->stroke_type, SVG_COLOR_SOLID);
  ASSERT_EQ(svg_shape_get_fill(shape)->solid.color, 0xff0000ffu);
  ASSERT_EQ(svg_shape_get_stroke(shape)->solid.color, 0xff000000u);
  ASSERT_EQ(shape->stroke_width, 5.0f);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, circle) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const char* content =
      "<circle cx=\"10\" cy=\"10\" r=\"30\" stroke=\"black\" fill=\"red\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->type, SVG_SHAPE_PATH);
  ASSERT_EQ(shape->fill_type, SVG_COLOR_SOLID);
  ASSERT_EQ(shape->stroke_type, SVG_COLOR_SOLID);
  ASSERT_EQ(svg_shape_get_fill(shape)->solid.color, 0xff0000ffu);
  ASSERT_EQ(svg_shape_get_stroke(shape)->solid.color, 0xff000000u);
  ASSERT_EQ(shape->stroke_width, 5.0f);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 20 30 40\" stroke=\"black\" fill=\"red\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->type, SVG_SHAPE_PATH);
  ASSERT_EQ(shape->fill_type, SVG_COLOR_SOLID);
  ASSERT_EQ(shape->stroke_type, SVG_COLOR_SOLID);
  ASSERT_EQ(svg_shape_get_fill(shape)->solid.color, 0xff0000ffu);
  ASSERT_EQ(svg_shape_get_stroke(shape)->solid.color, 0xff000000u);
  ASSERT_EQ(shape->stroke_width, 5.0f);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polyline) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const char* content =
      "<polyline points=\"10 20 30 40\" stroke=\"black\" fill=\"red\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->type, SVG_SHAPE_PATH);
  ASSERT_EQ(shape->fill_type, SVG_COLOR_SOLID);
  ASSERT_EQ(shape->stroke_type, SVG_COLOR_SOLID);
  ASSERT_EQ(svg_shape_get_fill(shape)->solid.color, 0xff0000ffu);
  ASSERT_EQ(svg_shape_get_stroke(shape)->solid.color, 0xff000000u);
  ASSERT_EQ(shape->stroke_width, 5.0f);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_lr) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"black\" fill=\"red\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->type, SVG_SHAPE_PATH);
  ASSERT_EQ(shape->fill_type, SVG_COLOR_SOLID);
  ASSERT_EQ(shape->stroke_type, SVG_COLOR_SOLID);
  ASSERT_EQ(svg_shape_get_fill(shape)->solid.color, 0xff0000ffu);
  ASSERT_EQ(svg_shape_get_stroke(shape)->solid.color, 0xff000000u);
  ASSERT_EQ(shape->stroke_width, 5.0f);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_no_fill1) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"black\" fill=\"none\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->type, SVG_SHAPE_PATH);
  ASSERT_EQ(shape->fill_type, SVG_COLOR_NULL);
  ASSERT_EQ(shape->stroke_type, SVG_COLOR_SOLID);
  ASSERT_EQ(svg_shape_get_stroke(shape)->solid.color, 0xff000000u);
  ASSERT_EQ(shape->stroke_width, 5.0f);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_no_fill2) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"black\" fill=\"none\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->type, SVG_SHAPE_PATH);
  ASSERT_EQ(shape->fill_type, SVG_COLOR_NULL);
  ASSERT_EQ(shape->stroke_type, SVG_COLOR_SOLID);
  ASSERT_EQ(svg_shape_get_stroke(shape)->solid.color, 0xff000000u);
  ASSERT_EQ(shape->stroke_width, 5.0f);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_no_fill3) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"black\" fill=\"transparent\" "
      "stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->type, SVG_SHAPE_PATH);
  ASSERT_EQ(shape->fill_type, SVG_COLOR_NULL);
  ASSERT_EQ(shape->stroke_type, SVG_COLOR_SOLID);
  ASSERT_EQ(svg_shape_get_stroke(shape)->solid.color, 0xff000000u);
  ASSERT_EQ(shape->stroke_width, 5.0f);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_no_stroke1) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"none\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->stroke_type, SVG_COLOR_NULL);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_no_stroke2) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"none\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->stroke_type, SVG_COLOR_NULL);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_no_stroke3) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"transparent\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->stroke_type, SVG_COLOR_NULL);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_gradient_fill) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const svg_color_t* fill = NULL;
  const char* content =
      "<linearGradient id=\"gradient\"><stop offset=\"0\" stop-color=\"red\"/>"
      "<stop offset=\"1\" stop-color=\"blue\"/></linearGradient>"
      "<polygon points=\"10 \n20 \r\n30 \t40\" fill=\"url(#gradient)\"/>"
      "fill=\"url(#gradient)\">";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->fill_type, SVG_COLOR_LINEAR_GRADIENT);
  fill = svg_shape_get_fill(shape);
  ASSERT_EQ(fill->linear_gradient.icolor.color, color_init(0xff, 0x00, 0x00, 0xff).color);
  ASSERT_EQ(fill->linear_gradient.ocolor.color, color_init(0x00, 0x00, 0xff, 0xff).color);

  TKMEM_FREE(out);
}

#include "tkc/fs.h"

TEST(SVGToBSVG, file) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"transparent\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  file_write("test.svg", s_buff, strlen(s_buff));
  svg_file_to_bsvg("test.svg", "test.bsvg");
  out = (uint32_t*)file_read("test.bsvg", &out_length);
  file_remove("test.svg");
  file_remove("test.bsvg");

  bsvg_init(&svg, out, out_length);
  shape = bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->stroke_type, SVG_COLOR_NULL);

  TKMEM_FREE(out);
}
