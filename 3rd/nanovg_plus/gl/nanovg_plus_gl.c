/**
 * File:   nanovg_plus_gl.c
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

#ifdef WITHOUT_GLAD
#include <SDL.h>

#ifdef IOS
#include <OpenGLES/gltypes.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#endif /*IOS*/

#else
#include <glad/glad.h>
#endif /*WITHOUT_GLAD*/

#include "nanovg_plus_gl.h"

#ifdef NVGP_GL3
#define NVGP_GL_USE_UNIFORMBUFFER 1
#endif

#define UNIFORM_OFFSET_ALIGNMENT  4

#define NVGP_GL_USE_STATE_FILTER 1

typedef enum _nvgp_gl_shader_type_t {
  NVGP_GL_SHADER_FILLGRAD,
  NVGP_GL_SHADER_FILLCOLOR,
  NVGP_GL_SHADER_FILLGLYPH,
  NVGP_GL_SHADER_FAST_FILLGLYPH,
  NVGP_GL_SHADER_FILLIMG,
  NVGP_GL_SHADER_FAST_FILLIMG,
  NVGP_GL_SHADER_FILLIMG_RGBA,
  NVGP_GL_SHADER_FAST_FILLIMG_RGBA,
  NVGP_GL_SHADER_REPEAT_FILLIMG,
  NVGP_GL_SHADER_FAST_REPEAT_FILLIMG,
  NVGP_GL_SHADER_REPEAT_FILLIMG_RGBA,
  NVGP_GL_SHADER_FAST_REPEAT_FILLIMG_RGBA,

  NVGP_GL_SHADER_FILL_STROKE,
  NVGP_GL_SHADER_FAST_FILL_COLOR,

  NVGP_GL_SHADER_COUNT,
}nvgp_gl_shader_type_t;

typedef enum _nvgp_gl_call_type_t {
  NVGP_GL_CALL_FAST_FILL_RECT,
  NVGP_GL_CALL_CONVEX_FILL,
  NVGP_GL_CALL_FILL,
  NVGP_GL_CALL_FAST_FILL_COLOR,
  NVGP_GL_CALL_IMAGE,
  NVGP_GL_CALL_STROKE,
  NVGP_GL_CALL_STROKE_IMAGE,
  NVGP_GL_CALL_TEXT,
} nvgp_gl_call_type_t;

typedef enum _nvgp_gl_uniform_loc_t { 
  NVGP_GL_LOC_VIEWSIZE, 
  NVGP_GL_LOC_TEX, 
  NVGP_GL_LOC_FRAG, 
  NVGP_GL_MAX_LOCS 
}nvgp_gl_uniform_loc_t;

typedef enum _nvgp_gl_uniform_bindings_t {
  NVGP_GL_FRAG_BINDING = 0,
}nvgp_gl_uniform_bindings_t;

typedef struct _nvgp_gl_texture_t {
  int32_t id;
  GLuint tex;
  uint32_t width, height;
  int32_t type;
  int32_t flags;
  const void* data;
} nvgp_gl_texture_t;

typedef struct _nvgp_gl_frag_uniforms_t {
#if NVGP_GL_USE_UNIFORMBUFFER
  float scissorMat[12];  // matrices are actually 3 vec4s
  float paintMat[12];
  nvgp_gl_color_t innerCol;
  union {
    nvgp_gl_color_t outerCol;
    float draw_image_rect[4];
  } other_info;
  float scissorExt[2];
  float scissorScale[2];
  float extent[2];
  float radius;
  float feather;
  float strokeMult;
  float strokeThr;
  float draw_info[2];
#else
// note: after modifying layout or size of uniform array,
// don't forget to also update the fragment shader source!
#define NVGP_GL_UNIFORMARRAY_SIZE 11
  union {
    struct {
      float scissorMat[12];  // matrices are actually 3 vec4s
      float paintMat[12];
      nvgp_gl_color_t innerCol;
      union {
        nvgp_gl_color_t outerCol;
        float draw_image_rect[4];
      } other_info;
      float scissorExt[2];
      float scissorScale[2];
      float extent[2];
      float radius;
      float feather;
      float strokeMult;
      float strokeThr;
      float draw_info[2];
    };
    float uniformArray[NVGP_GL_UNIFORMARRAY_SIZE][4];
  };
#endif
} nvgp_gl_frag_uniforms_t;

typedef struct _nvgp_gl_blend_t {
  GLenum src_rgb;
  GLenum dst_rgb;
  GLenum src_alpha;
  GLenum dst_alpha;
} nvgp_gl_blend_t;

typedef struct _nvgp_gl_path_t {
  uint32_t fill_offset;
  uint32_t fill_count;
  uint32_t stroke_offset;
  uint32_t stroke_count;
} nvgp_gl_path_t;

typedef struct _nvgp_gl_shader_t {
  GLuint prog;
  GLuint frag;
  GLuint vert;
  GLint loc[NVGP_GL_MAX_LOCS];
  
  
#ifdef NVGP_GL3
  GLuint vert_arr;
#endif
#ifdef NVGP_GL_USE_UNIFORMBUFFER
  GLuint frag_buf;
#endif
  int32_t frag_size;
  GLuint vert_buf;

  uint32_t setted_data;
  uint32_t cverts;
  uint32_t nverts;
  nvgp_vertex_t* verts;

  uint8_t* uniforms;
  int cuniforms;
  int nuniforms;
} nvgp_gl_shader_t;

typedef struct _nvgp_gl_call_t {
  nvgp_gl_shader_type_t shader_type;
  nvgp_gl_call_type_t call_type;
} nvgp_gl_call_t;

typedef struct _nvgp_gl_call_fast_fill_rect_t {
  nvgp_gl_call_t base;
  float x;
  float y;
  float w;
  float h;
  nvgp_gl_color_t color;
} nvgp_gl_call_fast_fill_rect_t;

typedef struct _nvgp_gl_call_convex_fill_t {
  nvgp_gl_call_t base;
  uint32_t path_index;
  uint32_t path_count;
  uint32_t uniform_offset;
  nvgp_gl_blend_t blend_func;
} nvgp_gl_call_convex_fill_t;

typedef struct _nvgp_gl_call_fast_fill_t {
  nvgp_gl_call_t base;
  GLenum mode;
  uint32_t path_index;
  uint32_t path_count;
  uint32_t uniform_offset;
} nvgp_gl_call_fast_fill_t;

typedef struct _nvgp_gl_call_fill_t {
  nvgp_gl_call_t base;
  uint32_t path_index;
  uint32_t path_count;
  uint32_t triangle_offset;
  uint32_t triangle_count;
  uint32_t uniform_offset;
  nvgp_gl_blend_t blend_func;
} nvgp_gl_call_fill_t;

typedef struct _nvgp_gl_call_stroke_t {
  nvgp_gl_call_t base;
  uint32_t path_index;
  uint32_t path_count;
  uint32_t triangle_offset;
  uint32_t triangle_count;
  uint32_t uniform_offset;
  nvgp_gl_blend_t blend_func;
} nvgp_gl_call_stroke_t;

typedef struct _nvgp_gl_call_stroke_image_t {
  nvgp_gl_call_t base;
  uint32_t path_index;
  uint32_t path_count;
  uint32_t triangle_offset;
  uint32_t triangle_count;
  uint32_t uniform_offset;
  int32_t image;
  nvgp_gl_blend_t blend_func;
} nvgp_gl_call_stroke_image_t;

typedef struct _nvgp_gl_call_image_t {
  nvgp_gl_call_t base;
  uint32_t path_index;
  uint32_t path_count;
  uint32_t triangle_offset;
  uint32_t triangle_count;
  uint32_t uniform_offset;
  int32_t image;
  nvgp_gl_blend_t blend_func;
} nvgp_gl_call_image_t;

typedef struct _nvgp_gl_call_text_t {
  nvgp_gl_call_t base;
  int32_t image;
  uint32_t triangle_offset;
  uint32_t triangle_count;
  uint32_t uniform_offset;
  nvgp_gl_blend_t blend_func;
} nvgp_gl_call_text_t;

typedef struct _nvgp_gl_context_t {
  int32_t flags;
  int32_t edge_anti_alias;
  float view[2];
  float pixel_ratio;
  float pixel_scale;
  nvgp_gl_shader_t shader_list[NVGP_GL_SHADER_COUNT];

  nvgp_darray_t calls;
  nvgp_darray_t paths;
  nvgp_darray_t textures;
  uint32_t texture_id;
  nvgp_line_cap_t line_cap;

// cached state
  GLuint curr_prog;
  GLuint bound_texture;
  GLuint stencil_mask;
  GLenum stencil_func;
  GLint stencil_func_ref;
  GLuint stencil_func_mask;
  nvgp_gl_blend_t blend_func;
} nvgp_gl_context_t;


#include "shader_prog/fill_vert_shader.inc"
#include "shader_prog/fill_frag_shader_0.inc"
#include "shader_prog/fill_frag_shader_1.inc"
#include "shader_prog/fill_frag_shader_2.inc"
#include "shader_prog/fill_frag_shader_4.inc"
#include "shader_prog/fill_frag_shader_3.inc"
#include "shader_prog/fill_frag_shader_5.inc"
#include "shader_prog/fill_frag_shader_6.inc"
#include "shader_prog/fill_frag_shader_7.inc"
#include "shader_prog/fill_frag_shader_8.inc"
#include "shader_prog/fill_frag_shader_9.inc"
#include "shader_prog/fill_frag_shader_10.inc"
#include "shader_prog/fill_frag_shader_11.inc"
#include "shader_prog/fill_frag_shader_12.inc"
#include "shader_prog/fill_frag_shader_13.inc"


static nvgp_gl_color_t nvgp_gl_to_color(nvgp_color_t color) {
  nvgp_gl_color_t gl_color;
  gl_color.r = color.rgba.r / 255.0f;
  gl_color.g = color.rgba.g / 255.0f;
  gl_color.b = color.rgba.b / 255.0f;
  gl_color.a = color.rgba.a / 255.0f;
  return gl_color;
}

static nvgp_gl_color_t nvgp_gl_to_premul_color(nvgp_color_t color) {
  nvgp_gl_color_t gl_color;
  gl_color.a = color.rgba.a / 255.0f;
  gl_color.r = color.rgba.r / 255.0f * gl_color.a;
  gl_color.g = color.rgba.g / 255.0f * gl_color.a;
  gl_color.b = color.rgba.b / 255.0f * gl_color.a;
  return gl_color;
}

static void nvgp_gl_vset(nvgp_vertex_t* vtx, float x, float y, float u, float v) {
  vtx->x = x;
  vtx->y = y;
  vtx->u = u;
  vtx->v = v;
}

static void nvgp_gl_call_destroy(void* data, void* ctx) {
  if (data != NULL) {
    NVGP_FREE(data);
  }
}

static void nvgp_gl_texture_destroy(void* data, void* ctx) {
  if (data != NULL) {
    nvgp_gl_texture_t* texture = (nvgp_gl_texture_t*)data;
    glDeleteTextures(1, &texture->tex);
  }
}

static void nvgp_gl_stencil_mask(nvgp_gl_context_t* gl, GLuint mask) {
  if (gl->stencil_mask != mask) {
    gl->stencil_mask = mask;
    glStencilMask(mask);
  }
}

static void nvgp_gl_stencil_func(nvgp_gl_context_t* gl, GLenum func, GLint ref, GLuint mask) {
  if ((gl->stencil_func != func) || (gl->stencil_func_ref != ref) || (gl->stencil_func_mask != mask)) {
    gl->stencil_func = func;
    gl->stencil_func_ref = ref;
    gl->stencil_func_mask = mask;
    glStencilFunc(func, ref, mask);
  }
}

static void nvgp_gl_blend_func_separate(nvgp_gl_context_t* gl, const nvgp_gl_blend_t* blend) {
  if ((gl->blend_func.src_rgb != blend->src_rgb) || (gl->blend_func.dst_rgb != blend->dst_rgb) ||
      (gl->blend_func.src_alpha != blend->src_alpha) || (gl->blend_func.dst_alpha != blend->dst_alpha)) {
    gl->blend_func = *blend;
    glBlendFuncSeparate(blend->src_rgb, blend->dst_rgb, blend->src_alpha, blend->dst_alpha);
  }
}

