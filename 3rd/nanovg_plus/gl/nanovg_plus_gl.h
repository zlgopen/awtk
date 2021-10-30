/**
 * File:   nanovg_plus_gl.h
 * Author: AWTK Develop Team
 * Brief:  nanovg plus by opengl.
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

#ifndef __NANOVG_PLUS_GL_H__
#define __NANOVG_PLUS_GL_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "../base/nanovg_plus.h"

#ifndef NVGP_GL_INIT_CALL_NUMBER 
#define NVGP_GL_INIT_CALL_NUMBER  128
#endif

#ifndef NVGP_GL_INIT_PATH_NUMBER 
#define NVGP_GL_INIT_PATH_NUMBER  128
#endif

typedef struct _nvgp_gl_context_t nvgp_gl_context_t;

typedef enum _nvgp_gl_flag_t {
  // Flag indicating if geometry based anti-aliasing is used (may not be needed when using MSAA).
  NVGP_GL_FLAG_ANTIALIAS = 1 << 0,
  // Flag indicating if strokes should be drawn using stencil buffer. The rendering will be a little
  // slower, but path overlaps (i.e. self-intersecting or sharp turns) will be drawn just once.
  NVGP_GL_FLAG_STENCIL_STROKES = 1 << 1,
  // Flag indicating that additional debug checks are done.
  NVGP_GL_FLAG_DEBUG = 1 << 2,
} nvgp_gl_flag_t;

typedef enum _nvgp_gl_image_flags_t {
  NVGP_GL_IMAGE_GENERATE_MIPMAPS	= 1<<0,     // Generate mipmaps during creation of the image.
  NVGP_GL_IMAGE_REPEATX			= 1<<1,		// Repeat image in X direction.
  NVGP_GL_IMAGE_REPEATY			= 1<<2,		// Repeat image in Y direction.
  NVGP_GL_IMAGE_FLIPY				= 1<<3,		// Flips (inverses) image in Y direction when rendered.
  NVGP_GL_IMAGE_PREMULTIPLIED		= 1<<4,		// Image data has premultiplied alpha.
  NVGP_GL_IMAGE_NEAREST			= 1<<5,		// Image interpolation is Nearest instead Linear
}nvg_gl_image_flags_t;

typedef enum _nvgp_gl_composite_operation_t {
  NVGP_GL_SOURCE_OVER,
  NVGP_GL_SOURCE_IN,
  NVGP_GL_SOURCE_OUT,
  NVGP_GL_ATOP,
  NVGP_GL_DESTINATION_OVER,
  NVGP_GL_DESTINATION_IN,
  NVGP_GL_DESTINATION_OUT,
  NVGP_GL_DESTINATION_ATOP,
  NVGP_GL_LIGHTER,
  NVGP_GL_COPY,
  NVGP_GL_XOR,
} nvgp_gl_composite_operation_t;

typedef struct _nvgp_gl_color_t {
	union {
		float rgba[4];
		struct {
			float r,g,b,a;
		};
	};
} nvgp_gl_color_t;

const nvgp_vtable_t* nvgp_gl_vtable();
nvgp_gl_context_t* nvgp_gl_create(int flags);

int32_t nvgp_gl_get_gpu_texture_id(nvgp_gl_context_t* gl, int32_t image_id);

#ifdef __cplusplus
}
#endif
#endif
