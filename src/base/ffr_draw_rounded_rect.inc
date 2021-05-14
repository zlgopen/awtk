/**
* History:
* ================================================================
* 2019-11-22 luozhiming@zlg.cn created
*
*/

#include <math.h>
#include "pixel.h"
#include "base/lcd.h"
#include "base/bitmap.h"
#include "../tkc/darray.h"
#include "../tkc/types_def.h"

#ifdef FRAGMENT_FRAME_BUFFER_SIZE
#include "../lcd/lcd_mem_fragment.h"
#else
#ifdef WITH_LCD_MONO
#include "../lcd/lcd_mono.h"
#else
#include "../lcd/lcd_mem.h"
#endif
#endif

/*
把二维坐标的象限通过45度分为上下区域，一共8个区域
*/
typedef enum _frr_quadrant_t {
  FRR_QUADRANT_FIRST_F = 0,  //二维坐标第一象限的上区域
  FRR_QUADRANT_FIRST_S,      //二维坐标第一象限的下区域

  FRR_QUADRANT_SECOND_F,  //二维坐标第二象限的上区域
  FRR_QUADRANT_SECOND_S,  //二维坐标第二象限的下区域

  FRR_QUADRANT_THIRD_F,  //二维坐标第三象限的上区域
  FRR_QUADRANT_THIRD_S,  //二维坐标第三象限的下区域

  FRR_QUADRANT_FOURTH_F,  //二维坐标第四象限的上区域
  FRR_QUADRANT_FOURTH_S,  //二维坐标第四象限的下区域
} frr_quadrant_t;

typedef enum _frr_vertex_type_t {
  FRR_VERTEXT_TYPE_TOP_REIGHT = 0,    //右上角
  FRR_VERTEXT_TYPE_TOP_LEFT,          //左上角
  FRR_VERTEXT_TYPE_BOTTOM_LEFT,       //左下角
  FRR_VERTEXT_TYPE_BOTTOM_REIGHT,     //右下角

  FRR_VERTEXT_TYPE_COUNT = FRR_VERTEXT_TYPE_BOTTOM_REIGHT + 1,
} frr_vertex_type_t;

typedef struct _frr_point_pix_t {
  int32_t x;
  int32_t y;
  float_t e;
} frr_point_pix_t;

typedef struct _frr_draw_info_t {
  int32_t x;
  int32_t y;
  int32_t v;
  int32_t h;
  uint32_t radius;
  float_t angle_v;
  float_t angle_h;
} frr_draw_info_t;

typedef struct _frr_image_info_t {
  uint32_t w;
  uint32_t h;
  uint32_t dx;
  uint32_t dy;
  uint32_t dw;
  uint32_t dh;
  uint32_t bpp;
  uint32_t stride;
  unsigned char* dst;
  bitmap_format_t format;
} frr_image_info_t;

typedef struct _frr_draw_arc_info_table_t {
  frr_quadrant_t quadrant_v;
  frr_quadrant_t quadrant_h;
} frr_draw_arc_info_table_t;

#ifdef M_PI_2
#define M_FRR_PI_2 (float_t)(M_PI_2)
#else
#define M_FRR_PI_2 (float_t)(M_PI / 2)
#endif /*M_FRR_PI_2*/

#ifdef M_PI_4
#define M_FRR_PI_4 (float_t)(M_PI_4)
#else
#define M_FRR_PI_4 (float_t)(M_PI / 4)
#endif /*M_FRR_PI_4*/

#ifndef FRR_POINT_PIX_CACHE_SIZE
#define FRR_POINT_PIX_CACHE_SIZE 256
#endif

#ifndef CMP_ACCURACY
#define CMP_ACCURACY 0.001f
#endif

#ifndef FFR_SUPPORT_MIN_RADIUS
#define FFR_SUPPORT_MIN_RADIUS 2
#endif

#define FFR_DRAW_SIN_45 0.70710678118654752440084436210485f

#define CMP_FLOAT_QE(a, b) (-CMP_ACCURACY <= ((a) - (b)) && ((a) - (b)) <= CMP_ACCURACY)

#define ANGLE2SIZE(angle, r) ((int)(sinf(angle) * (r)))

#define BLEND_DATA(d1, d2, a) ((((d1) * (a)) + ((d2) * (0xff - (a)))) >> 8)

#define FFR_LIMIT_UINT8(tmp, out) {                       \
  if((tmp) > 0xff) {                                      \
    (out) = 0xff;                                         \
  } else if((tmp) < 0) {                                  \
    (out) = 0;                                            \
  } else {                                                \
    (out) = (tmp);                                        \
  }                                                       \
}

#define IMAGE_COLOR2COLOR_BLEND(c1, c2, a) {                                                              \
  if (a < 0x8) {                                                                                          \
    c1.r = c2.r;                                                                                          \
    c1.g = c2.g;                                                                                          \
    c1.b = c2.b;                                                                                          \
    c1.a = c2.a;                                                                                          \
  } else if (a < 0xfa) {                                                                                  \
    if (c2.a > 0xf4) {                                                                                    \
      c1.r = BLEND_DATA(c1.r, c2.r, a);                                                                   \
      c1.g = BLEND_DATA(c1.g, c2.g, a);                                                                   \
      c1.b = BLEND_DATA(c1.b, c2.b, a);                                                                   \
      c1.a = 0xff;                                                                                        \
    } else {                                                                                              \
      uint8_t out_a = 0x0;                                                                                \
      FFR_LIMIT_UINT8(a + c2.a - ((a * c2.a) >> 8), out_a);                                               \
      if(out_a > 0) {                                                                                     \
        uint8_t d_a = (c2.a * (0xff - a)) >> 8;                                                           \
        c1.r = (c1.r * a + c2.r * d_a) / out_a;                                                           \
        c1.g = (c1.g * a + c2.g * d_a) / out_a;                                                           \
        c1.b = (c1.b * a + c2.b * d_a) / out_a;                                                           \
        c1.a = out_a;                                                                                     \
      } else {                                                                                            \
        c1.r = c1.g = c1.b = c1.a = 0x0;                                                                  \
      }                                                                                                   \
    }                                                                                                     \
  }                                                                                                       \
}

#define FFR_SET_STANDARD_RADIUS(is_h, w, h, radius) {                   \
  if ((is_h && *radius > h)) {                                          \
    *radius = h;                                                        \
  } else if ((!is_h && *radius > w))  {                                 \
    *radius = w;                                                        \
  }                                                                     \
  *radius = *radius <= FFR_SUPPORT_MIN_RADIUS ? 0 : *radius;            \
}


#ifndef WITH_NANOVG_GPU

static frr_point_pix_t frr_point_pix_cache_list[FRR_POINT_PIX_CACHE_SIZE];

const static frr_draw_arc_info_table_t WIDGET_DRAW_ARC_INFO_TABLE[FRR_VERTEXT_TYPE_COUNT] = {
    {FRR_QUADRANT_FIRST_S, FRR_QUADRANT_FIRST_F},     //右上角
    {FRR_QUADRANT_SECOND_S, FRR_QUADRANT_SECOND_F},   //左上角
    {FRR_QUADRANT_THIRD_F, FRR_QUADRANT_THIRD_S},     //左下角
    {FRR_QUADRANT_FOURTH_F, FRR_QUADRANT_FOURTH_S}};  //右下角

static inline int32_t frr_image_get_buff_point(frr_image_info_t* info, int32_t x,
                                                      int32_t y) {
#if defined(WITH_LCD_MONO)
  return 0;
#else
  int32_t p = -1;
  int32_t dx = 0;
  int32_t dy = 0;

  return_value_if_fail(info != NULL, -1);

  if (x < 0 || x >= (int32_t)info->w || y < 0 || y >= (int32_t)info->h || info->stride == 0 ||
      info->bpp == 0) {
    return p;
  }

  dx = x - info->dx;
  dy = y - info->dy;

  if (dx < 0 || dx >= (int32_t)info->dw || dy < 0 || dy >= (int32_t)info->dh) {
    return p;
  }

  p = dx * info->bpp + dy * info->stride;

  return p;
#endif
}

