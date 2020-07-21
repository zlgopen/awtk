/**
 * File:   bsvg_draw.c
 * Author: AWTK Develop Team
 * Brief:  bsvg_draw
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

/*https://www.w3.org/TR/SVG11/implnote.html#ArcImplementationNotes*/

typedef struct _draw_ctx_t {
  float x;
  float y;
  bsvg_t* bsvg;
  vgcanvas_t* canvas;
  const svg_shape_t* shape;

  /*for S/S_REL*/
  float last_x2;
  float last_y2;
  uint8_t last_type;

  /*for T/T_REL*/
  float last_x1;
  float last_y1;
} draw_ctx_t;

static ret_t bsvg_draw_arc_path(draw_ctx_t* ctx, const svg_path_arc_t* arc);

ret_t bsvg_draw_path(draw_ctx_t* ctx, const svg_path_t* path) {
  vgcanvas_t* canvas = ctx->canvas;

  switch (path->type) {
    case SVG_PATH_M: {
      const svg_path_move_t* p = (const svg_path_move_t*)path;

      ctx->x = p->x;
      ctx->y = p->y;
      vgcanvas_move_to(canvas, ctx->x, ctx->y);
      /* 屏蔽了 vg 的路径镂空函数，修复 gles 和 agge 的 svg 效果同步的问题，会导致 svg 没有了路径消除的逻辑 */
      // vgcanvas_path_winding(canvas, 0);
      break;
    }
    case SVG_PATH_M_REL: {
      const svg_path_move_t* p = (const svg_path_move_t*)path;

      ctx->x += p->x;
      ctx->y += p->y;
      vgcanvas_move_to(canvas, ctx->x, ctx->y);
      /* 屏蔽了 vg 的路径镂空函数，修复 gles 和 agge 的 svg 效果同步的问题，会导致 svg 没有了路径消除的逻辑 */
      // vgcanvas_path_winding(canvas, 0);
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

      vgcanvas_bezier_to(canvas, p->x1, p->y1, p->x2, p->y2, p->x, p->y);
      ctx->x = p->x;
      ctx->y = p->y;
      ctx->last_x2 = p->x2;
      ctx->last_y2 = p->y2;
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
      ctx->last_x2 = x2;
      ctx->last_y2 = y2;
      break;
    }
    case SVG_PATH_S: {
      const svg_path_scurve_to_t* p = (const svg_path_scurve_to_t*)path;
      float x1 = ctx->x;
      float y1 = ctx->y;
      if (ctx->last_type == SVG_PATH_C_REL || ctx->last_type == SVG_PATH_C ||
          ctx->last_type == SVG_PATH_S_REL || ctx->last_type == SVG_PATH_S) {
        x1 = 2 * ctx->x - ctx->last_x2;
        y1 = 2 * ctx->y - ctx->last_y2;
      }
      vgcanvas_bezier_to(canvas, x1, y1, p->x2, p->y2, p->x, p->y);

      ctx->x = p->x;
      ctx->y = p->y;
      ctx->last_x2 = p->x2;
      ctx->last_y2 = p->y2;
      break;
    }
    case SVG_PATH_S_REL: {
      const svg_path_scurve_to_t* p = (const svg_path_scurve_to_t*)path;
      float x1 = ctx->x;
      float y1 = ctx->y;
      float x2 = ctx->x + p->x2;
      float y2 = ctx->y + p->y2;
      float x = ctx->x + p->x;
      float y = ctx->y + p->y;
      if (ctx->last_type == SVG_PATH_C_REL || ctx->last_type == SVG_PATH_C ||
          ctx->last_type == SVG_PATH_S_REL || ctx->last_type == SVG_PATH_S) {
        x1 = 2 * ctx->x - ctx->last_x2;
        y1 = 2 * ctx->y - ctx->last_y2;
      }
      vgcanvas_bezier_to(canvas, x1, y1, x2, y2, x, y);

      ctx->x = x;
      ctx->y = y;
      ctx->last_x2 = x2;
      ctx->last_y2 = y2;
      break;
    }
    case SVG_PATH_Q: {
      const svg_path_qcurve_to_t* p = (const svg_path_qcurve_to_t*)path;
      vgcanvas_quad_to(canvas, p->x1, p->y1, p->x, p->y);
      ctx->x = p->x;
      ctx->y = p->y;
      ctx->last_x1 = p->x1;
      ctx->last_y1 = p->y1;
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
      ctx->last_x1 = x1;
      ctx->last_y1 = y1;
      break;
    }
    case SVG_PATH_T: {
      const svg_path_tcurve_to_t* p = (const svg_path_tcurve_to_t*)path;
      float x1 = 2 * ctx->x - ctx->last_x1;
      float y1 = 2 * ctx->y - ctx->last_y1;

      vgcanvas_quad_to(canvas, x1, y1, p->x, p->y);

      ctx->x = p->x;
      ctx->y = p->y;
      ctx->last_x1 = x1;
      ctx->last_y1 = y1;
      break;
    }
    case SVG_PATH_T_REL: {
      const svg_path_tcurve_to_t* p = (const svg_path_tcurve_to_t*)path;
      float x1 = 2 * ctx->x - ctx->last_x1;
      float y1 = 2 * ctx->y - ctx->last_y1;
      float x = ctx->x + p->x;
      float y = ctx->y + p->y;

      vgcanvas_quad_to(canvas, x1, y1, x, y);

      ctx->x = x;
      ctx->y = y;
      ctx->last_x1 = x1;
      ctx->last_y1 = y1;
      break;
    }
    case SVG_PATH_A: {
      const svg_path_arc_t* p = (const svg_path_arc_t*)path;
      bsvg_draw_arc_path(ctx, p);
      break;
    }
    case SVG_PATH_A_REL: {
      const svg_path_arc_t* p = (const svg_path_arc_t*)path;
      bsvg_draw_arc_path(ctx, p);
      break;
    }
    case SVG_PATH_Z: {
      vgcanvas_close_path(canvas);
      break;
    }
    case SVG_PATH_NULL:
    default: { break; }
  }
  ctx->last_type = path->type;

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
        float x = s->data[i];
        float y = s->data[i + 1];

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
        float x = s->data[i];
        float y = s->data[i + 1];

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
  float sx = 1;
  float sy = 1;
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
    sx = (float)(header->w) / (float)(header->viewport.w);
    sy = (float)(header->h) / (float)(header->viewport.h);
  }

  vgcanvas_scale(canvas, sx, sy);
  bsvg_visit(svg, &ctx, bsvg_draw_on_shape, bsvg_draw_on_path);
  vgcanvas_restore(canvas);

  return RET_OK;
}