static nvgp_gl_blend_t nvgp_gl_composite_operation_state(nvgp_gl_composite_operation_t op){
  nvgp_gl_blend_t state;
  GLenum sfactor, dfactor;

  switch (op) {
    case NVGP_GL_SOURCE_OVER:
      sfactor = GL_ONE;
      dfactor = GL_ONE_MINUS_SRC_ALPHA;
      break;
    case NVGP_GL_SOURCE_IN:
      sfactor = GL_DST_ALPHA;
      dfactor = GL_ZERO;
      break;
    case NVGP_GL_SOURCE_OUT:
      sfactor = GL_ONE_MINUS_DST_ALPHA;
      dfactor = GL_ZERO;
      break;
    case NVGP_GL_ATOP:
      sfactor = GL_DST_ALPHA;
      dfactor = GL_ONE_MINUS_SRC_ALPHA;
      break;
    case NVGP_GL_DESTINATION_OVER:
      sfactor = GL_ONE_MINUS_DST_ALPHA;
      dfactor = GL_ONE;
      break;
    case NVGP_GL_DESTINATION_IN:
      sfactor = GL_ZERO;
      dfactor = GL_SRC_ALPHA;
      break;
    case NVGP_GL_DESTINATION_OUT:
      sfactor = GL_ZERO;
      dfactor = GL_ONE_MINUS_SRC_ALPHA;
      break;
    case NVGP_GL_DESTINATION_ATOP:
      sfactor = GL_ONE_MINUS_DST_ALPHA;
      dfactor = GL_SRC_ALPHA;
      break;
    case NVGP_GL_LIGHTER:
      sfactor = GL_ONE;
      dfactor = GL_ONE;
      break;
    case NVGP_GL_COPY:
      sfactor = GL_ONE;
      dfactor = GL_ZERO;
      break;
    case NVGP_GL_XOR:
      sfactor = GL_ONE_MINUS_DST_ALPHA;
      dfactor = GL_ONE_MINUS_SRC_ALPHA;
      break;
    default:
      sfactor = GL_ONE;
      dfactor = GL_ZERO;
      break;
  }

  state.src_rgb = sfactor;
  state.dst_rgb = dfactor;
  state.src_alpha = sfactor;
  state.dst_alpha = dfactor;
  if (state.src_rgb == GL_INVALID_ENUM || state.dst_rgb == GL_INVALID_ENUM ||
      state.src_alpha == GL_INVALID_ENUM || state.dst_alpha == GL_INVALID_ENUM) {
    state.src_rgb = GL_ONE;
    state.dst_rgb = GL_ONE_MINUS_SRC_ALPHA;
    state.src_alpha = GL_ONE;
    state.dst_alpha = GL_ONE_MINUS_SRC_ALPHA;
  }
  return state;
}

static int nvgp_gl_alloc_verts(nvgp_gl_shader_t* shader, int32_t n) {
  int32_t ret = 0;
  if (shader->nverts + n > shader->cverts) {
    nvgp_vertex_t* verts;
    int32_t cverts = nvgp_max(shader->nverts + n, 4096) + shader->cverts / 2;  // 1.5x Overallocate
    verts = NVGP_REALLOCT(nvgp_vertex_t, shader->verts, cverts);
    if (verts == NULL) {
      return -1;
    }
    shader->verts = verts;
    shader->cverts = cverts;
  }
  ret = shader->nverts;
  shader->nverts += n;
  return ret;
}

static int nvgp_gl_alloc_frag_uniforms(nvgp_gl_shader_t* shader, int n) {
  int32_t ret = 0, structSize = shader->frag_size;
  if (shader->nuniforms + n > shader->cuniforms) {
    uint8_t* uniforms;
    int cuniforms = nvgp_max(shader->nuniforms + n, 128) + shader->cuniforms / 2;  // 1.5x Overallocate
    uniforms = NVGP_REALLOCT(uint8_t, shader->uniforms, structSize * cuniforms);
    if (uniforms == NULL) return -1;
    shader->uniforms = uniforms;
    shader->cuniforms = cuniforms;
  }
  ret = shader->nuniforms * structSize;
  shader->nuniforms += n;
  return ret;
}


static void nvgp_gl_dump_shader_error(GLuint shader, const char* name, const char* type) {
  GLchar str[512 + 1];
  GLsizei len = 0;
  glGetShaderInfoLog(shader, 512, &len, str);
  if (len > 512) {
    len = 512;
  }
  str[len] = '\0';
  NVGP_PRINTF("Shader %s/%s error:\n%s\n", name, type, str);
}

static void nvgp_gl_dump_program_error(GLuint prog, const char* name) {
  GLsizei len = 0;
  GLchar str[512 + 1];
  glGetProgramInfoLog(prog, 512, &len, str);
  if (len > 512) {
    len = 512;
  }
  str[len] = '\0';
  NVGP_PRINTF("Program %s error:\n%s\n", name, str);
}

static void nvgp_gl_check_error(nvgp_gl_context_t* gl, const char* str) {
  GLenum err;
  if ((gl->flags & NVGP_GL_FLAG_DEBUG) == 0) return;
  err = glGetError();
  if (err != GL_NO_ERROR) {
    NVGP_PRINTF("Error %08x after %s\n", err, str);
    return;
  }
}

static void nvgp_gl_delete_shader(nvgp_gl_shader_t* shader) {
  if (shader->prog != 0) {
    glDeleteProgram(shader->prog);
  }
  if (shader->vert != 0) {
    glDeleteShader(shader->vert);
  }
  if (shader->frag != 0) {
    glDeleteShader(shader->frag);
  }
#if NANOVG_GL3
#if NANOVG_GL_USE_UNIFORMBUFFER
  if (shader->frag_buf != 0) {
    glDeleteBuffers(1, &shader->frag_buf);
  }
#endif
  if (shader->vert_arr != 0) {
    glDeleteVertexArrays(1, &shader->vert_arr);
  }
#endif
  if (shader->vert_buf != 0) {
    glDeleteBuffers(1, &shader->vert_buf);
  }

  NVGP_FREE(shader->verts);
  NVGP_FREE(shader->uniforms);
  NVGP_MEMSET(shader, 0x0, sizeof(nvgp_gl_shader_t));
}

static void nvgp_gl_get_uniforms(nvgp_gl_shader_t* shader) {
  shader->loc[NVGP_GL_LOC_VIEWSIZE] = glGetUniformLocation(shader->prog, "viewSize");
  shader->loc[NVGP_GL_LOC_TEX] = glGetUniformLocation(shader->prog, "tex");

#if NVGP_GL_USE_UNIFORMBUFFER
  shader->loc[NVGP_GL_LOC_FRAG] = glGetUniformBlockIndex(shader->prog, "frag");
#else
  shader->loc[NVGP_GL_LOC_FRAG] = glGetUniformLocation(shader->prog, "frag");
#endif
}

static void  nvgp_gl_init_shader(nvgp_gl_shader_t* shader, int32_t align) {
  nvgp_gl_get_uniforms(shader);
#if NVGP_GL_USE_UNIFORMBUFFER
  glUniformBlockBinding(shader->prog, shader->loc[NVGP_GL_LOC_FRAG], NVGP_GL_FRAG_BINDING);
#endif

  // Create dynamic vertex array
#ifdef NVGP_GL3
  glGenVertexArrays(1, &shader->vert_arr);
#endif
  glGenBuffers(1, &shader->vert_buf);

#if NVGP_GL_USE_UNIFORMBUFFER
  // Create UBOs
  glGenBuffers(1, &shader->frag_buf);
  glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align);
#endif
  shader->frag_size = sizeof(nvgp_gl_frag_uniforms_t) + align - sizeof(nvgp_gl_frag_uniforms_t) % align;
}

static nvgp_bool_t nvgp_gl_create_shader(nvgp_gl_shader_t* shader, const char* name, const char* header,
                                 const char* opts, const char* vshader, const char* fshader) {
  GLint status;
  GLuint prog, vert, frag;
  const char* str[3];
  str[0] = header;
  str[1] = opts != NULL ? opts : "";

  NVGP_MEMSET(shader, 0, sizeof(nvgp_gl_shader_t));

  prog = glCreateProgram();
  vert = glCreateShader(GL_VERTEX_SHADER);
  frag = glCreateShader(GL_FRAGMENT_SHADER);
  str[2] = vshader;
  glShaderSource(vert, 3, str, 0);
  str[2] = fshader;
  glShaderSource(frag, 3, str, 0);

  glCompileShader(vert);
  glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    nvgp_gl_dump_shader_error(vert, name, "vert");
    return FALSE;
  }

  glCompileShader(frag);
  glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    nvgp_gl_dump_shader_error(frag, name, "frag");
    return FALSE;
  }

  glAttachShader(prog, vert);
  glAttachShader(prog, frag);

  glBindAttribLocation(prog, 0, "vertex");
  glBindAttribLocation(prog, 1, "tcoord");

  glLinkProgram(prog);
  glGetProgramiv(prog, GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    nvgp_gl_dump_program_error(prog, name);
    return FALSE;
  }

  shader->prog = prog;
  shader->vert = vert;
  shader->frag = frag;

  return TRUE;
}

static nvgp_bool_t nvgp_gl_render_create(nvgp_gl_context_t* gl_ctx) {
  int32_t i = 0;

  static const char* shader_header =
#if defined NVGP_GL2
      "#define NVGP_GL2 1\n"
#elif defined NVGP_GL3
      "#version 150 core\n"
      "#define NVGP_GL3 1\n"
#elif defined NVGP_GLES2
      "#version 100\n"
      "#define NVGP_GL2 1\n"
#elif defined NVGP_GLES3
      "#version 300 es\n"
      "#define NVGP_GL3 1\n"
#else
      "";
#endif

#if NVGP_GL_USE_UNIFORMBUFFER
      "#define USE_UNIFORMBUFFER 1\n"
#else
      "#define UNIFORMARRAY_SIZE 11\n"
#endif
      "\n";
  const char* fill_frag_shader_list[NVGP_GL_SHADER_COUNT] = { fill_frag_shader_0, fill_frag_shader_1, fill_frag_shader_2, fill_frag_shader_3, fill_frag_shader_4, fill_frag_shader_5, fill_frag_shader_6, fill_frag_shader_7, fill_frag_shader_8, fill_frag_shader_9, fill_frag_shader_10, fill_frag_shader_11, fill_frag_shader_12, fill_frag_shader_13 };
  nvgp_gl_check_error(gl_ctx, "init");

  for (i = 0; i < NVGP_GL_SHADER_COUNT; i++) {
    const char* tmp_fill_frag_shader = fill_frag_shader_list[i];
    if (tmp_fill_frag_shader != NULL) {
      if (gl_ctx->flags & NVGP_GL_FLAG_ANTIALIAS) {
        if (nvgp_gl_create_shader(&(gl_ctx->shader_list[i]), "shader", shader_header, "#define EDGE_AA 1\n",
                              fill_vert_shader, tmp_fill_frag_shader) == 0)
        return FALSE;
      } else {
        if (nvgp_gl_create_shader(&(gl_ctx->shader_list[i]), "shader", shader_header, NULL, fill_vert_shader,
                                tmp_fill_frag_shader) == 0)
        return FALSE;
      }
      nvgp_gl_init_shader(&(gl_ctx->shader_list[i]), UNIFORM_OFFSET_ALIGNMENT);
      nvgp_gl_check_error(gl_ctx, "uniform locations");
    }
  }

  nvgp_gl_check_error(gl_ctx, "create done");

  glFinish();
  return TRUE;
}

static void nvgp_gl_begin_frame(void* uptr, float width, float height, float pixel_ratio) {
  nvgp_gl_context_t* gl = (nvgp_gl_context_t*)uptr;
  gl->view[0] = width;
  gl->view[1] = height;
  gl->pixel_ratio = pixel_ratio;
  gl->pixel_scale = 1.0f / pixel_ratio;
}