static inline frr_image_info_t* ffr_image_info_create(frr_image_info_t* info, canvas_t* c) {
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  if (info == NULL) {
    info = TKMEM_ZALLOC(frr_image_info_t);
    return_value_if_fail(info != NULL, NULL);
  }
  if (vg != NULL) {
    info->dx = 0;
    info->dy = 0;
    info->dw = info->w = vg->w;
    info->dh = info->h = vg->h;
    info->stride = vg->stride;
    info->format = vg->format;
    info->dst = (unsigned char*)vg->buff;
  } else {
#ifdef FRAGMENT_FRAME_BUFFER_SIZE
    info->w = c->lcd->w;
    info->h = c->lcd->h;
    info->dx = c->lcd->dirty_rect.x;
    info->dy = c->lcd->dirty_rect.y;
    info->dw = c->lcd->dirty_rect.w;
    info->dh = c->lcd->dirty_rect.h;
    info->dst = lcd_mem_fragment_get_buff(c->lcd);
    info->format = lcd_get_desired_bitmap_format(c->lcd);
    info->stride = info->dw * bitmap_get_bpp_of_format(info->format);
#elif defined(WITH_LCD_MONO)
    lcd_mono_t* mem = (lcd_mono_t*)c->lcd;
    info->dx = 0;
    info->dy = 0;
    info->dst = mem->data;
    info->dw = info->w = c->lcd->w;
    info->dh = info->h = c->lcd->h;
    info->format = BITMAP_FMT_MONO;
#else
    lcd_mem_t* mem = (lcd_mem_t*)c->lcd;
    info->dx = 0;
    info->dy = 0;
    info->dst = mem->offline_fb;
    info->dw = info->w = c->lcd->w;
    info->dh = info->h = c->lcd->h;
    info->stride = mem->line_length;
    info->format = lcd_get_desired_bitmap_format(c->lcd);
#endif
  }
  info->bpp = bitmap_get_bpp_of_format(info->format);
  return info;
}

static inline void ffr_image_data2color(unsigned char* d, int32_t x, int32_t y, int32_t w, int32_t h, bitmap_format_t format, rgba_t* rgb) {
  uint32_t c_16 = 0;
  switch (format) {
    case BITMAP_FMT_RGBA8888:
      rgb->r = *d++;
      rgb->g = *d++;
      rgb->b = *d++;
      rgb->a = *d++;
      break;
    case BITMAP_FMT_ABGR8888:
      rgb->a = *d++;
      rgb->b = *d++;
      rgb->g = *d++;
      rgb->r = *d++;
      break;
    case BITMAP_FMT_BGRA8888:
      rgb->b = *d++;
      rgb->g = *d++;
      rgb->r = *d++;
      rgb->a = *d++;
      break;
    case BITMAP_FMT_ARGB8888:
      rgb->a = *d++;
      rgb->r = *d++;
      rgb->g = *d++;
      rgb->b = *d++;
      break;
    case BITMAP_FMT_RGB565:
      c_16 = (d[1] << 8) | d[0];
      rgb->b = (c_16 >> 11) << 3;
      rgb->g = ((c_16 >> 5) & 0x3f) << 2;
      rgb->r = (c_16 & 0x1f) << 3;
      rgb->a = 0xff;
      break;
    case BITMAP_FMT_BGR565:
      c_16 = (d[1] << 8) | d[0];
      rgb->r = (c_16 >> 11) << 3;
      rgb->g = ((c_16 >> 5) & 0x3f) << 2;
      rgb->b = (c_16 & 0x1f) << 3;
      rgb->a = 0xff;
      break;
    case BITMAP_FMT_RGB888:
      rgb->r = *d++;
      rgb->g = *d++;
      rgb->b = *d++;
      rgb->a = 255;
      break;
    case BITMAP_FMT_BGR888:
      rgb->b = *d++;
      rgb->g = *d++;
      rgb->r = *d++;
      rgb->a = 255;
      break;
#ifdef WITH_LCD_MONO
    case BITMAP_FMT_MONO: {
      uint8_t p = bitmap_mono_get_pixel(d, w, h, x, y);
      color_t c = color_from_mono(p);
      rgb->b = c.rgba.b;
      rgb->g = c.rgba.g;
      rgb->r = c.rgba.r;
      rgb->a = c.rgba.a;
      break;
    }
#endif
    default:
      break;
  }
}

static inline void ffr_image_color2data(unsigned char* d, int32_t x, int32_t y, int32_t w, int32_t h, bitmap_format_t format, rgba_t rgb) {
  switch (format) {
    case BITMAP_FMT_RGBA8888:
      *d++ = rgb.r;
      *d++ = rgb.g;
      *d++ = rgb.b;
      *d++ = rgb.a;
      break;
    case BITMAP_FMT_ABGR8888:
      *d++ = rgb.a;
      *d++ = rgb.b;
      *d++ = rgb.g;
      *d++ = rgb.r;
      break;
    case BITMAP_FMT_BGRA8888:
      *d++ = rgb.b;
      *d++ = rgb.g;
      *d++ = rgb.r;
      *d++ = rgb.a;
      break;
    case BITMAP_FMT_ARGB8888:
      *d++ = rgb.a;
      *d++ = rgb.r;
      *d++ = rgb.g;
      *d++ = rgb.b;
      break;
    case BITMAP_FMT_BGR565:
      *(uint16_t*)d = ((rgb.r >> 3) << 11) | ((rgb.g >> 2) << 5) | (rgb.b >> 3);
      break;
    case BITMAP_FMT_RGB565:
      *(uint16_t*)d = ((rgb.b >> 3) << 11) | ((rgb.g >> 2) << 5) | (rgb.r >> 3);
      break;
    case BITMAP_FMT_RGB888:
      *d++ = rgb.r;
      *d++ = rgb.g;
      *d++ = rgb.b;
      break;
    case BITMAP_FMT_BGR888:
      *d++ = rgb.b;
      *d++ = rgb.g;
      *d++ = rgb.r;
      break;
#ifdef WITH_LCD_MONO
    case BITMAP_FMT_MONO: 
      bitmap_mono_set_pixel(d, w, h, x, y, rgb.r);
      break;
#endif
    default:
      break;
  }
}

static inline void ffr_draw_antialiasing_for_point(frr_image_info_t* image_info,
                                                      canvas_t* canvas, int px, int py, float_t e,
                                                      rgba_t n_color, rgba_t o_color) {
  int32_t p = 0;
  unsigned char a = (unsigned char)(e * n_color.a);

  if (canvas->clip_left <= px && px <= canvas->clip_right && canvas->clip_top <= py &&
      py <= canvas->clip_bottom) {
    IMAGE_COLOR2COLOR_BLEND(n_color, o_color, a);
    p = frr_image_get_buff_point(image_info, px, py);
    if (p >= 0) {
      ffr_image_color2data(image_info->dst + p, px, py, image_info->w, image_info->h, image_info->format, n_color);
    }
  }
}

static inline bool_t ffr_get_circluar_point_to_quadrant(int32_t xc, int32_t yc, int32_t px,
                                                           int32_t py, frr_quadrant_t type,
                                                           int32_t* out_x, int32_t* out_y) {
  int32_t tmp_x = 0;
  int32_t tmp_y = 0;
  switch (type) {
    case FRR_QUADRANT_FIRST_F:
      tmp_x = xc + px;
      tmp_y = yc - py;
      break;
    case FRR_QUADRANT_FIRST_S:
      tmp_x = xc + py;
      tmp_y = yc - px;
      break;
    case FRR_QUADRANT_SECOND_F:
      tmp_x = xc - px;
      tmp_y = yc - py;
      break;
    case FRR_QUADRANT_SECOND_S:
      tmp_x = xc - py;
      tmp_y = yc - px;
      break;
    case FRR_QUADRANT_THIRD_F:
      tmp_x = xc - py;
      tmp_y = yc + px;
      break;
    case FRR_QUADRANT_THIRD_S:
      tmp_x = xc - px;
      tmp_y = yc + py;
      break;
    case FRR_QUADRANT_FOURTH_F:
      tmp_x = xc + py;
      tmp_y = yc + px;
      break;
    case FRR_QUADRANT_FOURTH_S:
      tmp_x = xc + px;
      tmp_y = yc + py;
      break;
    default:
      return FALSE;
  }

  *out_x = tmp_x;
  *out_y = tmp_y;

  return TRUE;
}

