/**
 * font_init_glyphs 在 params->shaping 为 FALSE/TRUE 时的行为测试
 */

#include <wchar.h>
#include "tkc/utils.h"
#include "base/font.h"
#include "base/font_manager.h"
#include "base/canvas.h"
#include "lcd_log.h"
#include "gtest/gtest.h"

#define TEST_FONT_NAME "test_shaping"
#define TEST_FONT_SIZE 20
#define TEST_GLYPH_ADVANCE 10
#define TEST_GLYPH_W 8
#define TEST_GLYPH_H 8

/*用于统计走了哪条分支*/
static int s_get_glyph_calls;
static int s_init_glyphs_calls;
static uint8_t s_glyph_data[TEST_GLYPH_W * TEST_GLYPH_H];

static ret_t test_font_get_glyph(font_t* f, wchar_t chr, font_size_t font_size, glyph_t* g) {
  s_get_glyph_calls++;
  memset(g, 0x00, sizeof(glyph_t));
  g->chr = (uint16_t)chr;
  g->w = TEST_GLYPH_W;
  g->h = TEST_GLYPH_H;
  g->advance = TEST_GLYPH_ADVANCE;
  g->data = s_glyph_data;
  (void)f;
  (void)font_size;

  return RET_OK;
}

/*通用（整形/bidi）路径的实现，字模属性与 get_glyph 保持一致，便于比对*/
static ret_t test_font_init_glyphs(font_t* f, glyphs_t* glyphs, const wchar_t* str, uint32_t len,
                                   font_size_t font_size, font_raster_params_t* params) {
  uint32_t i = 0;
  font_vmetrics_t vmetrics = font_get_vmetrics(f, font_size);
  ret_t ret = glyphs_init(glyphs, (wchar_t*)str, len, len, TRUE, font_size, &vmetrics);
  return_value_if_fail(ret == RET_OK, RET_FAIL);

  s_init_glyphs_calls++;
  glyphs->font = f;

  for (i = 0; i < len; i++) {
    glyph_t* g = &glyphs->glyphs[i];
    g->chr = (uint16_t)str[i];
    g->w = TEST_GLYPH_W;
    g->h = TEST_GLYPH_H;
    g->advance = TEST_GLYPH_ADVANCE;
    g->data = s_glyph_data;
    g->str_count = 1;
    g->glyph_count = 1;
    glyphs->l2v_maps[i] = (int32_t)i;
    glyphs->v2l_maps[i] = (int32_t)i;
  }

  (void)params;

  return RET_OK;
}

static bool_t test_font_match(font_t* f, const char* name, font_size_t font_size) {
  return tk_str_eq(f->name, name) && font_size == TEST_FONT_SIZE;
}

static void test_font_init(font_t* font) {
  memset(font, 0x00, sizeof(font_t));
  tk_strncpy(font->name, TEST_FONT_NAME, TK_NAME_LEN);
  font->match = test_font_match;
  font->get_glyph = test_font_get_glyph;
  font->init_glyphs = test_font_init_glyphs;
  font->params.shaping = TRUE;
}

static ret_t init_glyphs_with_shaping(font_t* f, glyphs_t* glyphs, const wchar_t* str, uint32_t len,
                                      bool_t shaping) {
  font_raster_params_t params;
  font_get_raster_params(f, &params);
  params.shaping = shaping;

  return font_init_glyphs(f, glyphs, str, len, TEST_FONT_SIZE, &params);
}

/*shaping 为 FALSE 时走 get_glyph 快路径，为 TRUE 时走 init_glyphs*/
TEST(FontInitGlyphs, dispatch_by_shaping) {
  glyphs_t glyphs;
  font_t font;
  font_manager_t fm;
  const wchar_t* str = L"abc";

  test_font_init(&font);
  font_manager_init(&fm, NULL);
  font_manager_add_font(&fm, &font);

  s_get_glyph_calls = 0;
  s_init_glyphs_calls = 0;
  ASSERT_EQ(init_glyphs_with_shaping(&font, &glyphs, str, 3, FALSE), RET_OK);
  ASSERT_EQ(s_init_glyphs_calls, 0);
  ASSERT_EQ(s_get_glyph_calls, 3);
  glyphs_deinit(&glyphs);

  s_get_glyph_calls = 0;
  s_init_glyphs_calls = 0;
  ASSERT_EQ(init_glyphs_with_shaping(&font, &glyphs, str, 3, TRUE), RET_OK);
  ASSERT_EQ(s_init_glyphs_calls, 1);
  ASSERT_EQ(s_get_glyph_calls, 0);
  glyphs_deinit(&glyphs);

  font_manager_deinit(&fm);
}

