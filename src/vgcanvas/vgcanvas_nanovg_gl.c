/**
 * File:   vgcanvas_nanovg_gl.c
 * Author: AWTK Develop Team
 * Brief:  vector graphics canvas base on nanovg-gl
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
 * 2018-04-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/system_info.h"

#if defined(WITH_NANOVG_GL3)
#define NANOVG_GL3_IMPLEMENTATION
#endif

#if defined(WITH_NANOVG_GLES2)
#define NANOVG_GLES2_IMPLEMENTATION
#endif

#if defined(WITH_NANOVG_GLES3)
#define NANOVG_GLES3_IMPLEMENTATION
#endif

#ifdef WITHOUT_GLAD
#include <SDL.h>

#ifdef IOS
#include <OpenGLES/gltypes.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#endif /*IOS*/

#else
#include "glad/glad.h"
#endif /*WITHOUT_GLAD*/

#include "nanovg.h"
#include "tkc/utf8.h"
#include "tkc/mem.h"
#include "base/vgcanvas.h"
#include "base/image_manager.h"
#include "base/native_window.h"

#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"

typedef struct _vgcanvas_nanovg_screen_shader_info_t {
  GLuint program_object;
  GLuint coord_loc;
  GLuint position_loc;
  GLuint screentexture_loc;
  GLuint indexs[6];
  GLuint vboIds[3];
#if defined NANOVG_GL3
  GLuint vao;
#endif
} vgcanvas_nanovg_screen_shader_info_t;

typedef struct _vgcanvas_nanovg_offline_fb_t {
  GLuint fbo;
  GLuint last_fbo;
  GLuint rbo;
  GLuint textureId;

  uint32_t width;
  uint32_t height;
} vgcanvas_nanovg_offline_fb_t;

typedef struct _vgcanvas_nanovg_t {
  vgcanvas_t base;

  int font_id;
  NVGcontext* vg;
  uint32_t text_align_v;
  uint32_t text_align_h;

  vgcanvas_nanovg_offline_fb_t* offline_fb;
  vgcanvas_nanovg_screen_shader_info_t* shader_program;

  native_window_t* window;
} vgcanvas_nanovg_t;

static const char* vgcanvas_nanovg_shader_header =
#if defined NANOVG_GL2
    " #define NANOVG_GL2 1                                  \n"
#elif defined NANOVG_GL3
    " #version 150 core                                     \n"
    " #define NANOVG_GL3 1                                  \n"
#elif defined NANOVG_GLES2
    " #version 100                                          \n"
    " #define NANOVG_GL2 1                                  \n"
#elif defined NANOVG_GLES3
    " #version 300 es                                       \n"
    " #define NANOVG_GL3 1                                  \n"
#endif
    ;

static inline GLuint vgcanvas_create_LoadShader(const char* g_strShaderHeader,
                                                const char* g_strVertexShader,
                                                const char* g_strFragmentShader) {
#define INFO_LOG_CHAR_LENGTH 512

  GLint is_Success = 0;
  GLint vertex_shader = 0;
  GLint shader_program = 0;
  GLint framgent_shader = 0;
  const char* strShaders[2] = {0};
  char log[INFO_LOG_CHAR_LENGTH] = {0};

  strShaders[0] = g_strShaderHeader != NULL ? g_strShaderHeader : "";
  strShaders[1] = g_strVertexShader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 2, (const GLchar* const*)&strShaders, NULL);
  glCompileShader(vertex_shader);

  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_Success);
  if (is_Success == 0) {
    memset(log, 0, INFO_LOG_CHAR_LENGTH);
    glGetShaderInfoLog(vertex_shader, INFO_LOG_CHAR_LENGTH, NULL, log);
    log_debug("Compile Vertex Shager fail message : %s \r\n", log);
  }

  strShaders[1] = g_strFragmentShader;
  framgent_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(framgent_shader, 2, (const GLchar* const*)&strShaders, NULL);
  glCompileShader(framgent_shader);

  glGetShaderiv(framgent_shader, GL_COMPILE_STATUS, &is_Success);
  if (is_Success == 0) {
    memset(log, 0, INFO_LOG_CHAR_LENGTH);
    glGetShaderInfoLog(framgent_shader, INFO_LOG_CHAR_LENGTH, NULL, log);
    log_debug("Compile Framgent Shader fail message : %s \r\n", log);
  }

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, framgent_shader);

  glLinkProgram(shader_program);

  glDeleteShader(vertex_shader);
  glDeleteShader(framgent_shader);

  glGetProgramiv(shader_program, GL_LINK_STATUS, &is_Success);
  if (is_Success == 0) {
    memset(log, 0, INFO_LOG_CHAR_LENGTH);
    glGetProgramInfoLog(shader_program, INFO_LOG_CHAR_LENGTH, NULL, log);
    log_debug("Link Program fail message : %s \r\n", log);
    glDeleteProgram(shader_program);
    return 0;
  }

  return shader_program;
}

