static ret_t vgcanvas_nanovg_reinit(vgcanvas_t* vg, uint32_t w, uint32_t h, uint32_t stride,
                                    bitmap_format_t format, void* data) {
  (void)vg;
  (void)w;
  (void)h;
  (void)format;
  (void)data;
  return RET_OK;
}

static ret_t vgcanvas_nanovg_begin_frame(vgcanvas_t* vgcanvas, rect_t* dirty_rect) {
  float_t angle = 0.0f;
  float_t anchor_x = 0.0f;
  float_t anchor_y = 0.0f;

  system_info_t* info = system_info();
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;

  native_window_gl_make_current(canvas->window);
  
  nvgBeginFrame(canvas->vg, info->lcd_w, info->lcd_h, info->device_pixel_ratio);

  switch (info->lcd_orientation)
  {
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
  
  if(info->lcd_orientation == LCD_ORIENTATION_90 || info->lcd_orientation == LCD_ORIENTATION_270) {
    nvgTranslate(canvas->vg, anchor_x, anchor_y);
    nvgRotate(canvas->vg, angle);
    nvgTranslate(canvas->vg, -anchor_y, -anchor_x);
  }
  else if(info->lcd_orientation == LCD_ORIENTATION_180) {
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
  native_window_swap_buffer(canvas->window);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_create_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  NVGLUframebuffer* handle = NULL;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  handle = nvgluCreateFramebuffer(vg, (int)(vgcanvas->w * vgcanvas->ratio),
                                  (int)(vgcanvas->h * vgcanvas->ratio), 0);
  return_value_if_fail(handle != NULL, RET_FAIL);

  fbo->w = vgcanvas->w;
  fbo->h = vgcanvas->h;
  fbo->handle = handle;
  fbo->id = handle->image;
  fbo->ratio = vgcanvas->ratio;

  return RET_OK;
}

static ret_t vgcanvas_nanovg_destroy_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  NVGLUframebuffer* handle = (NVGLUframebuffer*)fbo->handle;
  nvgluDeleteFramebuffer(handle);
  (void)vgcanvas;

  return RET_OK;
}

static ret_t vgcanvas_nanovg_bind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  NVGLUframebuffer* handle = (NVGLUframebuffer*)fbo->handle;

  nvgluBindFramebuffer(handle);
  glClearColor(0, 0, 0, 0);
  glViewport(0, 0, fbo->w * fbo->ratio, fbo->h * fbo->ratio);
  glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  nvgBeginFrame(vg, fbo->w, fbo->h, fbo->ratio);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_unbind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgEndFrame(vg);
  nvgluBindFramebuffer(NULL);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_destroy(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
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
