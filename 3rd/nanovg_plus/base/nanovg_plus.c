/**
 * File:   nanovg_plus.c
 * Author: AWTK Develop Team
 * Brief:  refactoring nanovg.
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-10-27 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#include <stdarg.h>
#include "nanovg_plus.h"

#include <stdio.h>
#include <memory.h>
#define FONTSTASH_IMPLEMENTATION 1
#include <fontstash.h>


#ifdef WITH_NANOVG_PLUS_GPU
#include "../gl/nanovg_plus_gl.h"
#endif

#ifdef _WIN64
 typedef double floatptr_t;
#else
 typedef float floatptr_t;
#endif

#define NVGP_INIT_FONTIMAGE_SIZE  512
#define NVGP_MAX_FONTIMAGE_SIZE   2048
#define NVGP_MAX_FONTIMAGES       4

typedef enum _nvgp_winding_t {
  NVGP_CCW = 1,       // Winding for solid shapes
  NVGP_CW = 2,        // Winding for holes
} nvgp_winding_t;

typedef enum _nvgp_command_t {
  NVGP_MOVETO = 0,
  NVGP_LINETO = 1,
  NVGP_BEZIERTO = 2,
  NVGP_CLOSE = 3,
  NVGP_WINDING = 4,
} nvgp_command_t;

typedef struct _nvgp_point_t {
  float x,y;
  float dx, dy;
  float len;
  float dmx, dmy;
  uint32_t flags;
} nvgp_point_t;

typedef struct _nvgp_path_cache_t {
  float bounds[4];
  uint32_t nverts;
  uint32_t cverts;
  nvgp_vertex_t* verts;
  nvgp_darray_t paths;
  nvgp_darray_t points;
} nvgp_path_cache_t;


typedef struct _nvgp_state_t {
  uint8_t alpha;
  uint8_t line_join;
  uint8_t line_cap;
  uint8_t shape_anti_alias;
  float miter_limit;
  
  nvgp_paint_t fill;
  nvgp_paint_t stroke;
  float stroke_width;

  uint32_t font_id;
  int32_t text_align;
  float font_size;
  float font_blur;
  float line_height;
  float letter_spacing;

  nvgp_matrix_t matrix;
  nvgp_scissor_t scissor;

  float text_scale;
  float text_width;
  const char* text_end;
  const char* text_string;
} nvgp_state_t;

typedef struct _nvgp_context_t {
  float ratio;
  float tess_tol;
  float dist_tol;
  float fringe_width;
  float width;
  float height;
  float context_width;
  float context_height;
  nvgp_mode_t nvgp_mode;

  float command_x;
  float command_y;

  nvgp_darray_t states;
  nvgp_darray_t commands;
  nvgp_path_cache_t cache;

  struct FONScontext* fs;
  int fontImages[NVGP_MAX_FONTIMAGES];
  int fontImageIdx;

  int drawCallCount;
  int fillTriCount;
  int strokeTriCount;
  int textTriCount;


  void* vt_ctx;
  const nvgp_vtable_t* vt;
} nvgp_context_t;

#define CHECK_OBJECT_IS_NULL(ctx) assert(ctx != NULL)

static nvgp_matrix_t* nvgp_matrix_init(nvgp_matrix_t* mat);
static nvgp_vertex_t* nvgp_alloc_temp_verts(nvgp_context_t* ctx, int nverts);
static void nvgp_vset(nvgp_vertex_t* vtx, float x, float y, float u, float v);
static nvgp_error_t nvgp_set_line_cap_by_statue(nvgp_context_t* ctx, nvgp_state_t* state, nvgp_line_cap_t cap);
static nvgp_error_t nvgp_set_line_join_by_statue(nvgp_context_t* ctx, nvgp_state_t* state, nvgp_line_join_t join);

static void nvgp_set_paint_color(nvgp_paint_t* p, nvgp_color_t color){
  NVGP_MEMSET(p, 0, sizeof(nvgp_paint_t));
  nvgp_matrix_init(&p->mat);
  p->radius = 0.0f;
  p->feather = 1.0f;
  p->inner_color = color;
  p->outer_color = color;
}

static nvgp_state_t* nvgp_get_state(nvgp_context_t* ctx) {
  return nvgp_darray_get_ptr(&ctx->states, ctx->states.size - 1, nvgp_state_t);
}

static nvgp_state_t* nvgp_get_empty_state_by_tail(nvgp_context_t* ctx) {
  return nvgp_darray_get_empty_by_tail(&ctx->states, nvgp_state_t);
}

void nvgp_reset_curr_state(nvgp_context_t* ctx) {
  nvgp_state_t* state = nvgp_get_state(ctx);
  NVGP_MEMSET(state, 0, sizeof(*state));

  nvgp_set_paint_color(&state->fill, nvgp_color_init(0xff, 0xff, 0xff,0xff));
  nvgp_set_paint_color(&state->stroke, nvgp_color_init(0, 0, 0, 0xff));
  state->shape_anti_alias = 1;
  state->stroke_width = 1.0f;
  state->miter_limit = 10.0f;
  nvgp_set_line_cap_by_statue(ctx, state, NVGP_BUTT);
  nvgp_set_line_join_by_statue(ctx, state, NVGP_MITER);
  state->alpha = 0xff;
  nvgp_matrix_init(&state->matrix);

  state->scissor.extent[0] = -1.0f;
  state->scissor.extent[1] = -1.0f;
  nvgp_matrix_init(&state->scissor.matrix);

  state->font_size = 16.0f;
  state->letter_spacing = 0.0f;
  state->line_height = 1.0f;
  state->font_blur = 0.0f;
  state->text_align = NVGP_ALIGN_LEFT | NVGP_ALIGN_BASELINE;
  state->font_id = 0;

}

static int nvgp_init(nvgp_context_t* context, nvgp_mode_t nvgp_mode, uint32_t w, uint32_t h) {
  context->nvgp_mode = nvgp_mode;
  context->context_width = w;
  context->context_height = h;
  nvgp_darray_init(&context->commands, NVGP_INIT_COMMANDS_SIZE, 0);
  nvgp_darray_init(&context->states, NVGP_INIT_STATES, sizeof(nvgp_state_t));

  context->cache.verts = NVGP_ZALLOCN(nvgp_vertex_t, NVGP_INIT_CACHE_VERTS_SIZE);
  if (context->cache.verts == NULL) {
    return 0;
  }
  context->cache.nverts = 0;
  context->cache.cverts = NVGP_INIT_CACHE_VERTS_SIZE;

  nvgp_darray_init(&context->cache.paths, NVGP_INIT_CACHE_PATHS_SIZE, sizeof(nvgp_path_t));
  nvgp_darray_init(&context->cache.points, NVGP_INIT_CACHE_POINTS_SIZE, sizeof(nvgp_point_t));

  nvgp_save(context);
  nvgp_reset_curr_state(context);
  nvgp_scissor(context, 0, 0, context->context_width, context->context_height);
  return 1;
}

nvgp_context_t* nvgp_create_by_vt(uint32_t w, uint32_t h, const nvgp_vtable_t* vt, void* ctx) {
  nvgp_context_t* context = NVGP_ZALLOC(nvgp_context_t);
  if (context != NULL) {
    context->vt = vt;
    context->vt_ctx = ctx;
    if (!nvgp_init(context, NVGP_MODE_SPECIAL, w, h)) {
      goto error;
    }
  }
  return context;
error:
  nvgp_destroy(context);
  return NULL;
}

nvgp_context_t* nvgp_create(nvgp_mode_t nvgp_mode, uint32_t w, uint32_t h) {
  nvgp_context_t* context = NVGP_ZALLOC(nvgp_context_t);
  if (context != NULL) {
#ifdef WITH_NANOVG_PLUS_GPU
    if (nvgp_mode == NVGP_MODE_GPU) {
      FONSparams fontParams;
      context->vt = nvgp_gl_vtable();
      context->vt_ctx = nvgp_gl_create(NVGP_GL_FLAG_ANTIALIAS | NVGP_GL_FLAG_STENCIL_STROKES);

      NVGP_MEMSET(&fontParams, 0, sizeof(fontParams));
      fontParams.width = NVGP_INIT_FONTIMAGE_SIZE;
      fontParams.height = NVGP_INIT_FONTIMAGE_SIZE;
      fontParams.flags = FONS_ZERO_TOPLEFT;
      fontParams.renderCreate = NULL;
      fontParams.renderUpdate = NULL;
      fontParams.renderDraw = NULL;
      fontParams.renderDelete = NULL;
      fontParams.userPtr = NULL;
      context->fs = fonsCreateInternal(&fontParams);
      if (context->fs == NULL) {
        goto error;
      }

      // Create font texture
      context->fontImages[0] = context->vt->create_texture(context->vt_ctx, NVGP_TEXTURE_ALPHA, fontParams.width, fontParams.height, 0, 0, NULL);
      if (context->fontImages[0] == 0) goto error;
      context->fontImageIdx = 0;
    } else 
#endif
    if (nvgp_mode == NVGP_MODE_CPU) {

    }
    if (!nvgp_init(context, nvgp_mode, w, h)) {
      goto error;
    }
  }
  return context;
error:
  nvgp_destroy(context);
  return NULL;
}

void nvgp_destroy(nvgp_context_t* ctx) {
  if (ctx != NULL) {
#ifdef WITH_NANOVG_PLUS_GPU
    uint32_t i = 0;
	  if (ctx->fs) {
		  fonsDeleteInternal(ctx->fs);
    }
	for (i = 0; i < NVGP_MAX_FONTIMAGES; i++) {
		if (ctx->fontImages[i] != 0) {
			nvgp_delete_image(ctx, ctx->fontImages[i]);
			ctx->fontImages[i] = 0;
		}
	}
#endif
    if (ctx->vt != NULL && ctx->vt->destroy != NULL) {
      ctx->vt->destroy(ctx->vt_ctx);
    }
    nvgp_darray_deinit(&ctx->states);
    nvgp_darray_deinit(&ctx->commands);

    NVGP_FREE(ctx->cache.verts);
    nvgp_darray_deinit(&ctx->cache.paths);
    nvgp_darray_deinit(&ctx->cache.points);
    NVGP_FREE(ctx);
  }
}

static nvgp_clear_path_cache(nvgp_path_cache_t* cache) {
  cache->nverts = 0;
  NVGP_MEMSET(cache->bounds, 0x0, sizeof(cache->bounds));
  nvgp_darray_clear(&cache->paths);
  nvgp_darray_clear(&cache->points);
}

void nvgp_begin_frame_ex(nvgp_context_t *ctx, float width, float height, float pixel_ratio, nvgp_bool_t reset) {
  CHECK_OBJECT_IS_NULL(ctx);
  ctx->width = width;
  ctx->height = height;
  ctx->ratio = pixel_ratio;
  ctx->tess_tol = 0.25f / pixel_ratio;
  ctx->dist_tol = 0.01f / pixel_ratio;
  ctx->fringe_width = 1.0f / pixel_ratio;

  if (reset) {
    nvgp_darray_clear(&ctx->states);
    nvgp_darray_clear(&ctx->commands);
    nvgp_clear_path_cache(&ctx->cache);
    nvgp_save(ctx);
    nvgp_reset_curr_state(ctx);
  }

  ctx->vt->begin_frame(ctx->vt_ctx, width, height, pixel_ratio);

  ctx->drawCallCount = 0;
  ctx->fillTriCount = 0;
  ctx->strokeTriCount = 0;
  ctx->textTriCount = 0;
}

void nvgp_begin_frame(nvgp_context_t *ctx, float width, float height, float pixel_ratio) {
  nvgp_begin_frame_ex(ctx, width, height, pixel_ratio, 1);
}

void nvgp_end_frame(nvgp_context_t *ctx) {
  CHECK_OBJECT_IS_NULL(ctx);
  ctx->vt->end_frame(ctx->vt_ctx);
}

void nvgp_save(nvgp_context_t *ctx) {
  nvgp_state_t* state = NULL;
  nvgp_state_t* state_next = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  if (ctx->states.size > 0) {
    state = nvgp_get_state(ctx);
    state_next = nvgp_get_empty_state_by_tail(ctx);
    CHECK_OBJECT_IS_NULL(state);
    CHECK_OBJECT_IS_NULL(state_next);
    NVGP_MEMCPY(state_next, state, sizeof(nvgp_state_t));
  } else {
    nvgp_get_empty_state_by_tail(ctx);
  }
}

void nvgp_restore(nvgp_context_t *ctx) {
  CHECK_OBJECT_IS_NULL(ctx);
  nvgp_darray_pop(&ctx->states);
}

int32_t nvgp_create_image_rgba(nvgp_context_t* ctx, int w, int h, int image_flags, const unsigned char* rgba_data) {
  CHECK_OBJECT_IS_NULL(ctx);
  return ctx->vt->create_texture(ctx->vt_ctx, NVGP_TEXTURE_RGBA, w, h, w * 4, image_flags, rgba_data);
}

void nvgp_get_image_size(nvgp_context_t* ctx, int image, int* w, int* h) {
  CHECK_OBJECT_IS_NULL(ctx);
  ctx->vt->get_texture_size(ctx->vt_ctx, image, w, h);
}

void nvgp_update_image_rgba(nvgp_context_t* ctx, int image, const unsigned char* rgba_data) {
  int32_t w, h;
  CHECK_OBJECT_IS_NULL(ctx);
  nvgp_get_image_size(ctx, image, &w, &h);
  ctx->vt->update_texture(ctx->vt_ctx, image, 0, 0, w, h, rgba_data);
}

int nvgp_create_font_mem(nvgp_context_t* ctx, const char* name, unsigned char* data, int ndata, nvgp_bool_t freeData) {
  CHECK_OBJECT_IS_NULL(ctx);
  return fonsAddFontMem(ctx->fs, name, data, ndata, freeData ? 1 : 0);
}

int nvgp_find_font(nvgp_context_t* ctx, const char* name) {
  CHECK_OBJECT_IS_NULL(ctx);
  if (name == NULL) return -1;
  return fonsGetFontByName(ctx->fs, name);
}

void nvgp_delete_font_by_name(nvgp_context_t* ctx, const char* name) {
#ifdef WITH_NANOVG_PLUS_GPU
  if (ctx->fs) {
    fontsDeleteFontByName(ctx->fs, name);
  }
#endif
}

void nvgp_delete_image(nvgp_context_t* ctx, int image) {
  ctx->vt->delete_texture(ctx->vt_ctx, image);
}

nvgp_bool_t nvgp_clear_cache(nvgp_context_t* ctx) {
  if(ctx->vt->clear_cache != NULL) {
    ctx->vt->clear_cache(ctx->vt_ctx);
  }
  return FALSE;
}

void* nvgp_get_vt_ctx(nvgp_context_t* ctx) {
  CHECK_OBJECT_IS_NULL(ctx);
  return ctx->vt_ctx;
}


/***********************************************************************************************
 *                                      数学公式函数                                            *
 ***********************************************************************************************/

