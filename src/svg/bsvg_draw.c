/**
 * File:   bsvg_draw.c
 * Author: AWTK Develop Team
 * Brief:  bsvg_draw
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-11-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "svg/bsvg_draw.h"

ret_t bsvg_draw_path(bsvg_draw_ctx_t* ctx, const svg_path_t* path) {
  vgcanvas_t* canvas = ctx->canvas;

  switch (path->type) {
    case SVG_PATH_M: {
      const svg_path_move_t* p = (const svg_path_move_t*)path;
      vgcanvas_move_to(canvas, p->x, p->y);
      /* 屏蔽了 vg 的路径镂空函数，修复 gles 和 agge 的 svg 效果同步的问题，会导致 svg 没有了路径消除的逻辑 */
      /* vgcanvas_path_winding(canvas, 0); */
      break;
    }
    case SVG_PATH_L: {
      const svg_path_line_t* p = (const svg_path_line_t*)path;
      vgcanvas_line_to(canvas, p->x, p->y);
      break;
    }
    case SVG_PATH_C: {
      const svg_path_curve_to_t* p = (const svg_path_curve_to_t*)path;
      vgcanvas_bezier_to(canvas, p->x1, p->y1, p->x2, p->y2, p->x, p->y);
      break;
    }
    case SVG_PATH_Z: {
      vgcanvas_close_path(canvas);
      break;
    }
    case SVG_PATH_NULL:
    default: {
      break;
    }
  }

  return RET_OK;
}

ret_t bsvg_draw_shape_end(bsvg_draw_ctx_t* ctx, const svg_shape_t* shape) {
  vgcanvas_t* canvas = ctx->canvas;

  if (shape->stroke_width > 0.0f) {
    vgcanvas_set_line_width(canvas, shape->stroke_width);
  }

  if (shape->fill_type == SVG_COLOR_SOLID) {
    color_t solid = svg_shape_get_fill(shape)->solid;
    vgcanvas_set_fill_color(canvas, solid);
    if (shape->type == SVG_SHAPE_TEXT) {
      /* canvas_set_text_color(c, solid); */
    }
  } else if (shape->fill_type == SVG_COLOR_LINEAR_GRADIENT) {
    const svg_color_linear_gradient_t* gradient = &(svg_shape_get_fill(shape)->linear_gradient);
    vgcanvas_set_fill_linear_gradient(canvas, gradient->sx, gradient->sy, gradient->ex,
                                      gradient->ey, gradient->icolor, gradient->ocolor);
    if (shape->type == SVG_SHAPE_TEXT) {
      /* canvas_set_text_color(c, gradient->ocolor); */
    }
  }
  if (shape->fill_type != SVG_COLOR_NULL) {
    vgcanvas_fill(canvas);
    if (shape->type == SVG_SHAPE_TEXT) {
      /**
       * 暂时不启用文本，原因如下：
       * 用canvas的话，无法匹配svg_image的旋转缩放等效果；用vgcanvas的话，agge暂不支持
       * svgtiny库解析不支持字体大小，实际用途很小
       */

      /* const svg_shape_text_t* s = (const svg_shape_text_t*)shape;  */
      /* const char* text = svg_shape_text_get_text(shape);           */
      /* canvas_set_font(c, NULL, TK_DEFAULT_FONT_SIZE);              */
      /* canvas_set_text_align(c, ALIGN_H_CENTER, ALIGN_V_MIDDLE);    */
      /* canvas_draw_utf8(c, text, s->x, s->y);                       */
    }
  }

  if (shape->stroke_type == SVG_COLOR_SOLID) {
    vgcanvas_set_stroke_color(canvas, svg_shape_get_stroke(shape)->solid);
  } else if (shape->stroke_type == SVG_COLOR_LINEAR_GRADIENT) {
    const svg_color_linear_gradient_t* gradient = &(svg_shape_get_stroke(shape)->linear_gradient);
    vgcanvas_set_stroke_linear_gradient(canvas, gradient->sx, gradient->sy, gradient->ex,
                                        gradient->ey, gradient->icolor, gradient->ocolor);
  }
  if (shape->stroke_type != SVG_COLOR_NULL) {
    vgcanvas_stroke(canvas);
  }

  return RET_OK;
}

ret_t bsvg_draw_shape(bsvg_draw_ctx_t* ctx, const svg_shape_t* shape) {
  vgcanvas_t* canvas = ctx->canvas;

  vgcanvas_begin_path(canvas);
  switch (shape->type) {
    case SVG_SHAPE_TEXT: {
      break;
    }
    case SVG_SHAPE_PATH: {
      return RET_OK;
    }
    case SVG_SHAPE_NULL: {
      return RET_OK;
    }
    default: {
      assert(!"not supported type");
      return RET_OK;
    }
  }

  return bsvg_draw_shape_end(ctx, shape);
}

static ret_t bsvg_draw_on_path(void* ctx, const void* data) {
  bsvg_draw_ctx_t* info = (bsvg_draw_ctx_t*)ctx;
  const svg_path_t* path = (const svg_path_t*)data;

  if (path->type == SVG_PATH_NULL) {
    return bsvg_draw_shape_end(info, info->shape);
  } else {
    bsvg_draw_path(info, path);
  }

  return RET_OK;
}

static ret_t bsvg_draw_on_shape(void* ctx, const void* data) {
  bsvg_draw_ctx_t* info = (bsvg_draw_ctx_t*)ctx;
  const svg_shape_t* shape = (const svg_shape_t*)data;

  info->shape = shape;
  bsvg_draw_shape(info, shape);

  return RET_OK;
}

ret_t bsvg_draw(bsvg_t* svg, vgcanvas_t* canvas) {
  bsvg_draw_ctx_t ctx;
  return_value_if_fail(svg != NULL && canvas != NULL, RET_BAD_PARAMS);

  ctx.bsvg = svg;
  ctx.shape = NULL;
  ctx.canvas = canvas;

  vgcanvas_save(canvas);
  /* vgcanvas_scale(canvas, 1, 1); */
  vgcanvas_set_line_cap(canvas, "butt");
  /* 默认应该是miter，但因为有bug，暂时先不设置 */
  /* vgcanvas_set_line_join(canvas, "miter"); */
  bsvg_visit(svg, &ctx, bsvg_draw_on_shape, bsvg_draw_on_path);
  vgcanvas_restore(canvas);

  return RET_OK;
}