static void nvgp_gl_reset_gl_state(nvgp_gl_context_t* gl) {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glStencilMask(0xffffffff);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glStencilFunc(GL_ALWAYS, 0, 0xffffffff);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
#if NVGP_GL_USE_STATE_FILTER
  gl->bound_texture = 0;
  gl->stencil_mask = 0xffffffff;
  gl->stencil_func = GL_ALWAYS;
  gl->stencil_func_ref = 0;
  gl->stencil_func_mask = 0xffffffff;
  gl->blend_func.src_rgb = GL_INVALID_ENUM;
  gl->blend_func.src_alpha = GL_INVALID_ENUM;
  gl->blend_func.dst_rgb = GL_INVALID_ENUM;
  gl->blend_func.dst_alpha = GL_INVALID_ENUM;
#endif
}

static void nvgp_gl_set_shader_data(nvgp_gl_context_t* gl, nvgp_gl_shader_t* shader, int32_t is_same_shader_prog) {
  if (is_same_shader_prog) {
    return;
  }
#ifdef NVGP_GL3
  if (!shader->setted_data) {
    shader->setted_data = 1;
#endif
#ifdef NVGP_GL_USE_UNIFORMBUFFER
    // Upload ubo for frag shaders
    glBindBuffer(GL_UNIFORM_BUFFER, shader->frag_buf);
    glBufferData(GL_UNIFORM_BUFFER, shader->nuniforms * shader->frag_size, shader->uniforms, GL_STREAM_DRAW);
#endif

    // Upload vertex data
#ifdef NVGP_GL3
    glBindVertexArray(shader->vert_arr);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, shader->vert_buf);
    glBufferData(GL_ARRAY_BUFFER, shader->nverts * sizeof(nvgp_vertex_t), shader->verts, GL_STREAM_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(nvgp_vertex_t), (const GLvoid*)(size_t)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(nvgp_vertex_t), (const GLvoid*)(0 + 2 * sizeof(float)));

    // Set view and texture just once per frame.
    if (shader->loc[NVGP_GL_LOC_TEX] >= 0) {
      glUniform1i(shader->loc[NVGP_GL_LOC_TEX], 0);
    }
    glUniform2fv(shader->loc[NVGP_GL_LOC_VIEWSIZE], 1, gl->view);
#ifdef NVGP_GL3
  } else {
    glBindVertexArray(shader->vert_arr);
    glBindBuffer(GL_ARRAY_BUFFER, shader->vert_buf);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
  }
#endif

#if NVGP_GL_USE_UNIFORMBUFFER
  glBindBuffer(GL_UNIFORM_BUFFER, shader->frag_buf);
#endif
}

static void nvgp_gl_bind_texture(nvgp_gl_context_t* gl, GLuint tex) {
#if NVGP_GL_USE_STATE_FILTER
  if (gl->bound_texture != tex) {
    gl->bound_texture = tex;
    glBindTexture(GL_TEXTURE_2D, tex);
  }
#else
  glBindTexture(GL_TEXTURE_2D, tex);
#endif
}

static nvgp_gl_texture_t* nvgp_gl_find_texture(nvgp_gl_context_t* gl, int32_t id) {
  uint32_t i;
  if (id > 0) {
    for (i = 0; i < gl->textures.size; i++) {
      nvgp_gl_texture_t* texture = nvgp_darray_get_ptr(&gl->textures, i, nvgp_gl_texture_t);
      if (texture->id == id) {
        return texture;
      }
    }
  }
  return NULL;
}

static nvgp_gl_frag_uniforms_t* nvgp_gl_frag_uniform_ptr(nvgp_gl_shader_t* shader, uint32_t i) {
  return (nvgp_gl_frag_uniforms_t*)&shader->uniforms[i];
}

static void nvgp_gl_set_uniforms(nvgp_gl_context_t* gl, nvgp_gl_shader_t* shader, uint32_t uniform_offset, int32_t image) {
#if NVGP_GL_USE_UNIFORMBUFFER
  glBindBufferRange(GL_UNIFORM_BUFFER, NVGP_GL_FRAG_BINDING, shader->frag_buf, uniform_offset,
                    sizeof(nvgp_gl_frag_uniforms_t));
#else
  nvgp_gl_frag_uniforms_t* frag = nvgp_gl_frag_uniform_ptr(shader, uniform_offset);
  glUniform4fv(shader->loc[NVGP_GL_LOC_FRAG], NVGP_GL_UNIFORMARRAY_SIZE, &(frag->uniformArray[0][0]));
#endif

  if (image != 0) {
    nvgp_gl_texture_t* tex = nvgp_gl_find_texture(gl, image);
    nvgp_gl_bind_texture(gl, tex != NULL ? tex->tex : 0);
    nvgp_gl_check_error(gl, "tex paint tex");
  } else {
    nvgp_gl_bind_texture(gl, 0);
  }
}

static void nvgp_gl_reset_shader(nvgp_gl_context_t* gl) {
  uint32_t i = 0;
  for (i = 0; i < nvgp_get_arrary_size(gl->shader_list); i++) {
    gl->shader_list[i].nverts = 0;
    gl->shader_list[i].nuniforms = 0;
    gl->shader_list[i].setted_data = 0;
  }
}

static nvgp_bool_t nvgp_gl_is_same_shader_prog(nvgp_gl_context_t* gl, nvgp_gl_shader_t* shader) {
  return gl->curr_prog == shader->prog;
}

static void nvgp_gl_use_shader_prog(nvgp_gl_context_t* gl, nvgp_gl_shader_t* shader) {
  if (gl->curr_prog != shader->prog) {
    glUseProgram(shader->prog);
    gl->curr_prog = shader->prog;
  }
}

static void nvgp_gl_flush_by_fast_fill_rect(nvgp_gl_context_t* gl, nvgp_gl_call_t* call_base) {
  nvgp_gl_call_fast_fill_rect_t* call = (nvgp_gl_call_fast_fill_rect_t*)call_base;
  uint32_t is_fill = call->w >= gl->view[0] * gl->pixel_ratio && call->h >= gl->view[1] * gl->pixel_ratio;
  if (!is_fill) {
    glScissor(call->x, call->y, call->w, call->h);
    glEnable(GL_SCISSOR_TEST);
  }
  glClearColor(call->color.r, call->color.g, call->color.b, call->color.a);
  glClear(GL_COLOR_BUFFER_BIT);
  if (!is_fill) {
    glDisable(GL_SCISSOR_TEST);
  }
}

static void nvgp_gl_flush_by_convex_fill_by_color(nvgp_gl_context_t* gl, nvgp_gl_call_t* call_base) {
  int32_t i;
  nvgp_gl_shader_t* shader = &gl->shader_list[call_base->shader_type];
  nvgp_gl_call_convex_fill_t* call = (nvgp_gl_call_convex_fill_t*)call_base;
  nvgp_gl_path_t* paths = nvgp_darray_get_ptr(&gl->paths, call->path_index, nvgp_gl_path_t);
  int32_t is_same_shader_prog = nvgp_gl_is_same_shader_prog(gl, shader);

  nvgp_gl_use_shader_prog(gl, shader);
  nvgp_gl_set_shader_data(gl, shader, is_same_shader_prog);
  nvgp_gl_blend_func_separate(gl, &call->blend_func);
  nvgp_gl_set_uniforms(gl, shader, call->uniform_offset, 0);
  nvgp_gl_check_error(gl, "convex fill");

  for (i = 0; i < call->path_count; i++) {
    glDrawArrays(GL_TRIANGLE_FAN, paths[i].fill_offset, paths[i].fill_count);
    // Draw fringes
    if (paths[i].stroke_count > 0) {
      glDrawArrays(GL_TRIANGLE_STRIP, paths[i].stroke_offset, paths[i].stroke_count);
    }
  }
}

static void nvgp_gl_flush_fill_by_color(nvgp_gl_context_t* gl, nvgp_gl_call_t* call_base) {
  int32_t i;
  nvgp_gl_shader_t* shader = &gl->shader_list[call_base->shader_type];
  nvgp_gl_call_fill_t* call = (nvgp_gl_call_fill_t*)call_base;
  int32_t is_same_shader_prog = nvgp_gl_is_same_shader_prog(gl, shader);

  nvgp_gl_use_shader_prog(gl, shader);
  nvgp_gl_set_shader_data(gl, shader, is_same_shader_prog);
  nvgp_gl_check_error(gl, "convex fill");

  nvgp_gl_blend_func_separate(gl, &call->blend_func);

  // Draw shapes
  glEnable(GL_STENCIL_TEST);
  nvgp_gl_stencil_mask(gl, 0xff);
  nvgp_gl_stencil_func(gl, GL_ALWAYS, 0, 0xff);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

  // set bindpoint for solid loc
  nvgp_gl_set_uniforms(gl, shader, call->uniform_offset, 0);
  nvgp_gl_check_error(gl, "fill simple");

  glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
  glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);
  glDisable(GL_CULL_FACE);
  for (i = 0; i < call->path_count; i++) {
    nvgp_gl_path_t* paths = nvgp_darray_get_ptr(&gl->paths, call->path_index + i, nvgp_gl_path_t);
    glDrawArrays(GL_TRIANGLE_FAN, paths->fill_offset, paths->fill_count);
  }
  glEnable(GL_CULL_FACE);

  // Draw anti-aliased pixels
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

  nvgp_gl_set_uniforms(gl, shader, call->uniform_offset + shader->frag_size, 0);
  nvgp_gl_check_error(gl, "fill fill");

  if (gl->flags & NVGP_GL_FLAG_ANTIALIAS) {
    nvgp_gl_stencil_func(gl, GL_EQUAL, 0x00, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    // Draw fringes
    for (i = 0; i < call->path_count; i++) {
      nvgp_gl_path_t* paths = nvgp_darray_get_ptr(&gl->paths, call->path_index + i, nvgp_gl_path_t);
      glDrawArrays(GL_TRIANGLE_STRIP, paths->stroke_offset, paths->stroke_count);
    }
  }

  // Draw fill
  nvgp_gl_stencil_func(gl, GL_NOTEQUAL, 0x0, 0xff);
  glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
  glDrawArrays(GL_TRIANGLE_STRIP, call->triangle_offset, call->triangle_count);

  glDisable(GL_STENCIL_TEST);
}

static void nvgp_gl_flush_fast_fill_by_color(nvgp_gl_context_t* gl, nvgp_gl_call_t* call_base) {
  int32_t i;
  nvgp_gl_shader_t* shader = &gl->shader_list[call_base->shader_type];
  nvgp_gl_call_fast_fill_t* call = (nvgp_gl_call_fast_fill_t*)call_base;
  nvgp_gl_path_t* paths = nvgp_darray_get_ptr(&gl->paths, call->path_index, nvgp_gl_path_t);
  int32_t is_same_shader_prog = nvgp_gl_is_same_shader_prog(gl, shader);

  nvgp_gl_use_shader_prog(gl, shader);
  nvgp_gl_set_shader_data(gl, shader, is_same_shader_prog);
  nvgp_gl_set_uniforms(gl, shader, call->uniform_offset, 0);
  nvgp_gl_check_error(gl, "fast fill");

  for (i = 0; i < call->path_count; i++) {
    if (paths[i].fill_count > 0) {
      glDrawArrays(call->mode, paths[i].fill_offset, paths[i].fill_count);
    }
    if (paths[i].stroke_count > 0) {
      glDrawArrays(call->mode, paths[i].stroke_offset, paths[i].stroke_count);
    }
  }
}