static inline void ffr_draw_circluar_point_to_quadrant(frr_image_info_t* image_info, canvas_t* c,
                                                          int32_t x_h, int32_t y_v, int32_t xc,
                                                          int32_t yc, int32_t px, int32_t py,
                                                          float_t e, frr_quadrant_t type,
                                                          rgba_t color) {
  int32_t p = 0;
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = 0;
  int32_t h = 0;
  rgba_t o_color = {0};
  if (ffr_get_circluar_point_to_quadrant(xc, yc, px, py, type, &x, &y)) {
    if (x_h > x) {
      w = x_h - x;
      x_h = x;
    } else {
      w = x - x_h;
      x--;
    }

    if (y_v > y) {
      h = y_v - y;
      y_v = y;
    } else {
      h = y - y_v;
      y--;
    }

    x += c->ox;
    y += c->oy;

    if (w > 0 && h > 0) {
      p = frr_image_get_buff_point(image_info, x, y);
      if (p >= 0) {
        ffr_image_data2color(image_info->dst + p, x, y, image_info->w, image_info->h, image_info->format, &o_color);

        ffr_draw_antialiasing_for_point(image_info, c, x, y, e, color, o_color);
      }
    }
  }
}

static inline void ffr_draw_circluar_point_to_quadrant_hline(frr_image_info_t* image_info,
                                                                canvas_t* c, int32_t x_h,
                                                                int32_t y_v, int32_t xc, int32_t yc,
                                                                int32_t px, int32_t py, float_t e,
                                                                frr_quadrant_t type, rgba_t color) {
  int32_t p = 0;
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = 0;
  int32_t h = 0;
  rgba_t o_color = {0};
  if (ffr_get_circluar_point_to_quadrant(xc, yc, px, py, type, &x, &y)) {
    if (x_h > x) {
      w = x_h - x;
      x_h = x;
    } else {
      w = x - x_h;
      x--;
    }

    if (y_v > y) {
      h = y_v - y;
      y_v = y;
    } else {
      h = y - y_v;
      y--;
    }

    if (w > 0 && h > 0) {
      p = frr_image_get_buff_point(image_info, x + c->ox, y + c->oy);
      if (p >= 0) {
        ffr_image_data2color(image_info->dst + p, x + c->ox, y + c->oy, image_info->w, image_info->h, image_info->format, &o_color);
      }
      canvas_fill_rect(c, x_h, y, w, 1);
      ffr_draw_antialiasing_for_point(image_info, c, x + c->ox, y + c->oy, e, color, o_color);
    }
  }
}

static inline void Wu_D_Circle(int32_t r, darray_t* point_list,
                               frr_point_pix_t* point_pix_cache_list) {
  int32_t x = 0;
  int32_t y = r;
  uint32_t i = 0;
  float_t e = 0.0f;
  int32_t yy = y * y;
  frr_point_pix_t* tmp = NULL;

  for (; x <= y; x++) {
    e = sqrtf(yy + x * x) - (float_t)r;
    if (e >= 1) {
      e--;
      y--;
      yy = y * y;
    }
    tmp = &(point_pix_cache_list[i++]);
    tmp->x = x;
    tmp->y = y;
    tmp->e = e;
    darray_push(point_list, tmp);
  }
}

static void ffr_draw_stroke_arc_point_list(frr_image_info_t* image_info, canvas_t* c, int32_t x_h,
                                              int32_t y_v, int32_t xc, int32_t yc,
                                              frr_quadrant_t type, const color_t* color,
                                              darray_t* point_list, bool_t is_last_antialiasing) {
  uint32_t i = 0;
  void** elms = point_list->elms;
  for (i = 0; i < point_list->size; i++) {
    frr_point_pix_t* iter = (frr_point_pix_t*)elms[i];

    ffr_draw_circluar_point_to_quadrant(image_info, c, x_h, y_v, xc, yc, iter->x, iter->y,
                                           1 - iter->e, type, color->rgba);

    if (point_list->size > i + 1 || is_last_antialiasing) {
      ffr_draw_circluar_point_to_quadrant(image_info, c, x_h, y_v, xc, yc, iter->x, iter->y - 1,
                                             iter->e, type, color->rgba);
    }
  }
}

static void ffr_draw_fill_arc_point_list(frr_image_info_t* image_info, canvas_t* c, int32_t x_h,
                                            int32_t y_v, int32_t xc, int32_t yc, frr_quadrant_t type,
                                            const color_t* color, darray_t* point_list) {
  uint32_t i = 0;
  void** elms = point_list->elms;
  for (i = 0; i < point_list->size; i++) {
    frr_point_pix_t* iter = (frr_point_pix_t*)elms[i];

    ffr_draw_circluar_point_to_quadrant_hline(image_info, c, x_h, y_v, xc, yc, iter->x, iter->y,
                                                 1 - iter->e, type, color->rgba);
  }
}

static void ffr_push_arc_point_list(float_t angle_1, float_t angle_2, int32_t r,
                                       darray_t* point_list, darray_t* point_list_45, bool_t is_v) {
  uint32_t i = 0;
  void** elms = point_list_45->elms;

  float_t x1 = sinf(angle_1) * r;
  float_t x2 = CMP_FLOAT_QE(angle_2, M_FRR_PI_4)
                   ? ((frr_point_pix_t*)elms[point_list_45->size - 1])->x
                   : sinf(angle_2) * r;
  if (is_v) {
    for (i = 0; i < point_list_45->size; i++) {
      frr_point_pix_t* iter = (frr_point_pix_t*)elms[i];
      if ((x1 < iter->x || CMP_FLOAT_QE(x1, iter->x)) &&
          (iter->x < x2 || CMP_FLOAT_QE(x2, iter->x))) {
        darray_push(point_list, iter);
      }
    }
  } else {
    int32_t y = ((frr_point_pix_t*)darray_head(point_list_45))->y;
    for (i = 0; i < point_list_45->size; i++) {
      frr_point_pix_t* iter = (frr_point_pix_t*)elms[i];
      if ((x1 < iter->x || CMP_FLOAT_QE(x1, iter->x)) &&
          (iter->x < x2 || CMP_FLOAT_QE(x2, iter->x))) {
        if (y != iter->y && i - 1 > 0) {
          darray_push(point_list, elms[i - 1]);
          y = iter->y;
        }
        if (i + 1 >= point_list_45->size) {
          darray_push(point_list, iter);
        }
      }
    }
  }
}

