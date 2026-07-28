#ifndef TK_VGCANVAS_NANOVG_SOFT_TEXT_INC
#define TK_VGCANVAS_NANOVG_SOFT_TEXT_INC

typedef enum _vgcanvas_soft_text_align_h_t {
  VGCANVAS_SOFT_TEXT_ALIGN_LEFT = 0,
  VGCANVAS_SOFT_TEXT_ALIGN_CENTER = 1,
  VGCANVAS_SOFT_TEXT_ALIGN_RIGHT = 2
} vgcanvas_soft_text_align_h_t;

typedef enum _vgcanvas_soft_text_align_v_t {
  VGCANVAS_SOFT_TEXT_BASELINE_TOP = 0,
  VGCANVAS_SOFT_TEXT_BASELINE_MIDDLE = 1,
  VGCANVAS_SOFT_TEXT_BASELINE_BOTTOM = 2
} vgcanvas_soft_text_align_v_t;

typedef struct _vgcanvas_soft_text_run_t {
  glyph_t glyph;
  int32_t pen_x;
} vgcanvas_soft_text_run_t;

typedef struct _vgcanvas_nanovg_soft_text_cache_item_t {
  int32_t id;
  uint8_t* data;
  struct _vgcanvas_nanovg_soft_text_cache_item_t* next;
} vgcanvas_nanovg_soft_text_cache_item_t;

static font_size_t vgcanvas_nanovg_soft_text_to_font_size(float_t size) {
  int32_t font_size = tk_roundi(size);
  if (font_size <= 0) {
    font_size = TK_DEFAULT_FONT_SIZE;
  }
  return (font_size_t)font_size;
}

static ret_t vgcanvas_nanovg_soft_text_release_cache(vgcanvas_t* vgcanvas, bool_t release_buffer) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  vgcanvas_nanovg_soft_text_cache_item_t* iter = canvas->text_cache_items;

  while (iter != NULL) {
    vgcanvas_nanovg_soft_text_cache_item_t* next = iter->next;
    if (iter->id > 0) {
      nvgDeleteImage(canvas->vg, iter->id);
    }
    if (release_buffer && iter->data != NULL) {
      TKMEM_FREE(iter->data);
    }
    TKMEM_FREE(iter);
    iter = next;
  }
  canvas->text_cache_items = NULL;

  return RET_OK;
}