/*params 为 NULL 时按 font->params.shaping 选择实现*/
TEST(FontInitGlyphs, null_params_use_font_params) {
  glyphs_t glyphs;
  font_t font;
  font_manager_t fm;
  const wchar_t* str = L"abc";

  test_font_init(&font);
  font_manager_init(&fm, NULL);
  font_manager_add_font(&fm, &font);

  font.params.shaping = FALSE;
  s_get_glyph_calls = 0;
  s_init_glyphs_calls = 0;
  ASSERT_EQ(font_init_glyphs(&font, &glyphs, str, 3, TEST_FONT_SIZE, NULL), RET_OK);
  ASSERT_EQ(s_init_glyphs_calls, 0);
  ASSERT_EQ(s_get_glyph_calls, 3);
  glyphs_deinit(&glyphs);

  font.params.shaping = TRUE;
  s_get_glyph_calls = 0;
  s_init_glyphs_calls = 0;
  ASSERT_EQ(font_init_glyphs(&font, &glyphs, str, 3, TEST_FONT_SIZE, NULL), RET_OK);
  ASSERT_EQ(s_init_glyphs_calls, 1);
  ASSERT_EQ(s_get_glyph_calls, 0);
  glyphs_deinit(&glyphs);

  font_manager_deinit(&fm);
}

/*ASCII 文本：两条路径的长度、宽度、逐字模属性和索引映射必须一致*/
TEST(FontInitGlyphs, ascii_equal_to_bidi_path) {
  uint32_t i = 0;
  glyphs_t g_fast;
  glyphs_t g_full;
  font_t font;
  font_manager_t fm;
  const wchar_t* str = L"Hello, AWTK!";
  uint32_t len = (uint32_t)wcslen(str);

  test_font_init(&font);
  font_manager_init(&fm, NULL);
  font_manager_add_font(&fm, &font);

  ASSERT_EQ(init_glyphs_with_shaping(&font, &g_fast, str, len, FALSE), RET_OK);
  ASSERT_EQ(init_glyphs_with_shaping(&font, &g_full, str, len, TRUE), RET_OK);

  ASSERT_EQ(glyphs_get_length(&g_fast), len);
  ASSERT_EQ(glyphs_get_length(&g_fast), glyphs_get_length(&g_full));
  ASSERT_GT(glyphs_measure(&g_fast, 0, len), 0);
  ASSERT_EQ(glyphs_measure(&g_fast, 0, len), glyphs_measure(&g_full, 0, len));

  for (i = 0; i < len; i++) {
    const glyph_t* a = glyphs_get(&g_fast, i);
    const glyph_t* b = glyphs_get(&g_full, i);
    ASSERT_TRUE(a != NULL && b != NULL);
    ASSERT_EQ(a->chr, b->chr);
    ASSERT_EQ(a->advance, b->advance);
    ASSERT_EQ(a->w, b->w);
    ASSERT_EQ(a->h, b->h);
    ASSERT_EQ(a->str_count, b->str_count);
    ASSERT_EQ(a->glyph_count, b->glyph_count);
    /*快路径没有映射表，按恒等返回；通用路径填的也是恒等表*/
    ASSERT_EQ(glyphs_get_glyph_index_from_str_index(&g_fast, i), (int32_t)i);
    ASSERT_EQ(glyphs_get_glyph_index_from_str_index(&g_full, i), (int32_t)i);
    ASSERT_EQ(glyphs_get_str_index_from_glyph_index(&g_fast, i), (int32_t)i);
    ASSERT_EQ(glyphs_get_str_index_from_glyph_index(&g_full, i), (int32_t)i);
  }

  glyphs_deinit(&g_fast);
  glyphs_deinit(&g_full);
  font_manager_deinit(&fm);
}

