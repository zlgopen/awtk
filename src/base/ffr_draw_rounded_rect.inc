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

/*
把二维坐标的象限通过45度分为上下区域，一共8个区域
*/
typedef enum _ffr_quadrant_t {
  FFR_QUADRANT_FIRST_F = 0,  //二维坐标第一象限的上区域
  FFR_QUADRANT_FIRST_S,      //二维坐标第一象限的下区域

  FFR_QUADRANT_SECOND_F,  //二维坐标第二象限的上区域
  FFR_QUADRANT_SECOND_S,  //二维坐标第二象限的下区域

  FFR_QUADRANT_THIRD_F,  //二维坐标第三象限的上区域
  FFR_QUADRANT_THIRD_S,  //二维坐标第三象限的下区域

  FFR_QUADRANT_FOURTH_F,  //二维坐标第四象限的上区域
  FFR_QUADRANT_FOURTH_S,  //二维坐标第四象限的下区域
} ffr_quadrant_t;

typedef enum _ffr_vertex_type_t {
  FFR_VERTEXT_TYPE_TOP_REIGHT = 0,    //右上角
  FFR_VERTEXT_TYPE_TOP_LEFT,          //左上角
  FFR_VERTEXT_TYPE_BOTTOM_LEFT,       //左下角
  FFR_VERTEXT_TYPE_BOTTOM_REIGHT,     //右下角

  FFR_VERTEXT_TYPE_COUNT = FFR_VERTEXT_TYPE_BOTTOM_REIGHT + 1,
} ffr_vertex_type_t;

typedef struct _ffr_point_pix_t {
  int32_t x;
  int32_t y;
  float_t e;
} ffr_point_pix_t;

typedef struct _ffr_draw_info_t {
  int32_t x;
  int32_t y;
  uint32_t radius;
  float_t angle_v;
  float_t angle_h;
} ffr_draw_info_t;

typedef struct _ffr_draw_arc_info_table_t {
  ffr_quadrant_t quadrant_v;
  ffr_quadrant_t quadrant_h;
} ffr_draw_arc_info_table_t;

#ifdef M_PI_2
#define M_FFR_PI_2 (float_t)(M_PI_2)
#else
#define M_FFR_PI_2 (float_t)(M_PI / 2)
#endif /*M_FFR_PI_2*/

#ifdef M_PI_4
#define M_FFR_PI_4 (float_t)(M_PI_4)
#else
#define M_FFR_PI_4 (float_t)(M_PI / 4)
#endif /*M_FFR_PI_4*/

#ifndef FFR_POINT_PIX_CACHE_SIZE
#define FFR_POINT_PIX_CACHE_SIZE 256
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

#define FFR_SET_STANDARD_RADIUS(is_h, w, h, radius) {                   \
  if ((is_h && *radius > h)) {                                          \
    *radius = h;                                                        \
  } else if ((!is_h && *radius > w))  {                                 \
    *radius = w;                                                        \
  }                                                                     \
  *radius = *radius <= FFR_SUPPORT_MIN_RADIUS ? 0 : *radius;            \
}

static ffr_point_pix_t ffr_point_pix_cache_list[FFR_POINT_PIX_CACHE_SIZE];

const static ffr_draw_arc_info_table_t WIDGET_DRAW_ARC_INFO_TABLE[FFR_VERTEXT_TYPE_COUNT] = {
    {FFR_QUADRANT_FIRST_S, FFR_QUADRANT_FIRST_F},     //右上角
    {FFR_QUADRANT_SECOND_S, FFR_QUADRANT_SECOND_F},   //左上角
    {FFR_QUADRANT_THIRD_F, FFR_QUADRANT_THIRD_S},     //左下角
    {FFR_QUADRANT_FOURTH_F, FFR_QUADRANT_FOURTH_S}};  //右下角

static void ffr_draw_antialiasing_for_point(canvas_t* c, int px, int py, float_t e, color_t color) {
  color.rgba.a = (unsigned char)(e * color.rgba.a);
  canvas_set_stroke_color(c, color);
  canvas_draw_vline(c, px, py, 1);
}