/*Adapt from https://github.com/mozilla/newtab-dev/blob/master/dom/svg/nsSVGPathDataParser.cpp
 * begin:*/
typedef struct _arc_info_t {
  uint32_t seg_index;
  uint32_t num_segs;
  float rx;
  float ry;
  float sin_phi;
  float cos_phi;
  pointf_t c;
  float delta;
  pointf_t from;
  float t;
  float theta;
} arc_info_t;

static float calc_vector_angle(float ux, float uy, float vx, float vy) {
  float ta = atan2(uy, ux);
  float tb = atan2(vy, vx);
  if (tb >= ta) return tb - ta;

  return 2 * M_PI - (ta - tb);
}

static ret_t arc_info_init(arc_info_t* info, pointf_t from, pointf_t to, pointf_t radii,
                           float angle, bool_t largeArcFlag, bool_t sweepFlag) {
  const float radPerDeg = M_PI / 180.0f;
  info->seg_index = 0;

  if (from.x == to.x && from.y == to.y) {
    info->num_segs = 0;
    return RET_OK;
  }
  /*
  // Convert to center parameterization as shown in
  // http://www.w3.org/TR/SVG/implnote.html
  */
  info->rx = fabs(radii.x);
  info->ry = fabs(radii.y);

  info->sin_phi = sin(angle * radPerDeg);
  info->cos_phi = cos(angle * radPerDeg);

  float x1dash = info->cos_phi * (from.x - to.x) / 2.0f + info->sin_phi * (from.y - to.y) / 2.0f;
  float y1dash = -info->sin_phi * (from.x - to.x) / 2.0f + info->cos_phi * (from.y - to.y) / 2.0f;

  float root;
  float numerator = info->rx * info->rx * info->ry * info->ry -
                    info->rx * info->rx * y1dash * y1dash - info->ry * info->ry * x1dash * x1dash;

  if (numerator < 0.0f) {
    /*
    //  If info->rx , info->ry and are such that there is no solution (basically,
    //  the ellipse is not big enough to reach from 'from' to 'to'
    //  then the ellipse is scaled up uniformly until there is
    //  exactly one solution (until the ellipse is just big enough).

    // -> find factor s, such that numerator' with info->rx'=s*info->rx and
    //    info->ry'=s*info->ry becomes 0 :
    */
    float s = sqrt(1.0f - numerator / (info->rx * info->rx * info->ry * info->ry));

    info->rx *= s;
    info->ry *= s;
    root = 0.0f;

  } else {
    root = (largeArcFlag == sweepFlag ? -1.0f : 1.0f) *
           sqrt(numerator /
                (info->rx * info->rx * y1dash * y1dash + info->ry * info->ry * x1dash * x1dash));
  }

  float cxdash = root * info->rx * y1dash / info->ry;
  float cydash = -root * info->ry * x1dash / info->rx;

  info->c.x = info->cos_phi * cxdash - info->sin_phi * cydash + (from.x + to.x) / 2.0f;
  info->c.y = info->sin_phi * cxdash + info->cos_phi * cydash + (from.y + to.y) / 2.0f;
  info->theta =
      calc_vector_angle(1.0f, 0.0f, (x1dash - cxdash) / info->rx, (y1dash - cydash) / info->ry);
  float dtheta = calc_vector_angle((x1dash - cxdash) / info->rx, (y1dash - cydash) / info->ry,
                                   (-x1dash - cxdash) / info->rx, (-y1dash - cydash) / info->ry);
  if (!sweepFlag && dtheta > 0)
    dtheta -= 2.0f * M_PI;
  else if (sweepFlag && dtheta < 0)
    dtheta += 2.0f * M_PI;

  /* Convert into cubic bezier segments <= 90deg */
  info->num_segs = (int32_t)(ceil(fabs(dtheta / (M_PI / 2.0f))));
  info->delta = dtheta / info->num_segs;
  info->t =
      8.0f / 3.0f * sin(info->delta / 4.0f) * sin(info->delta / 4.0f) / sin(info->delta / 2.0f);

  info->from = from;

  return RET_OK;
}

