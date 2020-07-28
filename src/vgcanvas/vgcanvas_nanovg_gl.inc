#include "tkc/utils.h"

static ret_t vgcanvas_nanovg_reinit(vgcanvas_t* vg, uint32_t w, uint32_t h, uint32_t stride,
                                    bitmap_format_t format, void* data) {
  (void)vg;
  (void)w;
  (void)h;
  (void)format;
  (void)data;
  return RET_OK;
}

static inline void vgcanvas_nanovg_set_offline_fb(vgcanvas_nanovg_t* canvas, uint32_t w,
                                                  uint32_t h, rect_t* dirty_rect) {
  GLint default_fbo = 0;

#if WITH_LCD_CLEAR_ALPHA
  rect_t r;
  system_info_t* info = system_info();
#endif

  vgcanvas_nanovg_offline_fb_t* offline_fb = canvas->offline_fb;
  if (offline_fb != NULL) {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &default_fbo);

    if (offline_fb->width != w || offline_fb->height != h) {
      vgcanvas_destroy_offline_fb(canvas->offline_fb);
      canvas->offline_fb = vgcanvas_create_offline_fb(w, h);
      offline_fb = canvas->offline_fb;
    }

    if (offline_fb != NULL) {
      offline_fb->last_fbo = (GLuint)default_fbo;
      glBindFramebuffer(GL_FRAMEBUFFER, offline_fb->fbo);
      glViewport(0, 0, w, h);
      // 去除这两行注释就可以显示脏矩形的工作原理。
      // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

#if WITH_LCD_CLEAR_ALPHA
      switch (info->lcd_orientation) {
        case LCD_ORIENTATION_0:
          r = rect_init(dirty_rect->x, h - dirty_rect->h - dirty_rect->y, dirty_rect->w, dirty_rect->h);
          break;
        case LCD_ORIENTATION_90:
          r = rect_init(w - dirty_rect->h - dirty_rect->y, h - dirty_rect->w - dirty_rect->x, dirty_rect->h, dirty_rect->w);
          break;
        case LCD_ORIENTATION_180:
          r = rect_init(w - dirty_rect->w - dirty_rect->x, dirty_rect->y, dirty_rect->w, dirty_rect->h);
          break;
        case LCD_ORIENTATION_270:
          r = rect_init(dirty_rect->y, dirty_rect->x, dirty_rect->h, dirty_rect->w);
          break;
      }
      
      // 把脏矩形的区域刷新为黑色透明度为0的颜色
      glScissor(r.x, r.y, r.w, r.h);
      glEnable(GL_SCISSOR_TEST);
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      glDisable(GL_SCISSOR_TEST);
#else
      (void)dirty_rect;
#endif

    }
  }
}