static bool_t ffr_get_circluar_point_to_quadrant(int32_t xc, int32_t yc, int32_t px,
                                                           int32_t py, ffr_quadrant_t type,
                                                           int32_t* out_x, int32_t* out_y) {
  int32_t tmp_x = 0;
  int32_t tmp_y = 0;
  switch (type) {
    case FFR_QUADRANT_FIRST_F:
      tmp_x = xc + px;
      tmp_y = yc - py;
      break;
    case FFR_QUADRANT_FIRST_S:
      tmp_x = xc + py;
      tmp_y = yc - px;
      break;
    case FFR_QUADRANT_SECOND_F:
      tmp_x = xc - px;
      tmp_y = yc - py;
      break;
    case FFR_QUADRANT_SECOND_S:
      tmp_x = xc - py;
      tmp_y = yc - px;
      break;
    case FFR_QUADRANT_THIRD_F:
      tmp_x = xc - py;
      tmp_y = yc + px;
      break;
    case FFR_QUADRANT_THIRD_S:
      tmp_x = xc - px;
      tmp_y = yc + py;
      break;
    case FFR_QUADRANT_FOURTH_F:
      tmp_x = xc + py;
      tmp_y = yc + px;
      break;
    case FFR_QUADRANT_FOURTH_S:
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

static int32_t ffr_draw_circluar_point_to_quadrant(canvas_t* c,
                                                  int32_t x_h, int32_t y_v, int32_t x,
                                                  int32_t y, float_t e, color_t color, int32_t limit_y) {
  int32_t w = 0;
  int32_t h = 0;
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

  if (w > 0 && h > 0 && limit_y != y) {
    ffr_draw_antialiasing_for_point(c, x, y, e, color);
    return y;
  }
  return -1;
}

static int32_t ffr_draw_circluar_point_to_quadrant_hline(canvas_t* c, const rect_t* rect, int32_t x_h,
                                                      int32_t y_v, int32_t x, int32_t y,
                                                      float_t e, const gradient_t* gradient, int32_t limit_y, int32_t* last_y) {
  int32_t w = 0;
  int32_t h = 0;
  if (x_h > x) {
    w = x_h - x - 1;
    x_h = x + 1;
  } else {
    w = x - x_h - 1;
    x--;
  }

  if (y_v > y) {
    h = y_v - y;
    y_v = y;
  } else {
    h = y - y_v;
    y--;
  }

  if (w > 0 && h > 0 && limit_y != y) {
    float_t offset = (float_t)(y - rect->y) / (float_t)rect->h;
    color_t color = gradient_get_color((gradient_t*)gradient, offset);
    canvas_set_fill_color(c, color);
    if (*last_y != y) {
      canvas_fill_rect(c, x_h, y, w, 1);
    }
    ffr_draw_antialiasing_for_point(c, x, y, e, color);
    *last_y = y;
    return y;
  }
  return -1;
}

static void Wu_D_Circle(int32_t r, darray_t* point_list,
                               ffr_point_pix_t* point_pix_cache_list) {
  int32_t x = 0;
  int32_t y = r;
  uint32_t i = 0;
  float_t e = 0.0f;
  int32_t yy = y * y;
  ffr_point_pix_t* tmp = NULL;

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

static int32_t ffr_draw_stroke_arc_point_list(canvas_t* c, int32_t x_h,
                                              int32_t y_v, int32_t xc, int32_t yc,
                                              ffr_quadrant_t type, color_t color,
                                              darray_t* point_list, bool_t is_last_antialiasing, int32_t limit_y) {
  uint32_t i = 0;
  int32_t end_y = 0;
  void** elms = point_list->elms;
  for (i = 0; i < point_list->size; i++) {
    int32_t x = 0;
    int32_t y = 0;
    ffr_point_pix_t* iter = (ffr_point_pix_t*)elms[i];

    if (ffr_get_circluar_point_to_quadrant(xc, yc, iter->x, iter->y, type, &x, &y)) {
      int32_t tmp = 0;
      if (limit_y >= 0 && limit_y == y) {
        break;
      }
      tmp = ffr_draw_circluar_point_to_quadrant(c, x_h, y_v, x, y, 1 - iter->e, color, limit_y);

      if (tmp >= 0) {
        end_y = tmp;
      }

      if (point_list->size > i + 1 || is_last_antialiasing) {
        if (ffr_get_circluar_point_to_quadrant(xc, yc, iter->x, iter->y - 1, type, &x, &y)) {
          ffr_draw_circluar_point_to_quadrant(c, x_h, y_v, x, y, iter->e, color, limit_y);
        }
      }
    }
  }
  return end_y;
}

static int32_t ffr_draw_fill_arc_point_list(canvas_t* c, const rect_t* rect, int32_t x_h,
                                            int32_t y_v, int32_t xc, int32_t yc, ffr_quadrant_t type,
                                            const gradient_t* gradient, darray_t* point_list, int32_t limit_y) {
  uint32_t i = 0;
  int32_t end_y = 0;
  int32_t last_y = -1;
  void** elms = point_list->elms;
  for (i = 0; i < point_list->size; i++) {
    int32_t x = 0;
    int32_t y = 0;
    ffr_point_pix_t* iter = (ffr_point_pix_t*)elms[i];

    if (ffr_get_circluar_point_to_quadrant(xc, yc, iter->x, iter->y, type, &x, &y)) {
      int32_t tmp = 0;
      if (limit_y >= 0 && limit_y == y) {
        break;
      }
      tmp = ffr_draw_circluar_point_to_quadrant_hline(c, rect, x_h, y_v, x, y, 1 - iter->e, gradient, limit_y, &last_y);
      if (tmp >= 0) {
        end_y = tmp;
      }
    }
  }
  return end_y;
}

static void ffr_push_arc_point_list(float_t angle_1, float_t angle_2, int32_t r,
                                       darray_t* point_list, darray_t* point_list_45) {
  uint32_t i = 0;
  void** elms = point_list_45->elms;

  float_t x1 = sinf(angle_1) * r;
  float_t x2 = CMP_FLOAT_QE(angle_2, M_FFR_PI_4)
                   ? ((ffr_point_pix_t*)elms[point_list_45->size - 1])->x
                   : sinf(angle_2) * r;
  for (i = 0; i < point_list_45->size; i++) {
    ffr_point_pix_t* iter = (ffr_point_pix_t*)elms[i];
    if ((x1 < iter->x || CMP_FLOAT_QE(x1, iter->x)) &&
        (iter->x < x2 || CMP_FLOAT_QE(x2, iter->x))) {
      darray_push(point_list, iter);
    }
  }
}

static void ffr_draw_arc_point_list(canvas_t* c, const rect_t* rect, int32_t x_h,
                                       int32_t y_v, int32_t xc, int32_t yc, float_t angle_h,
                                       float_t angle_v, int32_t r, uint32_t quadrant_id,
                                       const gradient_t* gradient, darray_t* point_list,
                                       darray_t* point_list_45, bool_t is_fill) {
  int32_t end_y = -1;
  float_t angle_v_1 = 0.0f;
  float_t angle_h_1 = 0.0f;
  float_t angle_v_2 = M_FFR_PI_4;
  float_t angle_h_2 = M_FFR_PI_4;

  if ((angle_h > M_FFR_PI_2 && !CMP_FLOAT_QE(angle_h, M_FFR_PI_2)) ||
      (angle_v > M_FFR_PI_2 && !CMP_FLOAT_QE(angle_v, M_FFR_PI_2))) {
    return;
  }

  if (angle_h < M_FFR_PI_4) {
    angle_v_2 = angle_h;
  } else {
    angle_h_1 = M_FFR_PI_2 - angle_h;
  }

  if (angle_v < M_FFR_PI_4) {
    angle_h_2 = angle_v;
  } else {
    angle_v_1 = M_FFR_PI_2 - angle_v;
  }

  if (angle_v > M_FFR_PI_4) {
    ffr_push_arc_point_list(angle_v_1, angle_v_2, r, point_list, point_list_45);
    if (is_fill) {
      end_y = ffr_draw_fill_arc_point_list(c, rect, x_h, y_v, xc, yc,
                                          WIDGET_DRAW_ARC_INFO_TABLE[quadrant_id].quadrant_v, gradient,
                                          point_list, -1);
    } else {
      color_t color = gradient_get_first_color((gradient_t*)gradient);
      end_y = ffr_draw_stroke_arc_point_list(c, x_h, y_v, xc, yc,
                                            WIDGET_DRAW_ARC_INFO_TABLE[quadrant_id].quadrant_v, color,
                                            point_list, TRUE,  -1);
    }
    point_list->size = 0;
  }

  if (angle_h > M_FFR_PI_4) {
    ffr_push_arc_point_list(angle_h_1, angle_h_2, r, point_list, point_list_45);
    if (is_fill) {
      ffr_draw_fill_arc_point_list(c, rect, x_h, y_v, xc, yc,
                                      WIDGET_DRAW_ARC_INFO_TABLE[quadrant_id].quadrant_h, gradient,
                                      point_list, end_y);
    } else {
      color_t color = gradient_get_first_color((gradient_t*)gradient);
      ffr_draw_stroke_arc_point_list(c, x_h, y_v, xc, yc,
                                        WIDGET_DRAW_ARC_INFO_TABLE[quadrant_id].quadrant_h, color,
                                        point_list, angle_v <= M_FFR_PI_4, end_y);
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

static float_t ffr_draw_rounded_get_vertex_angle_by_border_model(int32_t border_model, ffr_vertex_type_t type, bool_t is_h, float_t default_angle) {

  int32_t num = (int32_t)type;
  const static border_type_t s_angle_type_to_border_model[] = { BORDER_TOP, BORDER_RIGHT, BORDER_TOP, BORDER_LEFT, BORDER_BOTTOM, BORDER_LEFT, BORDER_BOTTOM, BORDER_RIGHT};

  return_value_if_fail(type < FFR_VERTEXT_TYPE_COUNT, default_angle);
  
  num = num * 2 + (is_h ? 0 : 1);

  if (s_angle_type_to_border_model[num] & border_model) {
    return default_angle;
  } else {
    return tk_min(M_FFR_PI_4, default_angle);
  }
}

static void ffr_draw_rounded_rect_draw_fill_with_vg(canvas_t* c, vgcanvas_t* vg, const gradient_t* gradient, const rect_t* r, ffr_draw_info_t draw_infos[FFR_VERTEXT_TYPE_COUNT]) {

  vgcanvas_save(vg);
  vgcanvas_set_global_alpha(vg, c->global_alpha / 255.0f);
  if (gradient->nr > 1) {  
    vg_gradient_t vg_gradient;
    vg_gradient_init_with_gradient(&vg_gradient, r, gradient);
    vgcanvas_set_fill_gradient(vg, &vg_gradient);
  } else {
    color_t color = gradient_get_first_color((gradient_t*)gradient);
    vgcanvas_set_fill_color(vg, color);
  }
  vgcanvas_translate(vg, (float_t)c->ox, (float_t)c->oy);
  vgcanvas_begin_path(vg);

  if (draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].radius > 0) { // 右上角
    vgcanvas_arc(vg, draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].x, draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].y, draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].radius, 
                  M_PI + M_FFR_PI_2 + draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].angle_v, M_PI + M_PI - draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].angle_h, TRUE);
  } else {
    vgcanvas_move_to(vg, draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].x, draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].y);
  }

  if (draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].radius > 0) { // 左上角
    vgcanvas_arc(vg, draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].x, draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].y, draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].radius, 
                M_PI + draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].angle_h, M_PI + M_FFR_PI_2 - draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].angle_v, TRUE); 
  } else {
    vgcanvas_line_to(vg, draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].x, draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].y);
  }

  if (draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].radius > 0) { // 左下角
    vgcanvas_arc(vg, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].x, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].y, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].radius, 
                M_FFR_PI_2 + draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].angle_v, M_PI - draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].angle_h, TRUE); 
  } else {
    vgcanvas_line_to(vg, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].x, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].y);
  }

  if (draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].radius > 0) { // 右下角
    vgcanvas_arc(vg, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].x, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].y, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].radius, 
                draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].angle_h, M_FFR_PI_2 - draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].angle_v, TRUE); 
  } else {
    vgcanvas_line_to(vg, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].x, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].y);
  }

  vgcanvas_close_path(vg);
  vgcanvas_fill(vg);

  vgcanvas_translate(vg, (float_t)-c->ox, (float_t)-c->oy);
  vgcanvas_restore(vg);
}