static void ffr_draw_arc_point_list(frr_image_info_t* image_info, canvas_t* c, int32_t x_h,
                                       int32_t y_v, int32_t xc, int32_t yc, float_t angle_h,
                                       float_t angle_v, int32_t r, uint32_t quadrant_id,
                                       const color_t* color, darray_t* point_list,
                                       darray_t* point_list_45, bool_t is_fill) {
  float_t angle_v_1 = 0.0f;
  float_t angle_h_1 = 0.0f;
  float_t angle_v_2 = M_FRR_PI_4;
  float_t angle_h_2 = M_FRR_PI_4;

  if ((angle_h > M_FRR_PI_2 && !CMP_FLOAT_QE(angle_h, M_FRR_PI_2)) ||
      (angle_v > M_FRR_PI_2 && !CMP_FLOAT_QE(angle_v, M_FRR_PI_2))) {
    return;
  }

  if (angle_h < M_FRR_PI_4) {
    angle_v_2 = angle_h;
  } else {
    angle_h_1 = M_FRR_PI_2 - angle_h;
  }

  if (angle_v < M_FRR_PI_4) {
    angle_h_2 = angle_v;
  } else {
    angle_v_1 = M_FRR_PI_2 - angle_v;
  }

  if (angle_v > M_FRR_PI_4) {
    ffr_push_arc_point_list(angle_v_1, angle_v_2, r, point_list, point_list_45, TRUE);
    if (is_fill) {
      ffr_draw_fill_arc_point_list(image_info, c, x_h, y_v, xc, yc,
                                      WIDGET_DRAW_ARC_INFO_TABLE[quadrant_id].quadrant_v, color,
                                      point_list);
    } else {
      ffr_draw_stroke_arc_point_list(image_info, c, x_h, y_v, xc, yc,
                                        WIDGET_DRAW_ARC_INFO_TABLE[quadrant_id].quadrant_v, color,
                                        point_list, TRUE);
    }
    point_list->size = 0;
  }

  if (angle_h > M_FRR_PI_4) {
    ffr_push_arc_point_list(angle_h_1, angle_h_2, r, point_list, point_list_45, !is_fill);
    if(point_list->size > 0) {
      point_list->size--;
    }
    if (is_fill) {
      ffr_draw_fill_arc_point_list(image_info, c, x_h, y_v, xc, yc,
                                      WIDGET_DRAW_ARC_INFO_TABLE[quadrant_id].quadrant_h, color,
                                      point_list);
    } else {
      ffr_draw_stroke_arc_point_list(image_info, c, x_h, y_v, xc, yc,
                                        WIDGET_DRAW_ARC_INFO_TABLE[quadrant_id].quadrant_h, color,
                                        point_list, angle_v <= M_FRR_PI_4);
    }
    point_list->size = 0;
  }
}

static bool_t ffr_draw_rounded_radius_equal(uint32_t radius_tl, uint32_t radius_tr, uint32_t radius_bl, uint32_t radius_br) {
  if ((radius_tl != radius_tr && radius_tl != 0 && radius_tr != 0) || 
      (radius_tl != radius_bl && radius_tl != 0 && radius_bl != 0) || 
      (radius_tl != radius_br && radius_tl != 0 && radius_br != 0) ||
      (radius_tr != radius_bl && radius_tr != 0 && radius_bl != 0) || 
      (radius_bl != radius_br && radius_bl != 0 && radius_br != 0) || 
      (radius_bl != radius_br && radius_bl != 0 && radius_br != 0)) {
    return FALSE;
  }
  return TRUE;
}

#endif

#ifndef WITH_NANOVG_GPU
static void ffr_draw_rounded_rect_draw_circle_limit(frr_vertex_type_t type, int32_t x, int32_t y, float_t w1, float_t w2, float_t h1, float_t h2, int32_t mid_lenght_v, int32_t mid_lenght_h, int32_t* v, int32_t* h) {
  int32_t h_x1 = x + (int32_t)w2;
  int32_t h_x2 = x + (int32_t)w2 + mid_lenght_h;
  int32_t v_y1 = y + (int32_t)h1;
  int32_t v_y2 = y + (int32_t)h1 + mid_lenght_v;

  switch (type)
  {
  case FRR_VERTEXT_TYPE_TOP_REIGHT:
    *h = h_x2;
    *v = v_y1;
    break;
  case FRR_VERTEXT_TYPE_TOP_LEFT:    
    *h = h_x1;
    *v = v_y1;
    break;  
  case FRR_VERTEXT_TYPE_BOTTOM_LEFT:
    *h = h_x1;
    *v = v_y2;
    break;  
  case FRR_VERTEXT_TYPE_BOTTOM_REIGHT:
    *h = h_x2;
    *v = v_y2;
    break;
  default:
    break;
  }
}
#endif

static float_t ffr_draw_rounded_get_vertex_angle_by_border_model(int32_t border_model, frr_vertex_type_t type, bool_t is_h, float_t default_angle) {

  int32_t num = (int32_t)type;
  const static border_type_t s_angle_type_to_border_model[] = { BORDER_TOP, BORDER_RIGHT, BORDER_TOP, BORDER_LEFT, BORDER_BOTTOM, BORDER_LEFT, BORDER_BOTTOM, BORDER_RIGHT};

  return_value_if_fail(type < FRR_VERTEXT_TYPE_COUNT, default_angle);
  
  num = num * 2 + (is_h ? 0 : 1);

  if (s_angle_type_to_border_model[num] & border_model) {
    return default_angle;
  } else {
    return tk_min(M_FRR_PI_4, default_angle);
  }
}

static void ffr_draw_rounded_rect_draw_fill_with_vg(canvas_t* c, vgcanvas_t* vg, const color_t* color, frr_draw_info_t draw_infos[FRR_VERTEXT_TYPE_COUNT]) {

  vgcanvas_save(vg);

  vgcanvas_set_fill_color(vg, *color);
  vgcanvas_translate(vg, (float_t)c->ox, (float_t)c->oy);

  vgcanvas_begin_path(vg);

  if (draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].radius > 0) { // 右上角
    vgcanvas_arc(vg, draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].x, draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].y, draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].radius, 
                  M_PI + M_FRR_PI_2 + draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].angle_v, M_PI + M_PI - draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].angle_h, TRUE);
  } else {
    vgcanvas_move_to(vg, draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].x, draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].y);
  }

  if (draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].radius > 0) { // 左上角
    vgcanvas_arc(vg, draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].x, draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].y, draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].radius, 
                M_PI + draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].angle_h, M_PI + M_FRR_PI_2 - draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].angle_v, TRUE); 
  } else {
    vgcanvas_line_to(vg, draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].x, draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].y);
  }

  if (draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].radius > 0) { // 左下角
    vgcanvas_arc(vg, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].x, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].y, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].radius, 
                M_FRR_PI_2 + draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].angle_v, M_PI - draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].angle_h, TRUE); 
  } else {
    vgcanvas_line_to(vg, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].x, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].y);
  }

  if (draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].radius > 0) { // 右下角
    vgcanvas_arc(vg, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].x, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].y, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].radius, 
                draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].angle_h, M_FRR_PI_2 - draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].angle_v, TRUE); 
  } else {
    vgcanvas_line_to(vg, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].x, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].y);
  }

  vgcanvas_close_path(vg);
  vgcanvas_fill(vg);

  vgcanvas_translate(vg, (float_t)-c->ox, (float_t)-c->oy);
  vgcanvas_restore(vg);
}

static frr_vertex_type_t ffr_draw_rounded_rect_get_stroke_start_vertext(int32_t border_model) {
  frr_vertex_type_t start_vertext = FRR_VERTEXT_TYPE_TOP_REIGHT;

  if (border_model == BORDER_TOP) {
    start_vertext = FRR_VERTEXT_TYPE_TOP_REIGHT;
  } else if (border_model == BORDER_BOTTOM) {
    start_vertext = FRR_VERTEXT_TYPE_BOTTOM_LEFT;
  } else if (border_model == BORDER_RIGHT) {
    start_vertext = FRR_VERTEXT_TYPE_BOTTOM_REIGHT;
  } else if (border_model == BORDER_LEFT) {
    start_vertext = FRR_VERTEXT_TYPE_TOP_LEFT;
  } else if (border_model == (BORDER_TOP | BORDER_LEFT | BORDER_BOTTOM)) {
    start_vertext = FRR_VERTEXT_TYPE_TOP_REIGHT;
  } else if (border_model == (BORDER_LEFT | BORDER_BOTTOM | BORDER_RIGHT)) {
    start_vertext = FRR_VERTEXT_TYPE_TOP_LEFT;
  } else if (border_model == (BORDER_BOTTOM | BORDER_RIGHT | BORDER_TOP)) {
    start_vertext = FRR_VERTEXT_TYPE_BOTTOM_LEFT;
  } else if (border_model == (BORDER_RIGHT | BORDER_TOP | BORDER_LEFT)) {
    start_vertext = FRR_VERTEXT_TYPE_BOTTOM_REIGHT;
  } else if (border_model == (BORDER_TOP | BORDER_LEFT)) {
    start_vertext = FRR_VERTEXT_TYPE_TOP_REIGHT;
  } else if (border_model == (BORDER_LEFT | BORDER_BOTTOM)) {
    start_vertext = FRR_VERTEXT_TYPE_TOP_LEFT;
  } else if (border_model == (BORDER_BOTTOM | BORDER_RIGHT)) {
    start_vertext = FRR_VERTEXT_TYPE_BOTTOM_LEFT;
  } else if (border_model == (BORDER_RIGHT | BORDER_TOP)) {
    start_vertext = FRR_VERTEXT_TYPE_BOTTOM_REIGHT;
  } else if (border_model == (BORDER_RIGHT | BORDER_LEFT)) {
    start_vertext = FRR_VERTEXT_TYPE_BOTTOM_REIGHT;
  }

  return start_vertext;
}

