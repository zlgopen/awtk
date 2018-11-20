/**
 * File:   bsvg_draw.c
 * Author: AWTK Develop Team
 * Brief:  bsvg_draw
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

typedef struct _draw_ctx_t {
  float_t x;
  float_t y;
  bsvg_t* bsvg;
  vgcanvas_t* canvas;
  const svg_shape_t* shape;
} draw_ctx_t;

ret_t bsvg_draw_path(draw_ctx_t* ctx, const svg_path_t* path) {
  vgcanvas_t* canvas = ctx->canvas;

  switch (path->type) {
    case SVG_PATH_M: {
      const svg_path_move_t* p = (const svg_path_move_t*)path;

      ctx->x = p->x;
      ctx->y = p->y;
      vgcanvas_move_to(canvas, ctx->x, ctx->y);

      break;
    }
    case SVG_PATH_M_REL: {
      const svg_path_move_t* p = (const svg_path_move_t*)path;

      ctx->x += p->x;
      ctx->y += p->y;
      vgcanvas_move_to(canvas, ctx->x, ctx->y);
      break;
    }
    case SVG_PATH_L: {
      const svg_path_line_t* p = (const svg_path_line_t*)path;

      ctx->x = p->x;
      ctx->y = p->y;
      vgcanvas_line_to(canvas, ctx->x, ctx->y);
      break;
    }
    case SVG_PATH_L_REL: {
      const svg_path_line_t* p = (const svg_path_line_t*)path;

      ctx->x += p->x;
      ctx->y += p->y;
      vgcanvas_line_to(canvas, ctx->x, ctx->y);
      break;
    }
    case SVG_PATH_H: {
      const svg_path_hline_t* p = (const svg_path_hline_t*)path;

      ctx->x = p->x;
      vgcanvas_line_to(canvas, ctx->x, ctx->y);
      break;
    }
    case SVG_PATH_H_REL: {
      const svg_path_hline_t* p = (const svg_path_hline_t*)path;

      ctx->x += p->x;
      vgcanvas_line_to(canvas, ctx->x, ctx->y);
      break;
    }
    case SVG_PATH_V: {
      const svg_path_vline_t* p = (const svg_path_vline_t*)path;

      ctx->y = p->y;
      vgcanvas_line_to(canvas, ctx->x, ctx->y);
      break;
    }
    case SVG_PATH_V_REL: {
      const svg_path_vline_t* p = (const svg_path_vline_t*)path;

      ctx->y += p->y;
      vgcanvas_line_to(canvas, ctx->x, ctx->y);
      break;
    }
    case SVG_PATH_C: {
      const svg_path_curve_to_t* p = (const svg_path_curve_to_t*)path;

      ctx->x = p->x;
      ctx->y = p->y;
      vgcanvas_bezier_to(canvas, p->x1, p->y1, p->x2, p->y2, ctx->x, ctx->y);
      break;
    }
    case SVG_PATH_C_REL: {
      const svg_path_curve_to_t* p = (const svg_path_curve_to_t*)path;
      float x1 = ctx->x + p->x1;
      float y1 = ctx->y + p->y1;
      float x2 = ctx->x + p->x2;
      float y2 = ctx->y + p->y2;
      float x = ctx->x + p->x;
      float y = ctx->y + p->y;

      vgcanvas_bezier_to(canvas, x1, y1, x2, y2, x, y);
      ctx->x = x;
      ctx->y = y;
      break;
    }
    case SVG_PATH_S: {
      const svg_path_scurve_to_t* p = (const svg_path_scurve_to_t*)path;
      (void)p;
      /*TODO*/
      break;
    }
    case SVG_PATH_S_REL: {
      const svg_path_scurve_to_t* p = (const svg_path_scurve_to_t*)path;
      (void)p;
      /*TODO*/
      break;
    }
    case SVG_PATH_Q: {
      const svg_path_qcurve_to_t* p = (const svg_path_qcurve_to_t*)path;
      ctx->x = p->x;
      ctx->y = p->y;
      vgcanvas_quad_to(canvas, p->x1, p->y1, ctx->x, ctx->y);
      break;
    }
    case SVG_PATH_Q_REL: {
      const svg_path_qcurve_to_t* p = (const svg_path_qcurve_to_t*)path;
      float x1 = ctx->x + p->x1;
      float y1 = ctx->y + p->y1;
      float x = ctx->x + p->x;
      float y = ctx->y + p->y;

      vgcanvas_quad_to(canvas, x1, y1, x, y);
      ctx->x = x;
      ctx->y = y;
      break;
    }
    case SVG_PATH_T: {
      const svg_path_tcurve_to_t* p = (const svg_path_tcurve_to_t*)path;
      (void)p;
      /*TODO*/
      break;
    }
    case SVG_PATH_T_REL: {
      const svg_path_tcurve_to_t* p = (const svg_path_tcurve_to_t*)path;
      (void)p;
      /*TODO*/
      break;
    }
    case SVG_PATH_A: {
      const svg_path_arc_t* p = (const svg_path_arc_t*)path;
      (void)p;
      /*TODO*/
      break;
    }
    case SVG_PATH_A_REL: {
      const svg_path_arc_t* p = (const svg_path_arc_t*)path;
      (void)p;
      /*TODO*/
      break;
    }
    case SVG_PATH_Z: {
      vgcanvas_close_path(canvas);
      break;
    }
    case SVG_PATH_NULL:
    default: { break; }
  }

  return RET_OK;
}