static void nvgp_gl_flush_stroke_by_color(nvgp_gl_context_t* gl, nvgp_gl_call_t* call_base) {
  int32_t i;
  nvgp_gl_shader_t* shader = &gl->shader_list[call_base->shader_type];
  nvgp_gl_call_stroke_t* call = (nvgp_gl_call_stroke_t*)call_base;
  int32_t is_same_shader_prog = nvgp_gl_is_same_shader_prog(gl, shader);

  nvgp_gl_use_shader_prog(gl, shader);
  nvgp_gl_set_shader_data(gl, shader, is_same_shader_prog);
  nvgp_gl_check_error(gl, "convex fill");

  nvgp_gl_blend_func_separate(gl, &call->blend_func);

  if (gl->flags & NVGP_GL_FLAG_STENCIL_STROKES) {
    glEnable(GL_STENCIL_TEST);
    nvgp_gl_stencil_mask(gl, 0xff);

    // Fill the stroke base without overlap
    nvgp_gl_stencil_func(gl, GL_EQUAL, 0x0, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    nvgp_gl_set_uniforms(gl, shader, call->uniform_offset + shader->frag_size, 0);
    nvgp_gl_check_error(gl, "stroke fill 0");
    for (i = 0; i < call->path_count; i++) {
      nvgp_gl_path_t* path = nvgp_darray_get_ptr(&gl->paths, call->path_index + i, nvgp_gl_path_t);
      glDrawArrays(GL_TRIANGLE_STRIP, path->stroke_offset, path->stroke_count);
    }

    // Draw anti-aliased pixels.
    nvgp_gl_set_uniforms(gl, shader, call->uniform_offset, 0);
    nvgp_gl_stencil_func(gl, GL_EQUAL, 0x00, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    for (i = 0; i < call->path_count; i++) {
      nvgp_gl_path_t* path = nvgp_darray_get_ptr(&gl->paths, call->path_index + i, nvgp_gl_path_t);
      glDrawArrays(GL_TRIANGLE_STRIP, path->stroke_offset, path->stroke_count);
    }

    // Clear stencil buffer.
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    nvgp_gl_stencil_func(gl, GL_ALWAYS, 0x0, 0xff);
    glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
    nvgp_gl_check_error(gl, "stroke fill 1");
    for (i = 0; i < call->path_count; i++) {
      nvgp_gl_path_t* path = nvgp_darray_get_ptr(&gl->paths, call->path_index + i, nvgp_gl_path_t);
      glDrawArrays(GL_TRIANGLE_STRIP, path->stroke_offset, path->stroke_count);
    }
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glDisable(GL_STENCIL_TEST);

    //    glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset + gl->fragSize),
    // paint, scissor, strokeWidth, fringe, 1.0f - 0.5f/255.0f);

  } else {
    nvgp_gl_set_uniforms(gl, shader, call->uniform_offset, 0);
    nvgp_gl_check_error(gl, "stroke fill");
    // Draw Strokes
    for (i = 0; i < call->path_count; i++) {
      nvgp_gl_path_t* path = nvgp_darray_get_ptr(&gl->paths, call->path_index + i, nvgp_gl_path_t);
      glDrawArrays(GL_TRIANGLE_STRIP, path->stroke_offset, path->stroke_count);
    }
  }
}

static void nvgp_gl_flush_stroke_by_image(nvgp_gl_context_t* gl, nvgp_gl_call_t* call_base) {
  int32_t i;
  nvgp_gl_shader_t* shader = &gl->shader_list[call_base->shader_type];
  nvgp_gl_call_stroke_image_t* call = (nvgp_gl_call_stroke_image_t*)call_base;
  int32_t is_same_shader_prog = nvgp_gl_is_same_shader_prog(gl, shader);

  nvgp_gl_use_shader_prog(gl, shader);
  nvgp_gl_set_shader_data(gl, shader, is_same_shader_prog);
  nvgp_gl_check_error(gl, "convex fill");

  nvgp_gl_blend_func_separate(gl, &call->blend_func);

  if (gl->flags & NVGP_GL_FLAG_STENCIL_STROKES) {
    glEnable(GL_STENCIL_TEST);
    nvgp_gl_stencil_mask(gl, 0xff);

    // Fill the stroke base without overlap
    nvgp_gl_stencil_func(gl, GL_EQUAL, 0x0, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    nvgp_gl_set_uniforms(gl, shader, call->uniform_offset + shader->frag_size, call->image);
    nvgp_gl_check_error(gl, "stroke fill 0");
    for (i = 0; i < call->path_count; i++) {
      nvgp_gl_path_t* path = nvgp_darray_get_ptr(&gl->paths, call->path_index + i, nvgp_gl_path_t);
      glDrawArrays(GL_TRIANGLE_STRIP, path->stroke_offset, path->stroke_count);
    }

    // Draw anti-aliased pixels.
    nvgp_gl_set_uniforms(gl, shader, call->uniform_offset, call->image);
    nvgp_gl_stencil_func(gl, GL_EQUAL, 0x00, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    for (i = 0; i < call->path_count; i++) {
      nvgp_gl_path_t* path = nvgp_darray_get_ptr(&gl->paths, call->path_index + i, nvgp_gl_path_t);
      glDrawArrays(GL_TRIANGLE_STRIP, path->stroke_offset, path->stroke_count);
    }

    // Clear stencil buffer.
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    nvgp_gl_stencil_func(gl, GL_ALWAYS, 0x0, 0xff);
    glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
    nvgp_gl_check_error(gl, "stroke fill 1");
    for (i = 0; i < call->path_count; i++) {
      nvgp_gl_path_t* path = nvgp_darray_get_ptr(&gl->paths, call->path_index + i, nvgp_gl_path_t);
      glDrawArrays(GL_TRIANGLE_STRIP, path->stroke_offset, path->stroke_count);
    }
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glDisable(GL_STENCIL_TEST);

    //    glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset + gl->fragSize),
    // paint, scissor, strokeWidth, fringe, 1.0f - 0.5f/255.0f);

  } else {
    nvgp_gl_set_uniforms(gl, shader, call->uniform_offset, 0);
    nvgp_gl_check_error(gl, "stroke fill");
    // Draw Strokes
    for (i = 0; i < call->path_count; i++) {
      nvgp_gl_path_t* path = nvgp_darray_get_ptr(&gl->paths, call->path_index + i, nvgp_gl_path_t);
      glDrawArrays(GL_TRIANGLE_STRIP, path->stroke_offset, path->stroke_count);
    }
  }
}

static void nvgp_gl_flush_draw_image(nvgp_gl_context_t* gl, nvgp_gl_call_t* call_base) {
  int32_t i;
  nvgp_gl_shader_t* shader = &gl->shader_list[call_base->shader_type];
  nvgp_gl_call_image_t* call = (nvgp_gl_call_image_t*)call_base;
  int32_t is_same_shader_prog = nvgp_gl_is_same_shader_prog(gl, shader);
  if (call->path_count == 1) {
    nvgp_gl_path_t* paths = nvgp_darray_get_ptr(&gl->paths, call->path_index, nvgp_gl_path_t);

    nvgp_gl_use_shader_prog(gl, shader);
    nvgp_gl_set_shader_data(gl, shader, is_same_shader_prog);
    nvgp_gl_blend_func_separate(gl, &call->blend_func);
    nvgp_gl_set_uniforms(gl, shader, call->uniform_offset, call->image);
    nvgp_gl_check_error(gl, "convex fill");

    for (i = 0; i < call->path_count; i++) {
      glDrawArrays(GL_TRIANGLE_FAN, paths[i].fill_offset, paths[i].fill_count);
      // Draw fringes
      if (paths[i].stroke_count > 0) {
        glDrawArrays(GL_TRIANGLE_STRIP, paths[i].stroke_offset, paths[i].stroke_count);
      }
    }
  } else {
    assert("not impl");
  }
}

static void nvgp_gl_flush_draw_text(nvgp_gl_context_t* gl, nvgp_gl_call_t* call_base) {
  nvgp_gl_shader_t* shader = &gl->shader_list[call_base->shader_type];
  nvgp_gl_call_text_t* call = (nvgp_gl_call_text_t*)call_base;
  int32_t is_same_shader_prog = nvgp_gl_is_same_shader_prog(gl, shader);

  nvgp_gl_use_shader_prog(gl, shader);
  nvgp_gl_set_shader_data(gl, shader, is_same_shader_prog);
  nvgp_gl_blend_func_separate(gl, &call->blend_func);
  nvgp_gl_set_uniforms(gl, shader, call->uniform_offset, call->image);
  nvgp_gl_check_error(gl, "triangles fill");

  glDrawArrays(GL_TRIANGLES, call->triangle_offset, call->triangle_count);
}

static void nvgp_gl_flush(nvgp_gl_context_t* gl) {
  uint32_t i = 0;
  nvgp_gl_reset_gl_state(gl);
  for (i = 0; i < gl->calls.size; i++) {
    nvgp_gl_call_t* call_base = nvgp_darray_get_ptr(&gl->calls, i, nvgp_gl_call_t);
    switch (call_base->call_type)
    {
    case NVGP_GL_CALL_FAST_FILL_RECT:
      nvgp_gl_flush_by_fast_fill_rect(gl, call_base);
      break;
    case NVGP_GL_CALL_CONVEX_FILL:
      nvgp_gl_flush_by_convex_fill_by_color(gl, call_base);
      break;
    case NVGP_GL_CALL_FILL:
      nvgp_gl_flush_fill_by_color(gl, call_base);
      break;
    case NVGP_GL_CALL_FAST_FILL_COLOR :
      nvgp_gl_flush_fast_fill_by_color(gl, call_base);
      break;
    case NVGP_GL_CALL_STROKE:
      nvgp_gl_flush_stroke_by_color(gl, call_base);
      break;
    case NVGP_GL_CALL_STROKE_IMAGE:
      nvgp_gl_flush_stroke_by_image(gl, call_base);
      break;
    case NVGP_GL_CALL_IMAGE:
      nvgp_gl_flush_draw_image(gl, call_base);
      break;
    case NVGP_GL_CALL_TEXT :
      nvgp_gl_flush_draw_text(gl, call_base);
      break;
    default:
      break;
    }
  }
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
#if defined NANOVG_GL3
  glBindVertexArray(0);
#endif
  glDisable(GL_CULL_FACE);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
  nvgp_gl_bind_texture(gl, 0);

  // Reset
  gl->curr_prog = 0;
  nvgp_gl_reset_shader(gl);
  nvgp_darray_clear(&gl->paths);
  nvgp_darray_clear_by_destroy_function(&gl->calls, nvgp_gl_call_destroy, NULL);
}

static void nvgp_gl_end_frame(void* uptr) {
  nvgp_gl_context_t* gl = (nvgp_gl_context_t*)uptr;
  nvgp_gl_flush(gl);
}

static nvgp_bool_t nvgp_gl_rect_intersect_f(float r1_x, float r1_y, float r1_w, float r1_h, float* r2_x, float* r2_y, float* r2_w, float* r2_h) {
  int32_t top = 0;
  int32_t left = 0;
  int32_t bottom = 0;
  int32_t right = 0;
  int32_t bottom1 = 0;
  int32_t right1 = 0;
  int32_t bottom2 = 0;
  int32_t right2 = 0;

  bottom1 = r1_y + r1_h - 1;
  bottom2 = *r2_y + *r2_h - 1;
  right1 = r1_x + r1_w - 1;
  right2 = *r2_x + *r2_w - 1;

  top = nvgp_max(r1_y, *r2_y);
  left = nvgp_max(r1_x, *r2_x);
  right = nvgp_min(right1, right2);
  bottom = nvgp_min(bottom1, bottom2);

  *r2_x = left;
  *r2_y = top;
  *r2_w = right >= left ? (right - left + 1) : 0;
  *r2_h = bottom >= top ? (bottom - top + 1) : 0;

  return *r2_w != 0 && *r2_h != 0;
}

static nvgp_bool_t nvgp_gl_color_is_translucent(nvgp_paint_t* paint) {
  if (paint->inner_color.rgba.a > 0xf8) {
    return FALSE;
  }
  return TRUE;
}

static nvgp_bool_t nvgp_gl_is_fast_draw_rect(nvgp_darray_t* paths) {
  if (paths->size == 1) {
    nvgp_path_t* path = nvgp_darray_get_ptr(paths, 0, nvgp_path_t);
    if (path->nfill == 4 && path->nstroke == 0) {
      if ((path->fill[0].x == path->fill[1].x && path->fill[0].y == path->fill[3].y && path->fill[2].x == path->fill[3].x && path->fill[2].y == path->fill[1].y) ||
          (path->fill[0].x == path->fill[3].x && path->fill[0].y == path->fill[1].y && path->fill[2].x == path->fill[1].x && path->fill[2].y == path->fill[3].y)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

static nvgp_bool_t nvgp_gl_is_fast_stroke(const nvgp_darray_t* paths) {
  if (paths->size == 1) {
    nvgp_path_t* path = nvgp_darray_get_ptr(paths, 0, nvgp_path_t);
    if (path->nfill == 0 && path->nstroke == 8) {
      if ((path->stroke[0].x == path->stroke[1].x && path->stroke[2].x == path->stroke[3].x && path->stroke[4].x == path->stroke[5].x && path->stroke[6].x == path->stroke[7].x &&
           path->stroke[0].y == path->stroke[2].y && path->stroke[2].y == path->stroke[4].y && path->stroke[4].y == path->stroke[6].y && path->stroke[1].y == path->stroke[3].y && path->stroke[3].y == path->stroke[5].y && path->stroke[5].y == path->stroke[7].y) 
          ||
          (path->stroke[0].x == path->stroke[2].x && path->stroke[2].x == path->stroke[4].x && path->stroke[4].x == path->stroke[6].x && path->stroke[1].x == path->stroke[3].x && path->stroke[3].x == path->stroke[5].x && path->stroke[5].x == path->stroke[7].x &&
           path->stroke[0].y == path->stroke[1].y && path->stroke[2].y == path->stroke[3].y && path->stroke[4].y == path->stroke[5].y && path->stroke[6].y == path->stroke[7].y)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

static nvgp_bool_t nvgp_gl_is_fast_draw_image(nvgp_darray_t* paths) {
  if (paths->size == 1) {
    nvgp_path_t* path = nvgp_darray_get_ptr(paths, 0, nvgp_path_t);
    if (path->nfill == 4) {
      if ((path->fill[0].x == path->fill[1].x && path->fill[0].y == path->fill[3].y && path->fill[2].x == path->fill[3].x && path->fill[2].y == path->fill[1].y) ||
          (path->fill[0].x == path->fill[3].x && path->fill[0].y == path->fill[1].y && path->fill[2].x == path->fill[1].x && path->fill[2].y == path->fill[3].y)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

static nvgp_bool_t nvgp_gl_check_is_matrix_skew(nvgp_matrix_t* mat) {
  if (mat->mat.skew_x == 0.0f && mat->mat.skew_y == 0.0f) {
    return FALSE;
  }
  return TRUE;
}

static nvgp_bool_t nvgp_gl_check_is_matrix_transformer(nvgp_matrix_t* mat, float scale) {
  if (mat->mat.scale_x == scale && mat->mat.scale_y == scale && mat->mat.skew_x == 0.0f && mat->mat.skew_y == 0.0f) {
    return FALSE;
  }
  return TRUE;
}

static nvgp_bool_t nvgp_gl_verts_in_scissor(nvgp_gl_context_t* gl, const nvgp_vertex_t* verts, uint32_t nverts, nvgp_scissor_t* scissor) {
  if (verts != NULL && nverts > 0) {
    int32_t i = 0;
    float cx = scissor->matrix.mat.trans_x * gl->pixel_ratio;
    float cy = scissor->matrix.mat.trans_y * gl->pixel_ratio;
    float hw = scissor->extent[0] * gl->pixel_ratio;
    float hh = scissor->extent[1] * gl->pixel_ratio;

    float l = cx - hw;
    float t = cy - hh;
    float r = l + 2 * hw;
    float b = t + 2 * hh;

    for (i = 0; i < nverts; i++) {
      const nvgp_vertex_t* iter = verts + i;
      int x = iter->x * gl->pixel_ratio;
      int y = iter->y * gl->pixel_ratio;
      if (x < l || x > r || y < t || y > b) {
        return FALSE;
      }
    }
  }

  return TRUE;
}

static int32_t nvgp_gl_paths_in_scissor(nvgp_gl_context_t* gl, const nvgp_darray_t* paths, nvgp_scissor_t* scissor) {
  uint32_t i = 0;
  for (i = 0; i < paths->size; i++) {
    const nvgp_path_t* path = nvgp_darray_get_ptr(paths, i, nvgp_path_t);
    if (!nvgp_gl_verts_in_scissor(gl, path->fill, path->nfill, scissor) || 
        !nvgp_gl_verts_in_scissor(gl, path->stroke, path->nstroke, scissor)) {
      return FALSE;
    }
  }
  return TRUE;
}

static void nvgp_gl_mat_to_mat3x4(float* m3, nvgp_matrix_t* t) {
  m3[0] = t->mat.scale_x;
  m3[1] = t->mat.skew_y;
  m3[2] = 0.0f;
  m3[3] = 0.0f;
  m3[4] = t->mat.skew_x;
  m3[5] = t->mat.scale_y;
  m3[6] = 0.0f;
  m3[7] = 0.0f;
  m3[8] = t->mat.trans_x;
  m3[9] = t->mat.trans_y;
  m3[10] = 1.0f;
  m3[11] = 0.0f;
}

static nvgp_bool_t nvgp_gl_convert_paint(nvgp_gl_context_t* gl, nvgp_gl_frag_uniforms_t* frag, nvgp_paint_t* paint, nvgp_scissor_t* scissor, float width, float fringe, float strokeThr) {
  nvgp_matrix_t invxform;
  nvgp_gl_texture_t* tex = NULL;

  NVGP_MEMSET(frag, 0, sizeof(*frag));

  frag->innerCol = nvgp_gl_to_premul_color(paint->inner_color);
  if (paint->image == 0) {
    frag->other_info.outerCol = nvgp_gl_to_premul_color(paint->outer_color);
  }

  if (scissor->extent[0] < -0.5f || scissor->extent[1] < -0.5f) {
    NVGP_MEMSET(frag->scissorMat, 0, sizeof(frag->scissorMat));
    frag->scissorExt[0] = 1.0f;
    frag->scissorExt[1] = 1.0f;
    frag->scissorScale[0] = 1.0f;
    frag->scissorScale[1] = 1.0f;
  } else {
    nvgp_transform_inverse(&invxform, &scissor->matrix);
    nvgp_gl_mat_to_mat3x4(frag->scissorMat, &invxform);
    frag->scissorExt[0] = scissor->extent[0];
    frag->scissorExt[1] = scissor->extent[1];
    frag->scissorScale[0] =
        nvgp_sqrtf(scissor->matrix.mat.scale_x * scissor->matrix.mat.scale_x + scissor->matrix.mat.skew_x * scissor->matrix.mat.skew_x) / fringe;
    frag->scissorScale[1] =
        nvgp_sqrtf(scissor->matrix.mat.scale_y * scissor->matrix.mat.scale_y + scissor->matrix.mat.skew_y * scissor->matrix.mat.skew_y) / fringe;
  }

  NVGP_MEMCPY(frag->extent, paint->extent, sizeof(frag->extent));
  frag->strokeMult = (width * 0.5f + fringe * 0.5f) / fringe;
  frag->strokeThr = strokeThr;

  if (paint->image != 0) {
    tex = nvgp_gl_find_texture(gl, paint->image);
    if (tex == NULL) {
      return FALSE;
    }
    if ((tex->flags & NVGP_GL_IMAGE_FLIPY) != 0) {
      nvgp_matrix_t m1, m2;
      nvgp_transform_translate(&m1, 0.0f, frag->extent[1] * 0.5f);
      nvgp_transform_multiply_to_t(&m1, &paint->mat);
      nvgp_transform_scale(&m2, 1.0f, -1.0f);
      nvgp_transform_multiply_to_t(&m2, &m1);
      nvgp_transform_translate(&m1, 0.0f, -frag->extent[1] * 0.5f);
      nvgp_transform_multiply_to_t(&m1, &m2);
      nvgp_transform_inverse(&invxform, &m1);
    } else {
      nvgp_transform_inverse(&invxform, &paint->mat);
    }
    if (paint->draw_type == NVGP_IMAGE_DRAW_REPEAT) {
      NVGP_MEMCPY(frag->draw_info, paint->draw_info, sizeof(frag->draw_info));
      NVGP_MEMCPY(frag->other_info.draw_image_rect, paint->draw_image_rect, sizeof(paint->draw_image_rect));
    }
  } else {
    frag->radius = paint->radius;
    frag->feather = paint->feather;
    nvgp_transform_inverse(&invxform, &paint->mat);
  }
  nvgp_gl_mat_to_mat3x4(frag->paintMat, &invxform);

  return TRUE;
}

static uint32_t nvgp_gl_max_vert_count(const nvgp_darray_t* paths) {
  uint32_t i, count = 0;
  for (i = 0; i < paths->size; i++) {
    nvgp_path_t* path = nvgp_darray_get_ptr(paths, i, nvgp_path_t);
    count += path->nfill;
    count += path->nstroke;
  }
  return count;
}

static nvgp_bool_t nvgp_gl_render_fast_fill_rect(nvgp_gl_context_t* gl, nvgp_paint_t* paint, nvgp_scissor_t* scissor, float fringe, const float* bounds, const nvgp_darray_t* paths) {
  if (paint->image == 0 && !nvgp_gl_color_is_translucent(paint) && !nvgp_gl_check_is_matrix_transformer(&paint->mat, 1.0f) && !nvgp_gl_check_is_matrix_transformer(&scissor->matrix, 1.0f) && nvgp_gl_is_fast_draw_rect((nvgp_darray_t*)paths)) {
    nvgp_path_t* path = nvgp_darray_get_ptr(paths, 0, nvgp_path_t);
    float l = path->fill[0].x * gl->pixel_ratio;
    float t = path->fill[0].y * gl->pixel_ratio;
    float r = path->fill[2].x * gl->pixel_ratio;
    float b = path->fill[2].y * gl->pixel_ratio;
    float w = r - l;
    float h = b - t;
    float cx = scissor->matrix.mat.trans_x * gl->pixel_ratio;
    float cy = scissor->matrix.mat.trans_y * gl->pixel_ratio;
    float hw = scissor->extent[0] * gl->pixel_ratio;
    float hh = scissor->extent[1] * gl->pixel_ratio;
    if(nvgp_gl_rect_intersect_f(cx - hw, cy - hh, hw * 2, hh * 2, &l, &t, &w, &h)){
      nvgp_gl_call_fast_fill_rect_t* call = NVGP_ZALLOC(nvgp_gl_call_fast_fill_rect_t);
      if (call == NULL) {
        return FALSE;
      }
      call->x = l;
      call->y = gl->view[1] * gl->pixel_ratio - t - h;
      call->w = w;
      call->h = h;
      call->color = nvgp_gl_to_color(paint->inner_color);
      call->base.call_type = NVGP_GL_CALL_FAST_FILL_RECT;
      nvgp_darray_push(&gl->calls, call);
    }
    return TRUE;
  }
  return FALSE;
}

static nvgp_bool_t nvgp_gl_render_convex_fill_by_color(nvgp_gl_context_t* gl, nvgp_paint_t* paint, nvgp_scissor_t* scissor, float fringe, const float* bounds, const nvgp_darray_t* paths, int32_t is_gradient) {
  nvgp_gl_shader_t* shader;
  int32_t max_verts, offset;
  nvgp_gl_frag_uniforms_t* frag = NULL;
  nvgp_path_t* path = nvgp_darray_get_ptr(paths, 0, nvgp_path_t);
  if (paint->image == 0 && paths->size == 1 && path->convex) {
    nvgp_gl_call_convex_fill_t* call = NVGP_ZALLOC(nvgp_gl_call_convex_fill_t);
    nvgp_gl_path_t* gl_path = nvgp_darray_get_empty_data_by_tail(&gl->paths);
    if (call == NULL || gl_path == NULL) {
      goto error;
    }
    NVGP_MEMSET(gl_path, 0x0, sizeof(nvgp_gl_path_t));
    call->base.call_type = NVGP_GL_CALL_CONVEX_FILL;
    call->base.shader_type = is_gradient ? NVGP_GL_SHADER_FILLGRAD : NVGP_GL_SHADER_FILLCOLOR;

    call->path_count = 1;
    call->path_index = gl->paths.size - 1;
    call->blend_func = nvgp_gl_composite_operation_state(NVGP_GL_SOURCE_OVER);

    shader = &(gl->shader_list[call->base.shader_type]);
    // Allocate vertices for all the paths.
    max_verts = path->nfill + path->nstroke;
    offset = nvgp_gl_alloc_verts(shader, max_verts);
    if (offset == -1) {
      NVGP_FREE(call);
      goto error;
    }

    if (path->nfill > 0) {
      gl_path->fill_offset = offset;
      gl_path->fill_count = path->nfill;
      NVGP_MEMCPY(shader->verts + offset, path->fill, sizeof(nvgp_vertex_t) * path->nfill);
      offset += path->nfill;
    }
    if (path->nstroke > 0) {
      gl_path->stroke_offset = offset;
      gl_path->stroke_count = path->nstroke;
      NVGP_MEMCPY(shader->verts + offset, path->stroke, sizeof(nvgp_vertex_t) * path->nstroke);
      offset += path->nstroke;
    }
    call->uniform_offset = nvgp_gl_alloc_frag_uniforms(shader, 1);
    if (call->uniform_offset == -1) {
      NVGP_FREE(call);
      goto error;
    }
    // Fill shader
    frag = nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset);
    nvgp_gl_convert_paint(gl, frag, paint, scissor, fringe, fringe, -1.0f);

    nvgp_darray_push(&gl->calls, call);
    return TRUE;
  }
error:
  return FALSE;
}

static nvgp_bool_t nvgp_gl_render_fill_by_color(nvgp_gl_context_t* gl, nvgp_paint_t* paint, nvgp_scissor_t* scissor, float fringe, const float* bounds, const nvgp_darray_t* paths, int32_t is_gradient) {
  nvgp_vertex_t* quad;
  nvgp_gl_shader_t* shader;
  int32_t i, max_verts, offset;
  nvgp_gl_frag_uniforms_t* frag = NULL;
  if (paint->image == 0) {
    nvgp_gl_call_fill_t* call = NVGP_ZALLOC(nvgp_gl_call_fill_t);
    if (call == NULL) {
      goto error;
    }
    call->base.call_type = NVGP_GL_CALL_FILL;
    call->base.shader_type =  is_gradient ? NVGP_GL_SHADER_FILLGRAD : NVGP_GL_SHADER_FILLCOLOR;

    call->triangle_count = 4;
    call->path_count = paths->size;
    call->path_index = gl->paths.size;
    call->blend_func = nvgp_gl_composite_operation_state(NVGP_GL_SOURCE_OVER);

    shader = &(gl->shader_list[call->base.shader_type]);
    max_verts = nvgp_gl_max_vert_count(paths) + call->triangle_count;
    offset = nvgp_gl_alloc_verts(shader, max_verts);
    if (offset == -1) {
      NVGP_FREE(call);
      goto error;
    }

    for (i = 0; i < paths->size; i++) {
      const nvgp_path_t* path = nvgp_darray_get_ptr(paths, i, nvgp_path_t);
      nvgp_gl_path_t* gl_path = nvgp_darray_get_empty_data_by_tail(&gl->paths);
      if (gl_path == NULL) {
        NVGP_FREE(call);
        goto error;
      }
      NVGP_MEMSET(gl_path, 0x0, sizeof(nvgp_gl_path_t));
      if (path->nfill > 0) {
        gl_path->fill_offset = offset;
        gl_path->fill_count = path->nfill;
        NVGP_MEMCPY(&shader->verts[offset], path->fill, sizeof(nvgp_vertex_t) * path->nfill);
        offset += path->nfill;
      }
      if (path->nstroke > 0) {
        gl_path->stroke_offset = offset;
        gl_path->stroke_count = path->nstroke;
        NVGP_MEMCPY(&shader->verts[offset], path->stroke, sizeof(nvgp_vertex_t) * path->nstroke);
        offset += path->nstroke;
      }
    }

    call->triangle_offset = offset;
    quad = &shader->verts[call->triangle_offset];
    nvgp_gl_vset(&quad[0], bounds[2], bounds[3], 0.5f, 1.0f);
    nvgp_gl_vset(&quad[1], bounds[2], bounds[1], 0.5f, 1.0f);
    nvgp_gl_vset(&quad[2], bounds[0], bounds[3], 0.5f, 1.0f);
    nvgp_gl_vset(&quad[3], bounds[0], bounds[1], 0.5f, 1.0f);

    call->uniform_offset = nvgp_gl_alloc_frag_uniforms(shader, 2);
    if (call->uniform_offset == -1) {
      NVGP_FREE(call);
      goto error;
    }
    // Simple shader for stencil
    frag = nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset);
    NVGP_MEMSET(frag, 0, sizeof(*frag));
    frag->strokeThr = -1.0f;
    // Fill shader
    nvgp_gl_convert_paint(gl, nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset + shader->frag_size), paint, scissor, fringe, fringe, -1.0f);

    nvgp_darray_push(&gl->calls, call);
    return TRUE;
  }
error:
  return FALSE;
}

static int32_t nvgp_gl_texture_is_premulti(nvgp_gl_texture_t* tex) {
  return tex->flags & NVGP_GL_IMAGE_PREMULTIPLIED;
}

static nvgp_bool_t nvgp_gl_render_draw_image(nvgp_gl_context_t* gl, nvgp_paint_t* paint, nvgp_scissor_t* scissor, float fringe, const float* bounds, const nvgp_darray_t* paths) {
  nvgp_vertex_t* quad;
  nvgp_gl_shader_t* shader;
  int32_t i, max_verts, offset;
  nvgp_gl_frag_uniforms_t* frag = NULL;
  nvgp_gl_texture_t* tex = nvgp_gl_find_texture(gl, paint->image);
  if (paint->image != 0 && tex != NULL) {
    int32_t is_premulti = nvgp_gl_texture_is_premulti(tex);
    nvgp_gl_call_image_t* call = NVGP_ZALLOC(nvgp_gl_call_image_t);
    int32_t support_fast_draw = !nvgp_gl_check_is_matrix_transformer(&scissor->matrix, 1.0f) && nvgp_gl_is_fast_draw_image((nvgp_darray_t*)paths) && nvgp_gl_paths_in_scissor(gl, paths, scissor);
    if (call == NULL) {
      goto error;
    }
    call->base.call_type = NVGP_GL_CALL_IMAGE;
    if (is_premulti) {
      support_fast_draw = support_fast_draw && !nvgp_gl_check_is_matrix_transformer(&paint->mat, gl->pixel_scale);
      call->base.shader_type = support_fast_draw ? NVGP_GL_SHADER_FAST_FILLIMG : NVGP_GL_SHADER_FILLIMG;
    } else {
      if (paint->draw_type == NVGP_IMAGE_DRAW_DEFAULT) {
        support_fast_draw = support_fast_draw && !nvgp_gl_check_is_matrix_transformer(&paint->mat, gl->pixel_scale);
        call->base.shader_type = support_fast_draw ? NVGP_GL_SHADER_FAST_FILLIMG_RGBA : NVGP_GL_SHADER_FILLIMG_RGBA;
      } else if (paint->draw_type == NVGP_IMAGE_DRAW_REPEAT) {
        support_fast_draw = support_fast_draw && !nvgp_gl_check_is_matrix_skew(&paint->mat);
        call->base.shader_type = support_fast_draw ? NVGP_GL_SHADER_FAST_REPEAT_FILLIMG_RGBA : NVGP_GL_SHADER_REPEAT_FILLIMG_RGBA;
      }
    }

    call->image = paint->image;
    call->triangle_count = 4;
    call->path_count = paths->size;
    call->path_index = gl->paths.size;
    call->blend_func = nvgp_gl_composite_operation_state(NVGP_GL_SOURCE_OVER);

    shader = &(gl->shader_list[call->base.shader_type]);
    max_verts = nvgp_gl_max_vert_count(paths) + call->triangle_count;
    offset = nvgp_gl_alloc_verts(shader, max_verts);
    if (offset == -1) {
      NVGP_FREE(call);
      goto error;
    }

    for (i = 0; i < paths->size; i++) {
      const nvgp_path_t* path = nvgp_darray_get_ptr(paths, i, nvgp_path_t);
      nvgp_gl_path_t* gl_path = nvgp_darray_get_empty_data_by_tail(&gl->paths);
      if (gl_path == NULL) {
        NVGP_FREE(call);
        goto error;
      }
      NVGP_MEMSET(gl_path, 0x0, sizeof(nvgp_gl_path_t));
      if (path->nfill > 0) {
        gl_path->fill_offset = offset;
        gl_path->fill_count = path->nfill;
        NVGP_MEMCPY(&shader->verts[offset], path->fill, sizeof(nvgp_vertex_t) * path->nfill);
        offset += path->nfill;
      }
      if (path->nstroke > 0) {
        gl_path->stroke_offset = offset;
        gl_path->stroke_count = path->nstroke;
        NVGP_MEMCPY(&shader->verts[offset], path->stroke, sizeof(nvgp_vertex_t) * path->nstroke);
        offset += path->nstroke;
      }
    }

    if (paths->size == 1) {
      call->uniform_offset = nvgp_gl_alloc_frag_uniforms(shader, 1);
      if (call->uniform_offset == -1) {
        NVGP_FREE(call);
        goto error;
      }
      frag = nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset);
      nvgp_gl_convert_paint(gl, frag, paint, scissor, fringe, fringe, -1.0f);
    } else {
      call->triangle_offset = offset;
      quad = &shader->verts[call->triangle_offset];
      nvgp_gl_vset(&quad[0], bounds[2], bounds[3], 0.5f, 1.0f);
      nvgp_gl_vset(&quad[1], bounds[2], bounds[1], 0.5f, 1.0f);
      nvgp_gl_vset(&quad[2], bounds[0], bounds[3], 0.5f, 1.0f);
      nvgp_gl_vset(&quad[3], bounds[0], bounds[1], 0.5f, 1.0f);

      call->uniform_offset = nvgp_gl_alloc_frag_uniforms(shader, 2);
      if (call->uniform_offset == -1) {
        NVGP_FREE(call);
        goto error;
      }
      // Simple shader for stencil
      frag = nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset);
      NVGP_MEMSET(frag, 0, sizeof(*frag));
      frag->strokeThr = -1.0f;
      // Fill shader
      nvgp_gl_convert_paint(gl, nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset + shader->frag_size), paint, scissor, fringe, fringe, -1.0f);
    }
    
    nvgp_darray_push(&gl->calls, call);
    return TRUE;
  }
error:
  return FALSE;
}

static nvgp_bool_t nvgp_gl_render_fast_stroke_by_color(nvgp_gl_context_t* gl, nvgp_paint_t* paint, nvgp_scissor_t* scissor, float fringe, const nvgp_darray_t* paths) {
  nvgp_gl_shader_t* shader;
  int32_t max_verts, offset;
  if (paint->image == 0 && paths->size == 1 && !nvgp_gl_check_is_matrix_transformer(&paint->mat, 1.0f) && !nvgp_gl_check_is_matrix_transformer(&scissor->matrix, 1.0f) && nvgp_gl_paths_in_scissor(gl, paths, scissor) && nvgp_gl_is_fast_stroke(paths)) {
    nvgp_gl_frag_uniforms_t* frag = NULL;
    nvgp_gl_call_fast_fill_t* call = NULL;
    const nvgp_path_t* path = nvgp_darray_get_ptr(paths, 0, nvgp_path_t);
    nvgp_gl_path_t* gl_path = nvgp_darray_get_empty_data_by_tail(&gl->paths);
    if (gl_path == NULL) {
      goto error;
    }
    NVGP_MEMSET(gl_path, 0x0, sizeof(nvgp_gl_path_t));

    call = NVGP_ZALLOC(nvgp_gl_call_fast_fill_t);
    if (call == NULL) {
      return FALSE;
    }
    call->base.call_type = NVGP_GL_CALL_FAST_FILL_COLOR;
    call->base.shader_type =  NVGP_GL_SHADER_FAST_FILL_COLOR;

    call->mode = GL_TRIANGLE_STRIP;
    call->path_count = paths->size;
    call->path_index = gl->paths.size - 1;

    shader = &(gl->shader_list[call->base.shader_type]);
    max_verts = nvgp_gl_max_vert_count(paths);
    offset = nvgp_gl_alloc_verts(shader, max_verts);
    if (offset == -1) {
      NVGP_FREE(call);
      goto error;
    }

    gl_path->stroke_offset = offset;
    gl_path->stroke_count = path->nstroke;
    memcpy(&shader->verts[offset], path->stroke, sizeof(nvgp_vertex_t) * path->nstroke);
    offset += path->nstroke;

    call->uniform_offset = nvgp_gl_alloc_frag_uniforms(shader, 1);
    if (call->uniform_offset == -1) {
      NVGP_FREE(call);
      goto error;
    }
    frag = nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset);
    nvgp_gl_convert_paint(gl, frag, paint, scissor, fringe, fringe, -1.0f);

    nvgp_darray_push(&gl->calls, call);
    return TRUE;
  }
error:
  return FALSE;
}

static nvgp_bool_t nvgp_gl_render_stroke_by_color(nvgp_gl_context_t* gl, nvgp_paint_t* paint, nvgp_scissor_t* scissor, float fringe, float stroke_width, const nvgp_darray_t* paths, int32_t is_gradient) {
  nvgp_gl_shader_t* shader;
  int32_t i, max_verts, offset;
  if (paint->image == 0) {
    nvgp_gl_call_stroke_t* call = NVGP_ZALLOC(nvgp_gl_call_stroke_t);
    if (call == NULL) {
      return FALSE;
    }
    call->base.call_type = NVGP_GL_CALL_STROKE;
    call->base.shader_type =  is_gradient ? NVGP_GL_SHADER_FILLGRAD : NVGP_GL_SHADER_FILL_STROKE;

    call->path_count = paths->size;
    call->path_index = gl->paths.size;
    call->blend_func = nvgp_gl_composite_operation_state(NVGP_GL_SOURCE_OVER);

    shader = &(gl->shader_list[call->base.shader_type]);
    max_verts = nvgp_gl_max_vert_count(paths);
    offset = nvgp_gl_alloc_verts(shader, max_verts);
    if (offset == -1) {
      NVGP_FREE(call);
      goto error;
    }

    for (i = 0; i < paths->size; i++) {
      const nvgp_path_t* path = nvgp_darray_get_ptr(paths, i, nvgp_path_t);
      nvgp_gl_path_t* gl_path = nvgp_darray_get_empty_data_by_tail(&gl->paths);
      if (gl_path == NULL) {
        NVGP_FREE(call);
        goto error;
      }
      NVGP_MEMSET(gl_path, 0x0, sizeof(nvgp_gl_path_t));
      if (path->nstroke) {
        gl_path->stroke_offset = offset;
        gl_path->stroke_count = path->nstroke;
        memcpy(&shader->verts[offset], path->stroke, sizeof(nvgp_vertex_t) * path->nstroke);
        offset += path->nstroke;
      }
    }

    if (gl->flags & NVGP_GL_FLAG_STENCIL_STROKES) {
      // Fill shader
      call->uniform_offset = nvgp_gl_alloc_frag_uniforms(shader, 2);
      if (call->uniform_offset == -1) {
        NVGP_FREE(call);
        goto error;
      }

      nvgp_gl_convert_paint(gl, nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset), paint, scissor,
                          stroke_width, fringe, -1.0f);
      nvgp_gl_convert_paint(gl, nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset + shader->frag_size), paint,
                          scissor, stroke_width, fringe, 1.0f - 0.5f / 255.0f);

    } else {
      // Fill shader
      call->uniform_offset = nvgp_gl_alloc_frag_uniforms(shader, 1);
      if (call->uniform_offset == -1) {
        NVGP_FREE(call);
        goto error;
      }
      nvgp_gl_convert_paint(gl, nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset), paint, scissor,
                          stroke_width, fringe, -1.0f);
    }
    nvgp_darray_push(&gl->calls, call);
    return TRUE;
  }
error:
  return FALSE;
}

static nvgp_bool_t nvgp_gl_render_stroke_by_image(nvgp_gl_context_t* gl, nvgp_paint_t* paint, nvgp_scissor_t* scissor, float fringe, float stroke_width, const nvgp_darray_t* paths) {
  nvgp_gl_shader_t* shader;
  int32_t i, max_verts, offset;
  nvgp_gl_texture_t* tex = nvgp_gl_find_texture(gl, paint->image);
  if (paint->image != 0 && tex != NULL) {    
    int32_t is_premulti = nvgp_gl_texture_is_premulti(tex);
    nvgp_gl_call_stroke_image_t* call = NVGP_ZALLOC(nvgp_gl_call_stroke_image_t);
    if (call == NULL) {
      return FALSE;
    }
    call->base.call_type = NVGP_GL_CALL_STROKE_IMAGE;
    call->base.shader_type = is_premulti ? NVGP_GL_SHADER_FILLIMG : NVGP_GL_SHADER_FILLIMG_RGBA;

    call->image = paint->image;
    call->path_count = paths->size;
    call->path_index = gl->paths.size;
    call->blend_func = nvgp_gl_composite_operation_state(NVGP_GL_SOURCE_OVER);

    shader = &(gl->shader_list[call->base.shader_type]);
    max_verts = nvgp_gl_max_vert_count(paths);
    offset = nvgp_gl_alloc_verts(shader, max_verts);
    if (offset == -1) {
      NVGP_FREE(call);
      goto error;
    }

    for (i = 0; i < paths->size; i++) {
      const nvgp_path_t* path = nvgp_darray_get_ptr(paths, i, nvgp_path_t);
      nvgp_gl_path_t* gl_path = nvgp_darray_get_empty_data_by_tail(&gl->paths);
      if (gl_path == NULL) {
        NVGP_FREE(call);
        goto error;
      }
      NVGP_MEMSET(gl_path, 0x0, sizeof(nvgp_gl_path_t));
      if (path->nstroke) {
        gl_path->stroke_offset = offset;
        gl_path->stroke_count = path->nstroke;
        memcpy(&shader->verts[offset], path->stroke, sizeof(nvgp_vertex_t) * path->nstroke);
        offset += path->nstroke;
      }
    }

    if (gl->flags & NVGP_GL_FLAG_STENCIL_STROKES) {
      // Fill shader
      call->uniform_offset = nvgp_gl_alloc_frag_uniforms(shader, 2);
      if (call->uniform_offset == -1) {
        NVGP_FREE(call);
        goto error;
      }

      nvgp_gl_convert_paint(gl, nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset), paint, scissor,
                          stroke_width, fringe, -1.0f);
      nvgp_gl_convert_paint(gl, nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset + shader->frag_size), paint,
                          scissor, stroke_width, fringe, 1.0f - 0.5f / 255.0f);

    } else {
      // Fill shader
      call->uniform_offset = nvgp_gl_alloc_frag_uniforms(shader, 1);
      if (call->uniform_offset == -1) {
        NVGP_FREE(call);
        goto error;
      }
      nvgp_gl_convert_paint(gl, nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset), paint, scissor,
                          stroke_width, fringe, -1.0f);
    }
    nvgp_darray_push(&gl->calls, call);
    return TRUE;
  }
error:
  return FALSE;
}

static nvgp_bool_t nvgp_gl_render_fast_draw_text(nvgp_gl_context_t* gl, nvgp_paint_t* paint, nvgp_scissor_t* scissor, nvgp_vertex_t* verts, uint32_t nverts) {
  if (!nvgp_gl_check_is_matrix_transformer(&paint->mat, 1.0f) && !nvgp_gl_check_is_matrix_transformer(&scissor->matrix, 1.0f) && nvgp_gl_verts_in_scissor(gl, verts, nverts, scissor)) {
    nvgp_gl_shader_t* shader;
    nvgp_gl_frag_uniforms_t* frag;
    float fringe = 1.0f / gl->pixel_ratio;
    nvgp_gl_call_text_t* call = NVGP_ZALLOC(nvgp_gl_call_text_t);
    if (call == NULL) {
      goto error;
    }
    call->base.call_type = NVGP_GL_CALL_TEXT;
    call->base.shader_type = NVGP_GL_SHADER_FAST_FILLGLYPH;
    call->blend_func = nvgp_gl_composite_operation_state(NVGP_GL_SOURCE_OVER);
    call->image = paint->image;
    shader = &(gl->shader_list[call->base.shader_type]);
    // Allocate vertices for all the paths.
    call->triangle_offset = nvgp_gl_alloc_verts(shader, nverts);
    if (call->triangle_offset == -1) {
      NVGP_FREE(call);
      goto error;
    }
    call->triangle_count = nverts;

    NVGP_MEMCPY(&shader->verts[call->triangle_offset], verts, sizeof(nvgp_vertex_t) * nverts);

    // Fill shader
    call->uniform_offset = nvgp_gl_alloc_frag_uniforms(shader, 1);
    if (call->uniform_offset == -1) {
      NVGP_FREE(call);
      goto error;
    }
    frag = nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset);
    nvgp_gl_convert_paint(gl, frag, paint, scissor, 1.0f, fringe, -1.0f);

    nvgp_darray_push(&gl->calls, call);
    return TRUE;
  }
error:
  return FALSE;
}

static int32_t nvgp_gl_render_draw_text_by_transformer(nvgp_gl_context_t* gl, nvgp_paint_t* paint, nvgp_scissor_t* scissor, nvgp_vertex_t* verts, uint32_t nverts) {
  nvgp_gl_shader_t* shader;
  nvgp_gl_frag_uniforms_t* frag;
  float fringe = 1.0f / gl->pixel_ratio;
  nvgp_gl_call_text_t* call = NVGP_ZALLOC(nvgp_gl_call_text_t);
  if (call == NULL) {
    goto error;
  }
  call->base.call_type = NVGP_GL_CALL_TEXT;
  call->base.shader_type = NVGP_GL_SHADER_FILLGLYPH;
  call->blend_func = nvgp_gl_composite_operation_state(NVGP_GL_SOURCE_OVER);
  call->image = paint->image;
  shader = &(gl->shader_list[call->base.shader_type]);
  // Allocate vertices for all the paths.
  call->triangle_offset = nvgp_gl_alloc_verts(shader, nverts);
  if (call->triangle_offset == -1) {
    NVGP_FREE(call);
    goto error;
  }
  call->triangle_count = nverts;

  NVGP_MEMCPY(&shader->verts[call->triangle_offset], verts, sizeof(nvgp_vertex_t) * nverts);

  // Fill shader
  call->uniform_offset = nvgp_gl_alloc_frag_uniforms(shader, 1);
  if (call->uniform_offset == -1) {
    NVGP_FREE(call);
    goto error;
  }
  frag = nvgp_gl_frag_uniform_ptr(shader, call->uniform_offset);
  nvgp_gl_convert_paint(gl, frag, paint, scissor, 1.0f, fringe, -1.0f);

  nvgp_darray_push(&gl->calls, call);
  return TRUE;
error:
  return FALSE;
}
 




static void nvgp_gl_render_fill(void* uptr, nvgp_paint_t* paint, nvgp_scissor_t* scissor, float fringe, const float* bounds, const nvgp_darray_t* paths) {
  nvgp_gl_context_t* gl = (nvgp_gl_context_t*)uptr;
  int32_t is_gradient = NVGP_MEMCMP(&(paint->inner_color), &(paint->outer_color), sizeof(paint->outer_color));
  if (paths->size <= 0) {
    return;
  }
  if (!is_gradient) {
    if (nvgp_gl_render_fast_fill_rect(gl, paint, scissor, fringe, bounds, paths)) {
      return;
    }
  }
  if (nvgp_gl_render_convex_fill_by_color(gl, paint, scissor, fringe, bounds, paths, is_gradient)) {
    return;
  } else if (nvgp_gl_render_fill_by_color(gl, paint, scissor, fringe, bounds, paths, is_gradient)) {
    return;
  } else if (nvgp_gl_render_draw_image(gl, paint, scissor, fringe, bounds, paths)) {
    return;
  }
}

static void nvgp_gl_render_stroke(void* uptr, nvgp_paint_t* paint, nvgp_scissor_t* scissor, float fringe, float stroke_width, const nvgp_darray_t* paths) {
  nvgp_gl_context_t* gl = (nvgp_gl_context_t*)uptr;
  int32_t is_gradient = NVGP_MEMCMP(&(paint->inner_color), &(paint->outer_color), sizeof(paint->outer_color));
  if (!is_gradient && gl->line_cap == NVGP_BUTT){
    if (nvgp_gl_render_fast_stroke_by_color(gl, paint, scissor, fringe, paths)) {
      return;
    }
  }
  if (nvgp_gl_render_stroke_by_color(gl, paint, scissor, fringe, stroke_width, paths, is_gradient)){
    return;
  } else if (nvgp_gl_render_stroke_by_image(gl, paint, scissor, fringe, stroke_width, paths)) {
    return;
  }
}

static void nvgp_gl_render_draw_text(void* uptr, nvgp_paint_t* paint, nvgp_scissor_t* scissor, nvgp_vertex_t* verts, uint32_t nverts) {
  nvgp_gl_context_t* gl = (nvgp_gl_context_t*)uptr;
  if (nvgp_gl_render_fast_draw_text(gl, paint, scissor, verts, nverts)) {
    return;
  } else if (nvgp_gl_render_draw_text_by_transformer(gl, paint, scissor, verts, nverts)) {
    return;
  }
}

static int32_t nvgp_gl_get_edge_anti_alias(void* uptr) {
  nvgp_gl_context_t* gl = (nvgp_gl_context_t*)uptr;
  return gl->edge_anti_alias;
}





#ifdef NVGP_GLES2
static uint32_t nvgp_gl_nearest_pow2(uint32_t num) {
  unsigned n = num > 0 ? num - 1 : 0;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n++;
  return n;
}
#endif

static int nvgp_gl_create_texture(void* uptr, int type, int w, int h, int stride, int image_flags, const unsigned char* data) {
  nvgp_gl_context_t* gl = (nvgp_gl_context_t*)uptr;
  nvgp_gl_texture_t* tex = nvgp_darray_get_empty_by_tail(&gl->textures, nvgp_gl_texture_t);
  if (tex != NULL) {
 
#ifdef NVGP_GLES2
    // Check for non-power of 2.
    if (nvgp_gl_nearest_pow2(w) != (uint32_t)w || nvgp_gl_nearest_pow2(h) != (uint32_t)h) {
      // No repeat
      if ((image_flags & NVGP_GL_IMAGE_REPEATX) != 0 || (image_flags & NVGP_GL_IMAGE_REPEATY) != 0) {
        // printf("Repeat X/Y is not supported for non power-of-two textures (%d x %d)\n", w, h);
        image_flags &= ~(NVGP_GL_IMAGE_REPEATX | NVGP_GL_IMAGE_REPEATY);
      }
      // No mips.
      if (image_flags & NVGP_GL_IMAGE_GENERATE_MIPMAPS) {
        // printf("Mip-maps is not support for non power-of-two textures (%d x %d)\n", w, h);
        image_flags &= ~NVGP_GL_IMAGE_GENERATE_MIPMAPS;
      }
    }
#endif

    glGenTextures(1, &tex->tex);
    tex->width = w;
    tex->height = h;
    tex->type = type;
    tex->id = ++gl->texture_id;
    tex->flags = image_flags;
    tex->data = data;
    nvgp_gl_bind_texture(gl, tex->tex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
#ifndef NVGP_GLES2
    glPixelStorei(GL_UNPACK_ROW_LENGTH, tex->width);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

#ifdef NVGP_GL2
    // GL 1.4 and later has support for generating mipmaps using a tex parameter.
    if (image_flags & NVGP_GL_IMAGE_GENERATE_MIPMAPS) {
      glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    }
#endif

    if (type == NVGP_TEXTURE_RGBA) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
#if defined(NVGP_GLES2) || defined(NVGP_GL2)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
#elif defined(NVGP_GLES3)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
#else
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
#endif
    }

    if (image_flags & NVGP_GL_IMAGE_GENERATE_MIPMAPS) {
      if (image_flags & NVGP_GL_IMAGE_NEAREST) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      }
    } else {
      if (image_flags & NVGP_GL_IMAGE_NEAREST) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      }
    }

    if (image_flags & NVGP_GL_IMAGE_NEAREST) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    if (image_flags & NVGP_GL_IMAGE_REPEATX)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    else
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    if (image_flags & NVGP_GL_IMAGE_REPEATY)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    else
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
#ifndef NVGP_GLES2
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

    // The new way to build mipmaps on GLES and GL3
#ifndef NVGP_GL2
    if (image_flags & NVGP_GL_IMAGE_GENERATE_MIPMAPS) {
      glGenerateMipmap(GL_TEXTURE_2D);
    }
#endif

    nvgp_gl_check_error(gl, "create tex");
    nvgp_gl_bind_texture(gl, 0);

    return tex->id;
  }
  return 0;
}

