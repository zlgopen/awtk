/**
 * File:   bsvg_to_svg.c
 * Author: AWTK Develop Team
 * Brief:  bsvg to svg
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
 * 2018-11-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/str.h"
#include "tkc/utils.h"
#include "svg/bsvg.h"
#include "svg/bsvg_to_svg.h"

typedef struct _ctx_info_t {
  str_t* str;
  const svg_shape_t* shape;
} ctx_info_t;

ret_t bsvg_to_svg_path(str_t* str, const svg_path_t* path) {
  char buff[128];

  switch (path->type) {
    case SVG_PATH_M: {
      const svg_path_move_t* p = (const svg_path_move_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " M%.1f %.1f", p->x, p->y);
      break;
    }
    case SVG_PATH_L: {
      const svg_path_line_t* p = (const svg_path_line_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " L%.1f %.1f", p->x, p->y);
      break;
    }
    case SVG_PATH_C: {
      const svg_path_curve_to_t* p = (const svg_path_curve_to_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " C%.1f %.1f %.1f %.1f %.1f %.1f", p->x1, p->y1, p->x2,
                  p->y2, p->x, p->y);
      break;
    }
    case SVG_PATH_Z: {
      tk_snprintf(buff, sizeof(buff) - 1, "z");
      break;
    }
    case SVG_PATH_NULL:
    default: {
      break;
    }
  }

  return str_append(str, buff);
}

ret_t bsvg_to_svg_shape_end(str_t* str, const svg_shape_t* shape) {
  char buff[128];

  if (shape->stroke_width) {
    tk_snprintf(buff, sizeof(buff) - 1, " stroke-width=\"%.1f\"", shape->stroke_width);
    return_value_if_fail(str_append(str, buff) == RET_OK, RET_OOM);
  }

  if (shape->fill_type == SVG_COLOR_NULL) {
    return_value_if_fail(str_append(str, " fill=\"none\"") == RET_OK, RET_OOM);
  } else if (shape->fill_type == SVG_COLOR_SOLID) {
    const rgba_t* rgba = &(svg_shape_get_fill(shape)->solid.rgba);
    tk_snprintf(buff, sizeof(buff) - 1, " fill=\"rgba(%d,%d,%d,%.2f)\"", (int)(rgba->r),
                (int)(rgba->g), (int)(rgba->b), (float)rgba->a / 255.0f);
    return_value_if_fail(str_append(str, buff) == RET_OK, RET_OOM);
  } else if (shape->fill_type == SVG_COLOR_LINEAR_GRADIENT) {
    return_value_if_fail(str_append(str, " fill=\"url(#linear_gradient)\"") == RET_OK, RET_OOM);
  }

  if (shape->stroke_type == SVG_COLOR_NULL) {
    return_value_if_fail(str_append(str, " stroke=\"none\"") == RET_OK, RET_OOM);
  } else if (shape->stroke_type == SVG_COLOR_SOLID) {
    const rgba_t* rgba = &(svg_shape_get_stroke(shape)->solid.rgba);
    tk_snprintf(buff, sizeof(buff) - 1, " stroke=\"rgba(%d,%d,%d,%.2f)\"", (int)(rgba->r),
                (int)(rgba->g), (int)(rgba->b), (float)rgba->a / 255.0f);
    return_value_if_fail(str_append(str, buff) == RET_OK, RET_OOM);
  } else if (shape->stroke_type == SVG_COLOR_LINEAR_GRADIENT) {
    return_value_if_fail(str_append(str, " stroke=\"url(#linear_gradient)\"") == RET_OK, RET_OOM);
  }

  if (shape->type == SVG_SHAPE_TEXT) {
    const char* text = svg_shape_text_get_text(shape);
    return_value_if_fail(text != NULL, RET_FAIL);
    return_value_if_fail(str_append(str, ">") == RET_OK, RET_OOM);
    return_value_if_fail(str_append(str, text) == RET_OK, RET_OOM);
    return_value_if_fail(str_append(str, "</text>\n") == RET_OK, RET_OOM);
  } else {
    return_value_if_fail(str_append(str, "/>\n") == RET_OK, RET_OOM);
  }

  return RET_OK;
}

ret_t bsvg_to_svg_shape(str_t* str, const svg_shape_t* shape) {
  char buff[128];
  switch (shape->type) {
    case SVG_SHAPE_TEXT: {
      svg_shape_text_t* s = (svg_shape_text_t*)shape;
      tk_snprintf(buff, sizeof(buff) - 1, "<text x=\"%.1f\" y=\"%.1f\"", s->x, s->y);
      str_append(str, buff);
      break;
    }
    case SVG_SHAPE_PATH: {
      str_append(str, "<path d=\"");
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

  return bsvg_to_svg_shape_end(str, shape);
}

static ret_t bsvg_to_svg_on_path(void* ctx, const void* data) {
  ctx_info_t* info = (ctx_info_t*)ctx;
  const svg_path_t* path = (const svg_path_t*)data;

  if (path->type == SVG_PATH_NULL) {
    str_append_char(info->str, '\"');
    bsvg_to_svg_shape_end(info->str, info->shape);
  } else {
    bsvg_to_svg_path(info->str, path);
  }

  return RET_OK;
}

static ret_t bsvg_to_svg_on_shape(void* ctx, const void* data) {
  ctx_info_t* info = (ctx_info_t*)ctx;
  const svg_shape_t* shape = (const svg_shape_t*)data;

  info->shape = shape;
  bsvg_to_svg_shape(info->str, shape);

  return RET_OK;
}

ret_t bsvg_to_svg(const uint32_t* data, uint32_t size, str_t* str) {
  bsvg_t svg;
  ctx_info_t ctx;
  return_value_if_fail(bsvg_init(&svg, data, size) != NULL, RET_BAD_PARAMS);

  ctx.str = str;
  ctx.shape = NULL;

  return bsvg_visit(&svg, &ctx, bsvg_to_svg_on_shape, bsvg_to_svg_on_path);
}
