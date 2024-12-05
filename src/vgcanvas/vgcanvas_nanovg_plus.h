#ifndef TK_VGCANVAS_NANOVG_PLUS_H
#define TK_VGCANVAS_NANOVG_PLUS_H
/* internal */
ret_t vgcanvas_nanovg_plus_init(vgcanvas_t* vgcanvas);
const vgcanvas_vtable_t* vgcanvas_nanovg_plus_get_vt();
ret_t vgcanvas_asset_manager_nanovg_plus_font_destroy(void* vg, const char* font_name,
                                                      void* specific);
ret_t vgcanvas_asset_manager_nanovg_plus_bitmap_destroy(void* vg, void* specific);
#endif