static int nvgp_gl_find_texture_by_data(void* uptr, const void* data) {
  uint32_t i;
  nvgp_gl_context_t* gl = (nvgp_gl_context_t*)uptr;
  for (i = 0; i < gl->textures.size; i++) {
    nvgp_gl_texture_t* texture = nvgp_darray_get_ptr(&gl->textures, i, nvgp_gl_texture_t);
    if (texture->data == data) {
      return texture->id;
    }
  }
  return -1;
}

static nvgp_bool_t nvgp_gl_delete_texture(void* uptr, int image) {
  uint32_t i;
  nvgp_gl_context_t* gl = (nvgp_gl_context_t*)uptr;
  for (i = 0; i < gl->textures.size; i++) {
    nvgp_gl_texture_t* texture = nvgp_darray_get_ptr(&gl->textures, i, nvgp_gl_texture_t);
    if (texture->id == image) {
      glDeleteTextures(1, &texture->tex);
      nvgp_darray_remove(&gl->textures, i, NULL, NULL);
      return TRUE;
    }
  }
  return FALSE;
}

static nvgp_bool_t nvgp_gl_update_texture(void* uptr, int image, int x, int y, int w, int h, const unsigned char* data) {
  nvgp_gl_context_t* gl = (nvgp_gl_context_t*)uptr;
  nvgp_gl_texture_t* tex = nvgp_gl_find_texture(gl, image);

  if (tex == NULL) {
    return FALSE;
  }
  nvgp_gl_bind_texture(gl, tex->tex);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

#ifndef NVGP_GLES2
  glPixelStorei(GL_UNPACK_ROW_LENGTH, tex->width);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, x);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, y);