static ffr_vertex_type_t ffr_draw_rounded_rect_get_stroke_start_vertext(int32_t border_model) {
  ffr_vertex_type_t start_vertext = FFR_VERTEXT_TYPE_TOP_REIGHT;

  if (border_model == BORDER_TOP) {
    start_vertext = FFR_VERTEXT_TYPE_TOP_REIGHT;
  } else if (border_model == BORDER_BOTTOM) {
    start_vertext = FFR_VERTEXT_TYPE_BOTTOM_LEFT;
  } else if (border_model == BORDER_RIGHT) {
    start_vertext = FFR_VERTEXT_TYPE_BOTTOM_REIGHT;
  } else if (border_model == BORDER_LEFT) {
    start_vertext = FFR_VERTEXT_TYPE_TOP_LEFT;
  } else if (border_model == (BORDER_TOP | BORDER_LEFT | BORDER_BOTTOM)) {
    start_vertext = FFR_VERTEXT_TYPE_TOP_REIGHT;
  } else if (border_model == (BORDER_LEFT | BORDER_BOTTOM | BORDER_RIGHT)) {
    start_vertext = FFR_VERTEXT_TYPE_TOP_LEFT;
  } else if (border_model == (BORDER_BOTTOM | BORDER_RIGHT | BORDER_TOP)) {
    start_vertext = FFR_VERTEXT_TYPE_BOTTOM_LEFT;
  } else if (border_model == (BORDER_RIGHT | BORDER_TOP | BORDER_LEFT)) {
    start_vertext = FFR_VERTEXT_TYPE_BOTTOM_REIGHT;
  } else if (border_model == (BORDER_TOP | BORDER_LEFT)) {
    start_vertext = FFR_VERTEXT_TYPE_TOP_REIGHT;
  } else if (border_model == (BORDER_LEFT | BORDER_BOTTOM)) {
    start_vertext = FFR_VERTEXT_TYPE_TOP_LEFT;
  } else if (border_model == (BORDER_BOTTOM | BORDER_RIGHT)) {
    start_vertext = FFR_VERTEXT_TYPE_BOTTOM_LEFT;
  } else if (border_model == (BORDER_RIGHT | BORDER_TOP)) {
    start_vertext = FFR_VERTEXT_TYPE_BOTTOM_REIGHT;
  } else if (border_model == (BORDER_RIGHT | BORDER_LEFT)) {
    start_vertext = FFR_VERTEXT_TYPE_BOTTOM_REIGHT;
  }

  return start_vertext;
}