static int nvgp_pt_equals(float x1, float y1, float x2, float y2, float tol) {
  float dx = x2 - x1;
  float dy = y2 - y1;
  return dx * dx + dy * dy < tol * tol;
}

static float nvgp_dist_pt_seg(float x, float y, float px, float py, float qx, float qy) {
  float pqx, pqy, dx, dy, d, t;
  pqx = qx - px;
  pqy = qy - py;
  dx = x - px;
  dy = y - py;
  d = pqx * pqx + pqy * pqy;
  t = pqx * dx + pqy * dy;
  if (d > 0) t /= d;
  if (t < 0) t = 0;
  else if (t > 1) t = 1;
  dx = px + t * pqx - x;
  dy = py + t * pqy - y;
  return dx * dx + dy * dy;
}

static float nvgp_normalize(float *x, float* y) {
  float d = nvgp_sqrtf((*x)*(*x) + (*y)*(*y));
  if (d > 1e-6f) {
    float id = 1.0f / d;
    *x *= id;
    *y *= id;
  }
  return d;
}

static float nvgp_get_average_scale(nvgp_matrix_t* t) {
  float sx = nvgp_sqrtf(t->mat.scale_x * t->mat.scale_x + t->mat.skew_x * t->mat.skew_x);
  float sy = nvgp_sqrtf(t->mat.skew_y * t->mat.skew_y  + t->mat.scale_y * t->mat.scale_y);
  return (sx + sy) * 0.5f;
}

static int32_t nvgp_curve_divs(float r, float arc, float tol) {
  float da = nvgp_acosf(r / (r + tol)) * 2.0f;
  return nvgp_max(2, (int32_t)ceilf(arc / da));
}

static float nvgp_quantize(float a, float d) {
  return ((int32_t)(a / d + 0.5f)) * d;
}

/***********************************************************************************************/





/***********************************************************************************************
 *                                          路径函数                                            *
 ***********************************************************************************************/

static void nvgp_transform_point(floatptr_t* dx, floatptr_t* dy, nvgp_matrix_t* matrix, floatptr_t sx, floatptr_t sy)
{
  // *dx = sx*xform[0] + sy*xform[2] + xform[4];
  // *dy = sx*xform[1] + sy*xform[3] + xform[5];
  *dx = sx * (matrix->mat.scale_x) + sy * (matrix->mat.skew_x) + (matrix->mat.trans_x);
  *dy = sx * (matrix->mat.skew_y) + sy * (matrix->mat.scale_y) + (matrix->mat.trans_y);
}

static void nvgp_append_commands(nvgp_context_t* ctx, floatptr_t* vals, int nvals) {
  uint32_t i;
  nvgp_state_t* state = nvgp_get_state(ctx);
  CHECK_OBJECT_IS_NULL(state);
  if ((int)vals[0] != NVGP_CLOSE && (int)vals[0] != NVGP_WINDING) {
    ctx->command_x = vals[nvals-2];
    ctx->command_y = vals[nvals-1];
  }

  // transform commands
  i = 0;
  while (i < nvals) {
    int cmd = (int)vals[i];
    switch (cmd) {
    case NVGP_MOVETO:
      nvgp_transform_point(&vals[i+1],&vals[i+2], &(state->matrix), vals[i+1],vals[i+2]);
      i += 3;
      break;
    case NVGP_LINETO:
      nvgp_transform_point(&vals[i+1],&vals[i+2], &(state->matrix), vals[i+1],vals[i+2]);
      i += 3;
      break;
    case NVGP_BEZIERTO:
      nvgp_transform_point(&vals[i+1],&vals[i+2], &(state->matrix), vals[i+1],vals[i+2]);
      nvgp_transform_point(&vals[i+3],&vals[i+4], &(state->matrix), vals[i+3],vals[i+4]);
      nvgp_transform_point(&vals[i+5],&vals[i+6], &(state->matrix), vals[i+5],vals[i+6]);
      i += 7;
      break;
    case NVGP_CLOSE:
      i++;
      break;
    case NVGP_WINDING:
      i += 2;
      break;
    default:
      i++;
    }
  }
  nvgp_darray_memcpy(&ctx->commands, floatptr_t, vals, nvals);
}

static void nvgp_add_path(nvgp_context_t* ctx) {
  nvgp_path_t* path;
  path = nvgp_darray_get_empty_by_tail(&ctx->cache.paths, nvgp_path_t);
  NVGP_MEMSET(path, 0, sizeof(nvgp_path_t));
  path->first = ctx->cache.points.size;
  path->winding = NVGP_CCW;
}

static nvgp_path_t* nvgp_last_path(nvgp_context_t* ctx) {
  return nvgp_darray_get_ptr(&ctx->cache.paths, ctx->cache.paths.size - 1, nvgp_path_t);
}

static nvgp_point_t* nvgp_last_point(nvgp_context_t* ctx) {
  return nvgp_darray_get_ptr(&ctx->cache.points, ctx->cache.points.size - 1, nvgp_point_t);
}

static void nvgp_add_point(nvgp_context_t* ctx, float x, float y, int flags) {
  nvgp_path_t* path = nvgp_last_path(ctx);
  nvgp_point_t* pt;
  if (path == NULL) return;

  if (path->count > 0 && ctx->cache.points.size > 0) {
    pt = nvgp_last_point(ctx);
    if (nvgp_pt_equals(pt->x,pt->y, x,y, ctx->dist_tol)) {
      pt->flags |= flags;
      return;
    }
  }

  pt = nvgp_darray_get_empty_by_tail(&ctx->cache.points, nvgp_point_t);
  NVGP_MEMSET(pt, 0, sizeof(nvgp_point_t));
  pt->x = x;
  pt->y = y;
  pt->flags = (unsigned char)flags;

  path->count++;
}

static void nvgp_tesselate_bezier(nvgp_context_t* ctx,
                floatptr_t x1, floatptr_t y1, floatptr_t x2, floatptr_t y2,
                floatptr_t x3, floatptr_t y3, floatptr_t x4, floatptr_t y4,
                int32_t level, int32_t type)
{
  floatptr_t x12,y12,x23,y23,x34,y34,x123,y123,x234,y234,x1234,y1234;
  floatptr_t dx,dy,d2,d3;

  if (level > 10) return;

  x12 = (x1+x2)*0.5f;
  y12 = (y1+y2)*0.5f;
  x23 = (x2+x3)*0.5f;
  y23 = (y2+y3)*0.5f;
  x34 = (x3+x4)*0.5f;
  y34 = (y3+y4)*0.5f;
  x123 = (x12+x23)*0.5f;
  y123 = (y12+y23)*0.5f;

  dx = x4 - x1;
  dy = y4 - y1;
  d2 = nvgp_abs(((x2 - x4) * dy - (y2 - y4) * dx));
  d3 = nvgp_abs(((x3 - x4) * dy - (y3 - y4) * dx));

  if ((d2 + d3)*(d2 + d3) < ctx->tess_tol * (dx*dx + dy*dy)) {
    nvgp_add_point(ctx, x4, y4, type);
    return;
  }

  x234 = (x23+x34)*0.5f;
  y234 = (y23+y34)*0.5f;
  x1234 = (x123+x234)*0.5f;
  y1234 = (y123+y234)*0.5f;

  nvgp_tesselate_bezier(ctx, x1,y1, x12,y12, x123,y123, x1234,y1234, level+1, 0);
  nvgp_tesselate_bezier(ctx, x1234,y1234, x234,y234, x34,y34, x4,y4, level+1, type);
}

static void nvgp_close_last_path(nvgp_context_t* ctx) {
  nvgp_path_t* path = nvgp_last_path(ctx);
  if (path == NULL) {
    return;
  }
  path->closed = 1;
}

static void nvgp_path_winding(nvgp_context_t* ctx, int32_t winding) {
  nvgp_path_t* path = nvgp_last_path(ctx);
  if (path == NULL) return;
  path->winding = winding;
}

void nvgp_begin_path(nvgp_context_t *ctx) {
  CHECK_OBJECT_IS_NULL(ctx);
  nvgp_darray_clear(&ctx->commands);
  nvgp_darray_clear(&ctx->cache.paths);
  nvgp_darray_clear(&ctx->cache.points);
}

void nvgp_close_path(nvgp_context_t *ctx) {
  floatptr_t vals[] = { NVGP_CLOSE };
  nvgp_append_commands(ctx, vals, nvgp_get_arrary_size(vals));
}

void nvgp_move_to(nvgp_context_t *ctx, float x, float y) {
  floatptr_t vals[] = { NVGP_MOVETO, x, y };
  CHECK_OBJECT_IS_NULL(ctx);
  nvgp_append_commands(ctx, vals, nvgp_get_arrary_size(vals));
}

void nvgp_line_to(nvgp_context_t *ctx, float x, float y) {
  floatptr_t vals[] = { NVGP_LINETO, x, y };
  CHECK_OBJECT_IS_NULL(ctx);
  nvgp_append_commands(ctx, vals, nvgp_get_arrary_size(vals));
  }

void nvgp_bezier_to(nvgp_context_t *ctx, float c1x, float c1y, float c2x, float c2y, float x, float y) {
  floatptr_t vals[] = { NVGP_BEZIERTO, c1x, c1y, c2x, c2y, x, y };
  CHECK_OBJECT_IS_NULL(ctx);
  nvgp_append_commands(ctx, vals, nvgp_get_arrary_size(vals));
}