static void ffr_draw_rounded_rect_draw_stroke_top_right_with_vg(vgcanvas_t* vg, frr_draw_info_t draw_infos[FRR_VERTEXT_TYPE_COUNT], int32_t border_model, bool_t start) {
  if (draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].radius > 0) { // 右上角
    if ((border_model & BORDER_TOP) != 0 || (border_model & BORDER_RIGHT) != 0) {
      float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_TOP_REIGHT, TRUE, draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].angle_h);
      float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_TOP_REIGHT, FALSE, draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].angle_v);
      
      if ((border_model & BORDER_RIGHT) == 0 && M_FRR_PI_4 < draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].angle_v) {
        float_t d = FFR_DRAW_SIN_45 * draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].radius;
        float_t move_x = draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].x + d;
        float_t move_y = draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].y - d;
        vgcanvas_move_to(vg, move_x, move_y);
      }
      vgcanvas_arc(vg, draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].x, draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].y, draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].radius, 
                    M_PI + M_FRR_PI_2 + tmp_angle_v, M_PI + M_PI - tmp_angle_h, TRUE);
    }
  } else {
    if (!start && (border_model & BORDER_RIGHT) != 0) {
      vgcanvas_line_to(vg, draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].x, draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].y);
    } else {
      vgcanvas_move_to(vg, draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].x, draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].y);
    }
  }
}

static void ffr_draw_rounded_rect_draw_stroke_top_left_with_vg(vgcanvas_t* vg, frr_draw_info_t draw_infos[FRR_VERTEXT_TYPE_COUNT], int32_t border_model, bool_t start) {
  if (draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].radius > 0) { // 左上角
    if ((border_model & BORDER_TOP) != 0 || (border_model & BORDER_LEFT) != 0) {
      float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_TOP_LEFT, TRUE, draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].angle_h);
      float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_TOP_LEFT, FALSE, draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].angle_v);
      
      if ((border_model & BORDER_TOP) == 0 && M_FRR_PI_4 < draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].angle_v) {
        float_t d = FFR_DRAW_SIN_45 * draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].radius;
        float_t move_x = draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].x - d;
        float_t move_y = draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].y - d;
        vgcanvas_move_to(vg, move_x, move_y);
      }
      vgcanvas_arc(vg, draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].x, draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].y, draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].radius, 
                  M_PI + tmp_angle_h, M_PI + M_FRR_PI_2 - tmp_angle_v, TRUE); 
    }
  } else {
    if (!start && (border_model & BORDER_TOP) != 0) {
      vgcanvas_line_to(vg, draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].x, draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].y);
    } else {
      vgcanvas_move_to(vg, draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].x, draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].y);
    }
  }
}

static void ffr_draw_rounded_rect_draw_stroke_bottom_left_with_vg(vgcanvas_t* vg, frr_draw_info_t draw_infos[FRR_VERTEXT_TYPE_COUNT], int32_t border_model, bool_t start) {
  if (draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].radius > 0) { // 左下角
    if ((border_model & BORDER_BOTTOM) != 0 || (border_model & BORDER_LEFT) != 0) {
      float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_BOTTOM_LEFT, TRUE, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].angle_h);
      float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_BOTTOM_LEFT, FALSE, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].angle_v);

      if ((border_model & BORDER_LEFT) == 0 && M_FRR_PI_4 < draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].angle_h) {
        float_t d = FFR_DRAW_SIN_45 * draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].radius;
        float_t move_x = draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].x - d;
        float_t move_y = draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].y + d;
        vgcanvas_move_to(vg, move_x, move_y);
      }
      vgcanvas_arc(vg, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].x, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].y, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].radius, 
                  M_FRR_PI_2 + tmp_angle_v, M_PI - tmp_angle_h, TRUE); 
    }
  } else {
    if (!start && (border_model & BORDER_LEFT) != 0) {
      vgcanvas_line_to(vg, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].x, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].y);
    } else {
      vgcanvas_move_to(vg, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].x, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].y);
    }
  }
}

static void ffr_draw_rounded_rect_draw_stroke_bottom_right_with_vg(vgcanvas_t* vg, frr_draw_info_t draw_infos[FRR_VERTEXT_TYPE_COUNT], int32_t border_model, bool_t start) {
  if (draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].radius > 0) { // 右下角
    if ((border_model & BORDER_BOTTOM) != 0 || (border_model & BORDER_RIGHT) != 0) {
      float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_BOTTOM_REIGHT, TRUE, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].angle_h);
      float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_BOTTOM_REIGHT, FALSE, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].angle_v);

      if ((border_model & BORDER_BOTTOM) == 0 && M_FRR_PI_4 < draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].angle_h) {
        float_t d = FFR_DRAW_SIN_45 * draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].radius;
        float_t move_x = draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].x + d;
        float_t move_y = draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].y + d;
        vgcanvas_move_to(vg, move_x, move_y);
      }
      vgcanvas_arc(vg, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].x, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].y, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].radius, 
                  tmp_angle_h, M_FRR_PI_2 - tmp_angle_v, TRUE); 
    }
  } else {
    if (!start && (border_model & BORDER_BOTTOM) != 0) {
      vgcanvas_line_to(vg, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].x, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].y);
    } else {
      vgcanvas_move_to(vg, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].x, draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].y);
    }
  }
}

static void ffr_draw_rounded_rect_draw_stroke_with_vg(canvas_t* c, vgcanvas_t* vg, uint32_t border_width, const color_t* color, frr_draw_info_t draw_infos[FRR_VERTEXT_TYPE_COUNT], int32_t border_model) {
  uint32_t i = 0;
  uint32_t n = 0;
  frr_vertex_type_t start_vertext = ffr_draw_rounded_rect_get_stroke_start_vertext(border_model);
  vgcanvas_save(vg);

  vgcanvas_set_stroke_color(vg, *color);
  vgcanvas_set_line_width(vg, (float_t)border_width);

  vgcanvas_translate(vg, (float_t)c->ox, (float_t)c->oy);

  vgcanvas_begin_path(vg);
  
  i = start_vertext;

  for (; n < FRR_VERTEXT_TYPE_COUNT; i++, n++) {
    frr_vertex_type_t vertext = (frr_vertex_type_t)(i % FRR_VERTEXT_TYPE_COUNT);
    switch (vertext)
    {
    case FRR_VERTEXT_TYPE_TOP_REIGHT :
      ffr_draw_rounded_rect_draw_stroke_top_right_with_vg(vg, draw_infos, border_model, start_vertext == FRR_VERTEXT_TYPE_TOP_REIGHT);
      break;
    case FRR_VERTEXT_TYPE_TOP_LEFT :
      ffr_draw_rounded_rect_draw_stroke_top_left_with_vg(vg, draw_infos, border_model, start_vertext == FRR_VERTEXT_TYPE_TOP_LEFT);
      break;
    case FRR_VERTEXT_TYPE_BOTTOM_REIGHT :
      ffr_draw_rounded_rect_draw_stroke_bottom_right_with_vg(vg, draw_infos, border_model, start_vertext == FRR_VERTEXT_TYPE_BOTTOM_REIGHT);
      break;
    case FRR_VERTEXT_TYPE_BOTTOM_LEFT :
      ffr_draw_rounded_rect_draw_stroke_bottom_left_with_vg(vg, draw_infos, border_model, start_vertext == FRR_VERTEXT_TYPE_BOTTOM_LEFT);
      break;
    default:
      break;
    }
  }

  if (border_model == BORDER_ALL) {
    vgcanvas_close_path(vg);
  }

  vgcanvas_stroke(vg);

  vgcanvas_translate(vg, (float_t)-c->ox, (float_t)-c->oy);
  vgcanvas_restore(vg);
}