static ret_t bsvg_draw_set_line_cap(vgcanvas_t* canvas, char line_cap) {
  if (line_cap == 'r') {
    vgcanvas_set_line_cap(canvas, "round");
  } else if (line_cap == 's') {
    vgcanvas_set_line_cap(canvas, "square");
  } else {
    vgcanvas_set_line_cap(canvas, "butt");
  }

  return RET_OK;
}

static ret_t bsvg_draw_set_line_join(vgcanvas_t* canvas, char line_join) {
  if (line_join == 'r') {
    vgcanvas_set_line_join(canvas, "round");
  } else if (line_join == 'b') {
    vgcanvas_set_line_join(canvas, "bevel");
  } else {
    vgcanvas_set_line_join(canvas, "miter");
  }

  return RET_OK;
}

ret_t bsvg_draw_shape_end(draw_ctx_t* ctx, const svg_shape_t* shape) {
  vgcanvas_t* canvas = ctx->canvas;
  bsvg_header_t* header = ctx->bsvg->header;

  if (shape->line_cap) {
    bsvg_draw_set_line_cap(canvas, shape->line_cap);
  } else if (header->line_cap) {
    bsvg_draw_set_line_cap(canvas, header->line_cap);
  }

  if (shape->line_join) {
    bsvg_draw_set_line_join(canvas, shape->line_join);
  } else if (header->line_join) {
    bsvg_draw_set_line_join(canvas, header->line_join);
  }

  if (shape->stroke_width) {
    vgcanvas_set_line_width(canvas, shape->stroke_width);
  } else if (header->stroke_width) {
    vgcanvas_set_line_width(canvas, header->stroke_width);
  } else {
    vgcanvas_set_line_width(canvas, 1);
  }

  if (shape->fill.rgba.a) {
    vgcanvas_set_fill_color(canvas, shape->fill);
  } else if (header->fill.rgba.a) {
    vgcanvas_set_fill_color(canvas, header->fill);
  }

  if (!shape->no_fill) {
    vgcanvas_fill(canvas);
  }

  if (shape->stroke.rgba.a) {
    vgcanvas_set_stroke_color(canvas, shape->stroke);
  } else if (header->stroke.rgba.a) {
    vgcanvas_set_stroke_color(canvas, header->stroke);
  }

  if (!shape->no_stroke) {
    vgcanvas_stroke(canvas);
  }

  return RET_OK;
}