void nvgp_quad_to(nvgp_context_t *ctx, float cx, float cy, float x, float y) {
  floatptr_t x0 = ctx->command_x;
  floatptr_t y0 = ctx->command_y;
  floatptr_t vals[] = { NVGP_BEZIERTO,
                        x0 + 2.0f/3.0f*(cx - x0), y0 + 2.0f/3.0f*(cy - y0),
                        x + 2.0f/3.0f*(cx - x), y + 2.0f/3.0f*(cy - y),
                        x, y };
  CHECK_OBJECT_IS_NULL(ctx);
  nvgp_append_commands(ctx, vals, nvgp_get_arrary_size(vals));
}

void nvgp_arc(nvgp_context_t *ctx, float cx, float cy, float r, float a0, float a1, nvgp_bool_t ccw) {
  floatptr_t vals[3 + 5 * 7 + 100];
  floatptr_t a = 0, da = 0, hda = 0, kappa = 0;
  floatptr_t px = 0, py = 0, ptanx = 0, ptany = 0;
  floatptr_t dx = 0, dy = 0, x = 0, y = 0, tanx = 0, tany = 0;

  int32_t i, ndivs, nvals;
  int32_t move = ctx->commands.size > 0 ? NVGP_LINETO : NVGP_MOVETO;
  CHECK_OBJECT_IS_NULL(ctx);
  // Clamp angles
  da = a1 - a0;
  if (!ccw) {
    if (nvgp_abs(da) >= NVGP_PI * 2.0f) {
      da = NVGP_PI * 2.0f;
    } else {
      while (da < 0.0f) da += NVGP_PI * 2.0f;
    }
  } else {
    if (nvgp_abs(da) >= NVGP_PI * 2.0f) {
      da = -NVGP_PI * 2.0f;
    } else {
      while (da > 0.0f) da -= NVGP_PI * 2.0f;
    }
  }

  // Split arc into max 90 degree segments.
  ndivs = nvgp_max(1, nvgp_min((int)(nvgp_abs(da) / (NVGP_PI * 0.5f) + 0.5f), 5));
  hda = (da / (floatptr_t)ndivs) / 2.0f;
  kappa = nvgp_abs(4.0f / 3.0f * (1.0f - nvgp_cosf(hda)) / nvgp_sinf(hda));

  if (ccw)
    kappa = -kappa;

  nvals = 0;
  for (i = 0; i <= ndivs; i++) {
    a = a0 + da * (i / (floatptr_t)ndivs);
    dx = nvgp_cosf(a);
    dy = nvgp_sinf(a);
    x = cx + dx * r;
    y = cy + dy * r;
    tanx = -dy * r * kappa;
    tany = dx * r * kappa;

    if (i == 0) {
      vals[nvals++] = (float)move;
      vals[nvals++] = x;
      vals[nvals++] = y;
    } else {
      vals[nvals++] = NVGP_BEZIERTO;
      vals[nvals++] = px+ptanx;
      vals[nvals++] = py+ptany;
      vals[nvals++] = x-tanx;
      vals[nvals++] = y-tany;
      vals[nvals++] = x;
      vals[nvals++] = y;
    }
    px = x;
    py = y;
    ptanx = tanx;
    ptany = tany;
  }

  nvgp_append_commands(ctx, vals, nvals);
}

void nvgp_arc_to(nvgp_context_t *ctx, float x1, float y1, float x2, float y2, float radius) {
  int32_t dir;
  float x0 = ctx->command_x;
  float y0 = ctx->command_y;
  float dx0, dy0, dx1, dy1, a, d, cx, cy, a0, a1;
  CHECK_OBJECT_IS_NULL(ctx);
  if (ctx->commands.size == 0) {
    return;
  }

  //  Handle degenerate cases.
  if (nvgp_pt_equals(x0,y0, x1,y1, ctx->dist_tol) ||
      nvgp_pt_equals(x1,y1, x2,y2, ctx->dist_tol) ||
      nvgp_dist_pt_seg(x1,y1, x0,y0, x2,y2) < ctx->dist_tol*ctx->dist_tol ||
      radius < ctx->dist_tol) {
    nvgp_line_to(ctx, x1, y1);
    return;
  }

  //  Calculate tangential circle to lines (x0,y0)-(x1,y1) and (x1,y1)-(x2,y2).
  dx0 = x0 - x1;
  dy0 = y0 - y1;
  dx1 = x2 - x1;
  dy1 = y2 - y1;
  nvgp_normalize(&dx0, &dy0);
  nvgp_normalize(&dx1, &dy1);
  a = nvgp_acosf(dx0 * dx1 + dy0 * dy1);
  d = radius / nvgp_tanf(a/2.0f);

  //    printf("a=%f° d=%f\n", a/NVG_PI*180.0f, d);

  if (d > 10000.0f) {
    nvgp_line_to(ctx, x1,y1);
    return;
  }

  if (nvgp_cross(dx0,dy0, dx1,dy1) > 0.0f) {
    cx = x1 + dx0*d + dy0*radius;
    cy = y1 + dy0*d + -dx0*radius;
    a0 = nvgp_atan2f(dx0, -dy0);
    a1 = nvgp_atan2f(-dx1, dy1);
    dir = 0;
  //    printf("CW c=(%f, %f) a0=%f° a1=%f°\n", cx, cy, a0/NVG_PI*180.0f, a1/NVG_PI*180.0f);
  } else {
    cx = x1 + dx0 * d + -dy0 * radius;
    cy = y1 + dy0 * d + dx0 * radius;
    a0 = nvgp_atan2f(-dx0, dy0);
    a1 = nvgp_atan2f(dx1, -dy1);
    dir = 1;
  //    printf("CCW c=(%f, %f) a0=%f° a1=%f°\n", cx, cy, a0/NVG_PI*180.0f, a1/NVG_PI*180.0f);
  }

  nvgp_arc(ctx, cx, cy, radius, a0, a1, dir);
}

void nvgp_rect(nvgp_context_t *ctx, float x, float y, float w, float h) {
  floatptr_t vals[] = {
    NVGP_MOVETO, x, y,
    NVGP_LINETO, x, y + h,
    NVGP_LINETO, x + w, y + h,
    NVGP_LINETO, x + w, y,
    NVGP_CLOSE
  };
  CHECK_OBJECT_IS_NULL(ctx);
  nvgp_append_commands(ctx, vals, nvgp_get_arrary_size(vals));
}

void nvgp_rounded_rect_varying(nvgp_context_t *ctx, float x,
                                      float y, float w, float h,
                                      float rad_top_left, float rad_top_right,
                                      float rad_bottom_right,
                                      float rad_bottom_left) {
  CHECK_OBJECT_IS_NULL(ctx);
  if(rad_top_left < 0.1f && rad_top_right < 0.1f && rad_bottom_right < 0.1f && rad_bottom_left < 0.1f) {
    nvgp_rect(ctx, x, y, w, h);
    return;
  } else {
    floatptr_t halfw = nvgp_abs(w) * 0.5f;
    floatptr_t halfh = nvgp_abs(h) * 0.5f;
    floatptr_t rxBL = nvgp_min(rad_bottom_left, halfw) * nvgp_signf(w), ryBL = nvgp_min(rad_bottom_left, halfh) * nvgp_signf(h);
    floatptr_t rxBR = nvgp_min(rad_bottom_right, halfw) * nvgp_signf(w), ryBR = nvgp_min(rad_bottom_right, halfh) * nvgp_signf(h);
    floatptr_t rxTR = nvgp_min(rad_top_right, halfw) * nvgp_signf(w), ryTR = nvgp_min(rad_top_right, halfh) * nvgp_signf(h);
    floatptr_t rxTL = nvgp_min(rad_top_left, halfw) * nvgp_signf(w), ryTL = nvgp_min(rad_top_left, halfh) * nvgp_signf(h);
    floatptr_t vals[] = {
      NVGP_MOVETO, x, y + ryTL,
      NVGP_LINETO, x, y + h - ryBL,
      NVGP_BEZIERTO, x, y + h - ryBL*(1 - NVGP_KAPPA90), x + rxBL*(1 - NVGP_KAPPA90), y + h, x + rxBL, y + h,
      NVGP_LINETO, x + w - rxBR, y + h,
      NVGP_BEZIERTO, x + w - rxBR*(1 - NVGP_KAPPA90), y + h, x + w, y + h - ryBR*(1 - NVGP_KAPPA90), x + w, y + h - ryBR,
      NVGP_LINETO, x + w, y + ryTR,
      NVGP_BEZIERTO, x + w, y + ryTR*(1 - NVGP_KAPPA90), x + w - rxTR*(1 - NVGP_KAPPA90), y, x + w - rxTR, y,
      NVGP_LINETO, x + rxTL, y,
      NVGP_BEZIERTO, x + rxTL*(1 - NVGP_KAPPA90), y, x, y + ryTL*(1 - NVGP_KAPPA90), x, y + ryTL,
      NVGP_CLOSE
    };
    nvgp_append_commands(ctx, vals, nvgp_get_arrary_size(vals));
  }
}

void nvgp_rounded_rect(nvgp_context_t *ctx, float x, float y, float w, float h, float r) {
  nvgp_rounded_rect_varying(ctx, x, y, w, h, r, r, r, r);
}

void nvgp_ellipse(nvgp_context_t *ctx, float cx, float cy, float rx, float ry) {
  floatptr_t vals[] = {
    NVGP_MOVETO, cx - rx, cy,
    NVGP_BEZIERTO, cx - rx, cy + ry * NVGP_KAPPA90, cx - rx * NVGP_KAPPA90, cy + ry, cx, cy + ry,
    NVGP_BEZIERTO, cx + rx * NVGP_KAPPA90, cy + ry, cx + rx, cy + ry * NVGP_KAPPA90, cx + rx, cy,
    NVGP_BEZIERTO, cx + rx, cy - ry * NVGP_KAPPA90, cx + rx * NVGP_KAPPA90, cy - ry, cx, cy - ry,
    NVGP_BEZIERTO, cx - rx * NVGP_KAPPA90, cy - ry, cx - rx, cy - ry * NVGP_KAPPA90, cx - rx, cy,
    NVGP_CLOSE
  };
  CHECK_OBJECT_IS_NULL(ctx);
  nvgp_append_commands(ctx, vals, nvgp_get_arrary_size(vals));
}

void nvgp_circle(nvgp_context_t *ctx, float cx, float cy, float r) {
  nvgp_ellipse(ctx, cx, cy, r, r);
}

/***********************************************************************************************/





/***********************************************************************************************
 *                                          矩形相关函数                                        *
 ***********************************************************************************************/

static nvgp_matrix_t* nvgp_matrix_init(nvgp_matrix_t* mat) {
  if (mat != NULL) {
    NVGP_MEMSET(mat, 0x0, sizeof(nvgp_matrix_t));
    mat->mat.pers2 = 1.0f;
    mat->mat.scale_x = 1.0f;
    mat->mat.scale_y = 1.0f;
  }
  return mat;
}

static nvgp_matrix_t* nvgp_transform_multiply(nvgp_matrix_t* t, nvgp_matrix_t* s) {
  // float t0 = t[0] * s[0] + t[1] * s[2];
  // float t2 = t[2] * s[0] + t[3] * s[2];
  // float t4 = t[4] * s[0] + t[5] * s[2] + s[4];
  // t[1] = t[0] * s[1] + t[1] * s[3];
  // t[3] = t[2] * s[1] + t[3] * s[3];
  // t[5] = t[4] * s[1] + t[5] * s[3] + s[5];
  // t[0] = t0;
  // t[2] = t2;
  // t[4] = t4;

  float t0 = t->mat.scale_x * s->mat.scale_x + t->mat.skew_y * s->mat.skew_x;
  float t2 = t->mat.skew_x * s->mat.scale_x + t->mat.scale_y * s->mat.skew_x;
  float t4 = t->mat.trans_x * s->mat.scale_x + t->mat.trans_y * s->mat.skew_x + s->mat.trans_x;
  float skew_y = t->mat.scale_x * s->mat.skew_y + t->mat.skew_y * s->mat.scale_y;
  float scale_y = t->mat.skew_x * s->mat.skew_y + t->mat.scale_y * s->mat.scale_y;
  float trans_y = t->mat.trans_x * s->mat.skew_y + t->mat.trans_y * s->mat.scale_y + s->mat.trans_y;
  float skew_x = t2;
  float scale_x = t0;
  float trans_x = t4;

  s->mat.skew_x = skew_x;
  s->mat.skew_y = skew_y;
  s->mat.scale_x = scale_x;
  s->mat.scale_y = scale_y;
  s->mat.trans_x = trans_x;
  s->mat.trans_y = trans_y;
  return s;
}