static inline void ffr_get_standard_rounded_rect_radius(const rect_t* r, uint32_t* radius_tl, uint32_t* radius_tr, 
                                                    uint32_t* radius_bl, uint32_t* radius_br) {
  int32_t w = r->w / 2;
  int32_t h = r->h / 2;
  bool_t is_h = w > h;
  FFR_SET_STANDARD_RADIUS(is_h, w, h, radius_tl);
  FFR_SET_STANDARD_RADIUS(is_h, w, h, radius_tr);
  FFR_SET_STANDARD_RADIUS(is_h, w, h, radius_bl);
  FFR_SET_STANDARD_RADIUS(is_h, w, h, radius_br);
}

static int32_t ffr_get_rounded_rect_mid_length(uint32_t radius, uint32_t total_length,
                                                  float_t* l1, float_t* l2, float_t* angle,
                                                  bool_t is_add) {
  float_t a = M_FRR_PI_2;
  int32_t mid_lenght = 0;
  if (total_length > radius * 2) {
    *l1 = *l2 = (float_t)radius;
    mid_lenght = total_length - radius * 2;
  } else if (radius * 2 == total_length) {
    *l1 = *l2 = (float_t)radius;
  } else {
    *l1 = (float_t)radius;
    if (is_add) {
      *l2 = (float_t)(total_length / 2);
      if (total_length % 2 != 0) {
        *l2 += 1;
      }
    } else {
      *l2 = (float_t)(total_length / 2.0f);
    }

    a = acosf(1.0f - *l2 / radius);
  }
  *angle = a;
  return mid_lenght;
}

static void ffr_draw_rounded_rect_draw_info_init(frr_draw_info_t draw_infos[FRR_VERTEXT_TYPE_COUNT], const rect_t* r, uint32_t radius_tl, uint32_t radius_tr, uint32_t radius_bl, uint32_t radius_br) {
  int32_t i = 0;
  int32_t j = 0;
  int32_t x = r->x;
  int32_t y = r->y;
  float_t w1 = 0.0f;
  float_t w2 = 0.0f;
  float_t h1 = 0.0f;
  float_t h2 = 0.0f;
  int32_t mid_lenght_v = 0;
  int32_t mid_lenght_h = 0;
  uint32_t radius_list[FRR_VERTEXT_TYPE_COUNT] = {radius_tr, radius_tl, radius_bl, radius_br};

  memset(draw_infos, 0x0, sizeof(frr_draw_info_t) * FRR_VERTEXT_TYPE_COUNT);

  draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].x = x + r->w - radius_list[FRR_VERTEXT_TYPE_TOP_REIGHT];
  draw_infos[FRR_VERTEXT_TYPE_TOP_REIGHT].y = y + radius_list[FRR_VERTEXT_TYPE_TOP_REIGHT];

  draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].x = x + radius_list[FRR_VERTEXT_TYPE_TOP_LEFT];
  draw_infos[FRR_VERTEXT_TYPE_TOP_LEFT].y = y + radius_list[FRR_VERTEXT_TYPE_TOP_LEFT];

  draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].x = x + radius_list[FRR_VERTEXT_TYPE_BOTTOM_LEFT];
  draw_infos[FRR_VERTEXT_TYPE_BOTTOM_LEFT].y = y + r->h - radius_list[FRR_VERTEXT_TYPE_BOTTOM_LEFT];

  draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].x = x + r->w - radius_list[FRR_VERTEXT_TYPE_BOTTOM_REIGHT];
  draw_infos[FRR_VERTEXT_TYPE_BOTTOM_REIGHT].y = y + r->h - radius_list[FRR_VERTEXT_TYPE_BOTTOM_REIGHT];

  for (i = 0; i < FRR_VERTEXT_TYPE_COUNT; i++) {
    uint32_t radius = radius_list[i];
    if (radius == 0) {
      continue;
    }
    draw_infos[i].radius = radius;
    mid_lenght_v = ffr_get_rounded_rect_mid_length(radius, r->h, &w1, &h1, &(draw_infos[i].angle_v), TRUE);
    mid_lenght_h = ffr_get_rounded_rect_mid_length(radius, r->w, &h2, &w2, &(draw_infos[i].angle_h), TRUE);
#ifndef WITH_NANOVG_GPU
    ffr_draw_rounded_rect_draw_circle_limit((frr_vertex_type_t)i, x, y, w1, w2, h1, h2, mid_lenght_v, mid_lenght_h, &(draw_infos[i].v), &(draw_infos[i].h));
#else
    (void)mid_lenght_v;
    (void)mid_lenght_h;
#endif

    for(j = i; j < FRR_VERTEXT_TYPE_COUNT; j++) {
      if (i != j && radius_list[j] == radius) {
        draw_infos[j].radius = radius;
        draw_infos[j].angle_v = draw_infos[i].angle_v;
        draw_infos[j].angle_h = draw_infos[i].angle_h;
#ifndef WITH_NANOVG_GPU
        ffr_draw_rounded_rect_draw_circle_limit((frr_vertex_type_t)j, x, y, w1, w2, h1, h2, mid_lenght_v, mid_lenght_h, &(draw_infos[j].v), &(draw_infos[j].h));
#endif
      }
    }
  }
}