ret_t bsvg_draw_shape(draw_ctx_t* ctx, const svg_shape_t* shape) {
  vgcanvas_t* canvas = ctx->canvas;

  vgcanvas_begin_path(canvas);
  switch (shape->type) {
    case SVG_SHAPE_RECT: {
      svg_shape_rect_t* s = (svg_shape_rect_t*)shape;
      vgcanvas_rounded_rect(canvas, s->x, s->y, s->w, s->h, s->r);
      break;
    }
    case SVG_SHAPE_CIRCLE: {
      svg_shape_circle_t* s = (svg_shape_circle_t*)shape;
      vgcanvas_arc(canvas, s->cx, s->cy, s->r, 0, M_PI * 2, FALSE);
      break;
    }
    case SVG_SHAPE_ELLIPSE: {
      svg_shape_ellipse_t* s = (svg_shape_ellipse_t*)shape;
      vgcanvas_ellipse(canvas, s->cx, s->cy, s->rx, s->ry);

      break;
    }
    case SVG_SHAPE_LINE: {
      svg_shape_line_t* s = (svg_shape_line_t*)shape;
      vgcanvas_move_to(canvas, s->x1, s->y1);
      vgcanvas_line_to(canvas, s->x2, s->y2);

      break;
    }
    case SVG_SHAPE_POLYGON: {
      uint32_t i = 0;
      svg_shape_polygon_t* s = (svg_shape_polygon_t*)shape;

      for (i = 0; i < s->nr; i += 2) {
        float_t x = s->data[i];
        float_t y = s->data[i + 1];

        if (i == 0) {
          vgcanvas_move_to(canvas, x, y);
        } else {
          vgcanvas_line_to(canvas, x, y);
        }
      }

      break;
    }
    case SVG_SHAPE_POLYLINE: {
      uint32_t i = 0;
      svg_shape_polygon_t* s = (svg_shape_polygon_t*)shape;
      for (i = 0; i < s->nr; i += 2) {
        float_t x = s->data[i];
        float_t y = s->data[i + 1];

        if (i == 0) {
          vgcanvas_move_to(canvas, x, y);
        } else {
          vgcanvas_line_to(canvas, x, y);
        }
      }
      vgcanvas_close_path(canvas);

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
  draw_ctx_t* info = (draw_ctx_t*)ctx;
  const svg_path_t* path = (const svg_path_t*)data;

  if (path->type == SVG_PATH_NULL) {
    return bsvg_draw_shape_end(info, info->shape);
  } else {
    bsvg_draw_path(info, path);
  }

  return RET_OK;
}

static ret_t bsvg_draw_on_shape(void* ctx, const void* data) {
  draw_ctx_t* info = (draw_ctx_t*)ctx;
  const svg_shape_t* shape = (const svg_shape_t*)data;

  info->shape = shape;
  bsvg_draw_shape(info, shape);

  return RET_OK;
}

ret_t bsvg_draw(bsvg_t* svg, vgcanvas_t* canvas) {
  float_t sx = 1;
  float_t sy = 1;
  draw_ctx_t ctx;
  bsvg_header_t* header = svg->header;
  return_value_if_fail(header != NULL && svg != NULL && canvas != NULL, RET_BAD_PARAMS);

  ctx.x = 0;
  ctx.y = 0;
  ctx.bsvg = svg;
  ctx.shape = NULL;
  ctx.canvas = canvas;

  vgcanvas_save(canvas);
  if (header->w && header->h && header->viewport.w && header->viewport.h) {
    sx = (float_t)(header->w) / (float_t)(header->viewport.w);
    sy = (float_t)(header->h) / (float_t)(header->viewport.h);
  }

  vgcanvas_scale(canvas, sx, sy);
  bsvg_visit(svg, &ctx, bsvg_draw_on_shape, bsvg_draw_on_path);
  vgcanvas_restore(canvas);

  return RET_OK;
}
