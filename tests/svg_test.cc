#include "svg/bsvg.h"
#include "svg/bsvg_builder.h"
#include "tkc/utils.h"
#include "gtest/gtest.h"
#include "svg/bsvg_to_svg.h"

static void cmp_shape(const svg_shape_t* shape1, const svg_shape_t* shape2) {
  if (shape2->type != SVG_SHAPE_NULL) {
    ASSERT_EQ(shape1->type, shape2->type);
    ASSERT_EQ(memcmp(shape1, shape2, svg_shape_size(shape1)), 0);
  }
}

static ret_t on_shape(void* ctx, const void* data) {
  cmp_shape((const svg_shape_t*)ctx, (const svg_shape_t*)data);

  return RET_OK;
}

static ret_t on_path_null(void* ctx, const void* data) {
  (void)ctx;
  (void)data;

  return RET_OK;
}

static void test_one_shape(svg_shape_t* shape, const char* str2) {
  str_t str;
  bsvg_t bsvg;
  bsvg_t* svg = &bsvg;
  uint32_t buff[1024];
  bsvg_builder_t bsvg_builder;
  bsvg_builder_t* w = &bsvg_builder;

  ASSERT_EQ(bsvg_builder_init(w, buff, sizeof(buff)), w);
  ASSERT_EQ(w->header->magic, BSVG_MAGIC);
  ASSERT_EQ(w->buff.cursor, sizeof(bsvg_header_t));

  ASSERT_EQ(bsvg_builder_add_shape(w, shape), RET_OK);
  ASSERT_EQ(bsvg_builder_done(w), RET_OK);

  ASSERT_EQ(bsvg_init(svg, buff, w->buff.cursor), svg);
  ASSERT_EQ(bsvg_visit(svg, shape, on_shape, on_path_null), RET_OK);

  str_init(&str, 100);
  ASSERT_EQ(bsvg_to_svg_shape(&str, shape), RET_OK);
  ASSERT_STREQ(str.str, str2);
  str_reset(&str);
}

TEST(SvgShape, text) {
  uint8_t buff[128];
  svg_shape_t* shape = svg_shape_text_init((svg_shape_text_t*)&buff, 3.0f, 3.0f);
  char* str = NULL;
  shape->stroke_width = 5;
  shape->fill_type = SVG_COLOR_SOLID;
  svg_color_solid_init(svg_shape_get_fill(shape), color_init(0xff, 0xff, 0xff, 0xff));
  shape->stroke_type = SVG_COLOR_NULL;
  str = svg_shape_text_get_text(shape);
  memcpy(str, "abc123", 7);

  test_one_shape(shape,
                 "<text x=\"3.0\" y=\"3.0\" stroke-width=\"5.0\" fill=\"rgba(255,255,255,1.00)\" "
                 "stroke=\"none\">abc123</text>\n");
}

TEST(SvgShape, path) {
  svg_shape_t s;
  test_one_shape(svg_shape_path_init(&s), "<path d=\"");
}

static void cmp_path(const svg_path_t* path1, const svg_path_t* path2) {
  if (path2->type != SVG_PATH_NULL) {
    ASSERT_EQ(path1->type, path2->type);
    ASSERT_EQ(memcmp(path1, path2, svg_path_size(path1)), 0);
  }
}

static uint32_t s_on_path_count = 0;
static ret_t on_path(void* ctx, const void* data) {
  cmp_path((const svg_path_t*)ctx, (const svg_path_t*)data);
  s_on_path_count++;

  return RET_OK;
}

static ret_t on_shape_null(void* ctx, const void* data) {
  (void)ctx;
  (void)data;

  return RET_OK;
}

static void test_one_path(svg_path_t* path, const char* str2) {
  str_t str;
  bsvg_t bsvg;
  bsvg_t* svg = &bsvg;
  uint32_t buff[1024];
  bsvg_builder_t bsvg_builder;
  bsvg_builder_t* w = &bsvg_builder;
  svg_shape_t s;
  svg_shape_t* shape = svg_shape_path_init(&s);

  ASSERT_EQ(bsvg_builder_init(w, buff, sizeof(buff)), w);
  ASSERT_EQ(w->header->magic, BSVG_MAGIC);
  ASSERT_EQ(w->buff.cursor, sizeof(bsvg_header_t));

  ASSERT_EQ(bsvg_builder_add_shape(w, shape), RET_OK);
  ASSERT_EQ(bsvg_builder_add_sub_path(w, path), RET_OK);
  ASSERT_EQ(bsvg_builder_done(w), RET_OK);

  ASSERT_EQ(bsvg_init(svg, buff, w->buff.cursor), svg);
  ASSERT_EQ(bsvg_visit(svg, path, on_shape_null, on_path), RET_OK);

  str_init(&str, 100);
  ASSERT_EQ(bsvg_to_svg_path(&str, path), RET_OK);
  ASSERT_STREQ(str.str, str2);
  str_reset(&str);
}

TEST(SvgPath, move) {
  svg_path_move_t path;
  svg_path_t* p = svg_path_move_init(&path, 1, 2);
  test_one_path(p, " M1.0 2.0");
}

TEST(SvgPath, line) {
  svg_path_line_t path;
  svg_path_t* p = svg_path_line_init(&path, 1, 2);
  test_one_path(p, " L1.0 2.0");
}

TEST(SvgPath, curve) {
  svg_path_curve_to_t path;
  svg_path_t* p = svg_path_curve_to_init(&path, 1, 2, 3, 4, 5, 6);
  test_one_path(p, " C1.0 2.0 3.0 4.0 5.0 6.0");
}