static ret_t ffr_draw_fill_rounded_rect(canvas_t* c, const rect_t* r, const color_t* color, uint32_t radius_tl, uint32_t radius_tr, uint32_t radius_bl, uint32_t radius_br) {

  uint32_t radius = radius_tl != 0 ? radius_tl : (radius_tr != 0 ? radius_tr : (radius_bl != 0 ? radius_bl : radius_br));

#ifndef WITH_NANOVG_GPU
  int32_t x = r->x;
  int32_t y = r->y;
  int32_t x1 = x + radius;
  int32_t y1 = y + radius;
  int32_t x2 = x + r->w - radius;
  int32_t y2 = y + r->h - radius;

  darray_t point_list_45 = {0};
  darray_t tmp_point_list_45 = {0};
  frr_image_info_t image_info = {0};
  frr_point_pix_t* point_pix_cache_list = NULL;

  if (radius != 0 && ffr_draw_rounded_radius_equal(radius_tl, radius_tr, radius_bl, radius_br)) {
    float_t w1 = 0.0f;
    float_t w2 = 0.0f;
    float_t h1 = 0.0f;
    float_t h2 = 0.0f;
    float_t angle_v = 0.0f;
    float_t angle_h = 0.0f;
    int32_t mid_lenght_v = ffr_get_rounded_rect_mid_length(radius, r->h, &w1, &h1, &angle_v, TRUE);
    int32_t mid_lenght_h = ffr_get_rounded_rect_mid_length(radius, r->w, &h2, &w2, &angle_h, TRUE);

    int32_t v_x1 = x + (int32_t)w2;
    int32_t v_y1 = y + (int32_t)h1;
    int32_t v_x2 = x + (int32_t)w2 + mid_lenght_h;
    int32_t v_y2 = y + (int32_t)h1 + mid_lenght_v;

    uint32_t size = ANGLE2SIZE(M_FRR_PI_4, radius) * 1.5f;
    if (size <= FRR_POINT_PIX_CACHE_SIZE) {
      point_pix_cache_list = frr_point_pix_cache_list;
    } else {
      point_pix_cache_list = TKMEM_ZALLOCN(frr_point_pix_t, size);
    }

    darray_init(&tmp_point_list_45, size, NULL, NULL);
    darray_init(&point_list_45, size, NULL, NULL);

    Wu_D_Circle(radius, &point_list_45, point_pix_cache_list);

    canvas_set_fill_color(c, *color);
    ffr_image_info_create(&image_info, c);

    if (radius_tr > 0) {
      ffr_draw_arc_point_list(&image_info, c, v_x2, v_y1, x2, y1, angle_h, angle_v, radius, FRR_VERTEXT_TYPE_TOP_REIGHT, color,
                              &tmp_point_list_45, &point_list_45, TRUE);  // 右上角
    }

    if (radius_tl > 0) {
      ffr_draw_arc_point_list(&image_info, c, v_x1, v_y1, x1, y1, angle_h, angle_v, radius, FRR_VERTEXT_TYPE_TOP_LEFT, color,
                              &tmp_point_list_45, &point_list_45, TRUE);  // 左上角
    }

    if (radius_bl > 0) {
      ffr_draw_arc_point_list(&image_info, c, v_x1, v_y2, x1, y2, angle_h, angle_v, radius, FRR_VERTEXT_TYPE_BOTTOM_LEFT, color,
                              &tmp_point_list_45, &point_list_45, TRUE);  // 左下角
    }

    if (radius_br > 0) {
      ffr_draw_arc_point_list(&image_info, c, v_x2, v_y2, x2, y2, angle_h, angle_v, radius, FRR_VERTEXT_TYPE_BOTTOM_REIGHT, color,
                              &tmp_point_list_45, &point_list_45, TRUE);  // 右下角
    }

    if (mid_lenght_v != 0) {
      canvas_fill_rect(c, x, v_y1, (wh_t)w2, mid_lenght_v);  //左边矩形
      if (mid_lenght_h == 0 && w2 + w2 > r->w) {
        canvas_fill_rect(c, v_x2, v_y1, (wh_t)(r->w - w2), mid_lenght_v);  //右边矩形
      } else {
        canvas_fill_rect(c, v_x2, v_y1, (wh_t)w2, mid_lenght_v);  //右边矩形
      }
    }

    if (mid_lenght_h != 0) {
      uint32_t tmp_x = v_x1;
      uint32_t tmp_w = mid_lenght_h;
      if (radius_tl == 0) {
        tmp_x = x;
        tmp_w += radius;
      }
      if (radius_tr == 0) {
        tmp_w += radius;
      }

      canvas_fill_rect(c, tmp_x, y, tmp_w, (wh_t)h1);  //上边矩形

      tmp_x = v_x1;
      tmp_w = mid_lenght_h;
      if (radius_bl == 0) {
        tmp_x = x;
        tmp_w += radius;
      }
      if (radius_br == 0) {
        tmp_w += radius;
      }
      if (mid_lenght_v == 0 && h1 + h1 > r->h) {
        canvas_fill_rect(c, tmp_x, v_y2, tmp_w, (wh_t)(r->h - h1));  //下边矩形
      } else {
        canvas_fill_rect(c, tmp_x, v_y2, tmp_w, (wh_t)h1);  //下边矩形
      }
    }

    if (mid_lenght_v != 0 && mid_lenght_h != 0) {
      canvas_fill_rect(c, v_x1, v_y1, (wh_t)mid_lenght_h, (wh_t)mid_lenght_v);  //中间矩形
    }

    darray_deinit(&point_list_45);
    darray_deinit(&tmp_point_list_45);

    if (size > FRR_POINT_PIX_CACHE_SIZE && point_pix_cache_list != NULL) {
      TKMEM_FREE(point_pix_cache_list);
      point_pix_cache_list = NULL;
    }
  } else
#endif
  {  
    vgcanvas_t* vg = canvas_get_vgcanvas(c);
    if (radius != 0 && vg != NULL) {
      frr_draw_info_t draw_infos[FRR_VERTEXT_TYPE_COUNT] = {0};
      ffr_draw_rounded_rect_draw_info_init(draw_infos, r, radius_tl, radius_tr, radius_bl, radius_br);
      ffr_draw_rounded_rect_draw_fill_with_vg(c, vg, color, draw_infos);
    } else {
      return RET_FAIL;
    }
  }
  return RET_OK;
}

