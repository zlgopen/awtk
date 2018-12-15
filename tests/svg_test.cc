#include "svg/bsvg.h"
#include "svg/bsvg_builder.h"
#include "svg/svg_path_parser.h"
#include "tkc/utils.h"
#include "gtest/gtest.h"
#include "svg/bsvg_to_svg.h"

#include <string>
using std::string;

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

static void test_one_shape(svg_shape_t* shape) {
  string str;
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
}

TEST(SvgShape, rect) {
  str_t str;
  str_init(&str, 100);
  svg_shape_rect_t s;
  test_one_shape(svg_shape_rect_init(&s, 10, 20, 30, 40, 5));

  s.shape.line_cap = 'r';
  s.shape.line_join = 'b';
  s.shape.stroke_width = 5;
  s.shape.fill = color_init(0xff, 0xff, 0xff, 0xff);
  s.shape.stroke = color_init(0, 0, 0, 0xff);

  ASSERT_EQ(bsvg_to_svg_shape(&str, (const svg_shape_t*)&s), RET_OK);
  ASSERT_EQ(
      string(str.str),
      string("<rect x=\"10.0\" y=\"20.0\" width=\"30.0\" height=\"40.0\" rx=\"5.0\" ry=\"5.0\" "
             "stroke-linecap=\"round\" stroke-linejoin=\"bevel\" stroke-width==\"5\" "
             "fill=\"rgba(255,255,255,1.00)\" stroke=\"rgba(0,0,0,1.00)\"/>\n"));

  str_reset(&str);
}

TEST(SvgShape, line) {
  str_t str;
  str_init(&str, 100);
  svg_shape_line_t s;

  test_one_shape(svg_shape_line_init(&s, 10, 20, 30, 40));
  s.shape.line_cap = 'r';
  s.shape.line_join = 'm';

  ASSERT_EQ(bsvg_to_svg_shape(&str, (const svg_shape_t*)&s), RET_OK);
  ASSERT_EQ(string(str.str), string("<line x1=\"10.0\" y1=\"20.0\" x2=\"30.0\" y2=\"40.0\" "
                                    "stroke-linecap=\"round\" stroke-linejoin=\"miter\"/>\n"));

  str_reset(&str);
}

TEST(SvgShape, circle) {
  str_t str;
  str_init(&str, 100);
  svg_shape_circle_t s;
  test_one_shape(svg_shape_circle_init(&s, 10, 20, 30));

  ASSERT_EQ(bsvg_to_svg_shape(&str, (const svg_shape_t*)&s), RET_OK);
  ASSERT_EQ(string(str.str), string("<circle cx=\"10.0\" cy=\"20.0\" r=\"30.0\"/>\n"));

  str_reset(&str);
}

TEST(SvgShape, ellipse) {
  str_t str;
  str_init(&str, 100);
  svg_shape_ellipse_t s;
  test_one_shape(svg_shape_ellipse_init(&s, 10, 20, 30, 40));

  ASSERT_EQ(bsvg_to_svg_shape(&str, (const svg_shape_t*)&s), RET_OK);
  ASSERT_EQ(string(str.str),
            string("<ellipse cx=\"10.0\" cy=\"20.0\" rx=\"30.0\" ry=\"40.0\"/>\n"));

  str_reset(&str);
}

TEST(SvgShape, polygon) {
  str_t str;
  str_init(&str, 100);
  uint32_t buff[100];
  svg_shape_polygon_t* s = (svg_shape_polygon_t*)(buff);
  svg_shape_polygon_init(s);

  s->nr = 4;
  s->data[0] = 1;
  s->data[1] = 2;
  s->data[2] = 3;
  s->data[3] = 4;

  test_one_shape((svg_shape_t*)s);
  ASSERT_EQ(bsvg_to_svg_shape(&str, (const svg_shape_t*)s), RET_OK);
  ASSERT_EQ(string(str.str), string("<polygon points=\"1.0 2.0 3.0 4.0 \"/>\n"));

  str_reset(&str);
}

TEST(SvgShape, polyline) {
  str_t str;
  str_init(&str, 100);
  uint32_t buff[100];
  svg_shape_polyline_t* s = (svg_shape_polyline_t*)(buff);
  svg_shape_polyline_init(s);

  s->nr = 4;
  s->data[0] = 1;
  s->data[1] = 2;
  s->data[2] = 3;
  s->data[3] = 4;

  test_one_shape((svg_shape_t*)s);
  ASSERT_EQ(bsvg_to_svg_shape(&str, (const svg_shape_t*)s), RET_OK);
  ASSERT_EQ(string(str.str), string("<polyline points=\"1.0 2.0 3.0 4.0 \"/>\n"));

  str_reset(&str);
}