bool_t arc_info_next(arc_info_t* info, pointf_t* cp1, pointf_t* cp2, pointf_t* to) {
  if (info->seg_index == info->num_segs) {
    return FALSE;
  }

  float cosTheta1 = cos(info->theta);
  float sinTheta1 = sin(info->theta);
  float theta2 = info->theta + info->delta;
  float cosTheta2 = cos(theta2);
  float sinTheta2 = sin(theta2);

  /* a) calculate endpoint of the segment:*/
  to->x = info->cos_phi * info->rx * cosTheta2 - info->sin_phi * info->ry * sinTheta2 + info->c.x;
  to->y = info->sin_phi * info->rx * cosTheta2 + info->cos_phi * info->ry * sinTheta2 + info->c.y;

  /* b) calculate gradients at start/end points of segment:*/
  cp1->x = info->from.x +
           info->t * (-info->cos_phi * info->rx * sinTheta1 - info->sin_phi * info->ry * cosTheta1);
  cp1->y = info->from.y +
           info->t * (-info->sin_phi * info->rx * sinTheta1 + info->cos_phi * info->ry * cosTheta1);

  cp2->x = to->x +
           info->t * (info->cos_phi * info->rx * sinTheta2 + info->sin_phi * info->ry * cosTheta2);
  cp2->y = to->y +
           info->t * (info->sin_phi * info->rx * sinTheta2 - info->cos_phi * info->ry * cosTheta2);

  /* do next segment*/
  info->theta = theta2;
  info->from = *to;
  ++info->seg_index;

  return TRUE;
}
/*Adapt from https://github.com/mozilla/newtab-dev/blob/master/dom/svg/nsSVGPathDataParser.cpp end*/

static ret_t bsvg_draw_arc_path(draw_ctx_t* ctx, const svg_path_arc_t* arc) {
  arc_info_t info;
  pointf_t cp1 = {0, 0};
  pointf_t cp2 = {0, 0};
  pointf_t end = {0, 0};
  pointf_t r = {arc->rx, arc->ry};
  pointf_t from = {ctx->x, ctx->y};
  pointf_t to = {arc->x, arc->y};
  vgcanvas_t* canvas = ctx->canvas;

  if (arc->path.type == SVG_PATH_A_REL) {
    to.x += ctx->x;
    to.y += ctx->y;
  }

  arc_info_init(&info, from, to, r, arc->rotation, arc->large_arc, arc->sweep);
  while (arc_info_next(&info, &cp1, &cp2, &end)) {
    vgcanvas_bezier_to(canvas, cp1.x, cp1.y, cp2.x, cp2.y, end.x, end.y);
  }

  ctx->x = to.x;
  ctx->y = to.y;

  return RET_OK;
}
