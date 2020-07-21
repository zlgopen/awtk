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

typedef struct _frr_point_pix_t {
  int32_t x;
  int32_t y;
  float_t e;
} frr_point_pix_t;

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

#define CMP_FLOAT_QE(a, b) (-CMP_ACCURACY <= ((a) - (b)) && ((a) - (b)) <= CMP_ACCURACY)

#define ANGLE2SIZE(angle, r) ((int)(sinf(angle) * (r)))

#define BLEND_DATA(d1, d2, a) ((((d1) * (a)) + ((d2) * (0xff - (a)))) >> 8)

#define IMAGE_COLOR2COLOR_BLEND(c1, c2, a) \
  {                                        \
    if (a < 0x8) {                         \
      c1.r = c2.r;                         \
      c1.g = c2.g;                         \
      c1.b = c2.b;                         \
      c1.a = c2.a;                         \
    } else if (a < 0xfa)  {                \
      c1.r = BLEND_DATA(c1.r, c2.r, a);    \
      c1.g = BLEND_DATA(c1.g, c2.g, a);    \
      c1.b = BLEND_DATA(c1.b, c2.b, a);    \
      c1.a = 0xff;                         \
    }                                      \
  }                                        \

#define WIDGET_SET_ROUNDED_RECT_VERTEX_STROKE(vg, o_x, o_y, radius, start_angle, end_angle, ccw)  \
  {                                                                                               \
    if (start_angle > end_angle) {                                                                \
      vgcanvas_arc(vg, (float_t)o_x, (float_t)o_y, (float_t)radius, start_angle, end_angle, ccw); \
    }                                                                                             \
  }

#ifndef WITH_NANOVG_GPU

static frr_point_pix_t frr_point_pix_cache_list[FRR_POINT_PIX_CACHE_SIZE];

const static frr_draw_arc_info_table_t WIDGET_DRAW_ARC_INFO_TABLE[] = {
    {FRR_QUADRANT_FIRST_S, FRR_QUADRANT_FIRST_F},     //右上角
    {FRR_QUADRANT_SECOND_S, FRR_QUADRANT_SECOND_F},   //左上角
    {FRR_QUADRANT_THIRD_F, FRR_QUADRANT_THIRD_S},     //左下角
    {FRR_QUADRANT_FOURTH_F, FRR_QUADRANT_FOURTH_S}};  //右下角

static inline int32_t widget_frr_image_get_buff_point(frr_image_info_t* info, int32_t x,
                                                      int32_t y) {
#if defined(WITH_LCD_MONO)
  return 0;
#else
  int32_t p = -1;
  int32_t dx = 0;
  int32_t dy = 0;

  return_value_if_fail(info != NULL, -1);

  if (x < 0 || x > (int32_t)info->w || y < 0 || y > (int32_t)info->h || info->stride == 0 ||
      info->bpp == 0) {
    return p;
  }

  dx = x - info->dx;
  dy = y - info->dy;

  if (dx < 0 || dx > (int32_t)info->dw || dy < 0 || dy > (int32_t)info->dh) {
    return p;
  }

  p = dx * info->bpp + dy * info->stride;

  return p;
#endif
}

