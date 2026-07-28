#include <algorithm>
#include <vector>

#include "base/font_manager.h"
#include "base/system_info.h"
#include "base/vgcanvas.h"
#include "gtest/gtest.h"
#include "tkc/utils.h"

#if defined(WITH_VGCANVAS) && defined(WITH_NANOVG_AGGE)

typedef struct _test_font_t {
  font_t base;
  font_size_t size;
  bool_t is_fallback;
} test_font_t;

static const uint8_t s_alpha_glyph_data[16] = {
    0x00, 0x60, 0x60, 0x00, 0x60, 0xFF, 0xFF, 0x60,
    0x60, 0xFF, 0xFF, 0x60, 0x00, 0x60, 0x60, 0x00};
static const uint8_t s_mono_glyph_data[4] = {0xF0, 0x90, 0x90, 0xF0};
static const uint8_t s_alpha2_glyph_data[4] = {0x1B, 0x6C, 0xC6, 0xE4};
static const uint8_t s_alpha4_glyph_data[8] = {0x0F, 0xF0, 0x4F, 0xF4, 0x8F, 0xF8, 0xFF, 0xFF};

static ret_t test_font_get_glyph(font_t* f, wchar_t chr, uint16_t font_size, glyph_t* g) {
  test_font_t* tf = (test_font_t*)f;
  (void)font_size;
  return_value_if_fail(g != NULL, RET_BAD_PARAMS);
  memset(g, 0x00, sizeof(glyph_t));

  g->x = 0;
  g->y = 0;
  g->w = 4;
  g->h = 4;
  g->advance = 5;

  if (tf->is_fallback) {
    if (chr != L'B') {
      return RET_FAIL;
    }
    g->format = GLYPH_FMT_MONO;
    g->pitch = 1;
    g->data = s_mono_glyph_data;
    return RET_OK;
  }

  if (chr == L'A') {
    g->format = GLYPH_FMT_ALPHA;
    g->pitch = 4;
    g->data = s_alpha_glyph_data;
    return RET_OK;
  } else if (chr == L'C') {
    g->format = GLYPH_FMT_ALPHA2;
    g->pitch = 1;
    g->data = s_alpha2_glyph_data;
    return RET_OK;
  } else if (chr == L'D') {
    g->format = GLYPH_FMT_ALPHA4;
    g->pitch = 2;
    g->data = s_alpha4_glyph_data;
    return RET_OK;
  }

  return RET_FAIL;
}

static bool_t test_font_match(font_t* f, const char* name, uint16_t font_size) {
  test_font_t* tf = (test_font_t*)f;
  return tk_str_eq(f->name, name) && tf->size == font_size;
}

static font_vmetrics_t test_font_get_vmetrics(font_t* f, uint16_t font_size) {
  font_vmetrics_t metrics = {0};
  (void)f;
  metrics.ascent = (int16_t)(font_size * 3 / 4);
  metrics.descent = (int16_t)(-font_size / 4);
  metrics.line_gap = 2;
  metrics.units_per_em = font_size;

  return metrics;
}

static ret_t test_font_destroy(font_t* f) {
  (void)f;
  return RET_OK;
}

static font_t* test_font_init(test_font_t* font, const char* name, font_size_t size, bool_t fallback) {
  memset(font, 0x00, sizeof(test_font_t));
  tk_strncpy(font->base.name, name, TK_NAME_LEN);
  font->size = size;
  font->is_fallback = fallback;
  font->base.match = test_font_match;
  font->base.get_glyph = test_font_get_glyph;
  font->base.get_vmetrics = test_font_get_vmetrics;
  font->base.destroy = test_font_destroy;

  return &font->base;
}

class VgcanvasTextSoftTest : public testing::Test {
 protected:
  void SetUp() override {
    old_fm_ = font_manager();
    font_manager_init(&fm_, NULL);
    font_manager_set(&fm_);

    font_size_t size = 16;
    const char* default_font = system_info()->default_font;
    font_manager_add_font(&fm_, test_font_init(&main_font_, "main", size, FALSE));
    font_manager_add_font(&fm_, test_font_init(&fallback_font_, "fallback", size, TRUE));
    font_manager_add_font(&fm_, test_font_init(&default_font_, default_font, size, TRUE));
  }