TEST(SvgShape, path) {
  str_t str;
  str_init(&str, 100);
  svg_shape_path_t s;
  test_one_shape(svg_shape_path_init(&s));

  ASSERT_EQ(bsvg_to_svg_shape(&str, (const svg_shape_t*)&s), RET_OK);
  ASSERT_EQ(string(str.str), string("<path d=\""));

  str_reset(&str);
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

static void test_one_path(svg_path_t* path) {
  string str;
  bsvg_t bsvg;
  svg_shape_path_t s;
  bsvg_t* svg = &bsvg;
  uint32_t buff[1024];
  bsvg_builder_t bsvg_builder;
  bsvg_builder_t* w = &bsvg_builder;
  svg_shape_t* shape = svg_shape_path_init(&s);

  ASSERT_EQ(bsvg_builder_init(w, buff, sizeof(buff)), w);
  ASSERT_EQ(w->header->magic, BSVG_MAGIC);
  ASSERT_EQ(w->buff.cursor, sizeof(bsvg_header_t));

  ASSERT_EQ(bsvg_builder_add_shape(w, shape), RET_OK);
  ;
  ASSERT_EQ(bsvg_builder_add_sub_path(w, path), RET_OK);
  ASSERT_EQ(bsvg_builder_done(w), RET_OK);
  ;

  ASSERT_EQ(bsvg_init(svg, buff, w->buff.cursor), svg);
  ASSERT_EQ(bsvg_visit(svg, path, on_shape_null, on_path), RET_OK);
}

TEST(SvgPath, move) {
  str_t s;
  str_init(&s, 100);
  svg_path_move_t path;
  test_one_path(svg_path_move_init(&path, 1, 2));

  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" M1.0 2.0"));

  test_one_path(svg_path_move_rel_init(&path, 1, 2));

  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" m1.0 2.0"));

  str_reset(&s);
}

TEST(SvgPath, line) {
  str_t s;
  str_init(&s, 100);
  svg_path_line_t path;

  test_one_path(svg_path_line_init(&path, 1, 2));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" L1.0 2.0"));

  test_one_path(svg_path_line_rel_init(&path, 1, 2));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" l1.0 2.0"));

  str_reset(&s);
}

TEST(SvgPath, hline) {
  str_t s;
  str_init(&s, 100);
  svg_path_hline_t path;

  test_one_path(svg_path_hline_init(&path, 12));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" H12.0"));

  test_one_path(svg_path_hline_rel_init(&path, 12));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" h12.0"));

  str_reset(&s);
}

TEST(SvgPath, vline) {
  str_t s;
  str_init(&s, 100);
  svg_path_vline_t path;
  test_one_path(svg_path_vline_init(&path, 12));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" V12.0"));

  test_one_path(svg_path_vline_rel_init(&path, 12));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" v12.0"));

  str_reset(&s);
}

TEST(SvgPath, curve) {
  str_t s;
  str_init(&s, 100);
  svg_path_curve_to_t path;
  test_one_path(svg_path_curve_to_init(&path, 1, 2, 3, 4, 5, 6));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" C1.0 2.0 3.0 4.0 5.0 6.0"));

  test_one_path(svg_path_curve_to_rel_init(&path, 1, 2, 3, 4, 5, 6));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" c1.0 2.0 3.0 4.0 5.0 6.0"));
  str_reset(&s);
}

TEST(SvgPath, scurve) {
  str_t s;
  str_init(&s, 100);
  svg_path_scurve_to_t path;
  test_one_path(svg_path_scurve_to_init(&path, 1, 2, 3, 4));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" S1.0 2.0 3.0 4.0"));

  test_one_path(svg_path_scurve_to_rel_init(&path, 1, 2, 3, 4));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" s1.0 2.0 3.0 4.0"));

  str_reset(&s);
}

TEST(SvgPath, qcurve) {
  str_t s;
  str_init(&s, 100);
  svg_path_qcurve_to_t path;
  test_one_path(svg_path_qcurve_to_init(&path, 1, 2, 3, 4));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" Q1.0 2.0 3.0 4.0"));

  test_one_path(svg_path_qcurve_to_rel_init(&path, 1, 2, 3, 4));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" q1.0 2.0 3.0 4.0"));

  str_reset(&s);
}

TEST(SvgPath, tcurve) {
  str_t s;
  str_init(&s, 100);
  svg_path_tcurve_to_t path;
  test_one_path(svg_path_tcurve_to_init(&path, 1, 2));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" T1.0 2.0"));

  test_one_path(svg_path_tcurve_to_rel_init(&path, 1, 2));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" t1.0 2.0"));

  str_reset(&s);
}

