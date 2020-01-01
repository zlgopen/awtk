/**
 * File:   bsvg.c
 * Author: AWTK Develop Team
 * Brief:  binary svg
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
 * 2018-11-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "svg/bsvg.h"

bsvg_t* bsvg_init(bsvg_t* svg, const uint32_t* data, uint32_t size) {
  return_value_if_fail(svg != NULL && data != NULL && size > BSVG_MIN_SIZE, NULL);
  svg->header = (bsvg_header_t*)data;
  return_value_if_fail(svg->header->magic == BSVG_MAGIC, NULL);

  svg->data = data;
  svg->size = size;

  return svg;
}

const uint8_t* bsvg_visit_path(const uint8_t* p, void* ctx, tk_visit_t on_path) {
  const svg_path_t* path = NULL;
  do {
    path = (const svg_path_t*)p;
    on_path(ctx, path);
    p += svg_path_size(path);
  } while (path->type != SVG_PATH_NULL);

  return p;
}

ret_t bsvg_visit(bsvg_t* svg, void* ctx, tk_visit_t on_shape, tk_visit_t on_path) {
  uint32_t size = 0;
  const uint8_t* p = NULL;
  return_value_if_fail(svg != NULL && on_shape != NULL && on_path != NULL, RET_BAD_PARAMS);

  p = ((const uint8_t*)(svg->data) + sizeof(bsvg_header_t));
  while ((p - (const uint8_t*)(svg->data)) < svg->size) {
    const svg_shape_t* shape = (const svg_shape_t*)p;

    on_shape(ctx, shape);
    size = svg_shape_size(shape);

    if (shape->type == SVG_SHAPE_PATH) {
      p = bsvg_visit_path(p + size, ctx, on_path);
    } else {
      p += size;
    }
  }

  return RET_OK;
}

const svg_shape_t* bsvg_get_first_shape(bsvg_t* svg) {
  return_value_if_fail(svg != NULL, NULL);

  return (const svg_shape_t*)((const uint8_t*)(svg->data) + sizeof(bsvg_header_t));
}
