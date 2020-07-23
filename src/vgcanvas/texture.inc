#include "tkc/utils.h"

static ret_t nanovg_on_bitmap_destroy(bitmap_t* img) {
  int32_t id = tk_pointer_to_int(img->specific);
  NVGcontext* vg = (NVGcontext*)(img->specific_ctx);

  if (vg != NULL && id >= 0) {
    nvgDeleteImage(vg, id);
  }
  img->specific = NULL;
  img->specific_ctx = NULL;
  img->specific_destroy = NULL;

  return RET_OK;
}

static int vgcanvas_nanovg_bitmap_flag_to_NVGimageFlags(bitmap_flag_t flags) {

  if(flags & BITMAP_FLAG_PREMULTI_ALPHA) {
    return NVG_IMAGE_PREMULTIPLIED | NVG_IMAGE_NEAREST;
  }
  return NVG_IMAGE_NEAREST;
}

static int vgcanvas_nanovg_ensure_image(vgcanvas_nanovg_t* canvas, bitmap_t* img) {
  int flag = 0;
  int32_t i = 0;
  uint8_t* data = NULL;
  uint32_t bpp = bitmap_get_bpp(img);
  uint8_t* img_data = bitmap_lock_buffer_for_read(img);

  if (img->flags & BITMAP_FLAG_TEXTURE) {
    i = tk_pointer_to_int(img->specific);

    if (img->flags & BITMAP_FLAG_CHANGED) {
      img->flags &= (~(BITMAP_FLAG_CHANGED));
      nvgUpdateImage(canvas->vg, i, img_data);
    }
    bitmap_unlock_buffer(img);

    return i;
  }

  flag = (int)vgcanvas_nanovg_bitmap_flag_to_NVGimageFlags(img->flags);

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

  if (i >= 0) {
    img->flags |= BITMAP_FLAG_TEXTURE;
    img->specific = tk_pointer_from_int(i);
    img->specific_ctx = canvas->vg;
    img->specific_destroy = nanovg_on_bitmap_destroy;
    image_manager_update_specific(img->image_manager, img);
  }
  bitmap_unlock_buffer(img);

  return i;
}
