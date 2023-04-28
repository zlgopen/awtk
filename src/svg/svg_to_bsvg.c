/**
 * File:   svg_to_bsvg.c
 * Author: AWTK Develop Team
 * Brief:  svg to  bsvg
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

#include "svgtiny/include/svgtiny.h"
#include "tkc/mem.h"
#include "svg/bsvg_builder.h"

#ifdef WITH_RES_TOOLS
static inline color_t convert_color(svgtiny_colour color) {
  return color_init(svgtiny_RED(color), svgtiny_GREEN(color), svgtiny_BLUE(color), 0xff);
}

static inline void svg_init_solid(svg_color_t* color, svgtiny_colour c) {
  svg_color_solid_init(color, convert_color(c));
}

static inline void svg_init_linear_gradient(svg_color_t* color, struct svgtiny_linear_gradient* l) {
  svg_color_linear_gradient_init(color, l->sx, l->sy, l->ex, l->ey, convert_color(l->icolor),
                                 convert_color(l->ocolor));
}

static svg_shape_t* svg_init_shape(svg_shape_t* shape, struct svgtiny_shape* s) {
  return_value_if_fail(shape != NULL && s != NULL, NULL);

  if (s->fill == svgtiny_TRANSPARENT) {
    shape->fill_type = SVG_COLOR_NULL;
  } else if (s->fill == svgtiny_LINEAR_GRADIENT) {
    shape->fill_type = SVG_COLOR_LINEAR_GRADIENT;
    svg_init_linear_gradient(svg_shape_get_fill(shape), &s->fill_linear_gradient);
  } else {
    shape->fill_type = SVG_COLOR_SOLID;
    svg_init_solid(svg_shape_get_fill(shape), s->fill);
  }

  if (s->stroke == svgtiny_TRANSPARENT) {
    shape->stroke_type = SVG_COLOR_NULL;
  } else if (s->stroke == svgtiny_LINEAR_GRADIENT) {
    shape->stroke_type = SVG_COLOR_LINEAR_GRADIENT;
    svg_init_linear_gradient(svg_shape_get_stroke(shape), &s->stroke_linear_gradient);
  } else {
    shape->stroke_type = SVG_COLOR_SOLID;
    svg_init_solid(svg_shape_get_stroke(shape), s->stroke);
  }

  shape->stroke_width = s->stroke_width;

  return shape;
}

static ret_t svg_visit(struct svgtiny_diagram* diagram, void* ctx, tk_visit_t on_shape,
                       tk_visit_t on_path, tk_visit_t on_text) {
  return_value_if_fail(diagram != NULL && on_shape != NULL && on_path != NULL && on_text != NULL,
                       RET_BAD_PARAMS);

  uint8_t shape_buff[128];

  for (unsigned int i = 0; i < diagram->shape_count; i++) {
    float* p = diagram->shape[i].path;
    char* t = diagram->shape[i].text;
    if (p) {
      svg_shape_t* shape = svg_shape_path_init((svg_shape_t*)&shape_buff);
      svg_init_shape(shape, &(diagram->shape[i]));
      on_shape(ctx, shape);

      while ((p - diagram->shape[i].path) < diagram->shape[i].path_length) {
        if (p[0] == svgtiny_PATH_MOVE) {
          svg_path_move_t path;
          memset(&path, 0, sizeof(path));
          float x = p[1];
          float y = p[2];
          svg_path_move_init(&path, x, y);
          on_path(ctx, (svg_path_t*)&path);
          p += 3;
        } else if (p[0] == svgtiny_PATH_CLOSE) {
          svg_path_t path;
          memset(&path, 0, sizeof(path));
          path.type = SVG_PATH_Z;
          on_path(ctx, (svg_path_t*)&path);
          p += 1;
        } else if (p[0] == svgtiny_PATH_LINE) {
          svg_path_line_t path;
          memset(&path, 0, sizeof(path));
          float x = p[1];
          float y = p[2];
          svg_path_line_init(&path, x, y);
          on_path(ctx, (svg_path_t*)&path);
          p += 3;
        } else if (p[0] == svgtiny_PATH_BEZIER) {
          svg_path_curve_to_t path;
          memset(&path, 0, sizeof(path));
          float x1 = p[1];
          float y1 = p[2];
          float x2 = p[3];
          float y2 = p[4];
          float x = p[5];
          float y = p[6];
          svg_path_curve_to_init(&path, x1, y1, x2, y2, x, y);
          on_path(ctx, (svg_path_t*)&path);
          p += 7;
        } else {
          assert(!"not supported");
        }
      }
    } else if (t) {
      svg_shape_t* shape = svg_shape_text_init((svg_shape_text_t*)&shape_buff,
                                               diagram->shape[i].text_x, diagram->shape[i].text_y);
      svg_init_shape(shape, &(diagram->shape[i]));
      on_shape(ctx, shape);
      on_text(ctx, diagram->shape[i].text);
    }
  }

  return RET_OK;
}

static ret_t svg_count_on_shape(void* ctx, const void* data) {
  uint32_t* total_size = (uint32_t*)ctx;
  const svg_shape_t* shape = (const svg_shape_t*)data;
  *total_size += svg_shape_size(shape);
  if (shape->type == SVG_SHAPE_PATH) {
    // 附加一个SVG_PATH_NULL
    *total_size += sizeof(svg_path_t);
  }
  return RET_OK;
}
static ret_t svg_count_on_path(void* ctx, const void* data) {
  uint32_t* total_size = (uint32_t*)ctx;
  const svg_path_t* path = (const svg_path_t*)data;
  *total_size += svg_path_size(path);
  return RET_OK;
}
static ret_t svg_count_on_text(void* ctx, const void* data) {
  uint32_t* total_size = (uint32_t*)ctx;
  const char* text = (const char*)data;
  *total_size += strlen(text) + 1;
  return RET_OK;
}
static ret_t svg_write_on_shape(void* ctx, const void* data) {
  bsvg_builder_t* builder = (bsvg_builder_t*)ctx;
  const svg_shape_t* shape = (const svg_shape_t*)data;
  bsvg_builder_add_shape(builder, shape);
  return RET_OK;
}
static ret_t svg_write_on_path(void* ctx, const void* data) {
  bsvg_builder_t* builder = (bsvg_builder_t*)ctx;
  const svg_path_t* path = (const svg_path_t*)data;
  bsvg_builder_add_sub_path(builder, path);
  return RET_OK;
}
static ret_t svg_write_on_text(void* ctx, const void* data) {
  bsvg_builder_t* builder = (bsvg_builder_t*)ctx;
  const char* text = (const char*)data;
  bsvg_builder_add_sub_text(builder, text);
  return RET_OK;
}

ret_t svg_to_bsvg(const char* xml, uint32_t size, uint32_t** out, uint32_t* out_size) {
  ret_t ret = RET_FAIL;
  struct svgtiny_diagram* diagram = NULL;
  svgtiny_code code = svgtiny_OK;
  bsvg_builder_t builder;
  uint32_t* buff = NULL;
  uint32_t buff_size = 0;
  return_value_if_fail(xml != NULL && out != NULL && out_size != NULL, RET_BAD_PARAMS);

  // xml -> svgtiny
  diagram = svgtiny_create();
  ret = RET_OOM;
  goto_error_if_fail(diagram != NULL);
  code = svgtiny_parse(diagram, xml, size, "", 100, 100);
  ret = RET_FAIL;
  goto_error_if_fail(code == svgtiny_OK);

  // svgtiny -> bsvg
  buff_size += sizeof(bsvg_header_t);
  ret = svg_visit(diagram, &buff_size, svg_count_on_shape, svg_count_on_path, svg_count_on_text);
  // 附加一个SVG_SHAPE_NULL
  buff_size += sizeof(svg_shape_t);
  // wbuffer_extend_delta()需要多一个
  buff_size += 1;
  goto_error_if_fail(ret == RET_OK);
  buff = (uint32_t*)TKMEM_ALLOC(buff_size);
  ret = RET_OOM;
  goto_error_if_fail(buff != NULL);

  memset(&builder, 0, sizeof(builder));
  bsvg_builder_init(&builder, buff, buff_size);
  builder.header->w = diagram->width;
  builder.header->h = diagram->height;
  svg_visit(diagram, &builder, svg_write_on_shape, svg_write_on_path, svg_write_on_text);
  bsvg_builder_done(&builder);

  // succ
  if (diagram) {
    svgtiny_free(diagram);
  }
  *out = buff;
  *out_size = builder.buff.cursor;
  ret = RET_OK;
  return ret;

error:
  // fail
  if (diagram) {
    svgtiny_free(diagram);
  }
  if (buff) {
    TKMEM_FREE(buff);
  }
  *out = NULL;
  *out_size = 0;
  return ret;
}

#include "tkc/fs.h"

ret_t svg_file_to_bsvg(const char* ifilename, const char* ofilename) {
  ret_t ret = RET_FAIL;
  uint32_t size = 0;
  uint8_t* data = NULL;
  uint32_t osize = 0;
  uint32_t* odata = NULL;
  return_value_if_fail(ifilename != NULL && ofilename != NULL, RET_BAD_PARAMS);
  data = file_read(ifilename, &size);
  return_value_if_fail(data != NULL, RET_BAD_PARAMS);

  if (svg_to_bsvg((const char*)data, size, &odata, &osize) == RET_OK) {
    ret = file_write(ofilename, odata, osize);
    TKMEM_FREE(odata);
  }
  TKMEM_FREE(data);

  return ret;
}
#else
ret_t svg_to_bsvg(const char* xml, uint32_t size, uint32_t** out, uint32_t* out_size) {
  (void)xml, (void)size, (void)out, (void)out_size;
  log_debug("Not support %s, please define WITH_RES_TOOLS!\r\n", __FUNCTION__);
  return RET_NOT_IMPL;
}

ret_t svg_file_to_bsvg(const char* ifilename, const char* ofilename) {
  (void)ifilename, (void)ofilename;
  log_debug("Not support %s, please define WITH_RES_TOOLS!\r\n", __FUNCTION__);
  return RET_NOT_IMPL;
}
#endif /*WITH_RES_TOOLS*/