#else
  // No support for all of skip, need to update a whole row at a time.
  if (tex->type == NVGP_TEXTURE_RGBA) {
    data += y * tex->width * 4;
  } else {
    data += y * tex->width;
  }
  x = 0;
  w = tex->width;
#endif

  if (tex->type == NVGP_TEXTURE_RGBA) {
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
  } else {
#if defined(NVGP_GLES2) || defined(NVGP_GL2)
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
#else
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RED, GL_UNSIGNED_BYTE, data);
#endif
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
#ifndef NVGP_GLES2
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

  nvgp_gl_bind_texture(gl, 0);

  return TRUE;
}

static nvgp_bool_t nvgp_gl_get_texture_size(void* uptr, int image, int* w, int* h) {
  nvgp_gl_context_t* gl = (nvgp_gl_context_t*)uptr;
  nvgp_gl_texture_t* tex = nvgp_gl_find_texture(gl, image);
  if (tex == NULL) {
    return FALSE;
  }
  *w = tex->width;
  *h = tex->height;
  return TRUE;
}

static void nvgp_gl_render_cancel(void* uptr) {
  uint32_t i = 0;
  nvgp_gl_context_t* gl = (nvgp_gl_context_t*)uptr;
  for (i = 0; i < nvgp_get_arrary_size(gl->shader_list); i++) {
    nvgp_darray_clear(&gl->paths);
    gl->shader_list[i].nverts = 0;
    gl->shader_list[i].nuniforms = 0;
  }
  nvgp_darray_clear_by_destroy_function(&gl->calls, nvgp_gl_call_destroy, NULL);
}

