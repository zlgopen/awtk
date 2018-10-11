#include "base/utils.h"

static ret_t fill_image(bitmap_t* fb, rect_t* dst, color_t c) {
  int x = 0;
  int y = 0;
  int w = dst->w;
  int h = dst->h;
  uint32_t a = c.rgba.a;
  uint32_t bpp = bitmap_get_bpp(fb);
  uint32_t offset = (fb->w - dst->w);
  pixel_dst_t data = pixel_dst_from_rgb(c.rgba.r, c.rgba.g, c.rgba.b);
  pixel_dst_t* p = (pixel_dst_t*)((fb->data) + (dst->y * fb->w + dst->x) * bpp);

  if (a > 0xf8) {
    if(fb->w == dst->w && fb->h == dst->h) {
      uint32_t size = w * h;
      if (bpp == 2) {
        tk_memset16((uint16_t*)p, *(uint16_t*)(&data), size);
      } else if (bpp == 4) {
        tk_memset32((uint32_t*)p, *(uint32_t*)(&data), size);
      } else if (bpp == 3) {
        tk_memset24((uint32_t*)p, &data, size);
      } else {
        assert(!"not supported");
      }
    } else {
      for (y = 0; y < h; y++) {
        if (bpp == 2) {
          tk_memset16((uint16_t*)p, *(uint16_t*)(&data), w);
        } else if (bpp == 4) {
          tk_memset32((uint32_t*)p, *(uint32_t*)(&data), w);
        } else if (bpp == 3) {
          tk_memset24((uint32_t*)p, &data, w);
        } else {
          assert(!"not supported");
        }
        p += fb->w;
      }
    }
  } else {
    for (y = 0; y < h; y++) {
      for (x = 0; x < w; x++) {
        rgba_t drgba = pixel_dst_to_rgba((*p));

        *p++ = blend_rgba(drgba, c.rgba, a);
      }
      p += offset;
    }
  }

  return RET_OK;
}
