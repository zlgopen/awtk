/**
 * Brief:  canvas 文本绘制/测量必须按字模数（glyphs_get_length）而不是字符数工作
 */

#include <string.h>
#include <wchar.h>
#include "tkc/utils.h"
#include "base/font.h"
#include "base/font_manager.h"
#include "base/canvas.h"
#include "lcd_log.h"
#include "gtest/gtest.h"

#define TEST_FONT_NAME "test_glyph_count"
#define TEST_FONT_SIZE 20
#define TEST_GLYPH_ADVANCE 10
#define TEST_GLYPH_W 8
#define TEST_GLYPH_H 8

static uint8_t s_glyph_data[TEST_GLYPH_W * TEST_GLYPH_H];

static ret_t test_font_get_glyph(font_t* f, wchar_t chr, font_size_t font_size, glyph_t* g) {
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

/**
 * 模拟"字模数小于字符数"的字体：每两个码元合成一个字模。
 * 2 字节 wchar_t 平台上的代理对（emoji、CJK 扩展 B 汉字）就是这个形态。
 */
static ret_t test_font_init_glyphs(font_t* f, glyphs_t* glyphs, const wchar_t* str, uint32_t len,
                                   font_size_t font_size, font_raster_params_t* params) {
  uint32_t i = 0;
  uint32_t glyph_len = len / 2;
  font_vmetrics_t vmetrics = font_get_vmetrics(f, font_size);
  ret_t ret = RET_FAIL;

  return_value_if_fail(glyph_len > 0, RET_BAD_PARAMS);

  ret = glyphs_init(glyphs, (wchar_t*)str, glyph_len, len, TRUE, font_size, &vmetrics);
  return_value_if_fail(ret == RET_OK, RET_FAIL);
  glyphs->font = f;

  for (i = 0; i < glyph_len; i++) {
    glyph_t* g = &glyphs->glyphs[i];
    g->chr = (uint16_t)str[i * 2];
    g->w = TEST_GLYPH_W;
    g->h = TEST_GLYPH_H;
    g->advance = TEST_GLYPH_ADVANCE;
    g->data = s_glyph_data;
    g->str_count = 2;
    g->glyph_count = 1;
    glyphs->l2v_maps[i * 2] = (int32_t)i;
    glyphs->l2v_maps[i * 2 + 1] = (int32_t)i;
    glyphs->v2l_maps[i] = (int32_t)(i * 2);
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

static int count_commands(const string& log, const char* cmd) {
  int n = 0;
  size_t pos = 0;
  size_t step = strlen(cmd);

  while ((pos = log.find(cmd, pos)) != string::npos) {
    n++;
    pos += step;
  }

  return n;
}

/*画布和字体准备*/
static canvas_t* canvas_prepare(canvas_t* c, font_t* font, font_manager_t* fm, lcd_t** lcd) {
  *lcd = lcd_log_init(400, 300);
  test_font_init(font);
  font_manager_init(fm, NULL);
  font_manager_add_font(fm, font);
  canvas_init(c, *lcd, fm);
  c->font = font;
  c->font_size = TEST_FONT_SIZE;

  return c;
}

/*6 个码元 = 3 个字模*/
static const wchar_t* s_text = L"ABCDEF";

/*测量：宽度必须按 3 个字模算，而不是按 6 个字符算*/
TEST(CanvasTextGlyphCount, measure_text) {
  glyphs_t ref;
  canvas_t c;
  font_t font;
  font_manager_t fm;
  lcd_t* lcd = NULL;
  float_t expected = 0;
  float_t w = 0;
  uint32_t len = (uint32_t)wcslen(s_text);

  canvas_prepare(&c, &font, &fm, &lcd);
  ASSERT_EQ(len, 6);

  /*用正确的接口算出期望值，避免依赖 device_pixel_ratio 的具体取值*/
  ASSERT_EQ(font_init_glyphs(&font, &ref, s_text, len, TEST_FONT_SIZE, NULL), RET_OK);
  ASSERT_EQ(glyphs_get_length(&ref), 3);
  expected = glyphs_measure(&ref, 0, glyphs_get_length(&ref));
  glyphs_deinit(&ref);
  ASSERT_GT(expected, 0);

  w = canvas_measure_text(&c, s_text, len);
  ASSERT_EQ(w, expected);

  canvas_reset(&c);
  font_manager_deinit(&fm);
  lcd_destroy(lcd);
}

/*draw_text_in_rect：必须画出 3 个字模（修复前因为 nr > 字模数而一个都不画）*/
TEST(CanvasTextGlyphCount, draw_text_in_rect) {
  rect_t r = rect_init(0, 0, 300, 40);
  canvas_t c;
  font_t font;
  font_manager_t fm;
  lcd_t* lcd = NULL;
  uint32_t len = (uint32_t)wcslen(s_text);

  canvas_prepare(&c, &font, &fm, &lcd);

  lcd_log_reset(lcd);
  ASSERT_EQ(canvas_draw_text_in_rect(&c, s_text, len, &r), RET_OK);
  ASSERT_EQ(count_commands(lcd_log_get_commands(lcd), "dg("), 3);

  canvas_reset(&c);
  font_manager_deinit(&fm);
  lcd_destroy(lcd);
}

/*draw_text：同样按字模数绘制*/
TEST(CanvasTextGlyphCount, draw_text) {
  canvas_t c;
  font_t font;
  font_manager_t fm;
  lcd_t* lcd = NULL;
  uint32_t len = (uint32_t)wcslen(s_text);

  canvas_prepare(&c, &font, &fm, &lcd);

  lcd_log_reset(lcd);
  ASSERT_EQ(canvas_draw_text(&c, s_text, len, 0, 0), RET_OK);
  ASSERT_EQ(count_commands(lcd_log_get_commands(lcd), "dg("), 3);

  canvas_reset(&c);
  font_manager_deinit(&fm);
  lcd_destroy(lcd);
}

/*省略号路径：测量和绘制都必须从 start 开始（修复前绘制固定从 0 开始）*/
TEST(CanvasTextEllipsesStart, draw_from_start) {
  uint32_t i = 0;
  uint32_t drawn = 0;
  uint32_t start = 1;
  uint32_t len = 3;
  float_t text_w = 0;
  float_t char_w = 0;
  float_t ellipses_w = 0;
  glyphs_t glyphs;
  glyphs_t ellipse_glyphs;
  canvas_t c;
  font_t font;
  font_manager_t fm;
  lcd_t* lcd = NULL;
  rect_t r = rect_init(0, 0, 0, 40);
  const wchar_t* text = L"ABCDEFGH";
  uint32_t text_len = (uint32_t)wcslen(text);

  canvas_prepare(&c, &font, &fm, &lcd);

  ASSERT_EQ(font_init_glyphs(&font, &glyphs, text, text_len, TEST_FONT_SIZE, NULL), RET_OK);
  ASSERT_EQ(glyphs_get_length(&glyphs), 4);
  ASSERT_EQ(font_init_glyphs(&font, &ellipse_glyphs, L"...", 3, TEST_FONT_SIZE, NULL), RET_OK);
  ellipses_w = glyphs_measure(&ellipse_glyphs, 0, glyphs_get_length(&ellipse_glyphs));
  char_w = glyphs_measure(&glyphs, start, 1);
  /*宽度只够放 1 个字模加省略号（2c < 2.5c <= 3c），与 device_pixel_ratio 的具体取值无关*/
  r.w = (wh_t)(char_w * 2.5f);

  /*按同样的规则预算出能放下的字模数，避免在测试里写死平台相关的数值*/
  for (i = start; i < start + len; i++) {
    float_t w = glyphs_measure(&glyphs, i, 1);
    if ((text_w + w + ellipses_w) >= r.w) {
      break;
    }
    text_w += w;
  }
  drawn = i - start;
  ASSERT_EQ(drawn, 1);

  lcd_log_reset(lcd);
  ASSERT_EQ(canvas_draw_text_bidi_in_rect_by_glyphs(&c, &glyphs, start, len, &r, TRUE), RET_OK);
  /*文本部分 drawn 个，加上省略号的全部字模*/
  ASSERT_EQ(count_commands(lcd_log_get_commands(lcd), "dg("),
            (int)(drawn + glyphs_get_length(&ellipse_glyphs)));

  glyphs_deinit(&glyphs);
  glyphs_deinit(&ellipse_glyphs);
  canvas_reset(&c);
  font_manager_deinit(&fm);
  lcd_destroy(lcd);
}