static void nvgp_gl_set_line_cap(void* uptr, int line_cap) {
  nvgp_gl_context_t* gl = (nvgp_gl_context_t*)uptr;
  gl->line_cap = line_cap;
}

static void nvgp_gl_destroy(void* uptr) {
  int32_t i = 0;
  nvgp_gl_context_t* ctx = (nvgp_gl_context_t*)uptr;
  if (ctx == NULL) {
    return;
  }

  for (i = 0; i < NVGP_GL_SHADER_COUNT; i++) {
    nvgp_gl_delete_shader(&ctx->shader_list[i]);
  }
  nvgp_darray_clear_by_destroy_function(&ctx->calls, nvgp_gl_call_destroy, NULL);
  nvgp_darray_clear_by_destroy_function(&ctx->textures, nvgp_gl_texture_destroy, NULL);
  nvgp_darray_deinit(&ctx->calls);
  nvgp_darray_deinit(&ctx->paths);
  nvgp_darray_deinit(&ctx->textures);
  NVGP_FREE(ctx);
}





int32_t nvgp_gl_get_gpu_texture_id(nvgp_gl_context_t* gl, int32_t image_id) {
  int32_t i;
  for (i = 0; i < gl->textures.size; i++){
    nvgp_gl_texture_t* texture = nvgp_darray_get_ptr(&gl->textures, i, nvgp_gl_texture_t);
    if (texture->id == image_id) {
      return texture->tex;
    }
  }
  return -1;
}

