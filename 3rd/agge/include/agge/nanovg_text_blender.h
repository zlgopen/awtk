#pragma once

#include "pixel.h"
#include "nanovg.h"

namespace agge {
template <typename PixelT, typename BitmapT>
class nanovg_text_blender {
 public:
  typedef PixelT pixel;
  typedef uint8_t cover_type;

 public:
  nanovg_text_blender(BitmapT* bitmap, float* matrix, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

  bool get_pixel(float x, float y, pixel32_rgba& ref) const;
  void operator()(pixel* pixels, int x, int y, count_t n) const;
  void operator()(pixel* pixels, int x, int y, count_t n, const cover_type* covers) const;

 private:
  BitmapT* _bitmap;
  float* _matrix;
  int _w;
  int _h;
  uint8_t _r;
  uint8_t _g;
  uint8_t _b;
  uint8_t _a;
};

template <typename PixelT, typename BitmapT>
inline nanovg_text_blender<PixelT, BitmapT>::nanovg_text_blender(BitmapT* bitmap, float* matrix,
                                                                 uint8_t r, uint8_t g, uint8_t b,
                                                                 uint8_t a)
    : _bitmap(bitmap), _matrix(matrix), _r(r), _g(g), _b(b), _a(a) {
  _w = bitmap->width();
  _h = bitmap->height();
}

template <typename PixelT, typename BitmapT>
inline bool nanovg_text_blender<PixelT, BitmapT>::get_pixel(float x, float y,
                                                            pixel32_rgba& ref) const {
  float ox = 0;
  float oy = 0;

  nvgTransformPoint(&ox, &oy, _matrix, x, y);

  int lx = (int)(ox + 0.5f);
  int ly = (int)(oy + 0.5f);

  if (ly >= 0 && ly < _h && lx >= 0 && lx < _w) {
    typename BitmapT::pixel p = _bitmap->row_ptr(ly)[lx];

    ref.a = (p.a * _a) >> 8;

    return true;
  }

  return false;
}

template <typename PixelT, typename BitmapT>
inline void nanovg_text_blender<PixelT, BitmapT>::operator()(pixel* pixels, int x, int y,
                                                             count_t n) const {
  pixel32_rgba p(_r, _g, _b, _a);
  for (count_t i = 0; i < n; i++, ++pixels) {
    if (!this->get_pixel((float)x + i, (float)y, p)) {
      continue;
    }

    pixel_blend<PixelT, pixel32_rgba>(*pixels, p, p.a);
  }
}

template <typename PixelT, typename BitmapT>
inline void nanovg_text_blender<PixelT, BitmapT>::operator()(pixel* pixels, int x, int y, count_t n,
                                                             const cover_type* covers) const {
  pixel32_rgba p(_r, _g, _b, _a);
  for (count_t i = 0; i < n; i++, ++pixels, ++covers) {
    if (!this->get_pixel((float)x + i, (float)y, p)) {
      continue;
    }

    uint8_t a = pixel_a(p, covers[0]);
    pixel_blend<PixelT, pixel32_rgba>(*pixels, p, a);
  }
}
}  // namespace agge
