#pragma once
#include <cstdio>
#include "pixel.h"

namespace agge {
template <typename PixelT>
class blender_linear_gradient {
 public:
  typedef PixelT pixel;
  typedef uint8_t cover_type;

 public:
  blender_linear_gradient(float sx, float sy, float ex, float ey, pixel32_rgba sc, pixel32_rgba ec);
  void operator()(pixel* pixels, int x, int y, count_t n, const cover_type* covers) const;
  bool gradient(float factor, pixel32_rgba& c) const;
  bool get_color(int x, int y, pixel32_rgba& c) const;

 private:
  float _sx;
  float _sy;
  float _ex;
  float _ey;
  float _dx;
  float _dy;
  float _dot_product_1;
  float _xw;
  float _yw;
  pixel32_rgba _sc;
  pixel32_rgba _ec;
};

template <typename PixelT>
inline blender_linear_gradient<PixelT>::blender_linear_gradient(float sx, float sy, float ex, float ey, 
    pixel32_rgba sc, pixel32_rgba ec) : _sx(sx), _sy(sy), _ex(ex), _ey(ey), _sc(sc), _ec(ec) {
  if(sx == ex && sy == ey) {
    assert(!"invalid params");
    _ex = _sx + 1;
    _ey = _sy + 1;
  }

  _dx = ex - sx;
  _dy = ey - sy;
  _dot_product_1 = 1/(_dx * _dx + _dy * _dy);
}

template <typename PixelT>
inline bool blender_linear_gradient<PixelT>::gradient(float factor, pixel32_rgba& c) const {
  if(factor <= 0) {
    c = _sc;
  } else if(factor >= 1.0f) {
    c = _ec;
  } else {
    c.r = _sc.r + (_ec.r - _sc.r) * factor;
    c.g = _sc.g + (_ec.g - _sc.g) * factor;
    c.b = _sc.b + (_ec.b - _sc.b) * factor;
    c.a = _sc.a + (_ec.a - _sc.a) * factor;
  }

  return true;
}

template <typename PixelT>
inline bool blender_linear_gradient<PixelT>::get_color(int x, int y, pixel32_rgba& c) const {
  if(_sx == _ex && _sy == _ey) {
    c = _sc;
    return true;
  } else if(_sx == _ex) {
    return this->gradient((y - _sy)/_dy, c);
  } else if(_sy == _ey) {
    return this->gradient((x - _sx)/_dx, c);
  } else {
    //https://github.com/SFML/SFML/wiki/Source:-Color-Gradient
    if(x < _sx || y < _sy) {
      c = _sc;
    } else if(x > _ex || y > _ey) {
      c = _ec;
    } else {
      float dot_product = (x - _sx) * _dx + (y - _sy) * _dy;
      float factor = dot_product * _dot_product_1;

      return this->gradient(factor, c);
    }
  }

  return true;
}

template <typename PixelT>
inline void blender_linear_gradient<PixelT>::operator()(pixel* pixels, int x, int y,
                                                        count_t n, const cover_type* covers) const {
  pixel32_rgba p(0, 0, 0, 0);

  for (; n; --n, ++pixels, ++covers) {
    if(this->get_color(x++, y, p)) {
      uint8_t a = (*covers * p.a) >> 8;
      pixel_blend<PixelT, pixel32_rgba>(*pixels, p, a);
    }
  }
}
}  // namespace agge