static void ffr_draw_rounded_rect_draw_stroke_top_right_with_vg(vgcanvas_t* vg, ffr_draw_info_t draw_infos[FFR_VERTEXT_TYPE_COUNT], int32_t border_model, bool_t start) {
  if (draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].radius > 0) { // 右上角
    if ((border_model & BORDER_TOP) != 0 || (border_model & BORDER_RIGHT) != 0) {
      float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_TOP_REIGHT, TRUE, draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].angle_h);
      float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_TOP_REIGHT, FALSE, draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].angle_v);
      
      if ((border_model & BORDER_RIGHT) == 0 && M_FFR_PI_4 < draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].angle_v) {
        float_t d = FFR_DRAW_SIN_45 * draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].radius;
        float_t move_x = draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].x + d;
        float_t move_y = draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].y - d;
        vgcanvas_move_to(vg, move_x, move_y);
      }
      vgcanvas_arc(vg, draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].x, draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].y, draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].radius, 
                    M_PI + M_FFR_PI_2 + tmp_angle_v, M_PI + M_PI - tmp_angle_h, TRUE);
    }
  } else {
    if (!start && (border_model & BORDER_RIGHT) != 0) {
      vgcanvas_line_to(vg, draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].x, draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].y);
    } else {
      vgcanvas_move_to(vg, draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].x, draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].y);
    }
  }
}

static void ffr_draw_rounded_rect_draw_stroke_top_left_with_vg(vgcanvas_t* vg, ffr_draw_info_t draw_infos[FFR_VERTEXT_TYPE_COUNT], int32_t border_model, bool_t start) {
  if (draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].radius > 0) { // 左上角
    if ((border_model & BORDER_TOP) != 0 || (border_model & BORDER_LEFT) != 0) {
      float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_TOP_LEFT, TRUE, draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].angle_h);
      float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_TOP_LEFT, FALSE, draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].angle_v);
      
      if ((border_model & BORDER_TOP) == 0 && M_FFR_PI_4 < draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].angle_v) {
        float_t d = FFR_DRAW_SIN_45 * draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].radius;
        float_t move_x = draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].x - d;
        float_t move_y = draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].y - d;
        vgcanvas_move_to(vg, move_x, move_y);
      }
      vgcanvas_arc(vg, draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].x, draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].y, draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].radius, 
                  M_PI + tmp_angle_h, M_PI + M_FFR_PI_2 - tmp_angle_v, TRUE); 
    }
  } else {
    if (!start && (border_model & BORDER_TOP) != 0) {
      vgcanvas_line_to(vg, draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].x, draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].y);
    } else {
      vgcanvas_move_to(vg, draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].x, draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].y);
    }
  }
}

