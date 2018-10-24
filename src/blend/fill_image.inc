#include "base/utils.h"

static ret_t fill_image(bitmap_t* dst, rect_t* dst_r, color_t c) {
  int x = 0;
  int y = 0;
  int w = dst_r->w;
  int h = dst_r->h;
  uint32_t a = c.rgba.a;
  pixel_dst_t* p = NULL;
  uint32_t bpp = bitmap_get_bpp(dst);
  uint32_t line_length = bitmap_get_line_length(dst);
  pixel_dst_t data = pixel_dst_from_rgb(c.rgba.r, c.rgba.g, c.rgba.b);

  if (a > 0xf8) {
    if (dst->w == dst_r->w && (dst->w * bpp) == line_length && dst->h == dst_r->h) {
      uint32_t size = w * h;
      p = (pixel_dst_t*)(dst->data);

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
        p = (pixel_dst_t*)(dst->data + (dst_r->y + y) * line_length + dst_r->x * bpp);

        if (bpp == 2) {
          tk_memset16((uint16_t*)p, *(uint16_t*)(&data), w);
        } else if (bpp == 4) {
          tk_memset32((uint32_t*)p, *(uint32_t*)(&data), w);
        } else if (bpp == 3) {
          tk_memset24((uint32_t*)p, &data, w);
        } else {
          assert(!"not supported");
        }
      }
    }
  } else {
    for (y = 0; y < h; y++) {
      p = (pixel_dst_t*)(dst->data + (dst_r->y + y) * line_length + dst_r->x * bpp);

      for (x = 0; x < w; x++) {
        rgba_t drgba = pixel_dst_to_rgba((*p));

        *p++ = blend_rgba(drgba, c.rgba, a);
      }
    }
  }

  return RET_OK;
}
