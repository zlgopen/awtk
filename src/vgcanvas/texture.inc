#include "tkc/utils.h"
#include "base/types_def.h"
#include "base/vgcanvas_asset_manager.h"

typedef struct _vgcanvas_nanovg_gl_texture_t {
  int32_t image_id;
  framebuffer_object_t* fbo;
} vgcanvas_nanovg_gl_texture_t;

static int vgcanvas_nanovg_bitmap_flag_to_NVGimageFlags(bitmap_flag_t flags) {

  if(flags & BITMAP_FLAG_PREMULTI_ALPHA) {
    return NVG_IMAGE_PREMULTIPLIED;
  }
  return 0;
}

static inline uint8_t* vgcanvas_nanovg_ensure_buffer(uint8_t* img_data, uint32_t img_w, uint32_t img_h, uint32_t bpp, uint32_t img_line_length) {
  int32_t j = 0;
  uint32_t size = 0;
  uint8_t* data = NULL;

  if (bpp * img_w == img_line_length) {
    data = (uint8_t*)(img_data);
  } else {
    size = bpp * img_w * img_h;
    size = TK_ROUND_TO(size, BITMAP_ALIGN_SIZE) + BITMAP_ALIGN_SIZE;
    data = (uint8_t*)TKMEM_ALLOC(size);
    return_value_if_fail(data != NULL, NULL);
    memset(data, 0x00, size);
    for (j = 0; j < img_h; j++) {
      memcpy(data + j * img_w * bpp, img_data + j * img_line_length, img_w * bpp);
    }
  }
  return data;
}

static int vgcanvas_nanovg_ensure_image(vgcanvas_nanovg_t* canvas, bitmap_t* img) {
  int flag = 0;
  int32_t i = 0;
  uint8_t* data = NULL;
  framebuffer_object_t* fbo = NULL;
  uint32_t bpp = bitmap_get_bpp(img);
  vgcanvas_nanovg_gl_texture_t* texture = NULL;
  uint8_t* img_data = bitmap_lock_buffer_for_read(img);
  uint32_t img_w = bitmap_get_physical_width(img);
  uint32_t img_h = bitmap_get_physical_height(img);
  uint32_t img_line_length = bitmap_get_physical_line_length(img);

  if (img->flags & BITMAP_FLAG_TEXTURE) {
    ret_t result = RET_FAIL;
    const void* specific = NULL;
    if (img->flags & BITMAP_FLAG_GPU_FBO_TEXTURE) {
      img->flags &= ~BITMAP_FLAG_GPU_FBO_TEXTURE;
      i = tk_pointer_to_int(img->specific);
      fbo = (framebuffer_object_t*)img->specific_ctx;
      goto create_texture_info;
    } else {
      specific = vgcanvas_asset_manager_get_image_specific(vgcanvas_asset_manager(), (vgcanvas_t*)canvas, img, &result);
    }
    if (result == RET_OK) {
      i = ((vgcanvas_nanovg_gl_texture_t*)specific)->image_id;

      if (bitmap_is_dirty(img)) {
        bitmap_set_dirty(img, FALSE);
        image_manager_update_specific(img->image_manager, img);
        data = vgcanvas_nanovg_ensure_buffer(img_data, img_w, img_h, bpp, img_line_length);
        if (data != NULL) {
          nvgUpdateImage(canvas->vg, i, data);
          if (data != img_data) {
            TKMEM_FREE(data);
          }
        } else {
          assert(!"data is NULL");
        }
      }
      bitmap_unlock_buffer(img);

      return i;
    }
  }

  flag = (int)vgcanvas_nanovg_bitmap_flag_to_NVGimageFlags((bitmap_flag_t)(img->flags));

  data = vgcanvas_nanovg_ensure_buffer(img_data, img_w, img_h, bpp, img_line_length);
  if (data != NULL) {
    i = nvgCreateImageRGBA(canvas->vg, img_w, img_h, flag, data);
    if (data != img_data) {
      TKMEM_FREE(data);
    }
  } else {
    assert(!"data is NULL");
  }

  if (bitmap_is_dirty(img)) {
    bitmap_set_dirty(img, FALSE);
  }

create_texture_info:
  if (i >= 0) {
    texture = TKMEM_ZALLOC(vgcanvas_nanovg_gl_texture_t);
    assert(texture != NULL);
    texture->fbo = fbo;
    texture->image_id = i;
    img->flags |= BITMAP_FLAG_TEXTURE;
    vgcanvas_asset_manager_add_image(vgcanvas_asset_manager(), (vgcanvas_t*)canvas, img, texture);
  }
  bitmap_unlock_buffer(img);

  return i;
}