static void ffr_draw_rounded_rect_draw_stroke_bottom_left_with_vg(vgcanvas_t* vg, ffr_draw_info_t draw_infos[FFR_VERTEXT_TYPE_COUNT], int32_t border_model, bool_t start) {
  if (draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].radius > 0) { // 左下角
    if ((border_model & BORDER_BOTTOM) != 0 || (border_model & BORDER_LEFT) != 0) {
      float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_BOTTOM_LEFT, TRUE, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].angle_h);
      float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_BOTTOM_LEFT, FALSE, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].angle_v);

      if ((border_model & BORDER_LEFT) == 0 && M_FFR_PI_4 < draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].angle_h) {
        float_t d = FFR_DRAW_SIN_45 * draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].radius;
        float_t move_x = draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].x - d;
        float_t move_y = draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].y + d;
        vgcanvas_move_to(vg, move_x, move_y);
      }
      vgcanvas_arc(vg, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].x, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].y, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].radius, 
                  M_FFR_PI_2 + tmp_angle_v, M_PI - tmp_angle_h, TRUE); 
    }
  } else {
    if (!start && (border_model & BORDER_LEFT) != 0) {
      vgcanvas_line_to(vg, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].x, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].y);
    } else {
      vgcanvas_move_to(vg, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].x, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].y);
    }
  }
}

static void ffr_draw_rounded_rect_draw_stroke_bottom_right_with_vg(vgcanvas_t* vg, ffr_draw_info_t draw_infos[FFR_VERTEXT_TYPE_COUNT], int32_t border_model, bool_t start) {
  if (draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].radius > 0) { // 右下角
    if ((border_model & BORDER_BOTTOM) != 0 || (border_model & BORDER_RIGHT) != 0) {
      float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_BOTTOM_REIGHT, TRUE, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].angle_h);
      float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_BOTTOM_REIGHT, FALSE, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].angle_v);

      if ((border_model & BORDER_BOTTOM) == 0 && M_FFR_PI_4 < draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].angle_h) {
        float_t d = FFR_DRAW_SIN_45 * draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].radius;
        float_t move_x = draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].x + d;
        float_t move_y = draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].y + d;
        vgcanvas_move_to(vg, move_x, move_y);
      }
      vgcanvas_arc(vg, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].x, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].y, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].radius, 
                  tmp_angle_h, M_FFR_PI_2 - tmp_angle_v, TRUE); 
    }
  } else {
    if (!start && (border_model & BORDER_BOTTOM) != 0) {
      vgcanvas_line_to(vg, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].x, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].y);
    } else {
      vgcanvas_move_to(vg, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].x, draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].y);
    }
  }
}

static void ffr_draw_rounded_rect_draw_stroke_with_vg(canvas_t* c, vgcanvas_t* vg, uint32_t border_width, color_t color, ffr_draw_info_t draw_infos[FFR_VERTEXT_TYPE_COUNT], int32_t border_model) {
  uint32_t i = 0;
  uint32_t n = 0;
  ffr_vertex_type_t start_vertext = ffr_draw_rounded_rect_get_stroke_start_vertext(border_model);
  vgcanvas_save(vg);

  vgcanvas_set_stroke_color(vg, color);
  vgcanvas_set_line_width(vg, (float_t)border_width);

  vgcanvas_translate(vg, (float_t)c->ox, (float_t)c->oy);

  vgcanvas_begin_path(vg);
  
  i = start_vertext;

  for (; n < FFR_VERTEXT_TYPE_COUNT; i++, n++) {
    ffr_vertex_type_t vertext = (ffr_vertex_type_t)(i % FFR_VERTEXT_TYPE_COUNT);
    switch (vertext)
    {
    case FFR_VERTEXT_TYPE_TOP_REIGHT :
      ffr_draw_rounded_rect_draw_stroke_top_right_with_vg(vg, draw_infos, border_model, start_vertext == FFR_VERTEXT_TYPE_TOP_REIGHT);
      break;
    case FFR_VERTEXT_TYPE_TOP_LEFT :
      ffr_draw_rounded_rect_draw_stroke_top_left_with_vg(vg, draw_infos, border_model, start_vertext == FFR_VERTEXT_TYPE_TOP_LEFT);
      break;
    case FFR_VERTEXT_TYPE_BOTTOM_REIGHT :
      ffr_draw_rounded_rect_draw_stroke_bottom_right_with_vg(vg, draw_infos, border_model, start_vertext == FFR_VERTEXT_TYPE_BOTTOM_REIGHT);
      break;
    case FFR_VERTEXT_TYPE_BOTTOM_LEFT :
      ffr_draw_rounded_rect_draw_stroke_bottom_left_with_vg(vg, draw_infos, border_model, start_vertext == FFR_VERTEXT_TYPE_BOTTOM_LEFT);
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

static void ffr_get_standard_rounded_rect_radius(const rect_t* r, uint32_t* radius_tl, uint32_t* radius_tr, 
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
  float_t a = M_FFR_PI_2;
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

static void ffr_draw_rounded_rect_draw_info_init(ffr_draw_info_t draw_infos[FFR_VERTEXT_TYPE_COUNT], const rect_t* r, uint32_t radius_tl, uint32_t radius_tr, uint32_t radius_bl, uint32_t radius_br) {
  int32_t i = 0;
  int32_t j = 0;
  int32_t x = r->x;
  int32_t y = r->y;
  float_t w1 = 0.0f;
  float_t w2 = 0.0f;
  float_t h1 = 0.0f;
  float_t h2 = 0.0f;
  uint32_t radius_list[FFR_VERTEXT_TYPE_COUNT] = {radius_tr, radius_tl, radius_bl, radius_br};

  memset(draw_infos, 0x0, sizeof(ffr_draw_info_t) * FFR_VERTEXT_TYPE_COUNT);

  draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].x = x + r->w - radius_list[FFR_VERTEXT_TYPE_TOP_REIGHT];
  draw_infos[FFR_VERTEXT_TYPE_TOP_REIGHT].y = y + radius_list[FFR_VERTEXT_TYPE_TOP_REIGHT];

  draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].x = x + radius_list[FFR_VERTEXT_TYPE_TOP_LEFT];
  draw_infos[FFR_VERTEXT_TYPE_TOP_LEFT].y = y + radius_list[FFR_VERTEXT_TYPE_TOP_LEFT];

  draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].x = x + radius_list[FFR_VERTEXT_TYPE_BOTTOM_LEFT];
  draw_infos[FFR_VERTEXT_TYPE_BOTTOM_LEFT].y = y + r->h - radius_list[FFR_VERTEXT_TYPE_BOTTOM_LEFT];

  draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].x = x + r->w - radius_list[FFR_VERTEXT_TYPE_BOTTOM_REIGHT];
  draw_infos[FFR_VERTEXT_TYPE_BOTTOM_REIGHT].y = y + r->h - radius_list[FFR_VERTEXT_TYPE_BOTTOM_REIGHT];

  for (i = 0; i < FFR_VERTEXT_TYPE_COUNT; i++) {
    uint32_t radius = radius_list[i];
    if (radius == 0) {
      continue;
    }
    draw_infos[i].radius = radius;
    ffr_get_rounded_rect_mid_length(radius, r->h, &w1, &h1, &(draw_infos[i].angle_v), TRUE);
    ffr_get_rounded_rect_mid_length(radius, r->w, &h2, &w2, &(draw_infos[i].angle_h), TRUE);

    for(j = i; j < FFR_VERTEXT_TYPE_COUNT; j++) {
      if (i != j && radius_list[j] == radius) {
        draw_infos[j].radius = radius;
        draw_infos[j].angle_v = draw_infos[i].angle_v;
        draw_infos[j].angle_h = draw_infos[i].angle_h;
      }
    }
  }
}