nvgp_gl_context_t* nvgp_gl_create(int flags) {
  nvgp_gl_context_t* context = NVGP_ZALLOC(nvgp_gl_context_t);
  if (context != NULL) {
    context->flags = flags;
    context->edge_anti_alias = flags & NVGP_GL_FLAG_ANTIALIAS ? 1 : 0;
    if (!nvgp_gl_render_create(context)) {
      goto error;
    }
    context->curr_prog = 0;
    context->texture_id = 0;
    nvgp_darray_init(&context->calls, NVGP_GL_INIT_CALL_NUMBER, 0);
    nvgp_darray_init(&context->paths, NVGP_GL_INIT_PATH_NUMBER, sizeof(nvgp_gl_path_t));
    nvgp_darray_init(&context->textures, NVGP_GL_INIT_CALL_NUMBER, sizeof(nvgp_gl_texture_t));
  }
  return context;
error:
  nvgp_gl_destroy(context);
  return NULL;
}



static const nvgp_vtable_t vt = {
  .clear_cache = NULL,
  .find_texture = nvgp_gl_find_texture_by_data,
  .create_texture = nvgp_gl_create_texture,
  .delete_texture = nvgp_gl_delete_texture,
  .update_texture = nvgp_gl_update_texture,
  .get_texture_size = nvgp_gl_get_texture_size,
  .get_edge_anti_alias = nvgp_gl_get_edge_anti_alias,

  .end_frame = nvgp_gl_end_frame,
  .begin_frame = nvgp_gl_begin_frame,
  .set_line_cap = nvgp_gl_set_line_cap,
  .set_line_join = NULL,
  .render_cancel = nvgp_gl_render_cancel,
  .render_fill = nvgp_gl_render_fill,
  .render_stroke = nvgp_gl_render_stroke,
  .render_draw_text = nvgp_gl_render_draw_text,
  .destroy = nvgp_gl_destroy,
  
};

const nvgp_vtable_t* nvgp_gl_vtable() {
  return &vt;
} 