  void TearDown() override {
    font_manager_set(old_fm_);
    font_manager_deinit(&fm_);
  }

  font_manager_t fm_;
  font_manager_t* old_fm_;
  test_font_t main_font_;
  test_font_t fallback_font_;
  test_font_t default_font_;
};

TEST_F(VgcanvasTextSoftTest, measure_and_metrics) {
  std::vector<uint8_t> buffer(64 * 32 * 4, 0);
  vgcanvas_t* vg = vgcanvas_create(64, 32, 64 * 4, BITMAP_FMT_BGRA8888, buffer.data());
  ASSERT_NE(vg, nullptr);

  ASSERT_EQ(vgcanvas_set_font_size(vg, 16), RET_OK);
  ASSERT_EQ(vgcanvas_set_font(vg, "main"), RET_OK);
  ASSERT_GT(vgcanvas_measure_text(vg, "AB"), vgcanvas_measure_text(vg, "A"));

  {
    float_t ascent = 0;
    float_t descent = 0;
    float_t line_hight = 0;
    ASSERT_EQ(vgcanvas_get_text_metrics(vg, &ascent, &descent, &line_hight), RET_OK);
    ASSERT_GT(ascent, 0);
    ASSERT_GT(line_hight, 0);
  }

  vgcanvas_destroy(vg);
}

TEST_F(VgcanvasTextSoftTest, fill_text_supports_formats_fallback_transform_and_clip) {
  std::vector<uint8_t> buffer(96 * 64 * 4, 0);
  vgcanvas_t* vg = vgcanvas_create(96, 64, 96 * 4, BITMAP_FMT_BGRA8888, buffer.data());
  ASSERT_NE(vg, nullptr);
  ASSERT_EQ(vgcanvas_set_font_size(vg, 16), RET_OK);
  ASSERT_EQ(vgcanvas_set_font(vg, "main"), RET_OK);
  ASSERT_EQ(vgcanvas_set_fill_color(vg, color_init(0xFF, 0xFF, 0xFF, 0xFF)), RET_OK);
  ASSERT_EQ(vgcanvas_set_text_align(vg, "left"), RET_OK);
  ASSERT_EQ(vgcanvas_set_text_baseline(vg, "top"), RET_OK);
  ASSERT_GT(vgcanvas_measure_text(vg, "ABCD"), 0);

  ASSERT_EQ(vgcanvas_begin_frame(vg, NULL), RET_OK);
  ASSERT_EQ(vgcanvas_fill_text(vg, "ABCD", 8, 8, 100), RET_OK);
  ASSERT_EQ(vgcanvas_end_frame(vg), RET_OK);

  std::fill(buffer.begin(), buffer.end(), 0);
  ASSERT_EQ(vgcanvas_begin_frame(vg, NULL), RET_OK);
  ASSERT_EQ(vgcanvas_save(vg), RET_OK);
  ASSERT_EQ(vgcanvas_translate(vg, 20, 20), RET_OK);
  ASSERT_EQ(vgcanvas_rotate(vg, TK_D2R(25)), RET_OK);
  ASSERT_EQ(vgcanvas_fill_text(vg, "ABCD", 0, 0, 100), RET_OK);
  ASSERT_EQ(vgcanvas_restore(vg), RET_OK);
  ASSERT_EQ(vgcanvas_end_frame(vg), RET_OK);

  std::fill(buffer.begin(), buffer.end(), 0);
  ASSERT_EQ(vgcanvas_begin_frame(vg, NULL), RET_OK);
  ASSERT_EQ(vgcanvas_clip_rect(vg, 0, 0, 20, 20), RET_OK);
  ASSERT_EQ(vgcanvas_fill_text(vg, "ABCD", 8, 8, 100), RET_OK);
  ASSERT_EQ(vgcanvas_end_frame(vg), RET_OK);

  vgcanvas_destroy(vg);
}

#endif /*defined(WITH_VGCANVAS) && defined(WITH_NANOVG_AGGE)*/