static void ffr_draw_rounded_rect_draw_fill_gradient(canvas_t* c, const rect_t* rect, xy_t x, xy_t y, wh_t w, wh_t h, const gradient_t* gradient) {
  uint32_t i = 0;
  for (i = 0; i < h; i++) {
    float_t offset = (float_t)(y + i - rect->y) / (float_t)rect->h;
    color_t color = gradient_get_color((gradient_t*)gradient, offset);
    canvas_set_stroke_color(c, color);
    canvas_draw_hline(c, x, y + i, w);
  }
}

static void ffr_draw_rounded_rect_draw_fill_with_canvas(canvas_t* c, const rect_t* r, const gradient_t* gradient, uint32_t radius, uint32_t radius_tl, uint32_t radius_tr, uint32_t radius_bl, uint32_t radius_br) {
  float_t w1 = 0.0f;
  float_t w2 = 0.0f;
  float_t h1 = 0.0f;
  float_t h2 = 0.0f;
  float_t angle_v = 0.0f;
  float_t angle_h = 0.0f;

  int32_t mid_lenght_v = ffr_get_rounded_rect_mid_length(radius, r->h, &w1, &h1, &angle_v, TRUE);
  int32_t mid_lenght_h = ffr_get_rounded_rect_mid_length(radius, r->w, &h2, &w2, &angle_h, TRUE);

  int32_t x = r->x;
  int32_t y = r->y;
  int32_t x1 = x + radius;
  int32_t y1 = y + radius;
  int32_t x2 = x + r->w - radius;
  int32_t y2 = y + r->h - radius;
  int32_t v_x1 = x + (int32_t)w2;
  int32_t v_y1 = y + (int32_t)h1;
  int32_t v_x2 = x + (int32_t)w2 + mid_lenght_h;
  int32_t v_y2 = y + (int32_t)h1 + mid_lenght_v;
  
  darray_t point_list_45 = {0};
  darray_t tmp_point_list_45 = {0};
  ffr_point_pix_t* point_pix_cache_list = NULL;

  uint32_t size = ANGLE2SIZE(M_FFR_PI_4, radius) * 1.5f;
  if (size <= FFR_POINT_PIX_CACHE_SIZE) {
    point_pix_cache_list = ffr_point_pix_cache_list;
  } else {
    point_pix_cache_list = TKMEM_ZALLOCN(ffr_point_pix_t, size);
  }

  darray_init(&tmp_point_list_45, size, NULL, NULL);
  darray_init(&point_list_45, size, NULL, NULL);

  Wu_D_Circle(radius, &point_list_45, point_pix_cache_list);

  if (radius_tr > 0) {
    ffr_draw_arc_point_list(c, r, v_x2, v_y1, x2, y1, angle_h, angle_v, radius, FFR_VERTEXT_TYPE_TOP_REIGHT, gradient,
                            &tmp_point_list_45, &point_list_45, TRUE);  // 右上角
  }

  if (radius_tl > 0) {
    ffr_draw_arc_point_list(c, r, v_x1, v_y1, x1, y1, angle_h, angle_v, radius, FFR_VERTEXT_TYPE_TOP_LEFT, gradient,
                            &tmp_point_list_45, &point_list_45, TRUE);  // 左上角
  }

  if (radius_bl > 0) {
    ffr_draw_arc_point_list(c, r, v_x1, v_y2, x1, y2, angle_h, angle_v, radius, FFR_VERTEXT_TYPE_BOTTOM_LEFT, gradient,
                            &tmp_point_list_45, &point_list_45, TRUE);  // 左下角
  }

  if (radius_br > 0) {
    ffr_draw_arc_point_list(c, r, v_x2, v_y2, x2, y2, angle_h, angle_v, radius, FFR_VERTEXT_TYPE_BOTTOM_REIGHT, gradient,
                            &tmp_point_list_45, &point_list_45, TRUE);  // 右下角
  }

  if (mid_lenght_v != 0) {
    ffr_draw_rounded_rect_draw_fill_gradient(c, r, x, v_y1, (wh_t)w2, mid_lenght_v, gradient);                  //左边矩形
    if (mid_lenght_h == 0 && w2 + w2 > r->w) {
      ffr_draw_rounded_rect_draw_fill_gradient(c, r, v_x2, v_y1, (wh_t)(r->w - w2), mid_lenght_v, gradient);    //右边矩形
    } else {
      ffr_draw_rounded_rect_draw_fill_gradient(c, r, v_x2, v_y1, (wh_t)w2, mid_lenght_v, gradient);             //右边矩形
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

    ffr_draw_rounded_rect_draw_fill_gradient(c, r, tmp_x, y, tmp_w, (wh_t)h1, gradient);                         //上边矩形

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
      ffr_draw_rounded_rect_draw_fill_gradient(c, r, tmp_x, v_y2, tmp_w, (wh_t)(r->h - h1), gradient);            //下边矩形
    } else {
      ffr_draw_rounded_rect_draw_fill_gradient(c, r, tmp_x, v_y2, tmp_w, (wh_t)h1, gradient);                     //下边矩形
    }
  }

  if (mid_lenght_v != 0 && mid_lenght_h != 0) {
    ffr_draw_rounded_rect_draw_fill_gradient(c, r, v_x1, v_y1, (wh_t)mid_lenght_h, (wh_t)mid_lenght_v, gradient);  //中间矩形
  }

  darray_deinit(&point_list_45);
  darray_deinit(&tmp_point_list_45);

  if (size > FFR_POINT_PIX_CACHE_SIZE && point_pix_cache_list != NULL) {
    TKMEM_FREE(point_pix_cache_list);
    point_pix_cache_list = NULL;
  }
}

