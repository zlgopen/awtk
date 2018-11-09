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
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;

  nvgBeginFrame(canvas->vg, vgcanvas->w, vgcanvas->h, vgcanvas->ratio);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_end_frame(vgcanvas_t* vgcanvas) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;

  nvgEndFrame(vg);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_create_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  return RET_OK;
}

static ret_t vgcanvas_nanovg_destroy_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  return RET_OK;
}

static ret_t vgcanvas_nanovg_bind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  return RET_OK;
}

static ret_t vgcanvas_nanovg_unbind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  return RET_OK;
}

static ret_t vgcanvas_nanovg_destroy(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgDeleteBGFX(vg);
  TKMEM_FREE(vgcanvas);

  return RET_OK;
}

