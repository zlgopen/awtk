#pragma once

#include "pixel.h"
#include "nanovg.h"

namespace agge {
template <typename PixelT, typename BitmapT>
class nanovg_image_blender {
 public:
  typedef PixelT pixel;
  typedef uint8_t cover_type;

 public:
  nanovg_image_blender(BitmapT* bitmap, float* matrix);

  bool get_pixel(float x, float y, pixel32_rgba& ref) const;
  void operator()(pixel* pixels, int x, int y, count_t n) const;
  void operator()(pixel* pixels, int x, int y, count_t n, const cover_type* covers) const;

 private:
  BitmapT* _bitmap;
  float* _matrix;
  int w;
  int h;
};

template <typename PixelT, typename BitmapT>
inline nanovg_image_blender<PixelT, BitmapT>::nanovg_image_blender(BitmapT* bitmap, float* matrix)
    : _bitmap(bitmap), _matrix(matrix) {
  this->w = bitmap->width();
  this->h = bitmap->height();
}

template <typename PixelT, typename BitmapT>
inline bool nanovg_image_blender<PixelT, BitmapT>::get_pixel(float x, float y,
                                                             pixel32_rgba& ref) const {
  float ox = 0;
  float oy = 0;

  nvgTransformPoint(&ox, &oy, _matrix, x, y);

  int lx = (int)(ox + 0.5f);
  int ly = (int)(oy + 0.5f);

  if (lx < 0) {
    lx = 0;
  }
  if (lx >= this->w) {
    lx = this->w - 1;
  }

  if (ly < 0) {
    ly = 0;
  }
  if (ly >= this->h) {
    ly = this->h - 1;
  }

  typename BitmapT::pixel p = _bitmap->row_ptr(ly)[lx];
  pixel_convert<pixel32_rgba, typename BitmapT::pixel>(ref, p);

  return true;
}

template <typename PixelT, typename BitmapT>
inline void nanovg_image_blender<PixelT, BitmapT>::operator()(pixel* pixels, int x, int y,
                                                              count_t n) const {
  pixel32_rgba p(0xff, 0xff, 0xff, 0xff);
  for (count_t i = 0; i < n; i++, ++pixels) {
    if (!this->get_pixel((float)x + i, (float)y, p)) {
      continue;
    }

    pixel_blend<PixelT, pixel32_rgba>(*pixels, p, p.a);
  }
}

template <typename PixelT, typename BitmapT>
inline void nanovg_image_blender<PixelT, BitmapT>::operator()(pixel* pixels, int x, int y,
                                                              count_t n,
                                                              const cover_type* covers) const {
  pixel32_rgba p(0xff, 0xff, 0xff, 0xff);
  for (count_t i = 0; i < n; i++, ++pixels, ++covers) {
    if (!this->get_pixel((float)x + i, (float)y, p)) {
      continue;
    }

    uint8_t a = pixel_a(p, covers[0]);
    pixel_blend<PixelT, pixel32_rgba>(*pixels, p, a);
  }
}
}  // namespace agge