TEST(SvgPath, arc) {
  str_t s;
  str_init(&s, 100);
  svg_path_arc_t path;
  test_one_path(svg_path_arc_init(&path, 1, 2, 3, 1, 1, 6, 7));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" A1.0 2.0 3.0 1 1 6.0 7.0"));

  test_one_path(svg_path_arc_rel_init(&path, 1, 2, 3, 1, 1, 6, 7));
  str_set(&s, "");
  bsvg_to_svg_path(&s, (svg_path_t*)&path);
  ASSERT_EQ(string(s.str), string(" a1.0 2.0 3.0 1 1 6.0 7.0"));

  str_reset(&s);
}

TEST(SVGPathParser, move) {
  svg_path_move_t path;
  svg_path_move_init(&path, 409.6, 281.6);
  svg_path_parse("M409.6 281.6", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("M409.6 281.6 409.6 281.6", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);

  svg_path_move_rel_init(&path, 409.6, 281.6);
  svg_path_parse("m409.6 281.6", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("m409.6 281.6 409.6 281.6", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);
}

TEST(SVGPathParser, line) {
  svg_path_line_t path;

  svg_path_line_init(&path, 128, -128.5);
  svg_path_parse("L128-128.5", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("L128-128.5 128-128.5", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);

  svg_path_line_rel_init(&path, 128, -128.5);
  svg_path_parse("l128-128.5", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("l128-128.5 128-128.5", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);
}

TEST(SVGPathParser, hline) {
  svg_path_hline_t path;
  svg_path_hline_init(&path, 179.2);
  svg_path_parse("H179.2", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("H179.2 179.2", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);

  svg_path_hline_rel_init(&path, 179.2);
  svg_path_parse("h179.2", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("h179.2 179.2", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);
}

TEST(SVGPathParser, vline) {
  svg_path_vline_t path;
  svg_path_vline_init(&path, -179.2);
  svg_path_parse("V-179.2", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("V-179.2-179.2", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);

  svg_path_vline_rel_init(&path, -179.2);
  svg_path_parse("v-179.2", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("v-179.2-179.2", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);
}

TEST(SVGPathParser, curve) {
  svg_path_curve_to_t path;
  svg_path_curve_to_init(&path, 1, 2, 3, 4, 5, 6);
  svg_path_parse("C1 2 3 4 5 6", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("C1 2 3 4 5 6 1 2 3 4 5 6 ", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);

  svg_path_curve_to_rel_init(&path, 1, 2, 3, 4, 5, 6);
  svg_path_parse("c1 2 3 4 5 6", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("c1 2 3 4 5 6 1 2 3 4 5 6 ", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);
}

TEST(SVGPathParser, scurve) {
  svg_path_scurve_to_t path;
  svg_path_scurve_to_init(&path, 1, 2, 3, 4);
  svg_path_parse("S1 2 3 4", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("S1 2 3 4 1 2 3 4", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);

  svg_path_scurve_to_rel_init(&path, 1, 2, 3, 4);
  svg_path_parse("s1 2 3 4", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("s1 2 3 4 1 2 3 4", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);
}

TEST(SVGPathParser, qcurve) {
  svg_path_qcurve_to_t path;
  svg_path_qcurve_to_init(&path, 1, 2, 3, 4);
  svg_path_parse("Q1 2 3 4", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("Q1 2 3 4 1 2 3 4", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);

  svg_path_qcurve_to_rel_init(&path, 1, 2, 3, 4);
  svg_path_parse("q1 2 3 4", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("q1 2 3 4 1 2 3 4", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);
}

TEST(SVGPathParser, tcurve) {
  svg_path_tcurve_to_t path;
  svg_path_tcurve_to_init(&path, 1, 2);
  svg_path_parse("T1 2", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("T1 2 1 2", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);

  svg_path_tcurve_to_rel_init(&path, 1, 2);
  svg_path_parse("t1 2", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("t1 2 1 2", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);
}

TEST(SVGPathParser, arc) {
  svg_path_arc_t path;
  svg_path_arc_init(&path, 1, 2, 3, 1, 1, 6, 7);
  svg_path_parse("A1 2 3 1 1 6 7", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("A1 2 3 1 1 6 7 1 2 3 1 1 6 7", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);

  svg_path_arc_rel_init(&path, 1, 2, 3, 1, 1, 6, 7);
  svg_path_parse("a1 2 3 1 1 6 7", &path, on_path);

  s_on_path_count = 0;
  svg_path_parse("a1 2 3 1 1 6 7 1 2 3 1 1 6 7", &path, on_path);
  ASSERT_EQ(s_on_path_count, 2);
}
