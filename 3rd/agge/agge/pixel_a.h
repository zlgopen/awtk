#pragma once

template <typename PixelT>
inline uint8_t pixel_a(const PixelT& p, uint8_t a) {
  return a;
}

template <>
inline uint8_t pixel_a(const pixel32_rgba& p, uint8_t a) {
  return (a * p.a) >> 8;
}

template <>
inline uint8_t pixel_a(const pixel32_abgr& p, uint8_t a) {
  return (a * p.a) >> 8;
}

template <>
inline uint8_t pixel_a(const pixel32_bgra& p, uint8_t a) {
  return (a * p.a) >> 8;
}

template <>
inline uint8_t pixel_a(const pixel32_argb& p, uint8_t a) {
  return (a * p.a) >> 8;
}

template <>
inline uint8_t pixel_a(const pixel8& p, uint8_t a) {
  return (a * p.a) >> 8;
}
