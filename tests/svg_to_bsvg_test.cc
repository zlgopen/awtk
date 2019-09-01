#include "tkc/mem.h"
#include "tkc/utils.h"
#include "gtest/gtest.h"
#include "svg/svg_to_bsvg.h"

static const char* s_template =
    "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
    "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg t=\"1542187942584\" "
    "class=\"icon\" style=\"\" viewBox=\"0 0 1024 1024\" version=\"1.1\" "
    "xmlns=\"http://www.w3.org/2000/svg\" p-id=\"1907\" "
    "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"16\" height=\"16\"><defs><style "
    "type=\"text/css\"></style></defs>%s</svg>";

static char s_buff[1024];

TEST(SVGToBSVG, parseNumbers) {
  float numbers[5] = {0, 0, 0, 0, 0};
  ASSERT_EQ(svg_parse_numbers("1.0 2.0", numbers, 5), 2);
  ASSERT_EQ(numbers[0], 1.0);
  ASSERT_EQ(numbers[1], 2.0);

  ASSERT_EQ(svg_parse_numbers("1.0 2.0 ", numbers, 5), 2);
  ASSERT_EQ(numbers[0], 1.0);
  ASSERT_EQ(numbers[1], 2.0);

  ASSERT_EQ(svg_parse_numbers("    1.0 2.0 ", numbers, 5), 2);
  ASSERT_EQ(numbers[0], 1.0);
  ASSERT_EQ(numbers[1], 2.0);

  ASSERT_EQ(svg_parse_numbers("    1.0 2.0 3.0", numbers, 5), 3);
  ASSERT_EQ(numbers[0], 1.0);
  ASSERT_EQ(numbers[1], 2.0);
  ASSERT_EQ(numbers[2], 3.0);

  ASSERT_EQ(svg_parse_numbers(" ", numbers, 5), 0);

  ASSERT_EQ(svg_parse_numbers(" 1", numbers, 5), 1);
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
  const svg_shape_rect_t* shape = NULL;
  const char* content =
      "<rect x=\"10\" y=\"10\" width=\"30\" height=\"40\" stroke=\"black\" fill=\"transparent\" "
      "stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = (const svg_shape_rect_t*)bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->shape.type, SVG_SHAPE_RECT);
  ASSERT_EQ(shape->shape.stroke.color, 0xff000000);
  ASSERT_EQ(shape->shape.stroke_width, 5);
  ASSERT_EQ(shape->x, 10);
  ASSERT_EQ(shape->y, 10);
  ASSERT_EQ(shape->w, 30);
  ASSERT_EQ(shape->h, 40);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, line) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_line_t* shape = NULL;
  const char* content =
      "<line x1=\"10\" y1=\"10\" x2=\"30\" y2=\"40\" stroke=\"black\" fill=\"red\" "
      "stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = (const svg_shape_line_t*)bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->shape.type, SVG_SHAPE_LINE);
  ASSERT_EQ(shape->shape.stroke.color, 0xff000000);
  ASSERT_EQ(shape->shape.fill.color, 0xff0000ff);
  ASSERT_EQ(shape->shape.stroke_width, 5);
  ASSERT_EQ(shape->x1, 10);
  ASSERT_EQ(shape->y1, 10);
  ASSERT_EQ(shape->x2, 30);
  ASSERT_EQ(shape->y2, 40);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, circle) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_circle_t* shape = NULL;
  const char* content =
      "<circle cx=\"10\" cy=\"10\" r=\"30\" stroke=\"black\" fill=\"red\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = (const svg_shape_circle_t*)bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->shape.type, SVG_SHAPE_CIRCLE);
  ASSERT_EQ(shape->shape.stroke.color, 0xff000000);
  ASSERT_EQ(shape->shape.fill.color, 0xff0000ff);
  ASSERT_EQ(shape->shape.stroke_width, 5);
  ASSERT_EQ(shape->cx, 10);
  ASSERT_EQ(shape->cy, 10);
  ASSERT_EQ(shape->r, 30);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_polygon_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 20 30 40\" stroke=\"black\" fill=\"red\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = (const svg_shape_polygon_t*)bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->shape.type, SVG_SHAPE_POLYGON);
  ASSERT_EQ(shape->shape.stroke.color, 0xff000000);
  ASSERT_EQ(shape->shape.fill.color, 0xff0000ff);
  ASSERT_EQ(shape->shape.stroke_width, 5);
  ASSERT_EQ(shape->nr, 4);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polyline) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_polyline_t* shape = NULL;
  const char* content =
      "<polyline points=\"10 20 30 40\" stroke=\"black\" fill=\"red\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = (const svg_shape_polyline_t*)bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->shape.type, SVG_SHAPE_POLYLINE);
  ASSERT_EQ(shape->shape.stroke.color, 0xff000000);
  ASSERT_EQ(shape->shape.fill.color, 0xff0000ff);
  ASSERT_EQ(shape->shape.stroke_width, 5);
  ASSERT_EQ(shape->nr, 4);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_lr) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_polygon_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"black\" fill=\"red\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = (const svg_shape_polygon_t*)bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->shape.type, SVG_SHAPE_POLYGON);
  ASSERT_EQ(shape->shape.stroke.color, 0xff000000);
  ASSERT_EQ(shape->shape.fill.color, 0xff0000ff);
  ASSERT_EQ(shape->shape.stroke_width, 5);
  ASSERT_EQ(shape->nr, 4);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_no_fill1) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_polygon_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"black\" fill=\"none\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = (const svg_shape_polygon_t*)bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->shape.type, SVG_SHAPE_POLYGON);
  ASSERT_EQ(shape->shape.stroke.color, 0xff000000);
  ASSERT_EQ(shape->shape.no_fill, TRUE);
  ASSERT_EQ(shape->shape.stroke_width, 5);
  ASSERT_EQ(shape->nr, 4);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_no_fill2) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_polygon_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"black\" fill=\"none\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = (const svg_shape_polygon_t*)bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->shape.type, SVG_SHAPE_POLYGON);
  ASSERT_EQ(shape->shape.stroke.color, 0xff000000);
  ASSERT_EQ(shape->shape.no_fill, TRUE);
  ASSERT_EQ(shape->shape.stroke_width, 5);
  ASSERT_EQ(shape->nr, 4);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_no_fill3) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_polygon_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"black\" fill=\"transparent\" "
      "stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = (const svg_shape_polygon_t*)bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->shape.type, SVG_SHAPE_POLYGON);
  ASSERT_EQ(shape->shape.stroke.color, 0xff000000);
  ASSERT_EQ(shape->shape.no_fill, TRUE);
  ASSERT_EQ(shape->shape.stroke_width, 5);
  ASSERT_EQ(shape->nr, 4);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_no_stroke1) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_polygon_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"none\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = (const svg_shape_polygon_t*)bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->shape.no_stroke, TRUE);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_no_stroke2) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_polygon_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"none\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = (const svg_shape_polygon_t*)bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->shape.no_stroke, TRUE);

  TKMEM_FREE(out);
}

TEST(SVGToBSVG, polygon_no_stroke3) {
  bsvg_t svg;
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const svg_shape_polygon_t* shape = NULL;
  const char* content =
      "<polygon points=\"10 \n20 \r\n30 \t40\" stroke=\"transparent\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff) - 1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);

  bsvg_init(&svg, out, out_length);
  shape = (const svg_shape_polygon_t*)bsvg_get_first_shape(&svg);

  ASSERT_EQ(shape->shape.no_stroke, TRUE);

  TKMEM_FREE(out);
}