static ret_t ffr_draw_stroke_rounded_rect(canvas_t* c, const rect_t* r, const color_t* color, uint32_t radius_tl, uint32_t radius_tr, uint32_t radius_bl, uint32_t radius_br,
                                            uint32_t border_width, int32_t border_model) {
                                              
  uint32_t radius = radius_tl != 0 ? radius_tl : (radius_tr != 0 ? radius_tr : (radius_bl != 0 ? radius_bl : radius_br));

#ifndef WITH_NANOVG_GPU
  float_t w1 = 0.0f;
  float_t w2 = 0.0f;
  float_t h1 = 0.0f;
  float_t h2 = 0.0f;
  float_t angle_v = 0.0f;
  float_t angle_h = 0.0f;

  darray_t point_list_45 = {0};
  darray_t tmp_point_list_45 = {0};

  frr_image_info_t image_info = {0};
  frr_point_pix_t* point_pix_cache_list = NULL;

  if (border_width == 1 && radius != 0 && ffr_draw_rounded_radius_equal(radius_tl, radius_tr, radius_bl, radius_br)) {
    int32_t x = r->x;
    int32_t y = r->y;
    int32_t x1 = x + radius;
    int32_t y1 = y + radius;
    int32_t x2 = x + r->w - radius;
    int32_t y2 = y + r->h - radius;

    int32_t mid_lenght_v = ffr_get_rounded_rect_mid_length(radius, r->h, &w1, &h1, &angle_v, TRUE);
    int32_t mid_lenght_h = ffr_get_rounded_rect_mid_length(radius, r->w, &h2, &w2, &angle_h, TRUE);

    int32_t v_x1 = x + (int32_t)w2;
    int32_t v_x2 = x + (int32_t)w2 + mid_lenght_h;
    int32_t v_y1 = y + (int32_t)h1;
    int32_t v_y2 = y + (int32_t)h1 + mid_lenght_v;

    uint32_t size = ANGLE2SIZE(M_FRR_PI_4, radius) + 1;
    if (size <= FRR_POINT_PIX_CACHE_SIZE) {
      point_pix_cache_list = frr_point_pix_cache_list;
    } else {
      point_pix_cache_list = TKMEM_ZALLOCN(frr_point_pix_t, size);
    }

    darray_init(&point_list_45, size, NULL, NULL);
    darray_init(&tmp_point_list_45, size, NULL, NULL);

    Wu_D_Circle(radius, &point_list_45, point_pix_cache_list);

    canvas_set_stroke_color(c, *color);

    ffr_image_info_create(&image_info, c);

    if (radius_tr > 0) {
      float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_TOP_REIGHT, TRUE, angle_h);
      float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_TOP_REIGHT, FALSE, angle_v);
      ffr_draw_arc_point_list(&image_info, c, v_x2, v_y1, x2, y1, tmp_angle_h, tmp_angle_v, radius, FRR_VERTEXT_TYPE_TOP_REIGHT,
                                color, &tmp_point_list_45, &point_list_45, FALSE);  // 右上角
    }
    if (radius_tl > 0) {
      float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_TOP_LEFT, TRUE, angle_h);
      float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_TOP_LEFT, FALSE, angle_v);
      ffr_draw_arc_point_list(&image_info, c, v_x1, v_y1, x1, y1, tmp_angle_h, tmp_angle_v, radius, FRR_VERTEXT_TYPE_TOP_LEFT,
                                 color, &tmp_point_list_45, &point_list_45, FALSE);  // 左上角
    }
    if (radius_bl > 0) {
      float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_BOTTOM_LEFT, TRUE, angle_h);
      float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_BOTTOM_LEFT, FALSE, angle_v);
      ffr_draw_arc_point_list(&image_info, c, v_x1, v_y2, x1, y2, tmp_angle_h, tmp_angle_v, radius, FRR_VERTEXT_TYPE_BOTTOM_LEFT,
                                color, &tmp_point_list_45, &point_list_45, FALSE);  // 左下角
    }

    if (radius_br > 0) {
      float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_BOTTOM_REIGHT, TRUE, angle_h);
      float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FRR_VERTEXT_TYPE_BOTTOM_REIGHT, FALSE, angle_v);
      ffr_draw_arc_point_list(&image_info, c, v_x2, v_y2, x2, y2, tmp_angle_h, tmp_angle_v, radius, FRR_VERTEXT_TYPE_BOTTOM_REIGHT,
                                color, &tmp_point_list_45, &point_list_45, FALSE);  // 右下角
    }

    if (mid_lenght_v != 0 || radius_tl != 0 || radius_bl != 0 || radius_tr != 0 || radius_br != 0) {
      uint32_t tmp_y1 = v_y1;
      uint32_t tmp_y2 = v_y1;
      uint32_t tmp_h1 = mid_lenght_v;
      uint32_t tmp_h2 = mid_lenght_v;
      if (radius_tl == 0) {
        if (border_model & BORDER_TOP) {
          tmp_y1 = y + 1;
          tmp_h1 += radius - 1;
        } else {
          tmp_y1 = y;
          tmp_h1 += radius;
        }
      }
      if (radius_bl == 0) {
        if (border_model & BORDER_BOTTOM) {
          tmp_h1 += radius - 1;
        } else {
          tmp_h1 += radius;
        }
      }

      if (radius_tr == 0) {
        if (border_model & BORDER_TOP) {
          tmp_y2 = y + 1;
          tmp_h2 += radius - 1;
        } else {
          tmp_y2 = y;
          tmp_h2 += radius;
        }
      }
      if (radius_br == 0) {
        if (border_model & BORDER_BOTTOM) {
          tmp_h2 += radius - 1;
        } else {
          tmp_h2 += radius;
        }
      }
      if (border_model & BORDER_LEFT && tmp_h1 != 0) {
        canvas_draw_vline(c, x, tmp_y1, tmp_h1);                      //左边边
      }
      if (border_model & BORDER_RIGHT && tmp_h2 != 0) {
        canvas_draw_vline(c, (xy_t)(v_x2 + w2 - 1), tmp_y2, tmp_h2);  //右边边
      }
    }

    if (mid_lenght_h != 0 || radius_tl != 0 || radius_bl != 0 || radius_tr != 0 || radius_br != 0) {
      uint32_t tmp_x1 = v_x1;
      uint32_t tmp_x2 = v_x1;
      uint32_t tmp_w1 = mid_lenght_h;
      uint32_t tmp_w2 = mid_lenght_h;

      if (radius_tl == 0) {
        tmp_x1 = x;
        tmp_w1 += radius;
      }
      if (radius_tr == 0) {
        tmp_w1 += radius;
      }

      if (radius_bl == 0) {
        tmp_x2 = x;
        tmp_w2 += radius;
      }
      if (radius_br == 0) {
        tmp_w2 += radius;
      }

      if (border_model & BORDER_TOP && tmp_w1 != 0) {
        canvas_draw_hline(c, tmp_x1, y, tmp_w1);                      //上边边
      }
      if (border_model & BORDER_BOTTOM && tmp_w2 != 0) {
        canvas_draw_hline(c, tmp_x2, (xy_t)(v_y2 + h1 - 1), tmp_w2);  //下边边
      }
    }

    darray_deinit(&point_list_45);
    darray_deinit(&tmp_point_list_45);

    if (size > FRR_POINT_PIX_CACHE_SIZE && point_pix_cache_list != NULL) {
      TKMEM_FREE(point_pix_cache_list);
      point_pix_cache_list = NULL;
    }
  } else
#endif  // WITH_NANOVG_GPU
  {
    vgcanvas_t* vg = canvas_get_vgcanvas(c);
    if (radius != 0 && vg != NULL) {
      frr_draw_info_t draw_infos[FRR_VERTEXT_TYPE_COUNT] = {0};
      ffr_draw_rounded_rect_draw_info_init(draw_infos, r, radius_tl, radius_tr, radius_bl, radius_br);

      ffr_draw_rounded_rect_draw_stroke_with_vg(c, vg, border_width, color, draw_infos, border_model);
    } else {
      return RET_FAIL;
    }
  }
  return RET_OK;
}

static inline ret_t ffr_draw_fill_rounded_rect_ex(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                                                  const color_t* color, uint32_t radius_tl, uint32_t radius_tr, 
                                                  uint32_t radius_bl, uint32_t radius_br) {
  uint32_t radius = 0;
  xy_t x = r->x + c->ox;
  xy_t y = r->y + c->oy;
  xy_t x2 = x + r->w - 1;
  xy_t y2 = y + r->h - 1;
  color_t tc = *color;

  if (x > c->clip_right || x2 < c->clip_left ||
      y > c->clip_bottom || y2 < c->clip_top) {
    return RET_OK;
  }
  
  if (bg_r == NULL) {
    ffr_get_standard_rounded_rect_radius(r, &radius_tl, &radius_tr, &radius_bl, &radius_br);
  } else {
    ffr_get_standard_rounded_rect_radius(bg_r, &radius_tl, &radius_tr, &radius_bl, &radius_br);
  }

  radius = tk_max(radius_tl, tk_max(radius_tr, tk_max(radius_bl, radius_br)));
  if (radius < FFR_SUPPORT_MIN_RADIUS) {
    return RET_FAIL;
  }

  tc.rgba.a = tc.rgba.a * c->global_alpha / 0xff;
  return ffr_draw_fill_rounded_rect(c, r, &tc, radius_tl, radius_tr, radius_bl, radius_br);
}

static inline ret_t ffr_draw_stroke_rounded_rect_ex(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                                                    const color_t* color, uint32_t radius_tl, uint32_t radius_tr, 
                                                    uint32_t radius_bl, uint32_t radius_br, uint32_t border_width, int32_t border_model) {
  uint32_t radius = 0;
  xy_t x = r->x + c->ox;
  xy_t y = r->y + c->oy;
  xy_t x2 = x + r->w - 1;
  xy_t y2 = y + r->h - 1;
  color_t tc = *color;

  if (x > c->clip_right || x2 < c->clip_left ||
      y > c->clip_bottom || y2 < c->clip_top || border_model == BORDER_NONE) {
    return RET_OK;
  }

  if (bg_r == NULL) {
    ffr_get_standard_rounded_rect_radius(r, &radius_tl, &radius_tr, &radius_bl, &radius_br);
  } else {
    ffr_get_standard_rounded_rect_radius(bg_r, &radius_tl, &radius_tr, &radius_bl, &radius_br);
  }

  radius = tk_max(radius_tl, tk_max(radius_tr, tk_max(radius_bl, radius_br)));

  if (radius < FFR_SUPPORT_MIN_RADIUS) {
    return RET_FAIL;
  }
  tc.rgba.a = tc.rgba.a * c->global_alpha / 0xff;
  if (border_model == (BORDER_LEFT | BORDER_RIGHT | BORDER_TOP | BORDER_BOTTOM)) {
    border_model = BORDER_ALL;
  }
  return ffr_draw_stroke_rounded_rect(c, r, &tc, radius_tl, radius_tr, radius_bl, radius_br, border_width, border_model);
}