nvgp_matrix_t* nvgp_transform_multiply_to_t(nvgp_matrix_t* t, nvgp_matrix_t* s) {
  // float t0 = t[0] * s[0] + t[1] * s[2];
  // float t2 = t[2] * s[0] + t[3] * s[2];
  // float t4 = t[4] * s[0] + t[5] * s[2] + s[4];
  // t[1] = t[0] * s[1] + t[1] * s[3];
  // t[3] = t[2] * s[1] + t[3] * s[3];
  // t[5] = t[4] * s[1] + t[5] * s[3] + s[5];
  // t[0] = t0;
  // t[2] = t2;
  // t[4] = t4;

  float t0 = t->mat.scale_x * s->mat.scale_x + t->mat.skew_y * s->mat.skew_x;
  float t2 = t->mat.skew_x * s->mat.scale_x + t->mat.scale_y * s->mat.skew_x;
  float t4 = t->mat.trans_x * s->mat.scale_x + t->mat.trans_y * s->mat.skew_x + s->mat.trans_x;
  float skew_y = t->mat.scale_x * s->mat.skew_y + t->mat.skew_y * s->mat.scale_y;
  float scale_y = t->mat.skew_x * s->mat.skew_y + t->mat.scale_y * s->mat.scale_y;
  float trans_y = t->mat.trans_x * s->mat.skew_y + t->mat.trans_y * s->mat.scale_y + s->mat.trans_y;
  float skew_x = t2;
  float scale_x = t0;
  float trans_x = t4;

  t->mat.skew_x = skew_x;
  t->mat.skew_y = skew_y;
  t->mat.scale_x = scale_x;
  t->mat.scale_y = scale_y;
  t->mat.trans_x = trans_x;
  t->mat.trans_y = trans_y;
  return t;
}

static void nvgp_transform_rotate(nvgp_matrix_t* mat, float a) {
  float cs = nvgp_cosf(a), sn = nvgp_sinf(a);
  mat->mat.scale_x = cs;
  mat->mat.scale_y = cs;
  mat->mat.skew_x = -sn;
  mat->mat.skew_y  = sn;
  mat->mat.trans_x = 0.0f;
  mat->mat.trans_y = 0.0f;
}

nvgp_bool_t nvgp_transform_inverse(nvgp_matrix_t* inv, const nvgp_matrix_t* t) {
  // double invdet, det = (double)t[0] * t[3] - (double)t[2] * t[1];
  // if (det > -1e-6 && det < 1e-6) {
  //   nvgTransformIdentity(inv);
  //   return 0;
  // }
  // invdet = 1.0 / det;
  // inv[0] = (float)(t[3] * invdet);
  // inv[2] = (float)(-t[2] * invdet);
  // inv[4] = (float)(((double)t[2] * t[5] - (double)t[3] * t[4]) * invdet);
  // inv[1] = (float)(-t[1] * invdet);
  // inv[3] = (float)(t[0] * invdet);
  // inv[5] = (float)(((double)t[1] * t[4] - (double)t[0] * t[5]) * invdet);

  double invdet, det = (double)t->mat.scale_x * t->mat.scale_y - (double)t->mat.skew_x * t->mat.skew_y;
  if (det > -1e-6 && det < 1e-6) {
    nvgp_matrix_init(inv);
    return FALSE;
  }
  invdet = 1.0 / det;
  inv->mat.scale_x  = (float)(t->mat.scale_y * invdet);
  inv->mat.skew_x = (float)(-t->mat.skew_x * invdet);
  inv->mat.trans_x = (float)(((double)t->mat.skew_x * t->mat.trans_y - (double)t->mat.scale_y * t->mat.trans_x) * invdet);
  inv->mat.skew_y = (float)(-t->mat.skew_y * invdet);
  inv->mat.scale_y = (float)(t->mat.scale_x * invdet);
  inv->mat.trans_y = (float)(((double)t->mat.skew_y * t->mat.trans_x - (double)t->mat.scale_x * t->mat.trans_y) * invdet);
  
  return TRUE;
}

void nvgp_transform_translate(nvgp_matrix_t* mat, float tx, float ty) {
  nvgp_matrix_init(mat);
  mat->mat.trans_x = tx;
  mat->mat.trans_y = ty;
}

