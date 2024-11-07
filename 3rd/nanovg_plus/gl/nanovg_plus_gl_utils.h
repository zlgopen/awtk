/**
 * File:   nanovg_plus_gl_utils.h
 * Author: AWTK Develop Team
 * Brief:  opengl utils function.
 *
 * Copyright (c) 2018 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef __NANOVG_PLUS_GL_UTILS_H__
#define __NANOVG_PLUS_GL_UTILS_H__

#include "nanovg_plus_gl.h"

typedef struct _nvgp_gl_util_framebuffer {
  nvgp_context_t* ctx;
  GLuint fbo;
  GLuint rbo;
  GLuint texture;
  int32_t image;
} nvgp_gl_util_framebuffer;

#if defined(NVGP_GL3) || defined(NVGP_GLES2) || defined(NVGP_GLES3)
#define NVGP_FBO_VALID 1
#elif defined(NVGP_GL2)
#ifdef __APPLE__
#include <OpenGL/glext.h>
#define NVGP_FBO_VALID 1
#endif
#endif

static GLint s_nvgp_gl_default_fbo = -1;

int nvgp_gl_get_curr_framebuffer() {
#ifdef NVGP_FBO_VALID
  GLint s_nvgp_gl_default_fbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &s_nvgp_gl_default_fbo);
  return s_nvgp_gl_default_fbo;
#else
  return -1;
#endif
}

void nvgp_gl_delete_framebuffer(nvgp_gl_util_framebuffer* fb) {
#ifdef NVGP_FBO_VALID
  if (fb == NULL) {
    return;
  }
  if (fb->fbo != 0) {
    glDeleteFramebuffers(1, &fb->fbo);
  }
  if (fb->rbo != 0) {
    glDeleteRenderbuffers(1, &fb->rbo);
  }
  if (fb->image >= 0) {
    nvgp_delete_image(fb->ctx, fb->image);
  }
  fb->ctx = NULL;
  fb->fbo = 0;
  fb->rbo = 0;
  fb->texture = 0;
  fb->image = -1;
  NVGP_FREE(fb);
#else
  (void)(fb);
#endif
}

nvgp_gl_util_framebuffer* nvgp_gl_create_framebuffer(nvgp_context_t* ctx, int32_t w, int32_t h, int32_t imageFlags) {
#ifdef NVGP_FBO_VALID
  GLint s_nvgp_gl_default_fbo;
  GLint defaultRBO;
  nvgp_gl_util_framebuffer* fb = NULL;

  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &s_nvgp_gl_default_fbo);
  glGetIntegerv(GL_RENDERBUFFER_BINDING, &defaultRBO);

  fb = NVGP_ZALLOC(nvgp_gl_util_framebuffer);
  if (fb == NULL) {
    goto error;
  }
  NVGP_MEMSET(fb, 0, sizeof(nvgp_gl_util_framebuffer));

  fb->image = nvgp_create_image_rgba(ctx, w, h, imageFlags | NVGP_GL_IMAGE_FLIPY | NVGP_GL_IMAGE_PREMULTIPLIED, NULL);

  fb->texture = nvgp_gl_get_gpu_texture_id((nvgp_gl_context_t*)nvgp_get_vt_ctx(ctx), fb->image);

  fb->ctx = ctx;

  // frame buffer object
  glGenFramebuffers(1, &fb->fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);

  // render buffer object
  glGenRenderbuffers(1, &fb->rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, fb->rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, w, h);

  // combine all
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb->texture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb->rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
#ifdef GL_DEPTH24_STENCIL8
    // If GL_STENCIL_INDEX8 is not supported, try GL_DEPTH24_STENCIL8 as a fallback.
    // Some graphics cards require a depth buffer along with a stencil.
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb->texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb->rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
#endif // GL_DEPTH24_STENCIL8
      goto error;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, s_nvgp_gl_default_fbo);
  glBindRenderbuffer(GL_RENDERBUFFER, defaultRBO);
  return fb;
error:
  glBindFramebuffer(GL_FRAMEBUFFER, s_nvgp_gl_default_fbo);
  glBindRenderbuffer(GL_RENDERBUFFER, defaultRBO);
  nvgp_gl_delete_framebuffer(fb);
  return NULL;
#else
  (void)(ctx);
  (void)(w);
  (void)(h);
  (void)(imageFlags);
  return NULL;
#endif
}

void nvgp_gl_bind_framebuffer(nvgp_gl_util_framebuffer* fb) {
#ifdef NVGP_FBO_VALID
  if (s_nvgp_gl_default_fbo == -1) {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &s_nvgp_gl_default_fbo);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, fb != NULL ? fb->fbo : s_nvgp_gl_default_fbo);
#else
  (void)(fb);
#endif
}

void nvgp_gl_read_current_framebuffer_data(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int width, unsigned int height, void* pixels) {
  if(x + w <= width && y + h <= height && pixels != NULL) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  }
}

#endif // NVGP_GL_UTILS_H

