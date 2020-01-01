/**
 * File:   svg_builder.c
 * Author: AWTK Develop Team
 * Brief:  svg builder
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

#include "svg/bsvg_builder.h"

bsvg_builder_t* bsvg_builder_init(bsvg_builder_t* svg, uint32_t* buff, uint32_t size) {
  return_value_if_fail(svg != NULL && buff != NULL && size > BSVG_MIN_SIZE, NULL);

  svg->header = (bsvg_header_t*)buff;
  memset(svg->header, 0x00, sizeof(bsvg_header_t));

  svg->header->version = 1;
  svg->header->stroke_width = 1;
  svg->header->magic = BSVG_MAGIC;

  svg->current_shape_type = SVG_SHAPE_NULL;
  wbuffer_init(&(svg->buff), (uint8_t*)buff, size);
  svg->buff.cursor = sizeof(bsvg_header_t);

  return svg;
}

ret_t bsvg_builder_add_shape(bsvg_builder_t* svg, const svg_shape_t* shape) {
  return_value_if_fail(svg != NULL && shape != NULL, RET_BAD_PARAMS);

  if (svg->current_shape_type == SVG_SHAPE_PATH) {
    svg_path_t path_null = {SVG_PATH_NULL};
    bsvg_builder_add_sub_path(svg, &path_null);
  }
  svg->current_shape_type = (svg_shape_type_t)(shape->type);

  return wbuffer_write_binary(&(svg->buff), shape, svg_shape_size(shape));
}

ret_t bsvg_builder_add_sub_path(bsvg_builder_t* svg, const svg_path_t* path) {
  return_value_if_fail(svg != NULL && path != NULL, RET_BAD_PARAMS);
  return_value_if_fail(svg->current_shape_type == SVG_SHAPE_PATH, RET_BAD_PARAMS);

  return wbuffer_write_binary(&(svg->buff), path, svg_path_size(path));
}

ret_t bsvg_builder_done(bsvg_builder_t* svg) {
  svg_shape_t shape;
  return_value_if_fail(svg != NULL, RET_BAD_PARAMS);

  memset(&shape, 0x00, sizeof(shape));
  shape.type = SVG_SHAPE_NULL;
  bsvg_builder_add_shape(svg, &shape);

  return RET_OK;
}
