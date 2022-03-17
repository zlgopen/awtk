#include "tkc/utils.h"
#include "base/types_def.h"
#include "base/vgcanvas_asset_manager.h"

static int vgcanvas_nanovg_bitmap_flag_to_NVGimageFlags(bitmap_flag_t flags) {

  if(flags & BITMAP_FLAG_PREMULTI_ALPHA) {
    return NVG_IMAGE_PREMULTIPLIED;
  }
  return 0;
}

static int vgcanvas_nanovg_ensure_image(vgcanvas_nanovg_t* canvas, bitmap_t* img) {
  int flag = 0;
  int32_t i = 0;
  uint8_t* data = NULL;
  framebuffer_object_t* fbo = NULL;
  uint32_t bpp = bitmap_get_bpp(img);
  vgcanvas_nanovg_gl_texture_t* texture = NULL;
  uint8_t* img_data = bitmap_lock_buffer_for_read(img);

  if (img->flags & BITMAP_FLAG_TEXTURE) {
    ret_t reuslt = RET_FAIL;
    const void* specific = NULL;
    if (img->flags & BITMAP_FLAG_GPU_FBO_TEXTURE) {
      img->flags &= ~BITMAP_FLAG_GPU_FBO_TEXTURE;
      i = tk_pointer_to_int(img->specific);
      fbo = (framebuffer_object_t*)img->specific_ctx;
      goto create_texture_info;
    } else {
      specific = vgcanvas_asset_manager_get_image_specific(vgcanvas_asset_manager(), (vgcanvas_t*)canvas, img, &reuslt);
    }
    if (reuslt == RET_OK) {
      i = ((vgcanvas_nanovg_gl_texture_t*)specific)->image_id;

      if (img->flags & BITMAP_FLAG_CHANGED) {
        img->flags &= (~(BITMAP_FLAG_CHANGED));
        nvgUpdateImage(canvas->vg, i, img_data);
      }
      bitmap_unlock_buffer(img);

      return i;
    }
  }
  assert(img->orientation == LCD_ORIENTATION_0);

  flag = (int)vgcanvas_nanovg_bitmap_flag_to_NVGimageFlags((bitmap_flag_t)(img->flags));

  if (bpp * img->w == img->line_length) {
    data = (uint8_t*)(img_data);
  } else {
    uint32_t size = 0;
    size = bpp * img->w * img->h;
    size = TK_ROUND_TO(size, BITMAP_ALIGN_SIZE) + BITMAP_ALIGN_SIZE;
    data = (uint8_t*)TKMEM_ALLOC(size);
    memset(data, 0x00, size);
    int j;
    for (j = 0; j < img->h; j++) {
      memcpy(data + j * img->w * bpp, img_data + j * img->line_length, img->w * bpp);
    }
  }

  i = nvgCreateImageRGBA(canvas->vg, img->w, img->h, flag, data);

  if (data != img_data) {
    TKMEM_FREE(data);
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
