#include "tkc/utils.h"
#include "vgcanvas_nanovg.inc" 

static enum NVGorientation vgcanvas_nanovg_lcd_orientation_to_NVGorientation(lcd_orientation_t orientation) {
  switch (orientation) {
  case LCD_ORIENTATION_0:
    return NVG_ORIENTATION_0;
  case LCD_ORIENTATION_90:
    return NVG_ORIENTATION_90;
  case LCD_ORIENTATION_180:
    return NVG_ORIENTATION_180;
  case LCD_ORIENTATION_270:
    return NVG_ORIENTATION_270;
  default :
    assert(!"not find orientation");
    break;
  }
  return NVG_ORIENTATION_0;
}

static ret_t vgcanvas_nanovg_reinit(vgcanvas_t* vg, uint32_t w, uint32_t h, uint32_t stride,
                                    bitmap_format_t format, void* data) {
  (void)vg;
  (void)w;
  (void)h;
  (void)format;
  (void)data;
  return RET_OK;
}

static ret_t vgcanvas_nanovg_begin_frame(vgcanvas_t* vgcanvas, const dirty_rects_t* dirty_rects) {
  system_info_t* info = system_info();
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  const rect_t* dirty_rect = dirty_rects != NULL ? &(dirty_rects->max) : NULL;

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

  nvgBeginFrame(canvas->vg, info->lcd_w, info->lcd_h, info->device_pixel_ratio, vgcanvas_nanovg_lcd_orientation_to_NVGorientation(info->lcd_orientation));

  return RET_OK;
}

static ret_t vgcanvas_nanovg_end_frame(vgcanvas_t* vgcanvas) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;

  nvgEndFrame(vg);

  native_window_swap_buffer(canvas->window);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_create_fbo(vgcanvas_t* vgcanvas, uint32_t w, uint32_t h, bool_t custom_draw_model, framebuffer_object_t* fbo) {
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
  fbo->custom_draw_model = custom_draw_model;
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
  system_info_t* info = system_info();
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGLUframebuffer* handle = (NVGLUframebuffer*)fbo->handle;
  
  vg = canvas->vg;

  fbo->online_fbo = nvgluGetCurrFramebuffer();
  fbo->online_dirty_rect = rect_init(vgcanvas->dirty_rect.x, vgcanvas->dirty_rect.y, vgcanvas->dirty_rect.w, vgcanvas->dirty_rect.h);

  handle->fbo = fbo->offline_fbo;
  vgcanvas->dirty_rect = rect_init(0, 0, fbo->w, fbo->h);
  nvgluBindFramebuffer(handle);
  if (!fbo->custom_draw_model) {
    glViewport(0, 0, fbo->w * fbo->ratio, fbo->h * fbo->ratio);
    if (!fbo->init) {
      glScissor(0, 0, fbo->w * fbo->ratio, fbo->h * fbo->ratio);
      glEnable(GL_SCISSOR_TEST);
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      glDisable(GL_SCISSOR_TEST);
      fbo->init = TRUE;
    }
    nvgBeginFrameEx(vg, fbo->w, fbo->h, fbo->ratio, FALSE, vgcanvas_nanovg_lcd_orientation_to_NVGorientation(info->lcd_orientation));
    nvgSave(vg);
    nvgReset(vg);
  }

  return RET_OK;
}

static ret_t vgcanvas_nanovg_unbind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  NVGcontext* vg = NULL;
  system_info_t* info = system_info();
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGLUframebuffer* handle = (NVGLUframebuffer*)fbo->handle;

  vg = canvas->vg;

  handle->fbo = fbo->online_fbo;
  if (!fbo->custom_draw_model) {
    nvgRestore(vg);
    nvgEndFrame(vg);
  }

  nvgluBindFramebuffer(handle);

  vgcanvas->dirty_rect = rect_init(fbo->online_dirty_rect.x, fbo->online_dirty_rect.y, fbo->online_dirty_rect.w, fbo->online_dirty_rect.h);
  glViewport(0, 0, vgcanvas->w * fbo->ratio, vgcanvas->h * fbo->ratio);
  glScissor(0, 0, vgcanvas->w * fbo->ratio, vgcanvas->h * fbo->ratio);
  if (!fbo->custom_draw_model) {
    nvgBeginFrameEx(vg, vgcanvas->w, vgcanvas->h, fbo->ratio, FALSE, vgcanvas_nanovg_lcd_orientation_to_NVGorientation(info->lcd_orientation));
  }
  return RET_OK;
}

static ret_t vgcanvas_nanovg_fbo_to_bitmap(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo,
                                           bitmap_t* img, const rect_t* r) {
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
          dst_data[0] = src_data[0] == src_data[3] ? 0xff : (src_data[0] << 8) / src_data[3];
          dst_data[1] = src_data[1] == src_data[3] ? 0xff : (src_data[1] << 8) / src_data[3];
          dst_data[2] = src_data[2] == src_data[3] ? 0xff : (src_data[2] << 8) / src_data[3];
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

  vgcanvas_asset_manager_remove_vg(vgcanvas_asset_manager(), vgcanvas);

  vgcanvas_nanovg_deinit(vgcanvas);
#if defined(WITH_NANOVG_GL3)
  nvgDeleteGL3(vg);
#elif defined(WITH_NANOVG_GLES2)
  nvgDeleteGLES2(vg);
#elif defined(WITH_NANOVG_GLES3)
  nvgDeleteGLES3(vg);
#endif

  TKMEM_FREE(vgcanvas);

  return RET_OK;
}
