#pragma once

#define PIXEL_CONVERT_NO_A(t, s) \
  t.r = s.r;                     \
  t.g = s.g;                     \
  t.b = s.b;

#define PIXEL_CONVERT_FROM_565(t, s) \
  t.r = s.r << 3;                    \
  t.g = s.g << 2;                    \
  t.b = s.b << 3;

#define PIXEL_CONVERT_TO_565(t, s) \
  t.r = s.r >> 3;                  \
  t.g = s.g >> 2;                  \
  t.b = s.b >> 3;

#define PIXEL_CONVERT_A(t, s) \
  t.r = s.r;                  \
  t.g = s.g;                  \
  t.b = s.b;                  \
  t.a = s.a;

template <typename PixelTargetT, typename PixelSrcT>
inline void pixel_convert(PixelTargetT& t, const PixelSrcT& s) {
  PIXEL_CONVERT_NO_A(t, s);
}
template <>
inline void pixel_convert(pixel32_rgba& t, const pixel32_rgba& s) {
  t = s;
}

template <>
inline void pixel_convert(pixel32_abgr& t, const pixel32_abgr& s) {
  t = s;
}

template <>
inline void pixel_convert(pixel32_bgra& t, const pixel32_bgra& s) {
  t = s;
}

template <>
inline void pixel_convert(pixel32_argb& t, const pixel32_argb& s) {
  t = s;
}

template <>
inline void pixel_convert(pixel24_rgb& t, const pixel24_rgb& s) {
  t = s;
}

template <>
inline void pixel_convert(pixel24_bgr& t, const pixel24_bgr& s) {
  t = s;
}

template <>
inline void pixel_convert(pixel16_rgb565& t, const pixel16_rgb565& s) {
  t = s;
}

template <>
inline void pixel_convert(pixel16_bgr565& t, const pixel16_bgr565& s) {
  t = s;
}

// pixel32_rgba
template <>
inline void pixel_convert(pixel32_rgba& t, const pixel32_abgr& s) {
  PIXEL_CONVERT_A(t, s);
}

template <>
inline void pixel_convert(pixel32_rgba& t, const pixel32_bgra& s) {
  PIXEL_CONVERT_A(t, s);
}

template <>
inline void pixel_convert(pixel32_rgba& t, const pixel32_argb& s) {
  PIXEL_CONVERT_A(t, s);
}

template <>
inline void pixel_convert(pixel32_rgba& t, const pixel16_bgr565& s) {
  PIXEL_CONVERT_FROM_565(t, s);
}

// pixel32_abgr
template <>
inline void pixel_convert(pixel32_abgr& t, const pixel32_rgba& s) {
  PIXEL_CONVERT_A(t, s);
}

template <>
inline void pixel_convert(pixel32_abgr& t, const pixel32_bgra& s) {
  PIXEL_CONVERT_A(t, s);
}

template <>
inline void pixel_convert(pixel32_abgr& t, const pixel32_argb& s) {
  PIXEL_CONVERT_A(t, s);
}

template <>
inline void pixel_convert(pixel32_abgr& t, const pixel16_bgr565& s) {
  PIXEL_CONVERT_FROM_565(t, s);
}

// pixel32_bgra
template <>
inline void pixel_convert(pixel32_bgra& t, const pixel32_abgr& s) {
  PIXEL_CONVERT_A(t, s);
}

template <>
inline void pixel_convert(pixel32_bgra& t, const pixel32_rgba& s) {
  PIXEL_CONVERT_A(t, s);
}

template <>
inline void pixel_convert(pixel32_bgra& t, const pixel32_argb& s) {
  PIXEL_CONVERT_A(t, s);
}

template <>
inline void pixel_convert(pixel32_bgra& t, const pixel16_bgr565& s) {
  PIXEL_CONVERT_FROM_565(t, s);
}

// pixel32_argb
template <>
inline void pixel_convert(pixel32_argb& t, const pixel32_rgba& s) {
  PIXEL_CONVERT_A(t, s);
}

template <>
inline void pixel_convert(pixel32_argb& t, const pixel32_abgr& s) {
  PIXEL_CONVERT_A(t, s);
}

template <>
inline void pixel_convert(pixel32_argb& t, const pixel32_bgra& s) {
  PIXEL_CONVERT_A(t, s);
}

template <>
inline void pixel_convert(pixel32_argb& t, const pixel16_bgr565& s) {
  PIXEL_CONVERT_FROM_565(t, s);
}