nvgp_error_t nvgp_translate(nvgp_context_t *ctx, float x, float y) {
  nvgp_matrix_t mat;
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    nvgp_transform_translate(&mat, x, y);
    nvgp_transform_multiply(&mat, &state->matrix);
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_rotate(nvgp_context_t *ctx, float angle) {
  nvgp_matrix_t mat;
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    nvgp_matrix_init(&mat);
    nvgp_transform_rotate(&mat, angle);
    nvgp_transform_multiply(&mat, &state->matrix);
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

void nvgp_transform_scale(nvgp_matrix_t* mat, float sx, float sy) {
  nvgp_matrix_init(mat);
  mat->mat.scale_x = sx;
  mat->mat.scale_y = sy;
}


nvgp_error_t nvgp_scale(nvgp_context_t *ctx, float x, float y) {
  nvgp_matrix_t mat;
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    nvgp_transform_scale(&mat, x, y);
    nvgp_transform_multiply(&mat, &state->matrix);
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_reset_transform(nvgp_context_t *ctx) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    nvgp_matrix_init(&state->matrix);
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_transform(nvgp_context_t *ctx, float a, float b, float c, float d, float e, float f) {
  nvgp_matrix_t mat;
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    nvgp_matrix_init(&mat);
    mat.mat.skew_x = c;
    mat.mat.skew_y = b;
    mat.mat.scale_x = a;
    mat.mat.scale_y = d;
    mat.mat.trans_x = e;
    mat.mat.trans_y = f;
    nvgp_transform_multiply(&mat, &state->matrix);
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

/***********************************************************************************************/





/***********************************************************************************************
 *                                          裁剪相关函数                                        *
 ***********************************************************************************************/
static void nvgp_isect_rects(float* dst,
                             float ax, float ay, float aw, float ah,
                             float bx, float by, float bw, float bh) {
  float minx = nvgp_max(ax, bx);
  float miny = nvgp_max(ay, by);
  float maxx = nvgp_min(ax+aw, bx+bw);
  float maxy = nvgp_min(ay+ah, by+bh);
  dst[0] = minx;
  dst[1] = miny;
  dst[2] = nvgp_max(0.0f, maxx - minx);
  dst[3] = nvgp_max(0.0f, maxy - miny);
}

nvgp_error_t nvgp_scissor(nvgp_context_t* ctx, float x, float y, float w, float h) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    w = nvgp_max(0.0f, w);
    h = nvgp_max(0.0f, h);
    /* 消除着色器精度不够引起的漏出颜色的问题 */
    if (w == 0.0f || h == 0.0f) {
      w = 0.0f;
      h = 0.0f;
    }
    
    nvgp_matrix_init(&state->scissor.matrix);
    state->scissor.matrix.mat.trans_x = x + w * 0.5f;
    state->scissor.matrix.mat.trans_y = y + h * 0.5f;
    nvgp_transform_multiply_to_t(&state->scissor.matrix, &state->matrix);

    state->scissor.extent[0] = w * 0.5f;
    state->scissor.extent[1] = h * 0.5f;
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_get_curr_clip_rect(nvgp_context_t* ctx, float* x, float* y, float* w, float* h) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    float rect[4];
    float ex, ey, tex, tey;
    nvgp_matrix_t pxform, invxorm;

    NVGP_MEMCPY(&pxform, &state->scissor.matrix, sizeof(nvgp_matrix_t));
    ex = state->scissor.extent[0];
    ey = state->scissor.extent[1];
    nvgp_transform_inverse(&invxorm, &state->matrix);
    nvgp_transform_multiply_to_t(&pxform, &invxorm);
    tex = ex * nvgp_abs(pxform.mat.scale_x) + ey * nvgp_abs(pxform.mat.skew_x);
    tey = ex * nvgp_abs(pxform.mat.skew_y) + ey * nvgp_abs(pxform.mat.scale_y);

    *x = pxform.mat.trans_x - tex;
    *y = pxform.mat.trans_y - tey;
    *w = tex * 2;
    *h = tey * 2;

    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_intersect_scissor(nvgp_context_t* ctx, float* x, float* y, float* w, float* h) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    float rect[4];
    float ex, ey, tex, tey;
    nvgp_matrix_t pxform, invxorm;

    // If no previous scissor has been set, set the scissor as current scissor.
    if (state->scissor.extent[0] < 0) {
      nvgp_scissor(ctx, *x, *y, *w, *h);
      return NVGP_OK;
    }

    // Transform the current scissor rect into current transform space.
    // If there is difference in rotation, this will be approximation.
    NVGP_MEMCPY(&pxform, &state->scissor.matrix, sizeof(nvgp_matrix_t));
    ex = state->scissor.extent[0];
    ey = state->scissor.extent[1];
    nvgp_transform_inverse(&invxorm, &state->matrix);
    nvgp_transform_multiply_to_t(&pxform, &invxorm);
    tex = ex * nvgp_abs(pxform.mat.scale_x) + ey * nvgp_abs(pxform.mat.skew_x);
    tey = ex * nvgp_abs(pxform.mat.skew_y) + ey * nvgp_abs(pxform.mat.scale_y);

    // Intersect rects.
    nvgp_isect_rects(rect, pxform.mat.trans_x - tex, pxform.mat.trans_y - tey, tex * 2, tey * 2, *x, *y, *w, *h);

    *x = rect[0];
    *y = rect[1];
    *w = rect[2];
    *h = rect[3];

    nvgp_scissor(ctx, rect[0], rect[1], rect[2], rect[3]);
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

/***********************************************************************************************/





/***********************************************************************************************
 *                                          全局配置函数                                        *
 ***********************************************************************************************/

nvgp_error_t nvgp_set_stroke_color(nvgp_context_t *ctx, nvgp_color_t color) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    nvgp_set_paint_color(&state->stroke, color);
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_set_fill_color(nvgp_context_t *ctx, nvgp_color_t color) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    nvgp_set_paint_color(&state->fill, color);
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_set_shape_anti_alias(nvgp_context_t* ctx, nvgp_bool_t enabled) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    state->shape_anti_alias = enabled;
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_set_miter_limit(nvgp_context_t *ctx, float limit) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    state->miter_limit = limit;
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

static nvgp_error_t nvgp_set_line_cap_by_statue(nvgp_context_t* ctx, nvgp_state_t* state, nvgp_line_cap_t cap) {
  if (state != NULL) {
    state->line_cap = cap;
    if (ctx->vt != NULL && ctx->vt->set_line_cap != NULL) {
      ctx->vt->set_line_cap(ctx->vt_ctx, cap);
    }
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_set_line_cap(nvgp_context_t* ctx, nvgp_line_cap_t cap) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  return nvgp_set_line_cap_by_statue(ctx, state, cap);
}

static nvgp_error_t nvgp_set_line_join_by_statue(nvgp_context_t* ctx, nvgp_state_t* state, nvgp_line_join_t join) {
  if (state != NULL) {
    state->line_join = join;
    if (ctx->vt != NULL && ctx->vt->set_line_join != NULL) {
      ctx->vt->set_line_join(ctx->vt_ctx, join);
    }
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_set_line_join(nvgp_context_t* ctx, nvgp_line_join_t join) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  return nvgp_set_line_join_by_statue(ctx, state, join);
}

nvgp_error_t nvgp_set_stroke_width(nvgp_context_t *ctx, float stroke_width) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    state->stroke_width = stroke_width;
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_set_global_alpha(nvgp_context_t *ctx, uint8_t alpha) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    state->alpha = alpha;
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_text_align(nvgp_context_t* ctx, int32_t align) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    state->text_align = align;
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_font_face_id(nvgp_context_t* ctx, int32_t font_id) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    state->font_id = font_id;
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_set_font_size(nvgp_context_t *ctx, float font_size) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    state->font_size = font_size;
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_set_fill_paint(nvgp_context_t* ctx, nvgp_paint_t paint) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    state->fill = paint;
    nvgp_transform_multiply_to_t(&state->fill.mat, &state->matrix);
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

nvgp_error_t nvgp_set_stroke_paint(nvgp_context_t* ctx, nvgp_paint_t paint) {
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  if (state != NULL) {
    state->stroke = paint;
    nvgp_transform_multiply_to_t(&state->stroke.mat, &state->matrix);
    return NVGP_OK;
  }
  return NVGP_FAIL;
}


/***********************************************************************************************/





/***********************************************************************************************
 *                                          画笔函数                                            *
 ***********************************************************************************************/

nvgp_paint_t nvgp_linear_gradient(nvgp_context_t* ctx, float sx, float sy, float ex, float ey, nvgp_color_t icol, nvgp_color_t ocol) {
  nvgp_paint_t p;
  float dx, dy, d;
  const float large = 1e5;

  (void)ctx;
  NVGP_MEMSET(&p, 0, sizeof(p));

  // Calculate transform aligned to the line
  dx = ex - sx;
  dy = ey - sy;
  d = sqrtf(dx*dx + dy*dy);
  if (d > 0.0001f) {
    dx /= d;
    dy /= d;
  } else {
    dx = 0;
    dy = 1;
  }
  p.mat.mat.skew_x = dx;
  p.mat.mat.skew_y = -dx;
  p.mat.mat.scale_x = dy;
  p.mat.mat.scale_y = dy;
  p.mat.mat.trans_x = sx - dx * large;
  p.mat.mat.trans_y = sy - dy * large;

  p.extent[0] = large;
  p.extent[1] = large + d * 0.5f;

  p.radius = 0.0f;
  p.feather = nvgp_max(1.0f, d);

  p.inner_color = icol;
  p.outer_color = ocol;

  return p;
}

nvgp_paint_t nvgp_radial_gradient(nvgp_context_t* ctx, float cx, float cy, float inr, float outr, nvgp_color_t icol, nvgp_color_t ocol) {
  nvgp_paint_t p;
  float r = (inr + outr) * 0.5f;
  float f = (outr - inr);
  (void)ctx;
  NVGP_MEMSET(&p, 0, sizeof(p));

  nvgp_matrix_init(&p.mat);
  p.mat.mat.trans_x = cx;
  p.mat.mat.trans_y = cy;

  p.extent[0] = r;
  p.extent[1] = r;

  p.radius = r;

  p.feather = nvgp_max(1.0f, f);

  p.inner_color = icol;
  p.outer_color = ocol;

  return p;
}

nvgp_paint_t nvgp_image_pattern(nvgp_context_t* ctx, float cx, float cy, float w, float h, float angle, int image, uint8_t alpha) {
  nvgp_paint_t p;
  (void)ctx;
  NVGP_MEMSET(&p, 0, sizeof(p));

  nvgp_transform_rotate(&p.mat, angle);
  p.mat.mat.trans_x = cx;
  p.mat.mat.trans_y = cy;

  p.extent[0] = w;
  p.extent[1] = h;

  p.image = image;
  p.draw_type = NVGP_IMAGE_DRAW_DEFAULT;

  p.inner_color.color = p.outer_color.color = 0xffffffff;
  p.inner_color.rgba.a = alpha;
  p.outer_color.rgba.a = alpha;

  return p;
}

nvgp_paint_t nvgp_image_pattern_repeat(nvgp_context_t* ctx, float sx, float sy, float sw, float sh, float dw, float dh, float image_w, float image_h, float angle, int image, uint8_t alpha) {
  nvgp_paint_t p;
  float scale_x = dw / sw;
  float scale_y = dh / sh;
  (void)ctx;
  NVGP_MEMSET(&p, 0, sizeof(p));

  nvgp_transform_rotate(&p.mat, angle);

  p.extent[0] = image_w;
  p.extent[1] = image_h;

  p.draw_info[0] = scale_x;
  p.draw_info[1] = scale_y;

  p.draw_image_rect[0] = sx;
  p.draw_image_rect[1] = sy;
  p.draw_image_rect[2] = sw;
  p.draw_image_rect[3] = sh;

  p.image = image;
  p.draw_type = NVGP_IMAGE_DRAW_REPEAT;

  p.inner_color.color = 0xffffffff;
  p.inner_color.rgba.a = alpha;

  return p;
}

/***********************************************************************************************/





/***********************************************************************************************
 *                                          文字相关函数                                        *
 ***********************************************************************************************/

static float nvgp_get_font_scale(nvgp_state_t* state) {
  return nvgp_min(nvgp_quantize(nvgp_get_average_scale(&state->matrix), 0.01f), 4.0f);
}

static void nvgp_render_text(nvgp_context_t* ctx, nvgp_vertex_t* verts, uint32_t nverts) {
  nvgp_state_t* state = nvgp_get_state(ctx);
  nvgp_paint_t paint = state->fill;

  // Render triangles.
  paint.image = ctx->fontImages[ctx->fontImageIdx];

  // Apply global alpha
  paint.inner_color.rgba.a = paint.outer_color.rgba.a * state->alpha / 255;
  paint.outer_color.rgba.a = paint.outer_color.rgba.a * state->alpha / 255;

  ctx->vt->render_draw_text(ctx->vt_ctx, &paint, &state->scissor, verts, nverts);

  ctx->drawCallCount++;
  ctx->textTriCount += nverts / 3;
}

static void nvgp_flush_text_texture(nvgp_context_t* ctx) {
  int32_t dirty[4];

  if (fonsValidateTexture(ctx->fs, dirty)) {
    int fontImage = ctx->fontImages[ctx->fontImageIdx];
    // Update texture
    if (fontImage != 0) {
      int iw, ih;
      const unsigned char* data = fonsGetTextureData(ctx->fs, &iw, &ih);
      int x = dirty[0];
      int y = dirty[1];
      int w = dirty[2] - dirty[0];
      int h = dirty[3] - dirty[1];
      ctx->vt->update_texture(ctx->vt_ctx, fontImage, x, y, w, h, data);
    }
  }
}

static int nvgp_alloc_text_atlas(nvgp_context_t* ctx) {
  int iw, ih;
  nvgp_flush_text_texture(ctx);
  if (ctx->fontImageIdx >= NVGP_MAX_FONTIMAGES-1)
    return 0;
  // if next fontImage already have a texture
  if (ctx->fontImages[ctx->fontImageIdx+1] != 0) {
    nvgp_get_image_size(ctx, ctx->fontImages[ctx->fontImageIdx + 1], &iw, &ih);
  } else { // calculate the new font image size and create it.
    nvgp_get_image_size(ctx, ctx->fontImages[ctx->fontImageIdx], &iw, &ih);
    if (iw > ih) {
      ih *= 2;
    } else {
      iw *= 2;
    }
    if (iw > NVGP_MAX_FONTIMAGE_SIZE || ih > NVGP_MAX_FONTIMAGE_SIZE) {
      iw = ih = NVGP_MAX_FONTIMAGE_SIZE;
    }
    ctx->fontImages[ctx->fontImageIdx+1] = ctx->vt->create_texture(ctx->vt_ctx, NVGP_TEXTURE_ALPHA, iw, ih, 0, 0, NULL);
  }
  ++ctx->fontImageIdx;
  fonsResetAtlas(ctx->fs, iw, ih);
  return 1;
}

void nvgp_text_metrics(nvgp_context_t* ctx, float* ascender, float* descender, float* lineh) {
  nvgp_state_t* state = NULL;
  float scale, invscale, width;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  scale = nvgp_get_font_scale(state) * ctx->ratio;
  invscale = 1.0f / scale;

  if (state->font_id == FONS_INVALID) {
    return;
  } 

  fonsSetSize(ctx->fs, state->font_size * scale);
  fonsSetSpacing(ctx->fs, state->letter_spacing * scale);
  fonsSetBlur(ctx->fs, state->font_blur * scale);
  fonsSetAlign(ctx->fs, state->text_align);
  fonsSetFont(ctx->fs, state->font_id);

  fonsVertMetrics(ctx->fs, ascender, descender, lineh);
  if (ascender != NULL)
    *ascender *= invscale;
  if (descender != NULL)
    *descender *= invscale;
  if (lineh != NULL)
    *lineh *= invscale;
}

float nvgp_text_bounds(nvgp_context_t* ctx, float x, float y, const char* string, const char* end, float* bounds) {
  nvgp_state_t* state = NULL;
  float scale, invscale, width;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  scale = nvgp_get_font_scale(state) * ctx->ratio;
  invscale = 1.0f / scale;

  if (state->font_id == FONS_INVALID) {
    return 0;
  }

  fonsSetSize(ctx->fs, state->font_size*scale);
  fonsSetSpacing(ctx->fs, state->letter_spacing*scale);
  fonsSetBlur(ctx->fs, state->font_blur*scale);
  fonsSetAlign(ctx->fs, state->text_align);
  fonsSetFont(ctx->fs, state->font_id);

  width = fonsTextBounds(ctx->fs, x*scale, y*scale, string, end, bounds);
  if (bounds != NULL) {
    // Use line bounds for height.
    fonsLineBounds(ctx->fs, y*scale, &bounds[1], &bounds[3]);
    bounds[0] *= invscale;
    bounds[1] *= invscale;
    bounds[2] *= invscale;
    bounds[3] *= invscale;
  }
  state->text_end = end;
  state->text_scale = scale;
  state->text_string = string;
  state->text_width = width * invscale;
  return state->text_width;
}

float nvgp_text(nvgp_context_t* ctx, float x, float y, const char* string, const char* end) {
  FONSquad q;
  uint32_t nverts = 0;
  uint32_t cverts = 0;
  nvgp_vertex_t* verts;
  nvgp_state_t* state = NULL;
  FONStextIter iter, prevIter;
  float scale, invscale;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  scale = nvgp_get_font_scale(state) * ctx->ratio;
  invscale = 1.0f / scale;

  if (state->font_id == FONS_INVALID) {
    return x;
  }

  if (end == NULL) {
    end = string + strlen(string);
  }

  fonsSetSize(ctx->fs, state->font_size*scale);
  fonsSetSpacing(ctx->fs, state->letter_spacing*scale);
  fonsSetBlur(ctx->fs, state->font_blur*scale);
  fonsSetAlign(ctx->fs, state->text_align);
  fonsSetFont(ctx->fs, state->font_id);

  cverts = nvgp_max(2, (int32_t)(end - string)) * 6; // conservative estimate.
  verts = nvgp_alloc_temp_verts(ctx, cverts);
  if (verts == NULL) return x;

  fonsTextIterInit(ctx->fs, &iter, x * scale, y * scale, string, end, FONS_GLYPH_BITMAP_REQUIRED);
  prevIter = iter;
  while (fonsTextIterNext(ctx->fs, &iter, &q)) {
    floatptr_t c[4 * 2];
    if (iter.prevGlyphIndex == -1) { // can not retrieve glyph?
      if (nverts != 0) {
        nvgp_render_text(ctx, verts, nverts);
        nverts = 0;
      }
      if (!nvgp_alloc_text_atlas(ctx)) {
        break; // no memory :(
      }
      iter = prevIter;
      fonsTextIterNext(ctx->fs, &iter, &q); // try again
      if (iter.prevGlyphIndex == -1) { // still can not find glyph?
        break;
      }
    }
    prevIter = iter;
    // Transform corners.
    nvgp_transform_point(&c[0],&c[1], &state->matrix, q.x0*invscale, q.y0*invscale);
    nvgp_transform_point(&c[2],&c[3], &state->matrix, q.x1*invscale, q.y0*invscale);
    nvgp_transform_point(&c[4],&c[5], &state->matrix, q.x1*invscale, q.y1*invscale);
    nvgp_transform_point(&c[6],&c[7], &state->matrix, q.x0*invscale, q.y1*invscale);
    // Create triangles
    if (nverts+6 <= cverts) {
      nvgp_vset(&verts[nverts], c[0], c[1], q.s0, q.t0); nverts++;
      nvgp_vset(&verts[nverts], c[4], c[5], q.s1, q.t1); nverts++;
      nvgp_vset(&verts[nverts], c[2], c[3], q.s1, q.t0); nverts++;
      nvgp_vset(&verts[nverts], c[0], c[1], q.s0, q.t0); nverts++;
      nvgp_vset(&verts[nverts], c[6], c[7], q.s0, q.t1); nverts++;
      nvgp_vset(&verts[nverts], c[4], c[5], q.s1, q.t1); nverts++;
    }
  }

  // TODO: add back-end bit to do this just once per frame.
  nvgp_flush_text_texture(ctx);

  nvgp_render_text(ctx, verts, nverts);

  return iter.nextx / scale;
}

/***********************************************************************************************/





/***********************************************************************************************
 *                                          绘图函数                                            *
 ***********************************************************************************************/

static floatptr_t nvgp_triarea2(float ax, float ay, float bx, float by, float cx, float cy) {
  floatptr_t abx = bx - ax;
  floatptr_t aby = by - ay;
  floatptr_t acx = cx - ax;
  floatptr_t acy = cy - ay;
  return acx*aby - abx*acy;
}

static floatptr_t nvgp_poly_area(nvgp_darray_t* points, int32_t first, int32_t npts) {
  int32_t i;
  float area = 0;
  for (i = 2; i < npts; i++) {
    nvgp_point_t* a = nvgp_darray_get_ptr(points, first, nvgp_point_t);
    nvgp_point_t* b = nvgp_darray_get_ptr(points, first + i - 1, nvgp_point_t);
    nvgp_point_t* c = nvgp_darray_get_ptr(points, first + i, nvgp_point_t);
    area += nvgp_triarea2(a->x, a->y, b->x, b->y, c->x, c->y);
  }
  return area * 0.5f;
}

static void nvgp_poly_reverse(nvgp_darray_t* points, int32_t first, int npts) {
  void* tmp = NULL;
  int32_t i = first, j = first + npts - 1;
  assert(points->data_type_size != 0);
  tmp = NVGP_MALLOC( points->data_type_size);
  while (i < j) {
    NVGP_MEMCPY(tmp, points->elms[i], points->data_type_size);
    NVGP_MEMCPY(points->elms[i], points->elms[j], points->data_type_size);
    NVGP_MEMCPY(points->elms[j], tmp, points->data_type_size);
    i++;
    j--;
  }
  NVGP_FREE(tmp);
}

static void nvgp_flatten_paths(nvgp_context_t* ctx) {
  int32_t i, j;
  floatptr_t p_0;
  floatptr_t p_1;
  floatptr_t area;
  nvgp_path_t* path;
  nvgp_point_t* p0;
  nvgp_point_t* p1;
  nvgp_point_t* pts;
  nvgp_point_t* last;
  nvgp_path_cache_t* cache = &ctx->cache;
  nvgp_darray_t* commands = &ctx->commands;

  // Flatten
  i = 0;
  while (i < commands->size) {
    int32_t cmd = (int32_t)nvgp_darray_get(commands, i, floatptr_t);
    switch (cmd) {
      case NVGP_MOVETO:{
        nvgp_add_path(ctx);
        p_0 = nvgp_darray_get(commands, i + 1, floatptr_t);
        p_1 = nvgp_darray_get(commands, i + 2, floatptr_t);
        nvgp_add_point(ctx, p_0, p_1, NVGP_PT_CORNER);
        i += 3;
        break;
      }
      case NVGP_LINETO: {
        p_0 = nvgp_darray_get(commands, i + 1, floatptr_t);
        p_1 = nvgp_darray_get(commands, i + 2, floatptr_t);
        nvgp_add_point(ctx, p_0, p_1, NVGP_PT_CORNER);
        i += 3;
        break;
      }
      case NVGP_BEZIERTO:
        last = nvgp_last_point(ctx);
        if (last != NULL) {
          floatptr_t cp1_0 = nvgp_darray_get(commands, i + 1, floatptr_t);
          floatptr_t cp1_1 = nvgp_darray_get(commands, i + 2, floatptr_t);
          floatptr_t cp2_0 = nvgp_darray_get(commands, i + 3, floatptr_t);
          floatptr_t cp2_1 = nvgp_darray_get(commands, i + 4, floatptr_t);
          p_0 = nvgp_darray_get(commands, i + 5, floatptr_t);
          p_1 = nvgp_darray_get(commands, i + 6, floatptr_t);
          nvgp_tesselate_bezier(ctx, last->x,last->y, cp1_0, cp1_1, cp2_0, cp2_1, p_0, p_1, 0, NVGP_PT_CORNER);
        }
        i += 7;
        break;
      case NVGP_CLOSE:{
        nvgp_close_last_path(ctx);
        i++;
        break;
      }
      case NVGP_WINDING: {
        floatptr_t p = nvgp_darray_get(commands, i + 1, floatptr_t);
        nvgp_path_winding(ctx, (int)p);
        i += 2;
        break;
      }
      default: {
        i++;
      }
    }
  }

  cache->bounds[0] = cache->bounds[1] = 1e6f;
  cache->bounds[2] = cache->bounds[3] = -1e6f;

  // Calculate the direction and length of line segments.
  for (j = 0; j < cache->paths.size; j++) {
    path = nvgp_darray_get_ptr(&cache->paths, j, nvgp_path_t);
    pts = nvgp_darray_get_ptr(&cache->points, path->first, nvgp_point_t);

    // If the first and last points are the same, remove the last, mark as closed path.
    p0 = nvgp_darray_get_ptr(&cache->points, path->first + path->count - 1, nvgp_point_t);
    p1 = nvgp_darray_get_ptr(&cache->points, path->first, nvgp_point_t);
    if (nvgp_pt_equals(p0->x,p0->y, p1->x,p1->y, ctx->dist_tol)) {
      path->count--;
      p0 = nvgp_darray_get_ptr(&cache->points, path->first + path->count - 1, nvgp_point_t);
      path->closed = 1;
    }

    // Enforce winding.
    if (path->count > 2) {
      area = nvgp_poly_area(&cache->points, path->first, path->count);
      if (path->winding == NVGP_CCW && area < 0.0f) {
        nvgp_poly_reverse(&cache->points, path->first, path->count);
        p0 = nvgp_darray_get_ptr(&cache->points, path->first + path->count - 1, nvgp_point_t);
        p1 = nvgp_darray_get_ptr(&cache->points, path->first, nvgp_point_t);
      }
      if (path->winding == NVGP_CW && area > 0.0f) {
        nvgp_poly_reverse(&cache->points, path->first, path->count);
        p0 = nvgp_darray_get_ptr(&cache->points, path->first + path->count - 1, nvgp_point_t);
        p1 = nvgp_darray_get_ptr(&cache->points, path->first, nvgp_point_t);
      }
    }

    for(i = 0; i < path->count; i++) {
      // Calculate segment direction and length
      p0->dx = p1->x - p0->x;
      p0->dy = p1->y - p0->y;
      p0->len = nvgp_normalize(&p0->dx, &p0->dy);
      // Update bounds
      cache->bounds[0] = nvgp_min(cache->bounds[0], p0->x);
      cache->bounds[1] = nvgp_min(cache->bounds[1], p0->y);
      cache->bounds[2] = nvgp_max(cache->bounds[2], p0->x);
      cache->bounds[3] = nvgp_max(cache->bounds[3], p0->y);
      // Advance
      p0 = nvgp_darray_get_ptr(&cache->points, path->first + i, nvgp_point_t);
      p1 = nvgp_darray_get_ptr(&cache->points, path->first + 1 + i, nvgp_point_t);
    }
  }
}

static void nvgp_calculate_joins(nvgp_context_t* ctx, float w, nvgp_line_join_t lineJoin, float miterLimit) {
  int32_t i, j;
  float iw = 0.0f;
  nvgp_path_cache_t* cache = &ctx->cache;

  if (w > 0.0f) {
    iw = 1.0f / w;
  }

  // Calculate which joins needs extra vertices to append, and gather vertex count.
  for (i = 0; i < cache->paths.size; i++) {
    int32_t nleft = 0;
    nvgp_path_t* path =nvgp_darray_get_ptr(&cache->paths, i, nvgp_path_t);
    nvgp_point_t* pts = nvgp_darray_get_ptr(&cache->points, path->first, nvgp_point_t);
    nvgp_point_t* p0 = nvgp_darray_get_ptr(&cache->points, path->first + path->count-1, nvgp_point_t);
    nvgp_point_t* p1 = pts;

    path->nbevel = 0;

    for (j = 0; j < path->count; j++) {
      float dlx0, dly0, dlx1, dly1, dmr2, cross, limit;
      dlx0 = p0->dy;
      dly0 = -p0->dx;
      dlx1 = p1->dy;
      dly1 = -p1->dx;
      // Calculate extrusions
      p1->dmx = (dlx0 + dlx1) * 0.5f;
      p1->dmy = (dly0 + dly1) * 0.5f;
      dmr2 = p1->dmx*p1->dmx + p1->dmy*p1->dmy;
      if (dmr2 > 0.000001f) {
        float scale = 1.0f / dmr2;
        if (scale > 600.0f) {
          scale = 600.0f;
        }
        p1->dmx *= scale;
        p1->dmy *= scale;
      }

      // Clear flags, but keep the corner.
      p1->flags = (p1->flags & NVGP_PT_CORNER) ? NVGP_PT_CORNER : 0;

      // Keep track of left turns.
      cross = p1->dx * p0->dy - p0->dx * p1->dy;
      if (cross > 0.0f) {
        nleft++;
        p1->flags |= NVGP_PT_LEFT;
      }

      // Calculate if we should use bevel or miter for inner join.
      limit = nvgp_max(1.01f, nvgp_min(p0->len, p1->len) * iw);
      if ((dmr2 * limit*limit) < 1.0f)
        p1->flags |= NVGP_PR_INNERBEVEL;

      // Check to see if the corner needs to be beveled.
      if (p1->flags & NVGP_PT_CORNER) {
        if ((dmr2 * miterLimit*miterLimit) < 1.0f || lineJoin == NVGP_BEVEL || lineJoin == NVGP_ROUND) {
          p1->flags |= NVGP_PT_BEVEL;
        }
      }

      if ((p1->flags & (NVGP_PT_BEVEL | NVGP_PR_INNERBEVEL)) != 0)
        path->nbevel++;

      p0 = p1;
      p1 = nvgp_darray_get_ptr(&cache->points, path->first + j + 1, nvgp_point_t);
    }

    path->convex = (nleft == path->count) ? 1 : 0;
  }
}

static void nvgp_vset(nvgp_vertex_t* vtx, float x, float y, float u, float v) {
  vtx->x = x;
  vtx->y = y;
  vtx->u = u;
  vtx->v = v;
}

static void nvgp_choose_bevel(int32_t bevel, nvgp_point_t* p0, nvgp_point_t* p1, float w, float* x0, float* y0, float* x1, float* y1) {
  if (bevel) {
    *x0 = p1->x + p0->dy * w;
    *y0 = p1->y - p0->dx * w;
    *x1 = p1->x + p1->dy * w;
    *y1 = p1->y - p1->dx * w;
  } else {
    *x0 = p1->x + p1->dmx * w;
    *y0 = p1->y + p1->dmy * w;
    *x1 = p1->x + p1->dmx * w;
    *y1 = p1->y + p1->dmy * w;
  }
}

static nvgp_vertex_t* nvgp_bevel_join(nvgp_vertex_t* dst, nvgp_point_t* p0, nvgp_point_t* p1, float lw, float rw, float lu, float ru, float fringe) {
  float rx0,ry0,rx1,ry1;
  float lx0,ly0,lx1,ly1;
  float dlx0 = p0->dy;
  float dly0 = -p0->dx;
  float dlx1 = p1->dy;
  float dly1 = -p1->dx;
  (void)fringe;

  if (p1->flags & NVGP_PT_LEFT) {
    nvgp_choose_bevel(p1->flags & NVGP_PR_INNERBEVEL, p0, p1, lw, &lx0, &ly0, &lx1, &ly1);

    nvgp_vset(dst, lx0, ly0, lu,1); dst++;
    nvgp_vset(dst, p1->x - dlx0 * rw, p1->y - dly0 * rw, ru, 1); dst++;

    if (p1->flags & NVGP_PT_BEVEL) {
      nvgp_vset(dst, lx0, ly0, lu,1); dst++;
      nvgp_vset(dst, p1->x - dlx0 * rw, p1->y - dly0 * rw, ru, 1); dst++;

      nvgp_vset(dst, lx1, ly1, lu,1); dst++;
      nvgp_vset(dst, p1->x - dlx1 * rw, p1->y - dly1 * rw, ru, 1); dst++;
    } else {
      rx0 = p1->x - p1->dmx * rw;
      ry0 = p1->y - p1->dmy * rw;

      nvgp_vset(dst, p1->x, p1->y, 0.5f,1); dst++;
      nvgp_vset(dst, p1->x - dlx0 * rw, p1->y - dly0 * rw, ru, 1); dst++;

      nvgp_vset(dst, rx0, ry0, ru,1); dst++;
      nvgp_vset(dst, rx0, ry0, ru,1); dst++;

      nvgp_vset(dst, p1->x, p1->y, 0.5f,1); dst++;
      nvgp_vset(dst, p1->x - dlx1 * rw, p1->y - dly1 * rw, ru, 1); dst++;
    }

    nvgp_vset(dst, lx1, ly1, lu,1); dst++;
    nvgp_vset(dst, p1->x - dlx1 * rw, p1->y - dly1 * rw, ru, 1); dst++;

  } else {
    nvgp_choose_bevel(p1->flags & NVGP_PR_INNERBEVEL, p0, p1, -rw, &rx0, &ry0, &rx1, &ry1);

    nvgp_vset(dst, p1->x + dlx0 * lw, p1->y + dly0 * lw, lu, 1); dst++;
    nvgp_vset(dst, rx0, ry0, ru,1); dst++;

    if (p1->flags & NVGP_PT_BEVEL) {
      nvgp_vset(dst, p1->x + dlx0 * lw, p1->y + dly0 * lw, lu, 1); dst++;
      nvgp_vset(dst, rx0, ry0, ru,1); dst++;

      nvgp_vset(dst, p1->x + dlx1 * lw, p1->y + dly1 * lw, lu, 1); dst++;
      nvgp_vset(dst, rx1, ry1, ru,1); dst++;
    } else {
      lx0 = p1->x + p1->dmx * lw;
      ly0 = p1->y + p1->dmy * lw;

      nvgp_vset(dst, p1->x + dlx0 * lw, p1->y + dly0 * lw, lu, 1); dst++;
      nvgp_vset(dst, p1->x, p1->y, 0.5f,1); dst++;

      nvgp_vset(dst, lx0, ly0, lu,1); dst++;
      nvgp_vset(dst, lx0, ly0, lu,1); dst++;

      nvgp_vset(dst, p1->x + dlx1 * lw, p1->y + dly1 * lw, lu, 1); dst++;
      nvgp_vset(dst, p1->x, p1->y, 0.5f,1); dst++;
    }

    nvgp_vset(dst, p1->x + dlx1 * lw, p1->y + dly1 * lw, lu, 1); dst++;
    nvgp_vset(dst, rx1, ry1, ru,1); dst++;
  }

  return dst;
}

static nvgp_vertex_t* nvgp_alloc_temp_verts(nvgp_context_t* ctx, int nverts) {
  if (nverts > ctx->cache.cverts) {
    nvgp_vertex_t* verts;
    int cverts = (nverts + 0xff) & ~0xff; // Round up to prevent allocations when things change just slightly.
    verts = NVGP_REALLOCT(nvgp_vertex_t, ctx->cache.verts, cverts);
    if (verts == NULL) return NULL;
    ctx->cache.verts = verts;
    ctx->cache.cverts = cverts;
  }

  return ctx->cache.verts;
}

static nvgp_error_t nvgp_expand_fill(nvgp_context_t* ctx, float w, nvgp_line_join_t lineJoin, float miterLimit) {
  nvgp_vertex_t* dst;
  nvgp_vertex_t* verts;
  int fringe = w > 0.0f;
  int cverts, convex, i, j;
  float aa = ctx->fringe_width;
  nvgp_path_cache_t* cache = &ctx->cache;

  nvgp_calculate_joins(ctx, w, lineJoin, miterLimit);

  // Calculate max vertex usage.
  cverts = 0;
  for (i = 0; i < cache->paths.size; i++) {
    nvgp_path_t* path = nvgp_darray_get_ptr(&cache->paths, i, nvgp_path_t);
    cverts += path->count + path->nbevel + 1;
    if (fringe)
      cverts += (path->count + path->nbevel*5 + 1) * 2; // plus one for loop
  }

  verts = nvgp_alloc_temp_verts(ctx, cverts);
  if (verts == NULL) {
    return NVGP_OOM;
  }

  convex = cache->paths.size == 1 && nvgp_darray_get_ptr(&cache->paths, 0, nvgp_path_t)->convex;

  for (i = 0; i < cache->paths.size; i++) {
    float ru, lu;
    float rw, lw, woff;
    nvgp_point_t* p0;
    nvgp_point_t* p1;
    nvgp_path_t* path = nvgp_darray_get_ptr(&cache->paths, i, nvgp_path_t);
    nvgp_point_t* pts = nvgp_darray_get_ptr(&cache->points, path->first, nvgp_point_t);

    // Calculate shape vertices.
    woff = 0.5f * aa;
    dst = verts;
    path->fill = dst;

    if (fringe) {
      // Looping
      p0 = nvgp_darray_get_ptr(&cache->points, path->first + path->count - 1, nvgp_point_t);
      p1 = nvgp_darray_get_ptr(&cache->points, path->first, nvgp_point_t);
      for (j = 0; j < path->count; ++j) {
        if (p1->flags & NVGP_PT_BEVEL) {
          float dlx0 = p0->dy;
          float dly0 = -p0->dx;
          float dlx1 = p1->dy;
          float dly1 = -p1->dx;
          if (p1->flags & NVGP_PT_LEFT) {
            float lx = p1->x + p1->dmx * woff;
            float ly = p1->y + p1->dmy * woff;
            nvgp_vset(dst, lx, ly, 0.5f,1); dst++;
          } else {
            float lx0 = p1->x + dlx0 * woff;
            float ly0 = p1->y + dly0 * woff;
            float lx1 = p1->x + dlx1 * woff;
            float ly1 = p1->y + dly1 * woff;
            nvgp_vset(dst, lx0, ly0, 0.5f,1); dst++;
            nvgp_vset(dst, lx1, ly1, 0.5f,1); dst++;
          }
        } else {
          nvgp_vset(dst, p1->x + (p1->dmx * woff), p1->y + (p1->dmy * woff), 0.5f,1); dst++;
        }
        p0 = p1;
        p1 = nvgp_darray_get_ptr(&cache->points, path->first + j + 1, nvgp_point_t);
      }
    } else {
      for (j = 0; j < path->count; ++j) {
        nvgp_point_t* p0 = nvgp_darray_get_ptr(&cache->points, path->first + j, nvgp_point_t);
        nvgp_vset(dst, p0->x, p0->y, 0.5f,1);
        dst++;
      }
    }

    path->nfill = (int)(dst - verts);
    verts = dst;

    // Calculate fringe
    if (fringe) {
      lw = w + woff;
      rw = w - woff;
      lu = 0;
      ru = 1;
      dst = verts;
      path->stroke = dst;

      // Create only half a fringe for convex shapes so that
      // the shape can be rendered without stenciling.
      if (convex) {
        lw = woff;  // This should generate the same vertex as fill inset above.
        lu = 0.5f;  // Set outline fade at middle.
      }

      // Looping
      p0 = nvgp_darray_get_ptr(&cache->points, path->first + path->count - 1, nvgp_point_t);
      p1 = nvgp_darray_get_ptr(&cache->points, path->first, nvgp_point_t);

      for (j = 0; j < path->count; ++j) {
        if ((p1->flags & (NVGP_PT_BEVEL | NVGP_PR_INNERBEVEL)) != 0) {
          dst = nvgp_bevel_join(dst, p0, p1, lw, rw, lu, ru, ctx->fringe_width);
        } else {
          nvgp_vset(dst, p1->x + (p1->dmx * lw), p1->y + (p1->dmy * lw), lu,1); dst++;
          nvgp_vset(dst, p1->x - (p1->dmx * rw), p1->y - (p1->dmy * rw), ru,1); dst++;
        }
        p0 = p1;
        p1 = nvgp_darray_get_ptr(&cache->points, path->first + j + 1, nvgp_point_t);
      }

      // Loop it
      nvgp_vset(dst, verts[0].x, verts[0].y, lu,1); dst++;
      nvgp_vset(dst, verts[1].x, verts[1].y, ru,1); dst++;

      path->nstroke = (int)(dst - verts);
      verts = dst;
    } else {
      path->stroke = NULL;
      path->nstroke = 0;
    }
  }

  return NVGP_OK;
}

static nvgp_vertex_t* nvgp_butt_cap_start(nvgp_vertex_t* dst, nvgp_point_t* p,
                                         float dx, float dy, float w, float d,
                                         float aa, float u0, float u1) {
  float px = p->x - dx*d;
  float py = p->y - dy*d;
  float dlx = dy;
  float dly = -dx;
  nvgp_vset(dst, px + dlx*w - dx*aa, py + dly*w - dy*aa, u0,0); dst++;
  nvgp_vset(dst, px - dlx*w - dx*aa, py - dly*w - dy*aa, u1,0); dst++;
  nvgp_vset(dst, px + dlx*w, py + dly*w, u0,1); dst++;
  nvgp_vset(dst, px - dlx*w, py - dly*w, u1,1); dst++;
  return dst;
}

static nvgp_vertex_t* nvgp_round_cap_start(nvgp_vertex_t* dst, nvgp_point_t* p,
                                           float dx, float dy, float w, int ncap,
                                           float aa, float u0, float u1) {
  int32_t i;
  float px = p->x;
  float py = p->y;
  float dlx = dy;
  float dly = -dx;
  (void)aa;
  for (i = 0; i < ncap; i++) {
    float a = i/(float)(ncap-1)*NVGP_PI;
    float ax = cosf(a) * w, ay = sinf(a) * w;
    nvgp_vset(dst, px - dlx*ax - dx*ay, py - dly*ax - dy*ay, u0,1); dst++;
    nvgp_vset(dst, px, py, 0.5f,1); dst++;
  }
  nvgp_vset(dst, px + dlx*w, py + dly*w, u0,1); dst++;
  nvgp_vset(dst, px - dlx*w, py - dly*w, u1,1); dst++;
  return dst;
}

static nvgp_vertex_t* nvgp_round_join(nvgp_vertex_t* dst, nvgp_point_t* p0, nvgp_point_t* p1,
                                      float lw, float rw, float lu, float ru, int ncap,
                                      float fringe) {
  int i, n;
  float dlx0 = p0->dy;
  float dly0 = -p0->dx;
  float dlx1 = p1->dy;
  float dly1 = -p1->dx;
  (void)fringe;

  if (p1->flags & NVGP_PT_LEFT) {
    float lx0,ly0,lx1,ly1,a0,a1;
    nvgp_choose_bevel(p1->flags & NVGP_PR_INNERBEVEL, p0, p1, lw, &lx0,&ly0, &lx1,&ly1);
    a0 = nvgp_atan2f(-dly0, -dlx0);
    a1 = nvgp_atan2f(-dly1, -dlx1);
    if (a1 > a0) {
      a1 -= NVGP_PI*2;
    }

    nvgp_vset(dst, lx0, ly0, lu,1); dst++;
    nvgp_vset(dst, p1->x - dlx0*rw, p1->y - dly0*rw, ru,1); dst++;

    n = nvgp_clamp((int)ceilf(((a0 - a1) / NVGP_PI) * ncap), 2, ncap);
    for (i = 0; i < n; i++) {
      float u = i/(float)(n-1);
      float a = a0 + u*(a1-a0);
      float rx = p1->x + nvgp_cosf(a) * rw;
      float ry = p1->y + nvgp_sinf(a) * rw;
      nvgp_vset(dst, p1->x, p1->y, 0.5f,1); dst++;
      nvgp_vset(dst, rx, ry, ru,1); dst++;
    }

    nvgp_vset(dst, lx1, ly1, lu,1); dst++;
    nvgp_vset(dst, p1->x - dlx1*rw, p1->y - dly1*rw, ru,1); dst++;

  } else {
    float rx0,ry0,rx1,ry1,a0,a1;
    nvgp_choose_bevel(p1->flags & NVGP_PR_INNERBEVEL, p0, p1, -rw, &rx0,&ry0, &rx1,&ry1);
    a0 = nvgp_atan2f(dly0, dlx0);
    a1 = nvgp_atan2f(dly1, dlx1);
    if (a1 < a0) {
      a1 += NVGP_PI*2;
    }

    nvgp_vset(dst, p1->x + dlx0*rw, p1->y + dly0*rw, lu,1); dst++;
    nvgp_vset(dst, rx0, ry0, ru,1); dst++;

    n = nvgp_clamp((int)ceilf(((a1 - a0) / NVGP_PI) * ncap), 2, ncap);
    for (i = 0; i < n; i++) {
      float u = i/(float)(n-1);
      float a = a0 + u*(a1-a0);
      float lx = p1->x + nvgp_cosf(a) * lw;
      float ly = p1->y + nvgp_sinf(a) * lw;
      nvgp_vset(dst, lx, ly, lu,1); dst++;
      nvgp_vset(dst, p1->x, p1->y, 0.5f,1); dst++;
    }

    nvgp_vset(dst, p1->x + dlx1 * rw, p1->y + dly1 * rw, lu, 1); dst++;
    nvgp_vset(dst, rx1, ry1, ru, 1); dst++;

  }
  return dst;
}

static nvgp_vertex_t* nvgp_butt_cap_end(nvgp_vertex_t* dst, nvgp_point_t* p,
                                      float dx, float dy, float w, float d,
                                      float aa, float u0, float u1) {
  float px = p->x + dx*d;
  float py = p->y + dy*d;
  float dlx = dy;
  float dly = -dx;
  nvgp_vset(dst, px + dlx*w, py + dly*w, u0,1); dst++;
  nvgp_vset(dst, px - dlx*w, py - dly*w, u1,1); dst++;
  nvgp_vset(dst, px + dlx*w + dx*aa, py + dly*w + dy*aa, u0,0); dst++;
  nvgp_vset(dst, px - dlx*w + dx*aa, py - dly*w + dy*aa, u1,0); dst++;
  return dst;
}

static nvgp_vertex_t* nvgp_round_cap_end(nvgp_vertex_t* dst, nvgp_point_t* p,
                                       float dx, float dy, float w, int ncap,
                                       float aa, float u0, float u1) {
  int i;
  float px = p->x;
  float py = p->y;
  float dlx = dy;
  float dly = -dx;
  (void)aa;
  nvgp_vset(dst, px + dlx * w, py + dly * w, u0, 1); dst++;
  nvgp_vset(dst, px - dlx * w, py - dly * w, u1, 1); dst++;
  for (i = 0; i < ncap; i++) {
    float a = i/(float)(ncap - 1) * NVGP_PI;
    float ax = nvgp_cosf(a) * w, ay = nvgp_sinf(a) * w;
    nvgp_vset(dst, px, py, 0.5f,1); dst++;
    nvgp_vset(dst, px - dlx * ax + dx * ay, py - dly * ax + dy * ay, u0, 1); dst++;
  }
  return dst;
}

static nvgp_error_t nvgp_expand_stroke(nvgp_context_t* ctx, float w, float fringe, int line_cap, int line_join, float miter_limit) {
  int cverts, i, j;
  float aa = fringe;//ctx->fringeWidth;
  float u0 = 0.0f, u1 = 1.0f;
  nvgp_vertex_t* dst;
  nvgp_vertex_t* verts;
  nvgp_path_cache_t* cache = &ctx->cache;
  int ncap = nvgp_curve_divs(w, NVGP_PI, ctx->tess_tol);  // Calculate divisions per half circle.

  w += aa * 0.5f;

  // Disable the gradient used for antialiasing when antialiasing is not used.
  if (aa == 0.0f) {
    u0 = 0.5f;
    u1 = 0.5f;
  }

  nvgp_calculate_joins(ctx, w, line_join, miter_limit);

  // Calculate max vertex usage.
  cverts = 0;
  for (i = 0; i < cache->paths.size; i++) {
    nvgp_path_t* path = nvgp_darray_get_ptr(&cache->paths, i, nvgp_path_t);
    int loop = (path->closed == 0) ? 0 : 1;
    if (line_join == NVGP_ROUND) {
      cverts += (path->count + path->nbevel * (ncap + 2) + 1) * 2; // plus one for loop
    } else {
      cverts += (path->count + path->nbevel * 5 + 1) * 2; // plus one for loop
    }
    if (loop == 0) {
      // space for caps
      if (line_cap == NVGP_ROUND) {
        cverts += (ncap * 2 + 2) * 2;
      } else {
        cverts += (3 + 3) * 2;
      }
    }
  }

  verts = nvgp_alloc_temp_verts(ctx, cverts);
  if (verts == NULL) {
    return NVGP_OOM;
  }

  for (i = 0; i < cache->paths.size; i++) {
    float dx, dy;
    int32_t s, e, loop, nn, first;
    nvgp_point_t* p0;
    nvgp_point_t* p1;
    nvgp_path_t* path = nvgp_darray_get_ptr(&cache->paths, i, nvgp_path_t);
    nvgp_point_t* pts = nvgp_darray_get_ptr(&cache->points, path->first, nvgp_point_t);

    path->fill = 0;
    path->nfill = 0;

    // Calculate fringe or stroke
    loop = (path->closed == 0) ? 0 : 1;
    dst = verts;
    path->stroke = dst;

    if (loop) {
      // Looping
      p0 = nvgp_darray_get_ptr(&cache->points, path->first + path->count - 1, nvgp_point_t);
      p1 = nvgp_darray_get_ptr(&cache->points, path->first, nvgp_point_t);
      s = 0;
      first = path->first;
      e = path->count;
    } else {
      // Add cap
      p0 = nvgp_darray_get_ptr(&cache->points, path->first, nvgp_point_t);
      p1 = nvgp_darray_get_ptr(&cache->points, path->first + 1, nvgp_point_t);
      first = path->first + 1;
      s = 1;
      e = path->count - 1;
    }

    if (loop == 0) {
      // Add cap
      dx = p1->x - p0->x;
      dy = p1->y - p0->y;
      nvgp_normalize(&dx, &dy);
      if (line_cap == NVGP_BUTT)
        dst = nvgp_butt_cap_start(dst, p0, dx, dy, w, -aa * 0.5f, aa, u0, u1);
      else if (line_cap == NVGP_BUTT || line_cap == NVGP_SQUARE)
        dst = nvgp_butt_cap_start(dst, p0, dx, dy, w, w-aa, aa, u0, u1);
      else if (line_cap == NVGP_ROUND)
        dst = nvgp_round_cap_start(dst, p0, dx, dy, w, ncap, aa, u0, u1);
    }

    for (j = s, nn = 1; j < e; ++j, ++nn) {
      if ((p1->flags & (NVGP_PT_BEVEL | NVGP_PR_INNERBEVEL)) != 0) {
        if (line_join == NVGP_ROUND) {
          dst = nvgp_round_join(dst, p0, p1, w, w, u0, u1, ncap, aa);
        } else {
          dst = nvgp_bevel_join(dst, p0, p1, w, w, u0, u1, aa);
        }
      } else {
        nvgp_vset(dst, p1->x + (p1->dmx * w), p1->y + (p1->dmy * w), u0,1); dst++;
        nvgp_vset(dst, p1->x - (p1->dmx * w), p1->y - (p1->dmy * w), u1,1); dst++;
      }
      p0 = p1;
      p1 = nvgp_darray_get_ptr(&cache->points, first + nn, nvgp_point_t);
    }

    if (loop) {
      // Loop it
      nvgp_vset(dst, verts[0].x, verts[0].y, u0,1); dst++;
      nvgp_vset(dst, verts[1].x, verts[1].y, u1,1); dst++;
    } else {
      // Add cap
      dx = p1->x - p0->x;
      dy = p1->y - p0->y;
      nvgp_normalize(&dx, &dy);
      if (line_cap == NVGP_BUTT)
        dst = nvgp_butt_cap_end(dst, p1, dx, dy, w, -aa * 0.5f, aa, u0, u1);
      else if (line_cap == NVGP_BUTT || line_cap == NVGP_SQUARE)
        dst = nvgp_butt_cap_end(dst, p1, dx, dy, w, w - aa, aa, u0, u1);
      else if (line_cap == NVGP_ROUND)
        dst = nvgp_round_cap_end(dst, p1, dx, dy, w, ncap, aa, u0, u1);
    }

    path->nstroke = (int)(dst - verts);

    verts = dst;
  }

  return 1;
}

nvgp_error_t nvgp_fill(nvgp_context_t* ctx) {
  int32_t i;
  nvgp_paint_t fill_paint;
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  CHECK_OBJECT_IS_NULL(state);
  fill_paint = state->fill;

  nvgp_flatten_paths(ctx);
  if (ctx->vt->get_edge_anti_alias(ctx->vt_ctx) && state->shape_anti_alias)
    nvgp_expand_fill(ctx, ctx->fringe_width, NVGP_MITER, 2.4f);
  else
    nvgp_expand_fill(ctx, 0.0f, NVGP_MITER, 2.4f);

  fill_paint.inner_color.rgba.a = fill_paint.inner_color.rgba.a * state->alpha / 255;
  fill_paint.outer_color.rgba.a = fill_paint.outer_color.rgba.a * state->alpha / 255;

  ctx->vt->render_fill(ctx->vt_ctx, &fill_paint, &state->scissor, ctx->fringe_width, ctx->cache.bounds, &ctx->cache.paths);

  // Count triangles
  for (i = 0; i < ctx->cache.paths.size; i++) {
    const nvgp_path_t* path = nvgp_darray_get_ptr(&ctx->cache.paths, i, nvgp_path_t);
    ctx->fillTriCount += path->nfill-2;
    ctx->fillTriCount += path->nstroke-2;
    ctx->drawCallCount += 2;
  }

  return NVGP_OK;
}

nvgp_error_t nvgp_stroke(nvgp_context_t* ctx) {
  int32_t i;
  float scale;
  float stroke_width;
  nvgp_paint_t stroke_paint;
  nvgp_state_t* state = NULL;
  CHECK_OBJECT_IS_NULL(ctx);
  state = nvgp_get_state(ctx);
  CHECK_OBJECT_IS_NULL(state);
  scale = nvgp_get_average_scale(&state->matrix);
  stroke_width = nvgp_clamp(state->stroke_width * scale, 0.0f, 200.0f);
  stroke_paint = state->stroke;


  if (stroke_width < ctx->fringe_width) {
    // If the stroke width is less than pixel size, use alpha to emulate coverage.
    // Since coverage is area, scale by alpha*alpha.
    float alpha = nvgp_clamp(stroke_width / ctx->fringe_width, 0.0f, 1.0f);
    stroke_paint.inner_color.rgba.a = (uint8_t)(stroke_paint.inner_color.rgba.a * alpha * alpha);
    stroke_paint.outer_color.rgba.a = (uint8_t)(stroke_paint.outer_color.rgba.a * alpha * alpha);
    stroke_width = ctx->fringe_width;
  }

  // Apply global alpha
  stroke_paint.inner_color.rgba.a = stroke_paint.inner_color.rgba.a * state->alpha / 255;
  stroke_paint.outer_color.rgba.a = stroke_paint.outer_color.rgba.a * state->alpha / 255;

  nvgp_flatten_paths(ctx);

  if (ctx->vt->get_edge_anti_alias(ctx->vt_ctx) && state->shape_anti_alias) {
    nvgp_expand_stroke(ctx, stroke_width * 0.5f, ctx->fringe_width, state->line_cap, state->line_join, state->miter_limit);
  } else {
    nvgp_expand_stroke(ctx, stroke_width * 0.5f, 0.0f, state->line_cap, state->line_join, state->miter_limit);
  }

  ctx->vt->render_stroke(ctx->vt_ctx, &stroke_paint, &state->scissor, ctx->fringe_width, stroke_width, &ctx->cache.paths);

  // Count triangles
  for (i = 0; i < ctx->cache.paths.size; i++) {
    const nvgp_path_t* path = nvgp_darray_get_ptr(&ctx->cache.paths, i, nvgp_path_t);
    ctx->strokeTriCount += path->nstroke - 2;
    ctx->drawCallCount++;
  }
  return NVGP_OK;
}

/***********************************************************************************************/

