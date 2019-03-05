static ret_t vgcanvas_nanovg_reinit(vgcanvas_t* vg, uint32_t w, uint32_t h, uint32_t stride,
                                    bitmap_format_t format, void* data) {
  (void)vg;
  (void)w;
  (void)h;
  (void)format;
  (void)data;
  resetBGFX(w * vg->ratio, h * vg->ratio, 0);
  return RET_OK;
}

static ret_t vgcanvas_nanovg_begin_frame(vgcanvas_t* vgcanvas, rect_t* dirty_rect) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;

  setBGFXViewRect(0, 0, 0, vgcanvas->w, vgcanvas->h);
  touchBGFX(0);
  nvgBeginFrame(canvas->vg, vgcanvas->w, vgcanvas->h, vgcanvas->ratio);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_end_frame(vgcanvas_t* vgcanvas) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;

  nvgEndFrame(vg);
  frameBGFX(false);

  return RET_OK;
}

enum { BEGIN_IDX = 1, MAX_VIEW_COUNT = 256 };
static bool_t arrayViewId[MAX_VIEW_COUNT] = {0};

static int32_t getFreeViewId(void) {
  int32_t index = BEGIN_IDX;
  for (int32_t i = BEGIN_IDX; i < MAX_VIEW_COUNT; ++i) {
    if (!arrayViewId[i]) {
      index = i;
      arrayViewId[i] = true;
      break;
    }
  }
  return index;
}

static ret_t vgcanvas_nanovg_create_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  struct NVGLUframebuffer* handle = NULL;
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  handle = nvgluCreateFramebufferByViewId(vg, (int)(vgcanvas->w * vgcanvas->ratio),
                                          (int)(vgcanvas->h * vgcanvas->ratio), 0, getFreeViewId());
  return_value_if_fail(handle != NULL, RET_FAIL);
  fbo->w = vgcanvas->w;
  fbo->h = vgcanvas->h;
  fbo->handle = handle;
  fbo->id = getBgfxFboImage(handle);
  fbo->ratio = vgcanvas->ratio;
  return RET_OK;
}

static ret_t vgcanvas_nanovg_destroy_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  struct NVGLUframebuffer* handle = (struct NVGLUframebuffer*)fbo->handle;
  nvgluDeleteFramebufferEx(handle);
  arrayViewId[getBgfxFboViewId(handle)] = false;
  (void)vgcanvas;
  return RET_OK;
}

static ret_t vgcanvas_nanovg_bind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;
  struct NVGLUframebuffer* handle = (struct NVGLUframebuffer*)fbo->handle;

  nvgluBindFramebufferEx(handle);
  touchBGFX(1);
  nvgBeginFrame(vg, fbo->w, fbo->h, fbo->ratio);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_unbind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgEndFrame(vg);
  frameBGFX(false);
  nvgluBindFramebufferEx(NULL);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_destroy(vgcanvas_t* vgcanvas) {
  NVGcontext* vg = ((vgcanvas_nanovg_t*)vgcanvas)->vg;

  nvgDeleteBGFX(vg);
  TKMEM_FREE(vgcanvas);

  return RET_OK;
}