static inline frr_image_info_t* widget_image_info_create(frr_image_info_t* info, canvas_t* c) {
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

static inline void widget_image_data2color(unsigned char* d, int32_t x, int32_t y, int32_t w, int32_t h, bitmap_format_t format, rgba_t* rgb) {
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

static inline void widget_image_color2data(unsigned char* d, int32_t x, int32_t y, int32_t w, int32_t h, bitmap_format_t format, rgba_t rgb) {
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

static inline void widget_draw_antialiasing_for_point(frr_image_info_t* image_info,
                                                      canvas_t* canvas, int px, int py, float_t e,
                                                      rgba_t n_color, rgba_t o_color) {
  int32_t p = 0;
  unsigned char a = (unsigned char)(e * n_color.a);

  if (canvas->clip_left <= px && px <= canvas->clip_right && canvas->clip_top <= py &&
      py <= canvas->clip_bottom) {
    IMAGE_COLOR2COLOR_BLEND(n_color, o_color, a);
    p = widget_frr_image_get_buff_point(image_info, px, py);
    if (p >= 0) {
      widget_image_color2data(image_info->dst + p, px, py, image_info->w, image_info->h, image_info->format, n_color);
    }
  }
}

static inline bool_t widget_get_circluar_point_to_quadrant(int32_t xc, int32_t yc, int32_t px,
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

static inline void widget_draw_circluar_point_to_quadrant(frr_image_info_t* image_info, canvas_t* c,
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
  if (widget_get_circluar_point_to_quadrant(xc, yc, px, py, type, &x, &y)) {
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
      p = widget_frr_image_get_buff_point(image_info, x, y);
      if (p >= 0) {
        widget_image_data2color(image_info->dst + p, x, y, image_info->w, image_info->h, image_info->format, &o_color);

        widget_draw_antialiasing_for_point(image_info, c, x, y, e, color, o_color);
      }
    }
  }
}

static inline void widget_draw_circluar_point_to_quadrant_hline(frr_image_info_t* image_info,
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
  if (widget_get_circluar_point_to_quadrant(xc, yc, px, py, type, &x, &y)) {
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
      p = widget_frr_image_get_buff_point(image_info, x + c->ox, y + c->oy);
      if (p >= 0) {
        widget_image_data2color(image_info->dst + p, x + c->ox, y + c->oy, image_info->w, image_info->h, image_info->format, &o_color);

        canvas_fill_rect(c, x_h, y, w, 1);

        widget_draw_antialiasing_for_point(image_info, c, x + c->ox, y + c->oy, e, color, o_color);
      }
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

static void widget_draw_stroke_arc_point_list(frr_image_info_t* image_info, canvas_t* c, int32_t x,
                                              int32_t y, int32_t xc, int32_t yc,
                                              frr_quadrant_t type, color_t* color,
                                              darray_t* point_list, bool_t is_last_antialiasing) {
  uint32_t i = 0;
  void** elms = point_list->elms;
  for (i = 0; i < point_list->size; i++) {
    frr_point_pix_t* iter = (frr_point_pix_t*)elms[i];

    widget_draw_circluar_point_to_quadrant(image_info, c, x, y, xc, yc, iter->x, iter->y,
                                           1 - iter->e, type, color->rgba);

    if (point_list->size > i + 1 || is_last_antialiasing) {
      widget_draw_circluar_point_to_quadrant(image_info, c, x, y, xc, yc, iter->x, iter->y - 1,
                                             iter->e, type, color->rgba);
    }
  }
}

static void widget_draw_fill_arc_point_list(frr_image_info_t* image_info, canvas_t* c, int32_t x,
                                            int32_t y, int32_t xc, int32_t yc, frr_quadrant_t type,
                                            color_t* color, darray_t* point_list) {
  uint32_t i = 0;
  void** elms = point_list->elms;
  for (i = 0; i < point_list->size; i++) {
    frr_point_pix_t* iter = (frr_point_pix_t*)elms[i];

    widget_draw_circluar_point_to_quadrant_hline(image_info, c, x, y, xc, yc, iter->x, iter->y,
                                                 1 - iter->e, type, color->rgba);
  }
}

static void widget_push_arc_point_list(float_t angle_1, float_t angle_2, int32_t r,
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

static void widget_draw_arc_point_list(frr_image_info_t* image_info, canvas_t* c, int32_t x,
                                       int32_t y, int32_t xc, int32_t yc, float_t angle_h,
                                       float_t angle_v, int32_t r, uint32_t quadrant_id,
                                       color_t* color, darray_t* point_list,
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
    widget_push_arc_point_list(angle_v_1, angle_v_2, r, point_list, point_list_45, TRUE);
    if (is_fill) {
      widget_draw_fill_arc_point_list(image_info, c, x, y, xc, yc,
                                      WIDGET_DRAW_ARC_INFO_TABLE[quadrant_id].quadrant_v, color,
                                      point_list);
    } else {
      widget_draw_stroke_arc_point_list(image_info, c, x, y, xc, yc,
                                        WIDGET_DRAW_ARC_INFO_TABLE[quadrant_id].quadrant_v, color,
                                        point_list, TRUE);
    }
    point_list->size = 0;
  }

  if (angle_h > M_FRR_PI_4) {
    widget_push_arc_point_list(angle_h_1, angle_h_2, r, point_list, point_list_45, !is_fill);
    if(point_list->size > 0) {
      point_list->size--;
    }
    if (is_fill) {
      widget_draw_fill_arc_point_list(image_info, c, x, y, xc, yc,
                                      WIDGET_DRAW_ARC_INFO_TABLE[quadrant_id].quadrant_h, color,
                                      point_list);
    } else {
      widget_draw_stroke_arc_point_list(image_info, c, x, y, xc, yc,
                                        WIDGET_DRAW_ARC_INFO_TABLE[quadrant_id].quadrant_h, color,
                                        point_list, angle_v <= M_FRR_PI_4);
    }
    point_list->size = 0;
  }
}
#endif

static inline uint32_t widget_get_standard_rounded_rect_radius(rect_t* r, uint32_t radius) {
  int32_t l = radius * 2;
  if (l > r->w || l <= 0) {
    radius = r->w / 2;
    l = r->w;
  }
  if (l > r->h) {
    radius = r->h / 2;
  }
  return radius;
}

static int32_t widget_get_rounded_rect_mid_length(uint32_t radius, uint32_t total_length,
                                                  float_t* l1, float_t* l2, float_t* angle,
                                                  bool_t is_add) {
  float_t a = M_FRR_PI_2;
  int32_t mid_lenght = 0;
  if (total_length > radius * 2) {
    *l1 = *l2 = (float_t)radius;
    mid_lenght = total_length - radius * 2;
  } else if (radius * 2 == total_length) {
    *l1 = *l2 = (float_t)radius;
    mid_lenght = 0;
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
    mid_lenght = 0;
  }
  *angle = a;
  return mid_lenght;
}

static void widget_draw_fill_rounded_rect(canvas_t* c, rect_t* r, color_t* color, uint32_t radius) {
  float_t w1 = 0.0f;
  float_t w2 = 0.0f;
  float_t h1 = 0.0f;
  float_t h2 = 0.0f;
  float_t angle_v = 0.0f;
  float_t angle_h = 0.0f;

  int32_t x = r->x;
  int32_t y = r->y;
  int32_t x1 = x + radius;
  int32_t y1 = y + radius;
  int32_t mid_lenght_v = 0;
  int32_t mid_lenght_h = 0;
  int32_t x2 = x + r->w - radius;
  int32_t y2 = y + r->h - radius;

  vgcanvas_t* vg = canvas_get_vgcanvas(c);

#ifndef WITH_NANOVG_GPU
  int32_t v_x1 = 0;
  int32_t v_y1 = 0;
  int32_t v_x2 = 0;
  int32_t v_y2 = 0;
  uint32_t size = 0;
  darray_t point_list_45 = {0};
  darray_t tmp_point_list_45 = {0};
  frr_image_info_t image_info = {0};
  frr_point_pix_t* point_pix_cache_list = NULL;
#endif

  mid_lenght_v = widget_get_rounded_rect_mid_length(radius, r->h, &w1, &h1, &angle_v, TRUE);
  mid_lenght_h = widget_get_rounded_rect_mid_length(radius, r->w, &h2, &w2, &angle_h, TRUE);

#ifdef WITH_NANOVG_GPU
  (void)w1;
  (void)w2;
  (void)h1;
  (void)h2;
  (void)mid_lenght_v;
  (void)mid_lenght_h;

  vgcanvas_save(vg);
  vgcanvas_set_fill_color(vg, *color);
  vgcanvas_translate(vg, (float_t)c->ox, (float_t)c->oy);
  vgcanvas_begin_path(vg);

  vgcanvas_arc(vg, (float_t)x1, (float_t)y1, radius, M_PI + M_FRR_PI_2 - angle_v, M_PI + angle_h,
               FALSE);  // 左上角
  vgcanvas_arc(vg, (float_t)x2, (float_t)y1, radius, M_PI + M_PI - angle_h,
               M_PI + M_FRR_PI_2 + angle_v,
               FALSE);  // 右上角
  vgcanvas_arc(vg, (float_t)x2, (float_t)y2, radius, M_FRR_PI_2 - angle_v, angle_h,
               FALSE);  // 右下角
  vgcanvas_arc(vg, (float_t)x1, (float_t)y2, radius, M_PI - angle_h, M_FRR_PI_2 + angle_v,
               FALSE);  // 左下角

  vgcanvas_close_path(vg);
  vgcanvas_fill(vg);

  vgcanvas_translate(vg, (float_t)-c->ox, (float_t)-c->oy);
  vgcanvas_restore(vg);
#else

  v_x1 = x + (int32_t)w2;
  v_x2 = x + (int32_t)w2 + mid_lenght_h;

  v_y1 = y + (int32_t)h1;
  v_y2 = y + (int32_t)h1 + mid_lenght_v;

  canvas_set_fill_color(c, *color);

  widget_image_info_create(&image_info, c);

  size = ANGLE2SIZE(M_FRR_PI_4, radius) + 1;
  if (size <= FRR_POINT_PIX_CACHE_SIZE) {
    point_pix_cache_list = frr_point_pix_cache_list;
  } else {
    point_pix_cache_list = TKMEM_ZALLOCN(frr_point_pix_t, size);
  }

  darray_init(&tmp_point_list_45, size, NULL, NULL);
  darray_init(&point_list_45, size, NULL, NULL);

  Wu_D_Circle(radius, &point_list_45, point_pix_cache_list);

  widget_draw_arc_point_list(&image_info, c, v_x2, v_y1, x2, y1, angle_h, angle_v, radius, 0, color,
                             &tmp_point_list_45, &point_list_45, TRUE);  // 右上角
  widget_draw_arc_point_list(&image_info, c, v_x1, v_y1, x1, y1, angle_h, angle_v, radius, 1, color,
                             &tmp_point_list_45, &point_list_45, TRUE);  // 左上角
  widget_draw_arc_point_list(&image_info, c, v_x1, v_y2, x1, y2, angle_h, angle_v, radius, 2, color,
                             &tmp_point_list_45, &point_list_45, TRUE);  // 左下角
  widget_draw_arc_point_list(&image_info, c, v_x2, v_y2, x2, y2, angle_h, angle_v, radius, 3, color,
                             &tmp_point_list_45, &point_list_45, TRUE);  // 右下角

  if (mid_lenght_v != 0) {
    canvas_fill_rect(c, x, v_y1, (wh_t)w2, mid_lenght_v);  //左边矩形
    if (mid_lenght_h == 0 && w2 + w2 > r->w) {
      canvas_fill_rect(c, v_x2, v_y1, (wh_t)(r->w - w2), mid_lenght_v);  //右边矩形
    } else {
      canvas_fill_rect(c, v_x2, v_y1, (wh_t)w2, mid_lenght_v);  //右边矩形
    }
  }

  if (mid_lenght_h != 0) {
    canvas_fill_rect(c, v_x1, y, mid_lenght_h, (wh_t)h1);  //上边矩形
    if (mid_lenght_v == 0 && h1 + h1 > r->h) {
      canvas_fill_rect(c, v_x1, v_y2, mid_lenght_h, (wh_t)(r->h - h1));  //下边矩形
    } else {
      canvas_fill_rect(c, v_x1, v_y2, mid_lenght_h, (wh_t)h1);  //下边矩形
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
#endif  // WITH_NANOVG_GPU
}

static void widget_draw_stroke_rounded_rect(canvas_t* c, rect_t* r, color_t* color, uint32_t radius,
                                            uint32_t border_width) {
  float_t w1 = 0.0f;
  float_t w2 = 0.0f;
  float_t h1 = 0.0f;
  float_t h2 = 0.0f;
  float_t angle_v = 0.0f;
  float_t angle_h = 0.0f;

  int32_t x = r->x;
  int32_t y = r->y;
  int32_t x1 = x + radius;
  int32_t y1 = y + radius;
  int32_t mid_lenght_v = 0;
  int32_t mid_lenght_h = 0;
  int32_t x2 = x + r->w - radius;
  int32_t y2 = y + r->h - radius;

#ifndef WITH_NANOVG_GPU
  uint32_t size = 0;

  int32_t v_x1 = 0;
  int32_t v_y1 = 0;
  int32_t v_x2 = 0;
  int32_t v_y2 = 0;

  darray_t point_list_45 = {0};
  darray_t tmp_point_list_45 = {0};

  frr_image_info_t image_info = {0};
  frr_point_pix_t* point_pix_cache_list = NULL;
#endif

  mid_lenght_v = widget_get_rounded_rect_mid_length(radius, r->h, &w1, &h1, &angle_v, TRUE);
  mid_lenght_h = widget_get_rounded_rect_mid_length(radius, r->w, &h2, &w2, &angle_h, TRUE);

#ifndef WITH_NANOVG_GPU
  if (border_width == 1) {
    v_x1 = x + (int32_t)w2;
    v_x2 = x + (int32_t)w2 + mid_lenght_h;
    v_y1 = y + (int32_t)h1;
    v_y2 = y + (int32_t)h1 + mid_lenght_v;

    canvas_set_stroke_color(c, *color);

    size = ANGLE2SIZE(M_FRR_PI_4, radius) + 1;
    if (size <= FRR_POINT_PIX_CACHE_SIZE) {
      point_pix_cache_list = frr_point_pix_cache_list;
    } else {
      point_pix_cache_list = TKMEM_ZALLOCN(frr_point_pix_t, size);
    }

    darray_init(&point_list_45, size, NULL, NULL);
    darray_init(&tmp_point_list_45, size, NULL, NULL);

    Wu_D_Circle(radius, &point_list_45, point_pix_cache_list);

    widget_image_info_create(&image_info, c);
    color->rgba.a = color->rgba.a * c->global_alpha / 0xff;

    widget_draw_arc_point_list(&image_info, c, v_x2, v_y1, x2, y1, angle_h, angle_v, radius, 0,
                               color, &tmp_point_list_45, &point_list_45, FALSE);  // 右上角
    widget_draw_arc_point_list(&image_info, c, v_x1, v_y1, x1, y1, angle_h, angle_v, radius, 1,
                               color, &tmp_point_list_45, &point_list_45, FALSE);  // 左上角
    widget_draw_arc_point_list(&image_info, c, v_x1, v_y2, x1, y2, angle_h, angle_v, radius, 2,
                               color, &tmp_point_list_45, &point_list_45, FALSE);  // 左下角
    widget_draw_arc_point_list(&image_info, c, v_x2, v_y2, x2, y2, angle_h, angle_v, radius, 3,
                               color, &tmp_point_list_45, &point_list_45, FALSE);  // 右下角

    if (mid_lenght_v != 0) {
      canvas_draw_vline(c, x, v_y1, mid_lenght_v);                      //左边边
      canvas_draw_vline(c, (xy_t)(v_x2 + w2 - 1), v_y1, mid_lenght_v);  //右边边
    }

    if (mid_lenght_h != 0) {
      canvas_draw_hline(c, v_x1, y, mid_lenght_h);                      //上边边
      canvas_draw_hline(c, v_x1, (xy_t)(v_y2 + h1 - 1), mid_lenght_h);  //下边边
    }

    ////设置直线内部边缘虚化，但是会增加性能消耗
    //if (mid_lenght_v != 0 || mid_lenght_h != 0)
    //{
    //	color->rgba.a *= 0.05f;
    //	canvas_set_stroke_color(c, *color);
    //	if (mid_lenght_v != 0) {
    //		canvas_draw_vline(c, (xy_t)(x + 1), (xy_t)v_y1, (wh_t)mid_lenght_v);//左边边
    //		canvas_draw_vline(c, (xy_t)(v_x2 + w2 - 2), (xy_t)v_y1, (wh_t)mid_lenght_v);//右边边
    //	}
    //	if (mid_lenght_h != 0) {
    //		canvas_draw_hline(c, (xy_t)v_x1, (xy_t)(y + 1), (wh_t)mid_lenght_h);//上边边
    //		canvas_draw_hline(c, (xy_t)v_x1, (xy_t)(v_y2 + h1 - 2), (wh_t)mid_lenght_h);//下边边
    //	}
    //}
  } else
#endif  // WITH_NANOVG_GPU
  {
    vgcanvas_t* vg = canvas_get_vgcanvas(c);
    if (vg != NULL) {
      (void)mid_lenght_v;
      (void)mid_lenght_h;
      vgcanvas_save(vg);

      vgcanvas_set_line_width(vg, (float_t)border_width);
      vgcanvas_set_stroke_color(vg, *color);

      vgcanvas_translate(vg, (float_t)c->ox, (float_t)c->oy);

      vgcanvas_begin_path(vg);
      WIDGET_SET_ROUNDED_RECT_VERTEX_STROKE(vg, x1, y1, radius, M_PI + angle_h,
                                            M_PI + M_FRR_PI_2 - angle_v,
                                            TRUE);  // 左上角
      WIDGET_SET_ROUNDED_RECT_VERTEX_STROKE(vg, x1, y2, radius, M_FRR_PI_2 + angle_v,
                                            M_PI - angle_h, TRUE);  // 左下角
      WIDGET_SET_ROUNDED_RECT_VERTEX_STROKE(vg, x2, y2, radius, angle_h, M_FRR_PI_2 - angle_v,
                                            TRUE);  // 右下角
      WIDGET_SET_ROUNDED_RECT_VERTEX_STROKE(vg, x2, y1, radius, M_PI + M_FRR_PI_2 + angle_v,
                                            M_PI + M_PI - angle_h,
                                            TRUE);  // 右上角
      vgcanvas_close_path(vg);

      vgcanvas_stroke(vg);
      vgcanvas_translate(vg, (float_t)-c->ox, (float_t)-c->oy);
      vgcanvas_restore(vg);
    } else {
      canvas_stroke_rect(c, 0, 0, r->w, r->h);
    }
  }

#ifndef WITH_NANOVG_GPU

  darray_deinit(&point_list_45);
  darray_deinit(&tmp_point_list_45);

  if (size > FRR_POINT_PIX_CACHE_SIZE && point_pix_cache_list != NULL) {
    TKMEM_FREE(point_pix_cache_list);
    point_pix_cache_list = NULL;
  }
#endif  // WITH_NANOVG_GPU
}

static inline ret_t widget_draw_fill_rounded_rect_ex(canvas_t* c, rect_t* r, rect_t* bg_r,
                                                     color_t* color, uint32_t radius) {
  xy_t x = r->x + c->ox;
  xy_t y = r->y + c->oy;
  xy_t x2 = x + r->w - 1;
  xy_t y2 = y + r->h - 1;

  if (x > c->clip_right || x2 < c->clip_left ||
      y > c->clip_bottom || y2 < c->clip_top) {
    return RET_OK;
  }
  
  if (bg_r == NULL) {
    radius = widget_get_standard_rounded_rect_radius(r, radius);
  } else {
    radius = widget_get_standard_rounded_rect_radius(bg_r, radius);
  }
  if (radius < 2) {
    return RET_FAIL;
  }

  color->rgba.a = color->rgba.a * c->global_alpha / 0xff;
  widget_draw_fill_rounded_rect(c, r, color, radius);
  return RET_OK;
}

static inline ret_t widget_draw_stroke_rounded_rect_ex(canvas_t* c, rect_t* r, rect_t* bg_r,
                                                       color_t* color, uint32_t radius,
                                                       uint32_t border_width) {
  xy_t x = r->x + c->ox;
  xy_t y = r->y + c->oy;
  xy_t x2 = x + r->w - 1;
  xy_t y2 = y + r->h - 1;

  if (x > c->clip_right || x2 < c->clip_left ||
      y > c->clip_bottom || y2 < c->clip_top) {
    return RET_OK;
  }

  if (bg_r == NULL) {
    radius = widget_get_standard_rounded_rect_radius(r, radius);
  } else {
    radius = widget_get_standard_rounded_rect_radius(bg_r, radius);
  }
  if (radius < 2) {
    return RET_FAIL;
  }
  widget_draw_stroke_rounded_rect(c, r, color, radius, border_width);
  return RET_OK;
}