/*中日韩：BMP 内的 CJK 字符不参与重排和变形，两条路径也必须一致*/
TEST(FontInitGlyphs, cjk_equal_to_bidi_path) {
  uint32_t i = 0;
  glyphs_t g_fast;
  glyphs_t g_full;
  font_t font;
  font_manager_t fm;
  const wchar_t* str = L"\u4e2d\u6587\u65e5\u672c\u8a9e\ud55c\uad6d\uc5b4ABC";
  uint32_t len = (uint32_t)wcslen(str);

  test_font_init(&font);
  font_manager_init(&fm, NULL);
  font_manager_add_font(&fm, &font);

  ASSERT_EQ(init_glyphs_with_shaping(&font, &g_fast, str, len, FALSE), RET_OK);
  ASSERT_EQ(init_glyphs_with_shaping(&font, &g_full, str, len, TRUE), RET_OK);

  ASSERT_EQ(glyphs_get_length(&g_fast), len);
  ASSERT_EQ(glyphs_get_length(&g_fast), glyphs_get_length(&g_full));
  ASSERT_EQ(glyphs_measure(&g_fast, 0, len), glyphs_measure(&g_full, 0, len));

  for (i = 0; i < len; i++) {
    const glyph_t* a = glyphs_get(&g_fast, i);
    const glyph_t* b = glyphs_get(&g_full, i);
    ASSERT_TRUE(a != NULL && b != NULL);
    ASSERT_EQ(a->chr, b->chr);
    ASSERT_EQ(a->advance, b->advance);
    ASSERT_EQ(glyphs_get_glyph_index_from_str_index(&g_fast, i), (int32_t)i);
    ASSERT_EQ(glyphs_get_glyph_index_from_str_index(&g_full, i), (int32_t)i);
  }

  glyphs_deinit(&g_fast);
  glyphs_deinit(&g_full);
  font_manager_deinit(&fm);
}

/*canvas 层：measure_text 的宽度和 draw_text 的绘制命令在两条路径下必须一致*/
TEST(CanvasTextShaping, measure_and_draw_equal) {
  canvas_t c;
  font_t font;
  font_manager_t fm;
  float_t w_full = 0;
  float_t w_fast = 0;
  lcd_t* lcd = lcd_log_init(400, 300);
  const wchar_t* str = L"\u4e2d\u6587ABC";
  uint32_t len = (uint32_t)wcslen(str);

  test_font_init(&font);
  font_manager_init(&fm, NULL);
  font_manager_add_font(&fm, &font);
  canvas_init(&c, lcd, &fm);
  c.font = &font;
  c.font_size = TEST_FONT_SIZE;
  font.params.shaping = TRUE;
  w_full = canvas_measure_text(&c, str, len);
  /*canvas 只有一条测量缓存，用另一个串把它顶掉，保证下面真正重新计算*/
  canvas_measure_text(&c, L"x", 1);
  font.params.shaping = FALSE;
  w_fast = canvas_measure_text(&c, str, len);
  ASSERT_GT(w_full, 0);
  ASSERT_EQ(w_full, w_fast);

  lcd_log_reset(lcd);
  font.params.shaping = TRUE;
  ASSERT_EQ(canvas_draw_text(&c, str, len, 0, 0), RET_OK);
  string log_full = lcd_log_get_commands(lcd);

  lcd_log_reset(lcd);
  font.params.shaping = FALSE;
  ASSERT_EQ(canvas_draw_text(&c, str, len, 0, 0), RET_OK);
  string log_fast = lcd_log_get_commands(lcd);

  ASSERT_GT(log_full.size(), 0);
  ASSERT_EQ(log_full, log_fast);

  canvas_reset(&c);
  font_manager_deinit(&fm);
  lcd_destroy(lcd);
}
