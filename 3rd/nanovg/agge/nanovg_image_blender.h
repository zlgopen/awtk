#pragma once

#include "agge/math.h"
#include "agge/pixel.h"
#include "nanovg.h"

namespace agge {
template <typename PixelT, typename BitmapT>
class nanovg_image_blender {
 public:
  typedef PixelT pixel;
  typedef uint8_t cover_type;

 public:
  nanovg_image_blender(BitmapT* bitmap, float* matrix);
  nanovg_image_blender(BitmapT* bitmap, float* matrix, float alpha);

  bool get_pixel(float x, float y, pixel32_rgba& ref) const;
  void operator()(pixel* pixels, int x, int y, count_t n) const;
  void operator()(pixel* pixels, int x, int y, count_t n, const cover_type* covers) const;

 private:
  BitmapT* _bitmap;
  float* _matrix;
  float _alpha;
  int w;
  int h;
};

template <typename PixelT, typename BitmapT>
inline nanovg_image_blender<PixelT, BitmapT>::nanovg_image_blender(BitmapT* bitmap, float* matrix)
    : _bitmap(bitmap), _matrix(matrix), _alpha(1.0f) {
  this->w = bitmap->width();
  this->h = bitmap->height();
}

template <typename PixelT, typename BitmapT>
inline nanovg_image_blender<PixelT, BitmapT>::nanovg_image_blender(BitmapT* bitmap, float* matrix, float alpha)
    : _bitmap(bitmap), _matrix(matrix), _alpha(alpha) {
  this->w = bitmap->width();
  this->h = bitmap->height();
}

template <typename PixelTargetT, typename PixelSrcT>
inline void pixel_linear(PixelTargetT& t, const PixelSrcT& s, float a) {
 float ma = 1 - a;
 t.r = s.r*a + t.r*ma;
 t.g = s.g*a + t.g*ma;
 t.b = s.b*a + t.b*ma;
 t.a = s.a*a + t.a*ma;
}

template <typename PixelT, typename BitmapT>
inline bool nanovg_image_blender<PixelT, BitmapT>::get_pixel(float x, float y,
                                                             pixel32_rgba& ref) const {
  float ox = 0;
  float oy = 0;

  nvgTransformPoint(&ox, &oy, _matrix, x, y);

  pixel32_rgba p[4];   //default is zero
  int x1 = floor(ox);
  int x2 = ceil(ox);
  int y1 = floor(oy);
  int y2 = ceil(oy);
  if (x1 >= 0 && x1 < this->w){
   if (y1 >= 0 && y1 < this->h){
    p[0].a = 0xff;
    pixel_convert<pixel32_rgba, typename BitmapT::pixel>(p[0], _bitmap->row_ptr(y1)[x1]);
   }
   if (y2 >= 0 && y2 < this->h) {
    p[2].a = 0xff;
    pixel_convert<pixel32_rgba, typename BitmapT::pixel>(p[2], _bitmap->row_ptr(y2)[x1]);
   }
  }
  if (x2 >= 0 && x2 < this->w) {
   if (y1 >= 0 && y1 < this->h) {
    p[1].a = 0xff;
    pixel_convert<pixel32_rgba, typename BitmapT::pixel>(p[1], _bitmap->row_ptr(y1)[x2]);
   }
   if (y2 >= 0 && y2 < this->h) {
    p[3].a = 0xff;
    pixel_convert<pixel32_rgba, typename BitmapT::pixel>(p[3], _bitmap->row_ptr(y2)[x2]);
   }
  }

  pixel_linear(p[0], p[1], ox - x1);
  pixel_linear(p[2], p[3], ox - x1);
  pixel_linear(p[0], p[2], oy - y1);
  ref = p[0];

  ref.a *= _alpha;

  if (_bitmap->flags() & NVG_IMAGE_PREMULTIPLIED) {
    ref.r = ref.r * _alpha;
    ref.g = ref.g * _alpha;
    ref.b = ref.b * _alpha;
  }

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

    if (_bitmap->flags() & NVG_IMAGE_PREMULTIPLIED) {
      pixel_blend_premulti_alpha<PixelT, pixel32_rgba>(*pixels, p, p.a, 1);
    } else {
      pixel_blend<PixelT, pixel32_rgba>(*pixels, p, p.a);
    }
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
    if (_bitmap->flags() & NVG_IMAGE_PREMULTIPLIED) {
      pixel_blend_premulti_alpha<PixelT, pixel32_rgba>(*pixels, p, a, covers[0]);
    } else {
      pixel_blend<PixelT, pixel32_rgba>(*pixels, p, a);
    }
  }
}
}  // namespace agge