vgcanvas_nanovg_screen_shader_info_t* vgcanvas_create_init_screen_shader() {
  const GLuint indexs[] = {0, 1, 3, 1, 2, 3};

  const GLfloat vertexs[] = {
      // Position
      1.0f,  1.0f,  0.0f,  // top right
      1.0f,  -1.0f, 0.0f,  // bottm right
      -1.0f, -1.0f, 0.0f,  // bottm left
      -1.0f, 1.0f,  0.0f   // top left
  };

  const GLfloat tcoords[] = {
      // texture coords
      1.0f, 1.0f,  // top right
      1.0f, 0.0f,  // bottm right
      0.0f, 0.0f,  // bottm left
      0.0f, 1.0f   // top left
  };

  const char* vertex_shader =
      " #ifdef NANOVG_GL3                                                                     \n"
      "   in vec3 g_vPosition;                                                                \n"
      "   in vec2 tcoord;                                                                     \n"
      "   out vec2 ftcoord;                                                                   \n"
      " #else                                                                                 \n"
      "   attribute vec3 g_vPosition;                                                         \n"
      "   attribute vec2 tcoord;                                                              \n"
      "   varying vec2 ftcoord;                                                               \n"
      " #endif                                                                                \n"
      " void main(void) {                                                                     \n"
      "    ftcoord = tcoord;                                                                  \n"
#if defined(WITH_SCREEN_GL_FLIP_VERTICAL) && defined(WITH_SCREEN_GL_FLIP_HORIZONTAL)
      "    gl_Position = vec4(-g_vPosition.x, -g_vPosition.y, g_vPosition.z, 1.0);            \n"
#elif defined(WITH_SCREEN_GL_FLIP_VERTICAL)
      "    gl_Position = vec4(g_vPosition.x, -g_vPosition.y, g_vPosition.z, 1.0);             \n"
#elif defined(WITH_SCREEN_GL_FLIP_HORIZONTAL)
      "    gl_Position = vec4(-g_vPosition.x, g_vPosition.y, g_vPosition.z, 1.0);             \n"
#else
      "    gl_Position = vec4(g_vPosition.x, g_vPosition.y, g_vPosition.z, 1.0);              \n"
#endif
      " }                                                                                     \n";

  const char* fragment_shader =
      " #ifdef GL_ES                                                                          \n"
      " #if defined(GL_FRAGMENT_PRECISION_HIGH) || defined(NANOVG_GL3)                        \n"
      "   precision highp float;                                                              \n"
      " #else                                                                                 \n"
      "   precision mediump float;                                                            \n"
      " #endif                                                                                \n"
      " #endif                                                                                \n"
      " #ifdef NANOVG_GL3                                                                     \n"
      "   in vec2 ftcoord;                                                                    \n"
      "   out vec4 outColor;                                                                  \n"
      " #else                                                                                 \n"
      "   varying vec2 ftcoord;                                                               \n"
      " #endif                                                                                \n"
      " uniform sampler2D screentexture;                                                      \n"
      " void main() {                                                                         \n"
      " #ifdef NANOVG_GL3                                                                     \n"
      "    vec4 color = texture(screentexture, ftcoord);                                      \n"
#if defined(WITH_SCREEN_GL_BGRA)
      "    outColor = vec4(color.b, color.g, color.r, color.a);                               \n"
#else
      "    outColor = vec4(color.r, color.g, color.b, color.a);                               \n"
#endif
      " #else                                                                                 \n"
      "    vec4 color = texture2D(screentexture, ftcoord);                                    \n"
#if defined(WITH_SCREEN_GL_BGRA)
      "    gl_FragColor = vec4(color.b, color.g, color.r, color.a);                           \n"
#else
      "    gl_FragColor = vec4(color.r, color.g, color.b, color.a);                           \n"
#endif
      " #endif                                                                                \n"
      " }                                                                                     \n";

  vgcanvas_nanovg_screen_shader_info_t* shader_info =
      (vgcanvas_nanovg_screen_shader_info_t*)TKMEM_ZALLOC(vgcanvas_nanovg_screen_shader_info_t);
  return_value_if_fail(shader_info != NULL, NULL);

  memcpy(shader_info->indexs, indexs, sizeof(indexs));

  shader_info->program_object =
      vgcanvas_create_LoadShader(vgcanvas_nanovg_shader_header, vertex_shader, fragment_shader);

  if (shader_info->program_object == 0) {
    TKMEM_FREE(shader_info);
    return NULL;
  }

  shader_info->coord_loc = glGetAttribLocation(shader_info->program_object, "tcoord");
  shader_info->position_loc = glGetAttribLocation(shader_info->program_object, "g_vPosition");
  shader_info->screentexture_loc =
      glGetUniformLocation(shader_info->program_object, "screentexture");

  glGenBuffers(sizeof(shader_info->vboIds) / sizeof(GLuint), shader_info->vboIds);

#if defined NANOVG_GL3
  glGenVertexArrays(1, &shader_info->vao);

  glBindVertexArray(shader_info->vao);
  glBindBuffer(GL_ARRAY_BUFFER, shader_info->vboIds[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, shader_info->vboIds[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tcoords), tcoords, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader_info->vboIds[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(shader_info->indexs), shader_info->indexs,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
#else
  glBindBuffer(GL_ARRAY_BUFFER, shader_info->vboIds[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, shader_info->vboIds[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tcoords), tcoords, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, shader_info->vboIds[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(shader_info->indexs), shader_info->indexs, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif

  return shader_info;
}

static void vgcanvas_destroy_offline_fb(vgcanvas_nanovg_offline_fb_t* offline_fb) {
  glDeleteFramebuffers(1, &offline_fb->fbo);
  glDeleteRenderbuffers(1, &offline_fb->rbo);
  glDeleteTextures(1, &offline_fb->textureId);
  TKMEM_FREE(offline_fb);
}

static vgcanvas_nanovg_offline_fb_t* vgcanvas_create_offline_fb(uint32_t width, uint32_t height) {
  GLenum status = 0;
  GLint default_fbo = 0;
  GLint default_rbo = 0;
  GLint max_render_buffer_size = 0;
  vgcanvas_nanovg_offline_fb_t* offline_fb =
      (vgcanvas_nanovg_offline_fb_t*)TKMEM_ZALLOC(vgcanvas_nanovg_offline_fb_t);

  return_value_if_fail(offline_fb != NULL, NULL);

  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &default_fbo);
  glGetIntegerv(GL_RENDERBUFFER_BINDING, &default_rbo);
  glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &max_render_buffer_size);

  if ((max_render_buffer_size <= width) || (max_render_buffer_size <= height)) {
    log_debug("Cannot use framebuffer objects! \r\n");
    TKMEM_FREE(offline_fb);
    return NULL;
  }
  offline_fb->width = width;
  offline_fb->height = height;

  glGenFramebuffers(1, &offline_fb->fbo);
  glGenRenderbuffers(1, &offline_fb->rbo);

  glBindRenderbuffer(GL_RENDERBUFFER, offline_fb->rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, width, height);

  glGenTextures(1, &offline_fb->textureId);
  glBindTexture(GL_TEXTURE_2D, offline_fb->textureId);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindFramebuffer(GL_FRAMEBUFFER, offline_fb->fbo);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offline_fb->textureId,
                         0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                            offline_fb->rbo);

  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    log_debug("Framebuffer object is not complete! \r\n");
    return FALSE;
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, default_fbo);
  glBindRenderbuffer(GL_RENDERBUFFER, default_rbo);

  return offline_fb;
}

#include "texture.inc"
#include "vgcanvas_nanovg_gl.inc"
#include "vgcanvas_nanovg.inc"

vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format,
                            void* win) {
  native_window_info_t info;
  native_window_t* window = NATIVE_WINDOW(win);
  return_value_if_fail(native_window_get_info(win, &info) == RET_OK, NULL);
  vgcanvas_nanovg_t* nanovg = (vgcanvas_nanovg_t*)TKMEM_ZALLOC(vgcanvas_nanovg_t);
  return_value_if_fail(nanovg != NULL, NULL);

  (void)format;

  nanovg->base.w = w;
  nanovg->base.h = h;
  nanovg->base.vt = &vt;
  nanovg->window = window;
  nanovg->base.ratio = info.ratio;

#if defined(WITH_NANOVG_GL3)
  nanovg->vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#elif defined(WITH_NANOVG_GLES2)
  nanovg->vg = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#elif defined(WITH_NANOVG_GLES3)
  nanovg->vg = nvgCreateGLES3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#endif

  if (nanovg->vg == NULL) {
    assert(!"OpenGL is not supported!");
  }

  nanovg->shader_program = vgcanvas_create_init_screen_shader();
  nanovg->offline_fb = vgcanvas_create_offline_fb(w * info.ratio, h * info.ratio);

  return &(nanovg->base);
}