static inline void vgcanvas_nanovg_offline_fb_flush(vgcanvas_nanovg_t* canvas) {
  system_info_t* info = system_info();
  vgcanvas_nanovg_offline_fb_t* offline_fb = canvas->offline_fb;
  vgcanvas_nanovg_screen_shader_info_t* shader_info = canvas->shader_program;
  if (offline_fb != NULL && shader_info != NULL) {
    glBindFramebuffer(GL_FRAMEBUFFER, offline_fb->last_fbo);

    glViewport(0, 0, info->lcd_w * info->device_pixel_ratio,
               info->lcd_h * info->device_pixel_ratio);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(shader_info->program_object);

#if defined NANOVG_GL3
    glBindVertexArray(shader_info->vao);
#endif

    glBindBuffer(GL_ARRAY_BUFFER, shader_info->vboIds[0]);
    glVertexAttribPointer(shader_info->position_loc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(shader_info->position_loc);

    glBindBuffer(GL_ARRAY_BUFFER, shader_info->vboIds[1]);
    glVertexAttribPointer(shader_info->coord_loc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(shader_info->coord_loc);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, offline_fb->textureId);
    glUniform1i(shader_info->screentexture_loc, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader_info->vboIds[2]);
    glDrawElements(GL_TRIANGLES, sizeof(shader_info->indexs) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(shader_info->position_loc);
    glDisableVertexAttribArray(shader_info->coord_loc);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

#if defined NANOVG_GL3
    glBindVertexArray(0);
#endif
  }
}

static ret_t vgcanvas_nanovg_begin_frame(vgcanvas_t* vgcanvas, rect_t* dirty_rect) {
  float_t angle = 0.0f;
  float_t anchor_x = 0.0f;
  float_t anchor_y = 0.0f;

  system_info_t* info = system_info();
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;

  native_window_gl_make_current(canvas->window);

  if (dirty_rect != NULL) {
    canvas->base.dirty_rect = rect_init(dirty_rect->x, dirty_rect->y, dirty_rect->w, dirty_rect->h);
  } else {
    if (info->lcd_orientation == LCD_ORIENTATION_90 ||
        info->lcd_orientation == LCD_ORIENTATION_270) {
      canvas->base.dirty_rect = rect_init(0, 0, info->lcd_h, info->lcd_w);
    } else {
      canvas->base.dirty_rect = rect_init(0, 0, info->lcd_w, info->lcd_h);
    }
  }

  vgcanvas_nanovg_set_offline_fb(canvas, info->lcd_w * info->device_pixel_ratio,
                                 info->lcd_h * info->device_pixel_ratio, &canvas->base.dirty_rect);

  nvgBeginFrame(canvas->vg, info->lcd_w, info->lcd_h, info->device_pixel_ratio);

  switch (info->lcd_orientation) {
    case LCD_ORIENTATION_0:
      angle = 0.0f;
      break;
    case LCD_ORIENTATION_90:
      angle = TK_D2R(90);
      break;
    case LCD_ORIENTATION_180:
      angle = TK_D2R(180);
      break;
    case LCD_ORIENTATION_270:
      angle = TK_D2R(270);
      break;
  }

  anchor_x = info->lcd_w / 2.0f;
  anchor_y = info->lcd_h / 2.0f;

  nvgSave(canvas->vg);

  if (info->lcd_orientation == LCD_ORIENTATION_90 || info->lcd_orientation == LCD_ORIENTATION_270) {
    nvgTranslate(canvas->vg, anchor_x, anchor_y);
    nvgRotate(canvas->vg, angle);
    nvgTranslate(canvas->vg, -anchor_y, -anchor_x);
  } else if (info->lcd_orientation == LCD_ORIENTATION_180) {
    nvgTranslate(canvas->vg, anchor_x, anchor_y);
    nvgRotate(canvas->vg, angle);
    nvgTranslate(canvas->vg, -anchor_x, -anchor_y);
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_end_frame(vgcanvas_t* vgcanvas) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;

  nvgRestore(vg);
  nvgEndFrame(vg);

  vgcanvas_nanovg_offline_fb_flush(canvas);

  native_window_swap_buffer(canvas->window);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_create_fbo(vgcanvas_t* vgcanvas, uint32_t w, uint32_t h, framebuffer_object_t* fbo) {
  NVGLUframebuffer* handle = NULL;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  handle = nvgluCreateFramebuffer(vg, (int)(w * vgcanvas->ratio),
                                  (int)(h * vgcanvas->ratio), 0);
  return_value_if_fail(handle != NULL, RET_FAIL);

  fbo->w = w;
  fbo->h = h;
  fbo->init = FALSE;
  fbo->handle = handle;
  fbo->id = handle->image;
  fbo->ratio = vgcanvas->ratio;
  fbo->offline_fbo = handle->fbo;
  fbo->online_fbo = nvgluGetCurrFramebuffer();

  return RET_OK;
}

static ret_t vgcanvas_nanovg_destroy_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  NVGLUframebuffer* handle = (NVGLUframebuffer*)fbo->handle;
  handle->fbo = fbo->offline_fbo;
  nvgluDeleteFramebuffer(handle);
  (void)vgcanvas;

  return RET_OK;
}

static ret_t vgcanvas_nanovg_bind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  NVGcontext* vg = NULL;
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGLUframebuffer* handle = (NVGLUframebuffer*)fbo->handle;
  
  vg = canvas->vg;

  fbo->online_fbo = nvgluGetCurrFramebuffer();
  fbo->online_dirty_rect = rect_init(vgcanvas->dirty_rect.x, vgcanvas->dirty_rect.y, vgcanvas->dirty_rect.w, vgcanvas->dirty_rect.h);

  handle->fbo = fbo->offline_fbo;
  vgcanvas->dirty_rect = rect_init(0, 0, fbo->w, fbo->h);
  nvgluBindFramebuffer(handle);
  glViewport(0, 0, fbo->w * fbo->ratio, fbo->h * fbo->ratio);

  if (!fbo->init) {
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    fbo->init = TRUE;
  }

  nvgBeginFrame(vg, fbo->w, fbo->h, fbo->ratio);

  
  return RET_OK;
}

static ret_t vgcanvas_nanovg_unbind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  NVGcontext* vg = NULL;
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGLUframebuffer* handle = (NVGLUframebuffer*)fbo->handle;

  vg = canvas->vg;

  handle->fbo = fbo->online_fbo;
  nvgEndFrame(vg);
  nvgluBindFramebuffer(handle);

  vgcanvas->dirty_rect = rect_init(fbo->online_dirty_rect.x, fbo->online_dirty_rect.y, fbo->online_dirty_rect.w, fbo->online_dirty_rect.h);
  glViewport(0, 0, vgcanvas->w * fbo->ratio, vgcanvas->h * fbo->ratio);
  nvgBeginFrame(vg, vgcanvas->w, vgcanvas->h, fbo->ratio);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_fbo_to_bitmap(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo,
                                           bitmap_t* img, rect_t* r) {
  uint32_t i = 0;
  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t height = 0;
  uint8_t* p = NULL;
  uint8_t* data = NULL;
  uint8_t* img_data = NULL;

  int online_fbo = nvgluGetCurrFramebuffer();
  NVGLUframebuffer* handle = (NVGLUframebuffer*)fbo->handle;

  handle->fbo = fbo->offline_fbo;
  nvgluBindFramebuffer(handle);
  data = TKMEM_ZALLOCN(uint8_t, img->h * img->line_length);
  img_data = (uint8_t*)bitmap_lock_buffer_for_write(img);
  height = fbo->h * fbo->ratio;
  if (r != NULL) {
    x = r->x;
    y = r->y;
  }

  /* 因为 opengles 的原点坐标为左下角，所以需要把 AWTK 的坐标（AWTK 是右上角为原点的坐标系）转换为左下角为原点的坐标系*/
  nvgluReadCurrentFramebufferData(x, height - img->h - y, img->w, img->h, fbo->w * fbo->ratio,
                                  height, data);

  p = data + ((img->h - 1) * img->line_length);

  /* 
   * 图像数据垂直翻转
   * opengles 出来的数据是预乘的数据，需要反算预乘。 
   */
  while (TRUE) {
    uint8_t* src_data = p;
    uint8_t* dst_data = img_data;
    for (i = 0; i < img->w; i++) {
        if (0x0 < src_data[3] && src_data[3] < 0xff) {
          dst_data[0] = (src_data[0] << 8) / src_data[3];
          dst_data[1] = (src_data[1] << 8) / src_data[3];
          dst_data[2] = (src_data[2] << 8) / src_data[3];
        } else {
          dst_data[0] = src_data[0];
          dst_data[1] = src_data[1];
          dst_data[2] = src_data[2];
        }
        dst_data[3] = src_data[3];

        src_data += 4;
        dst_data += 4;
    }
    if (p == data) {
      break;
    }
    p -= img->line_length;
    img_data += img->line_length;
  }

  bitmap_unlock_buffer(img);
  handle->fbo = online_fbo;
  nvgluBindFramebuffer(handle);
  TKMEM_FREE(data);
  return RET_OK;
}

static ret_t vgcanvas_nanovg_destroy(vgcanvas_t* vgcanvas) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;

#if defined(WITH_NANOVG_GL3)
  nvgDeleteGL3(vg);
#elif defined(WITH_NANOVG_GLES2)
  nvgDeleteGLES2(vg);
#elif defined(WITH_NANOVG_GLES3)
  nvgDeleteGLES3(vg);
#endif

  TKMEM_FREE(canvas->offline_fb);
  TKMEM_FREE(canvas->shader_program);
  TKMEM_FREE(vgcanvas);

  return RET_OK;
}
