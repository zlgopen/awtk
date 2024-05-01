
static ret_t vgcanvas_asset_manager_nanovg_font_destroy(void* vg, const char* font_name,
                                                        void* specific) {
  int32_t id = tk_pointer_to_int(specific);
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vg;
  if (canvas != NULL && canvas->vg != NULL && id >= 0) {
    nvgDeleteFontByName(canvas->vg, font_name);
  }
  return RET_OK;
}

static ret_t vgcanvas_asset_manager_nanovg_bitmap_destroy(void* vg, void* specific) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vg;
  vgcanvas_nanovg_gl_texture_t* texture = (vgcanvas_nanovg_gl_texture_t*)specific;
  if (canvas != NULL && canvas->vg != NULL && texture != NULL) {
    if (texture->fbo != NULL) {
      vgcanvas_destroy_fbo((vgcanvas_t*)canvas, texture->fbo);
      TKMEM_FREE(texture->fbo);
    } else {
      nvgDeleteImage(canvas->vg, texture->image_id);
    }
  }
  if (texture) {
    TKMEM_FREE(texture);
  }
  return RET_OK;
}