static ret_t ffr_draw_fill_rounded_rect(canvas_t* c, const rect_t* r, const gradient_t* gradient, uint32_t radius_tl, uint32_t radius_tr, uint32_t radius_bl, uint32_t radius_br) {
  lcd_type_t lcd_type = lcd_get_type(c->lcd);
  bool_t is_cpu_lcd = lcd_type != LCD_VGCANVAS && lcd_type != LCD_REGISTER;
  uint32_t radius = radius_tl != 0 ? radius_tl : (radius_tr != 0 ? radius_tr : (radius_bl != 0 ? radius_bl : radius_br));
  if (is_cpu_lcd && radius != 0 && (gradient->nr == 1 || (gradient->nr == 2 && gradient->degree == 180)) && ffr_draw_rounded_radius_equal(radius_tl, radius_tr, radius_bl, radius_br)) {
    ffr_draw_rounded_rect_draw_fill_with_canvas(c, r, gradient, radius, radius_tl, radius_tr, radius_bl, radius_br);
  } else {  
    vgcanvas_t* vg = canvas_get_vgcanvas(c);
    if (radius != 0 && vg != NULL) {
#if defined(AWTK_WEB)
      rect_t rect = {r->x, r->y, r->w, r->h};
#else
      rect_t rect = {r->x + c->ox, r->y + c->oy, r->w, r->h};
#endif
      ffr_draw_info_t draw_infos[FFR_VERTEXT_TYPE_COUNT];
      memset(draw_infos, 0x0, sizeof(draw_infos));
      ffr_draw_rounded_rect_draw_info_init(draw_infos, r, radius_tl, radius_tr, radius_bl, radius_br);
      ffr_draw_rounded_rect_draw_fill_with_vg(c, vg, gradient, &rect, draw_infos);
    } else {
      return RET_FAIL;
    }
  }
  return RET_OK;
}