static ret_t vgcanvas_nanovg_soft_text_ensure_font(vgcanvas_nanovg_t* canvas, const char* name_hint) {
  font_manager_t* fm = font_manager();
  return_value_if_fail(fm != NULL, RET_BAD_PARAMS);

  if (canvas->awtk_font_size == 0) {
    canvas->awtk_font_size = vgcanvas_nanovg_soft_text_to_font_size(canvas->base.font_size);
  }

  if (canvas->awtk_font == NULL) {
    canvas->awtk_font = font_manager_get_font(fm, name_hint, canvas->awtk_font_size);
    canvas->awtk_fallback_font = font_manager_get_font(fm, NULL, canvas->awtk_font_size);
  }

  return_value_if_fail(canvas->awtk_font != NULL || canvas->awtk_fallback_font != NULL, RET_FAIL);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_soft_text_get_glyph(vgcanvas_nanovg_t* canvas, wchar_t chr, glyph_t* glyph) {
  ret_t ret = RET_FAIL;
  font_t* font = canvas->awtk_font;

  if (font != NULL) {
    ret = font_get_glyph(font, chr, canvas->awtk_font_size, glyph);
    if (ret == RET_OK && glyph->data != NULL) {
      return RET_OK;
    }
  }

  if (canvas->awtk_fallback_font != NULL && canvas->awtk_fallback_font != font) {
    ret = font_get_glyph(canvas->awtk_fallback_font, chr, canvas->awtk_font_size, glyph);
    if (ret == RET_OK && glyph->data != NULL) {
      return RET_OK;
    }
  }

  return RET_FAIL;
}

static uint32_t vgcanvas_nanovg_soft_text_get_row_pitch(const glyph_t* glyph) {
  if (glyph->pitch > 0) {
    return glyph->pitch;
  }

  switch (glyph->format) {
    case GLYPH_FMT_ALPHA:
      return glyph->w;
    case GLYPH_FMT_MONO:
      return (glyph->w + 7) >> 3;
    case GLYPH_FMT_ALPHA2:
      return (glyph->w + 3) >> 2;
    case GLYPH_FMT_ALPHA4:
      return (glyph->w + 1) >> 1;
    case GLYPH_FMT_RGBA:
      return glyph->w * 4;
    default:
      return glyph->w;
  }
}

static uint8_t vgcanvas_nanovg_soft_text_alpha_from_glyph(const glyph_t* glyph, int32_t x, int32_t y) {
  uint32_t row_pitch = vgcanvas_nanovg_soft_text_get_row_pitch(glyph);
  const uint8_t* row = glyph->data + y * row_pitch;

  switch (glyph->format) {
    case GLYPH_FMT_ALPHA:
      return row[x];
    case GLYPH_FMT_MONO: {
      uint8_t v = row[x >> 3];
      uint8_t shift = 7 - (x & 0x7);
      return ((v >> shift) & 0x1) ? 0xFF : 0;
    }
    case GLYPH_FMT_ALPHA2: {
      uint8_t v = row[x >> 2];
      uint8_t shift = (uint8_t)((3 - (x & 0x3)) * 2);
      return (uint8_t)(((v >> shift) & 0x3) * 85);
    }
    case GLYPH_FMT_ALPHA4: {
      uint8_t v = row[x >> 1];
      uint8_t shift = (uint8_t)((1 - (x & 0x1)) * 4);
      return (uint8_t)(((v >> shift) & 0xF) * 17);
    }
    case GLYPH_FMT_RGBA:
      return row[x * 4 + 3];
    default:
      return 0;
  }
}

static ret_t vgcanvas_nanovg_soft_text_render_runs(vgcanvas_nanovg_t* canvas,
                                                    const vgcanvas_soft_text_run_t* runs,
                                                    uint32_t nr, int32_t min_x, int32_t min_y,
                                                    int32_t max_x, int32_t max_y, int32_t* image_id) {
  vgcanvas_nanovg_soft_text_cache_item_t* item = NULL;
  uint32_t i = 0;
  uint32_t row = 0;
  int32_t width = max_x - min_x;
  int32_t height = max_y - min_y;
  uint32_t stride = (uint32_t)width;
  uint32_t bytes = stride * (uint32_t)height;
  uint8_t* text_image_data = NULL;
  int32_t text_image_id = 0;
  return_value_if_fail(width > 0 && height > 0, RET_OK);
  return_value_if_fail(image_id != NULL, RET_BAD_PARAMS);

  text_image_data = (uint8_t*)TKMEM_ALLOC(bytes);
  return_value_if_fail(text_image_data != NULL, RET_OOM);
  memset(text_image_data, 0x00, bytes);

  for (i = 0; i < nr; i++) {
    const glyph_t* glyph = &(runs[i].glyph);
    int32_t glyph_x = runs[i].pen_x + glyph->x - min_x;
    int32_t glyph_y = glyph->y - min_y;
    int32_t gx = 0;
    int32_t gy = 0;

    if (glyph->data == NULL || glyph->w == 0 || glyph->h == 0) {
      continue;
    }

    for (gy = 0; gy < glyph->h; gy++) {
      int32_t dst_y = glyph_y + gy;
      if (dst_y < 0 || dst_y >= height) {
        continue;
      }
      for (gx = 0; gx < glyph->w; gx++) {
        int32_t dst_x = glyph_x + gx;
        uint8_t value = 0;
        if (dst_x < 0 || dst_x >= width) {
          continue;
        }
        value = vgcanvas_nanovg_soft_text_alpha_from_glyph(glyph, gx, gy);
        if (value > 0) {
          uint8_t* pixel = text_image_data + dst_y * stride + dst_x;
          if (value > *pixel) {
            *pixel = value;
          }
        }
      }
    }
  }

  text_image_id =
      nvgCreateImageRaw(canvas->vg, width, height, NVG_TEXTURE_ALPHA, stride, NVG_IMAGE_NEAREST,
                        NVG_ORIENTATION_0, text_image_data);
  return_value_if_fail(text_image_id > 0, RET_FAIL);

  for (row = 0; row < (uint32_t)height; row++) {
    uint32_t col = 0;
    const uint8_t* line = text_image_data + row * stride;
    for (col = 0; col < (uint32_t)width; col++) {
      if (line[col] != 0) {
        item = TKMEM_ZALLOC(vgcanvas_nanovg_soft_text_cache_item_t);
        return_value_if_fail(item != NULL, RET_OOM);
        item->id = text_image_id;
        item->data = text_image_data;
        item->next = canvas->text_cache_items;
        canvas->text_cache_items = item;
        *image_id = text_image_id;
        return RET_OK;
      }
    }
  }

  nvgDeleteImage(canvas->vg, text_image_id);
  TKMEM_FREE(text_image_data);

  return RET_STOP;
}

static ret_t vgcanvas_nanovg_soft_text_set_font_size(vgcanvas_t* vgcanvas, float_t size) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  canvas->awtk_font_size = vgcanvas_nanovg_soft_text_to_font_size(size);

  if (canvas->awtk_font != NULL || canvas->awtk_fallback_font != NULL) {
    font_manager_t* fm = font_manager();
    const char* name = vgcanvas->font;
    if (fm != NULL) {
      canvas->awtk_font = font_manager_get_font(fm, name, canvas->awtk_font_size);
      canvas->awtk_fallback_font = font_manager_get_font(fm, NULL, canvas->awtk_font_size);
    }
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_soft_text_set_font(vgcanvas_t* vgcanvas, const char* name) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  font_manager_t* fm = font_manager();
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(fm != NULL, RET_BAD_PARAMS);

  if (canvas->awtk_font_size == 0) {
    canvas->awtk_font_size = vgcanvas_nanovg_soft_text_to_font_size(vgcanvas->font_size);
  }

  canvas->awtk_font = font_manager_get_font(fm, name, canvas->awtk_font_size);
  canvas->awtk_fallback_font = font_manager_get_font(fm, NULL, canvas->awtk_font_size);

  return_value_if_fail(canvas->awtk_font != NULL || canvas->awtk_fallback_font != NULL, RET_FAIL);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_soft_text_set_text_align(vgcanvas_t* vgcanvas, const char* text_align) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;

  if (text_align[0] == 'r') {
    canvas->text_align_h = VGCANVAS_SOFT_TEXT_ALIGN_RIGHT;
  } else if (text_align[0] == 'c') {
    canvas->text_align_h = VGCANVAS_SOFT_TEXT_ALIGN_CENTER;
  } else {
    canvas->text_align_h = VGCANVAS_SOFT_TEXT_ALIGN_LEFT;
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_soft_text_set_text_baseline(vgcanvas_t* vgcanvas,
                                                          const char* text_baseline) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;

  if (text_baseline[0] == 'b') {
    canvas->text_align_v = VGCANVAS_SOFT_TEXT_BASELINE_BOTTOM;
  } else if (text_baseline[0] == 'm') {
    canvas->text_align_v = VGCANVAS_SOFT_TEXT_BASELINE_MIDDLE;
  } else {
    canvas->text_align_v = VGCANVAS_SOFT_TEXT_BASELINE_TOP;
  }

  return RET_OK;
}

static float_t vgcanvas_nanovg_soft_text_measure_text(vgcanvas_t* vgcanvas, const char* text) {
  uint32_t i = 0;
  int32_t width = 0;
  uint32_t text_len = 0;
  wchar_t* ws = NULL;
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  ret_t ret = vgcanvas_nanovg_soft_text_ensure_font(canvas, vgcanvas->font);
  return_value_if_fail(ret == RET_OK && text != NULL, 0);

  text_len = (uint32_t)strlen(text);
  ws = (wchar_t*)TKMEM_ZALLOCN(wchar_t, text_len + 1);
  return_value_if_fail(ws != NULL, 0);

  tk_utf8_to_utf16(text, ws, text_len + 1);
  for (i = 0; ws[i] != 0; i++) {
    glyph_t glyph;
    memset(&glyph, 0x00, sizeof(glyph));
    if (vgcanvas_nanovg_soft_text_get_glyph(canvas, ws[i], &glyph) == RET_OK) {
      width += glyph.advance;
    }
  }
  TKMEM_FREE(ws);

  return (float_t)width;
}

static ret_t vgcanvas_nanovg_soft_text_get_text_metrics(vgcanvas_t* vgcanvas, float_t* ascent,
                                                         float_t* descent, float_t* line_hight) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  font_vmetrics_t metrics;
  ret_t ret = vgcanvas_nanovg_soft_text_ensure_font(canvas, vgcanvas->font);
  return_value_if_fail(ret == RET_OK, RET_FAIL);

  metrics = font_get_vmetrics(canvas->awtk_font != NULL ? canvas->awtk_font : canvas->awtk_fallback_font,
                              canvas->awtk_font_size);
  if (ascent != NULL) {
    *ascent = (float_t)metrics.ascent;
  }
  if (descent != NULL) {
    *descent = (float_t)metrics.descent;
  }
  if (line_hight != NULL) {
    *line_hight = (float_t)(metrics.ascent - metrics.descent + metrics.line_gap);
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_soft_text_fill_text(vgcanvas_t* vgcanvas, const char* text, float_t x,
                                                  float_t y, float_t max_width) {
  int32_t pen_x = 0;
  int32_t min_x = 0;
  int32_t min_y = 0;
  int32_t max_x = 0;
  int32_t max_y = 0;
  int32_t base_x = 0;
  int32_t base_y = 0;
  uint32_t text_len = 0;
  uint32_t i = 0;
  uint32_t run_nr = 0;
  int32_t image_id = 0;
  wchar_t* ws = NULL;
  NVGpaint paint;
  vgcanvas_soft_text_run_t* runs = NULL;
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  ret_t ret = vgcanvas_nanovg_soft_text_ensure_font(canvas, vgcanvas->font);
  return_value_if_fail(ret == RET_OK && text != NULL, RET_BAD_PARAMS);
  (void)max_width;

  text_len = (uint32_t)strlen(text);
  ws = (wchar_t*)TKMEM_ZALLOCN(wchar_t, text_len + 1);
  runs = (vgcanvas_soft_text_run_t*)TKMEM_ZALLOCN(vgcanvas_soft_text_run_t, text_len + 1);
  return_value_if_fail(ws != NULL && runs != NULL, RET_OOM);

  tk_utf8_to_utf16(text, ws, text_len + 1);
  for (i = 0; ws[i] != 0; i++) {
    glyph_t glyph;
    memset(&glyph, 0x00, sizeof(glyph));
    if (vgcanvas_nanovg_soft_text_get_glyph(canvas, ws[i], &glyph) != RET_OK) {
      continue;
    }

    runs[run_nr].glyph = glyph;
    runs[run_nr].pen_x = pen_x;
    if (run_nr == 0) {
      min_x = pen_x + glyph.x;
      min_y = glyph.y;
      max_x = pen_x + glyph.x + glyph.w;
      max_y = glyph.y + glyph.h;
    } else {
      min_x = tk_min(min_x, pen_x + glyph.x);
      min_y = tk_min(min_y, (int32_t)glyph.y);
      max_x = tk_max(max_x, pen_x + glyph.x + glyph.w);
      max_y = tk_max(max_y, (int32_t)(glyph.y + glyph.h));
    }
    pen_x += glyph.advance;
    run_nr++;
  }

  if (run_nr == 0 || max_x <= min_x || max_y <= min_y) {
    TKMEM_FREE(ws);
    TKMEM_FREE(runs);
    return RET_OK;
  }

  base_x = tk_roundi(x);
  base_y = tk_roundi(y);
  if (canvas->text_align_h == VGCANVAS_SOFT_TEXT_ALIGN_CENTER) {
    base_x -= (min_x + max_x) / 2;
  } else if (canvas->text_align_h == VGCANVAS_SOFT_TEXT_ALIGN_RIGHT) {
    base_x -= max_x;
  } else {
    base_x -= min_x;
  }

  if (canvas->text_align_v == VGCANVAS_SOFT_TEXT_BASELINE_MIDDLE) {
    base_y -= (min_y + max_y) / 2;
  } else if (canvas->text_align_v == VGCANVAS_SOFT_TEXT_BASELINE_BOTTOM) {
    base_y -= max_y;
  } else {
    base_y -= min_y;
  }

  ret = vgcanvas_nanovg_soft_text_render_runs(canvas, runs, run_nr, min_x, min_y, max_x, max_y,
                                               &image_id);
  if (ret == RET_OK) {
    int32_t draw_x = base_x + min_x;
    int32_t draw_y = base_y + min_y;
    NVGcontext* vg = canvas->vg;
    color_t c = vgcanvas->fill_color;
    float_t alpha = vgcanvas->global_alpha;
    if (alpha <= 0 || alpha > 1) {
      alpha = 1;
    }

    nvgSave(vg);
    nvgGlobalAlpha(vg, alpha);
    paint = nvgImagePattern(vg, draw_x, draw_y, max_x - min_x, max_y - min_y, 0, image_id, 1.0f);
    paint.innerColor = paint.outerColor = nvgRGBAf(c.rgba.r / 255.0f, c.rgba.g / 255.0f,
                                                   c.rgba.b / 255.0f, c.rgba.a / 255.0f);
    nvgBeginPath(vg);
    nvgRect(vg, draw_x, draw_y, max_x - min_x, max_y - min_y);
    nvgClosePath(vg);
    nvgFillPaint(vg, paint);
    nvgFill(vg);
    nvgRestore(vg);
  }

  TKMEM_FREE(ws);
  TKMEM_FREE(runs);

  return ret == RET_STOP ? RET_OK : ret;
}

static ret_t vgcanvas_nanovg_soft_text_on_frame_end(vgcanvas_t* vgcanvas) {
  return vgcanvas_nanovg_soft_text_release_cache(vgcanvas, TRUE);
}

static ret_t vgcanvas_nanovg_soft_text_on_clear_cache(vgcanvas_t* vgcanvas) {
  return vgcanvas_nanovg_soft_text_release_cache(vgcanvas, TRUE);
}

static ret_t vgcanvas_nanovg_soft_text_deinit(vgcanvas_t* vgcanvas) {
  return vgcanvas_nanovg_soft_text_release_cache(vgcanvas, TRUE);
}

#endif /*TK_VGCANVAS_NANOVG_SOFT_TEXT_INC*/
