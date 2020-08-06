#include "nanovg.h"
#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "base/vgcanvas.h"
#include "base/system_info.h"
#include "base/image_manager.h"
#include "base/font_manager.h"
#include "base/assets_manager.h"

static ret_t vgcanvas_nanovg_set_antialias(vgcanvas_t* vgcanvas, bool_t value);

static ret_t vgcanvas_nanovg_reset(vgcanvas_t* vgcanvas) {
  /*TODO: unload fonts*/
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  nvgDeleteFontByName(vg, NULL);
  return RET_OK;
}

static ret_t vgcanvas_nanovg_flush(vgcanvas_t* vgcanvas) {
#ifdef WITH_NANOVG_GPU
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  nvgEndFrame(vg);
#endif
  return RET_OK;
}

static ret_t vgcanvas_nanovg_clear_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                        float_t h, color_t c) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  color_t fill_color = vgcanvas->fill_color;
  vgcanvas_set_fill_color(vgcanvas, c);
  nvgBeginPath(vg);
  nvgRect(vg, x, y, w, h);
  nvgClosePath(vg);
  nvgFill(vg);
  vgcanvas_set_fill_color(vgcanvas, fill_color);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_begin_path(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgBeginPath(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_move_to(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgMoveTo(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_line_to(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgLineTo(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_quad_to(vgcanvas_t* vgcanvas, float_t cpx, float_t cpy, float_t x,
                                     float_t y) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgQuadTo(vg, cpx, cpy, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_bezier_to(vgcanvas_t* vgcanvas, float_t cp1x, float_t cp1y,
                                       float_t cp2x, float_t cp2y, float_t x, float_t y) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgBezierTo(vg, cp1x, cp1y, cp2x, cp2y, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_arc_to(vgcanvas_t* vgcanvas, float_t x1, float_t y1, float_t x2,
                                    float_t y2, float_t r) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgArcTo(vg, x1, y1, x2, y2, r);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_arc(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t r,
                                 float_t start, float_t end, bool_t ccw) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgArc(vg, x, y, r, start, end, ccw ? NVG_CCW : NVG_CW);

  return RET_OK;
}

static bool_t vgcanvas_nanovg_is_point_in_path(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  (void)vg;
  (void)x;
  (void)y;

  /*TODO*/

  return FALSE;
}

static ret_t vgcanvas_nanovg_rotate(vgcanvas_t* vgcanvas, float_t rad) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgRotate(vg, rad);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_scale(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgScale(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_translate(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgTranslate(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_transform(vgcanvas_t* vgcanvas, float_t a, float_t b, float_t c,
                                       float_t d, float_t e, float_t f) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgTransform(vg, a, b, c, d, e, f);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_transform(vgcanvas_t* vgcanvas, float_t a, float_t b, float_t c,
                                           float_t d, float_t e, float_t f) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgResetTransform(vg);
  nvgTransform(vg, a, b, c, d, e, f);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_rounded_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                          float_t h, float_t r) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgRoundedRect(vg, x, y, w, h, r);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_ellipse(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t rx,
                                     float_t ry) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgEllipse(vg, x, y, rx, ry);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_close_path(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgClosePath(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_path_winding(vgcanvas_t* vgcanvas, bool_t dir) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgPathWinding(vg, dir);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_intersect_clip_rect(vgcanvas_t* vgcanvas, float_t* x, float_t* y,
                                                 float_t* w, float_t* h) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgIntersectScissor_ex(vg, x, y, w, h);

  return RET_OK;
}

#if WITH_NANOVG_GPU
#define CMP_FLOAT_QE(a, b, CMP_ACCURACY) \
  (-CMP_ACCURACY <= ((a) - (b)) && ((a) - (b)) <= CMP_ACCURACY)

static bool_t vgcanvas_nanovg_is_system_orientation_0_or_180(float_t xform[6]) {
  float_t sin_180 = sinf(TK_D2R(180));
  system_info_t* info = system_info();

  if (info->lcd_orientation == LCD_ORIENTATION_0 && xform[0] == 1.0f && xform[1] == 0.0f &&
      xform[2] == 0.0f && xform[3] == 1.0f) {
    return TRUE;
  } else if (info->lcd_orientation == LCD_ORIENTATION_180 && xform[0] == -1.0f &&
             CMP_FLOAT_QE(xform[1], sin_180, 0.0001f) && CMP_FLOAT_QE(xform[2], sin_180, 0.0001f) &&
             xform[3] == -1.0f) {
    return TRUE;
  }
  return FALSE;
}

static bool_t vgcanvas_nanovg_is_system_orientation_90_or_270(float_t xform[6]) {
  float_t cos_90 = cosf(TK_D2R(90));
  system_info_t* info = system_info();

  if (CMP_FLOAT_QE(xform[0], cos_90, 0.0001f) && CMP_FLOAT_QE(xform[3], cos_90, 0.0001f)) {
    if (info->lcd_orientation == LCD_ORIENTATION_90 && xform[1] == 1.0f && xform[2] == -1.0f) {
      return TRUE;
    } else if (info->lcd_orientation == LCD_ORIENTATION_270 && xform[1] == -1.0f &&
               xform[2] == 1.0f) {
      return TRUE;
    }
  }
  return FALSE;
}
#endif

static ret_t vgcanvas_nanovg_clip_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                       float_t h) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

#if WITH_NANOVG_GPU
  rect_t r1;
  rect_t cilp_rect;
  float_t xform[6];
  nvgGetStateXfrom(vg, xform);

  /* 判断如果 vg 没有旋转和缩放的坐标系，直接使用 rect_intersect 函数做交集提高效率 */
  if (vgcanvas_nanovg_is_system_orientation_0_or_180(xform) ||
      vgcanvas_nanovg_is_system_orientation_90_or_270(xform)) {
    r1 = rect_init(x, y, w, h);
    cilp_rect = rect_intersect((const rect_t*)&vgcanvas->dirty_rect, (const rect_t*)&r1);
    nvgScissor(vg, (float_t)cilp_rect.x, (float_t)cilp_rect.y, (float_t)cilp_rect.w,
               (float_t)cilp_rect.h);
  } else {
    nvgIntersectScissorForOtherRect(
        vg, x, y, w, h, (float_t)vgcanvas->dirty_rect.x, (float_t)vgcanvas->dirty_rect.y,
        (float_t)vgcanvas->dirty_rect.w, (float_t)vgcanvas->dirty_rect.h);
  }
#else
  nvgScissor(vg, x, y, w, h);
#endif

  return RET_OK;
}

static ret_t vgcanvas_nanovg_fill(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgFill(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_stroke(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgStroke(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_paint(vgcanvas_t* vgcanvas, bool_t stroke, bitmap_t* img) {
  int iw = img->w;
  int ih = img->h;
  NVGpaint imgPaint;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  int id = vgcanvas_nanovg_ensure_image(canvas, img);
  return_value_if_fail(id >= 0, RET_BAD_PARAMS);

  imgPaint = nvgImagePattern(vg, 0, 0, iw, ih, 0, id, 1);

  if (stroke) {
    nvgStrokePaint(vg, imgPaint);
    nvgStroke(vg);
  } else {
    nvgFillPaint(vg, imgPaint);
    nvgClosePath(vg);
    nvgFill(vg);
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_font_size(vgcanvas_t* vgcanvas, float_t size) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  nvgFontSize(vg, size);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_font(vgcanvas_t* vgcanvas, const char* name) {
  int font_id = 0;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  return_value_if_fail(name && *name, RET_BAD_PARAMS);

  font_id = nvgFindFont(vg, name);
  if (font_id < 0) {
    assets_manager_t* am = vgcanvas->assets_manager ? vgcanvas->assets_manager : assets_manager();
    const asset_info_t* r = assets_manager_ref(am, ASSET_TYPE_FONT, name);

    if (r == NULL || r->subtype != ASSET_TYPE_FONT_TTF) {
      font_id = nvgFindFont(vg, name);
      if (font_id >= 0) {
        canvas->font_id = font_id;
        return RET_OK;
      }

      if (r == NULL) {
        font_t* font = font_manager_get_font(font_manager(), NULL, TK_DEFAULT_FONT_SIZE);
        if (font != NULL) {
          r = assets_manager_ref(assets_manager(), ASSET_TYPE_FONT, font->name);
        }
      }
    }

    if (r != NULL && r->subtype == ASSET_TYPE_FONT_TTF) {
      font_id = nvgCreateFontMem(vg, name, (unsigned char*)r->data, r->size, 0);
    }

    if (r != NULL) {
      assets_manager_unref(am, r);
    }
  }

  return_value_if_fail(font_id >= 0, RET_FAIL);

  canvas->font_id = font_id;
  nvgFontFaceId(vg, font_id);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_text_align(vgcanvas_t* vgcanvas, const char* text_align) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;

  if (text_align[0] == 'r') {
    canvas->text_align_h = NVG_ALIGN_RIGHT;
  } else if (text_align[0] == 'c') {
    canvas->text_align_h = NVG_ALIGN_CENTER;
  } else {
    canvas->text_align_h = NVG_ALIGN_LEFT;
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_text_baseline(vgcanvas_t* vgcanvas, const char* text_baseline) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;

  if (text_baseline[0] == 'b') {
    canvas->text_align_v = NVG_ALIGN_BOTTOM;
  } else if (text_baseline[0] == 'm') {
    canvas->text_align_v = NVG_ALIGN_MIDDLE;
  } else {
    canvas->text_align_v = NVG_ALIGN_TOP;
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_fill_text(vgcanvas_t* vgcanvas, const char* text, float_t x, float_t y,
                                       float_t max_width) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgTextAlign(vg, canvas->text_align_v | canvas->text_align_h);
  nvgText(vg, x, y, text, text + strlen(text));

  return RET_OK;
}

static float_t vgcanvas_nanovg_measure_text(vgcanvas_t* vgcanvas, const char* text) {
  float bounds[4];
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgTextAlign(vg, canvas->text_align_v | canvas->text_align_h);

  return nvgTextBounds(vg, 0, 0, text, text + strlen(text), bounds);
}

#ifdef WITH_NANOVG_GPU
static ret_t vgcanvas_nanovg_get_text_metrics(vgcanvas_t* vgcanvas, float_t* ascent, 
                                      float_t* descent, float_t* line_hight) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  float a = 0;
  float  d = 0;
  float  h = 0;

  nvgTextMetrics(vg, &a, &d, &h);
  if(ascent != NULL) {
    *ascent = a;
  }

  if(descent != NULL) {
    *descent = d;
  }

  if(line_hight != NULL) {
    *line_hight = h;
  }

  return RET_OK;
}
#else
#define vgcanvas_nanovg_get_text_metrics NULL
#endif/*WITH_NANOVG_GPU*/

static ret_t vgcanvas_nanovg_draw_image(vgcanvas_t* vgcanvas, bitmap_t* img, float_t sx, float_t sy,
                                        float_t sw, float_t sh, float_t dx, float_t dy, float_t dw,
                                        float_t dh) {
  int iw = img->w;
  int ih = img->h;
  NVGpaint imgPaint;
  float scaleX = (float)dw / sw;
  float scaleY = (float)dh / sh;
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  int id = vgcanvas_nanovg_ensure_image(canvas, img);
  return_value_if_fail(id >= 0, RET_BAD_PARAMS);

  imgPaint = nvgImagePattern(vg, 0, 0, iw, ih, 0, id, 1);

  nvgSave(vg);
  nvgBeginPath(vg);
  nvgTranslate(vg, dx - (sx * scaleX), dy - (sy * scaleY));
  nvgScale(vg, scaleX, scaleY);
  nvgRect(vg, sx, sy, sw, sh);
  nvgFillPaint(vg, imgPaint);
  nvgClosePath(vg);
  nvgFill(vg);
  nvgBeginPath(vg);
  nvgRestore(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_antialias(vgcanvas_t* vgcanvas, bool_t value) {
#ifdef WITH_NANOVG_GPU
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;
  nvgShapeAntiAlias(vg, value);
#else
  (void)vgcanvas;
  (void)value;
  /*always*/
#endif

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_line_width(vgcanvas_t* vgcanvas, float_t value) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgStrokeWidth(vg, value);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_global_alpha(vgcanvas_t* vgcanvas, float_t value) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  vgcanvas->global_alpha = value;
  nvgGlobalAlpha(vg, value);

  return RET_OK;
}

static NVGcolor toNVGcolor(color_t c) {
  return nvgRGBA(c.rgba.r, c.rgba.g, c.rgba.b, c.rgba.a);
}

static ret_t vgcanvas_nanovg_set_fill_color(vgcanvas_t* vgcanvas, color_t c) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgFillColor(vg, toNVGcolor(c));

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_fill_linear_gradient(vgcanvas_t* vgcanvas, float_t sx, float_t sy,
                                                      float_t ex, float_t ey, color_t icolor,
                                                      color_t ocolor) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  NVGpaint paint = nvgLinearGradient(vg, sx, sy, ex, ey, toNVGcolor(icolor), toNVGcolor(ocolor));
  nvgFillPaint(vg, paint);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_fill_radial_gradient(vgcanvas_t* vgcanvas, float_t cx, float_t cy,
                                                      float_t inr, float_t outr, color_t icolor,
                                                      color_t ocolor) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  NVGpaint paint = nvgRadialGradient(vg, cx, cy, inr, outr, toNVGcolor(icolor), toNVGcolor(ocolor));
  nvgFillPaint(vg, paint);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_stroke_color(vgcanvas_t* vgcanvas, color_t c) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgStrokeColor(vg, nvgRGBA(c.rgba.r, c.rgba.g, c.rgba.b, c.rgba.a));

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_stroke_linear_gradient(vgcanvas_t* vgcanvas, float_t sx,
                                                        float_t sy, float_t ex, float_t ey,
                                                        color_t icolor, color_t ocolor) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  NVGpaint paint = nvgLinearGradient(vg, sx, sy, ex, ey, toNVGcolor(icolor), toNVGcolor(ocolor));
  nvgStrokePaint(vg, paint);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_stroke_radial_gradient(vgcanvas_t* vgcanvas, float_t cx,
                                                        float_t cy, float_t inr, float_t outr,
                                                        color_t icolor, color_t ocolor) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  NVGpaint paint = nvgRadialGradient(vg, cx, cy, inr, outr, toNVGcolor(icolor), toNVGcolor(ocolor));
  nvgStrokePaint(vg, paint);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_line_cap(vgcanvas_t* vgcanvas, const char* value) {
  int line_cap = NVG_BUTT;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  if (*value == 'r') {
    line_cap = NVG_ROUND;
  } else if (*value == 's') {
    line_cap = NVG_SQUARE;
  }

  nvgLineCap(vg, line_cap);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_line_join(vgcanvas_t* vgcanvas, const char* value) {
  int line_join = NVG_MITER;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  if (*value == 'r') {
    line_join = NVG_ROUND;
  } else if (*value == 'b') {
    line_join = NVG_BEVEL;
  }

  nvgLineJoin(vg, line_join);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_set_miter_limit(vgcanvas_t* vgcanvas, float_t value) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgMiterLimit(vg, value);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_save(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgSave(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_restore(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgRestore(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_clear_cache(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgClearCache(vg);
  nvgDeleteFontByName(vg, NULL);
  return RET_OK;
}

static const vgcanvas_vtable_t vt = {
    .reinit = vgcanvas_nanovg_reinit,
    .begin_frame = vgcanvas_nanovg_begin_frame,
    .reset = vgcanvas_nanovg_reset,
    .flush = vgcanvas_nanovg_flush,
    .clear_rect = vgcanvas_nanovg_clear_rect,
    .begin_path = vgcanvas_nanovg_begin_path,
    .move_to = vgcanvas_nanovg_move_to,
    .line_to = vgcanvas_nanovg_line_to,
    .arc = vgcanvas_nanovg_arc,
    .arc_to = vgcanvas_nanovg_arc_to,
    .bezier_to = vgcanvas_nanovg_bezier_to,
    .quad_to = vgcanvas_nanovg_quad_to,
    .is_point_in_path = vgcanvas_nanovg_is_point_in_path,
    .ellipse = vgcanvas_nanovg_ellipse,
    .rounded_rect = vgcanvas_nanovg_rounded_rect,
    .close_path = vgcanvas_nanovg_close_path,
    .scale = vgcanvas_nanovg_scale,
    .rotate = vgcanvas_nanovg_rotate,
    .translate = vgcanvas_nanovg_translate,
    .transform = vgcanvas_nanovg_transform,
    .set_transform = vgcanvas_nanovg_set_transform,
    .clip_rect = vgcanvas_nanovg_clip_rect,
    .intersect_clip_rect = vgcanvas_nanovg_intersect_clip_rect,
    .path_winding = vgcanvas_nanovg_path_winding,
    .fill = vgcanvas_nanovg_fill,
    .stroke = vgcanvas_nanovg_stroke,
    .paint = vgcanvas_nanovg_paint,
    .set_font = vgcanvas_nanovg_set_font,
    .set_font_size = vgcanvas_nanovg_set_font_size,
    .set_text_align = vgcanvas_nanovg_set_text_align,
    .set_text_baseline = vgcanvas_nanovg_set_text_baseline,
    .fill_text = vgcanvas_nanovg_fill_text,
    .measure_text = vgcanvas_nanovg_measure_text,
    .draw_image = vgcanvas_nanovg_draw_image,
    .set_antialias = vgcanvas_nanovg_set_antialias,
    .set_global_alpha = vgcanvas_nanovg_set_global_alpha,
    .set_line_width = vgcanvas_nanovg_set_line_width,
    .set_fill_color = vgcanvas_nanovg_set_fill_color,
    .set_fill_linear_gradient = vgcanvas_nanovg_set_fill_linear_gradient,
    .set_fill_radial_gradient = vgcanvas_nanovg_set_fill_radial_gradient,
    .set_stroke_color = vgcanvas_nanovg_set_stroke_color,
    .set_stroke_linear_gradient = vgcanvas_nanovg_set_stroke_linear_gradient,
    .set_stroke_radial_gradient = vgcanvas_nanovg_set_stroke_radial_gradient,
    .set_line_join = vgcanvas_nanovg_set_line_join,
    .set_line_cap = vgcanvas_nanovg_set_line_cap,
    .set_miter_limit = vgcanvas_nanovg_set_miter_limit,
    .get_text_metrics = vgcanvas_nanovg_get_text_metrics,
    .save = vgcanvas_nanovg_save,
    .restore = vgcanvas_nanovg_restore,
    .end_frame = vgcanvas_nanovg_end_frame,
    .create_fbo = vgcanvas_nanovg_create_fbo,
    .bind_fbo = vgcanvas_nanovg_bind_fbo,
    .destroy_fbo = vgcanvas_nanovg_destroy_fbo,
    .unbind_fbo = vgcanvas_nanovg_unbind_fbo,
    .fbo_to_bitmap = vgcanvas_nanovg_fbo_to_bitmap,
    .clear_cache = vgcanvas_nanovg_clear_cache,
    .destroy = vgcanvas_nanovg_destroy};