static void ffr_draw_rounded_rect_draw_stroke_with_canvas(canvas_t* c, const rect_t* r, color_t color, uint32_t radius, uint32_t radius_tl, uint32_t radius_tr, uint32_t radius_bl, uint32_t radius_br,
                                            uint32_t border_width, int32_t border_model) {
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
  int32_t x2 = x + r->w - radius;
  int32_t y2 = y + r->h - radius;
  int32_t mid_lenght_v = ffr_get_rounded_rect_mid_length(radius, r->h, &w1, &h1, &angle_v, TRUE);
  int32_t mid_lenght_h = ffr_get_rounded_rect_mid_length(radius, r->w, &h2, &w2, &angle_h, TRUE);

  int32_t v_x1 = x + (int32_t)w2;
  int32_t v_x2 = x + (int32_t)w2 + mid_lenght_h;
  int32_t v_y1 = y + (int32_t)h1;
  int32_t v_y2 = y + (int32_t)h1 + mid_lenght_v;

  gradient_t gradient;
  darray_t point_list_45 = {0};
  darray_t tmp_point_list_45 = {0};

  ffr_point_pix_t* point_pix_cache_list = NULL;
  uint32_t size = ANGLE2SIZE(M_FFR_PI_4, radius) + 1;

  gradient_init_simple(&gradient, color.color);
  if (size <= FFR_POINT_PIX_CACHE_SIZE) {
    point_pix_cache_list = ffr_point_pix_cache_list;
  } else {
    point_pix_cache_list = TKMEM_ZALLOCN(ffr_point_pix_t, size);
  }

  darray_init(&point_list_45, size, NULL, NULL);
  darray_init(&tmp_point_list_45, size, NULL, NULL);

  Wu_D_Circle(radius, &point_list_45, point_pix_cache_list);

  canvas_set_stroke_color(c, color);

  if (radius_tr > 0) {
    float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_TOP_REIGHT, TRUE, angle_h);
    float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_TOP_REIGHT, FALSE, angle_v);
    ffr_draw_arc_point_list(c, r, v_x2, v_y1, x2, y1, tmp_angle_h, tmp_angle_v, radius, FFR_VERTEXT_TYPE_TOP_REIGHT,
                              &gradient, &tmp_point_list_45, &point_list_45, FALSE);  // 右上角
  }
  if (radius_tl > 0) {
    float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_TOP_LEFT, TRUE, angle_h);
    float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_TOP_LEFT, FALSE, angle_v);
    ffr_draw_arc_point_list(c, r, v_x1, v_y1, x1, y1, tmp_angle_h, tmp_angle_v, radius, FFR_VERTEXT_TYPE_TOP_LEFT,
                                 &gradient, &tmp_point_list_45, &point_list_45, FALSE);  // 左上角
  }
  if (radius_bl > 0) {
    float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_BOTTOM_LEFT, TRUE, angle_h);
    float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_BOTTOM_LEFT, FALSE, angle_v);
    ffr_draw_arc_point_list(c, r, v_x1, v_y2, x1, y2, tmp_angle_h, tmp_angle_v, radius, FFR_VERTEXT_TYPE_BOTTOM_LEFT,
                               &gradient, &tmp_point_list_45, &point_list_45, FALSE);  // 左下角
  }

  if (radius_br > 0) {
    float_t tmp_angle_h = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_BOTTOM_REIGHT, TRUE, angle_h);
    float_t tmp_angle_v = ffr_draw_rounded_get_vertex_angle_by_border_model(border_model, FFR_VERTEXT_TYPE_BOTTOM_REIGHT, FALSE, angle_v);
    ffr_draw_arc_point_list(c, r, v_x2, v_y2, x2, y2, tmp_angle_h, tmp_angle_v, radius, FFR_VERTEXT_TYPE_BOTTOM_REIGHT,
                               &gradient, &tmp_point_list_45, &point_list_45, FALSE);  // 右下角
  }

  canvas_set_stroke_color(c, color);
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

  if (size > FFR_POINT_PIX_CACHE_SIZE && point_pix_cache_list != NULL) {
    TKMEM_FREE(point_pix_cache_list);
    point_pix_cache_list = NULL;
  }
}

static ret_t ffr_draw_stroke_rounded_rect(canvas_t* c, const rect_t* r, color_t color, uint32_t radius_tl, uint32_t radius_tr, uint32_t radius_bl, uint32_t radius_br,
                                            uint32_t border_width, int32_t border_model) {
  lcd_type_t lcd_type = lcd_get_type(c->lcd);
  bool_t is_cpu_lcd = lcd_type != LCD_VGCANVAS && lcd_type != LCD_REGISTER;
  uint32_t radius = radius_tl != 0 ? radius_tl : (radius_tr != 0 ? radius_tr : (radius_bl != 0 ? radius_bl : radius_br));
  if (is_cpu_lcd && border_width == 1 && radius != 0 && ffr_draw_rounded_radius_equal(radius_tl, radius_tr, radius_bl, radius_br)) {
    ffr_draw_rounded_rect_draw_stroke_with_canvas(c, r, color, radius, radius_tl, radius_tr, radius_bl, radius_br, border_width, border_model);
  } else {
    vgcanvas_t* vg = canvas_get_vgcanvas(c);
    if (radius != 0 && vg != NULL) {
      ffr_draw_info_t draw_infos[FFR_VERTEXT_TYPE_COUNT];
      memset(draw_infos, 0x0, sizeof(draw_infos));
      ffr_draw_rounded_rect_draw_info_init(draw_infos, r, radius_tl, radius_tr, radius_bl, radius_br);

      ffr_draw_rounded_rect_draw_stroke_with_vg(c, vg, border_width, color, draw_infos, border_model);
    } else {
      return RET_FAIL;
    }
  }
  return RET_OK;
}

static ret_t ffr_draw_fill_rounded_rect_ex(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                                                  const gradient_t* gradient, uint32_t radius_tl, uint32_t radius_tr, 
                                                  uint32_t radius_bl, uint32_t radius_br) {
  uint32_t radius = 0;
  xy_t x = r->x + c->ox;
  xy_t y = r->y + c->oy;
  xy_t x2 = x + r->w - 1;
  xy_t y2 = y + r->h - 1;

  return_value_if_fail(c != NULL && r != NULL && gradient != NULL, RET_BAD_PARAMS);
  if (!canvas_is_rect_in_clip_rect(c, x, y, x2, y2)) {
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

  return ffr_draw_fill_rounded_rect(c, r, gradient, radius_tl, radius_tr, radius_bl, radius_br);
}

static ret_t ffr_draw_stroke_rounded_rect_ex(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                                                    const color_t* color, uint32_t radius_tl, uint32_t radius_tr, 
                                                    uint32_t radius_bl, uint32_t radius_br, uint32_t border_width, int32_t border_model) {
  color_t tc;
  uint32_t radius = 0;
  xy_t x = r->x + c->ox;
  xy_t y = r->y + c->oy;
  xy_t x2 = x + r->w - 1;
  xy_t y2 = y + r->h - 1;
  return_value_if_fail(c != NULL && r != NULL && color != NULL, RET_BAD_PARAMS);
  tc = *color;
  if (!canvas_is_rect_in_clip_rect(c, x, y, x2, y2) || border_model == BORDER_NONE) {
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
  if (border_model == (BORDER_LEFT | BORDER_RIGHT | BORDER_TOP | BORDER_BOTTOM)) {
    border_model = BORDER_ALL;
  }
  return ffr_draw_stroke_rounded_rect(c, r, tc, radius_tl, radius_tr, radius_bl, radius_br, border_width, border_model);
}

