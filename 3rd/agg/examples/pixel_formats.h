#ifndef PIXEL_FORMATS_INCLUDED
#define PIXEL_FORMATS_INCLUDED

#if defined(AGG_GRAY8)

#include "agg_pixfmt_gray.h"
const agg::pix_format_e pix_format = agg::pix_format_gray8;
typedef agg::pixfmt_gray8 pixfmt;
typedef agg::pixfmt_gray8_pre pixfmt_pre;
typedef agg::gray8 color_type;
typedef agg::gray8 gray_type;

#elif defined(AGG_SGRAY8)

#include "agg_pixfmt_gray.h"
const agg::pix_format_e pix_format = agg::pix_format_sgray8;
typedef agg::pixfmt_sgray8 pixfmt;
typedef agg::pixfmt_sgray8_pre pixfmt_pre;
typedef agg::sgray8 color_type;
typedef agg::sgray8 gray_type;

#elif defined(AGG_GRAY16)

#include "agg_pixfmt_gray.h"
const agg::pix_format_e pix_format = agg::pix_format_gray16;
typedef agg::pixfmt_gray16 pixfmt;
typedef agg::pixfmt_gray16_pre pixfmt_pre;
typedef agg::gray16 color_type;
typedef agg::gray16 gray_type;

#elif defined(AGG_GRAY32)

#include "agg_pixfmt_gray.h"
const agg::pix_format_e pix_format = agg::pix_format_gray32;
typedef agg::pixfmt_gray32 pixfmt;
typedef agg::pixfmt_gray32_pre pixfmt_pre;
typedef agg::gray32 color_type;
typedef agg::gray32 gray_type;

#elif defined(AGG_BGR24)

#include "agg_pixfmt_rgb.h"
const agg::pix_format_e pix_format = agg::pix_format_bgr24;
typedef agg::pixfmt_bgr24 pixfmt;
typedef agg::pixfmt_bgr24_pre pixfmt_pre;
#define pixfmt_gamma agg::pixfmt_bgr24_gamma
typedef agg::rgba8 color_type;
typedef agg::order_bgr component_order;
typedef agg::gray8 gray_type;

#elif defined(AGG_RGB24)

#include "agg_pixfmt_rgb.h"
const agg::pix_format_e pix_format = agg::pix_format_rgb24;
typedef agg::pixfmt_rgb24 pixfmt;
typedef agg::pixfmt_rgb24_pre pixfmt_pre;
#define pixfmt_gamma agg::pixfmt_rgb24_gamma
typedef agg::rgba8 color_type;
typedef agg::order_rgb component_order;
typedef agg::gray8 gray_type;

#elif defined(AGG_SBGR24)

#include "agg_pixfmt_rgb.h"
const agg::pix_format_e pix_format = agg::pix_format_sbgr24;
typedef agg::pixfmt_sbgr24 pixfmt;
typedef agg::pixfmt_sbgr24_pre pixfmt_pre;
#define pixfmt_gamma agg::pixfmt_sbgr24_gamma
typedef agg::srgba8 color_type;
typedef agg::order_bgr component_order;
typedef agg::sgray8 gray_type;

#elif defined(AGG_SRGB24)

#include "agg_pixfmt_rgb.h"
const agg::pix_format_e pix_format = agg::pix_format_srgb24;
typedef agg::pixfmt_srgb24 pixfmt;
typedef agg::pixfmt_srgb24_pre pixfmt_pre;
#define pixfmt_gamma agg::pixfmt_srgb24_gamma
typedef agg::srgba8 color_type;
typedef agg::order_rgb component_order;
typedef agg::sgray8 gray_type;

#elif defined(AGG_BGR48)

#include "agg_pixfmt_rgb.h"
const agg::pix_format_e pix_format = agg::pix_format_bgr48;
typedef agg::pixfmt_bgr48 pixfmt;
typedef agg::pixfmt_bgr48_pre pixfmt_pre;
#define pixfmt_gamma agg::pixfmt_bgr48_gamma
typedef agg::rgba16 color_type;
typedef agg::order_bgr component_order;
typedef agg::gray16 gray_type;

#elif defined(AGG_RGB48)

#include "agg_pixfmt_rgb.h"
const agg::pix_format_e pix_format = agg::pix_format_rgb48;
typedef agg::pixfmt_rgb48 pixfmt;
typedef agg::pixfmt_rgb48_pre pixfmt_pre;
#define pixfmt_gamma agg::pixfmt_rgb48_gamma
typedef agg::rgba16 color_type;
typedef agg::order_rgb component_order;
typedef agg::gray16 gray_type;

#elif defined(AGG_BGR96)

#include "agg_pixfmt_rgb.h"
const agg::pix_format_e pix_format = agg::pix_format_bgr96;
typedef agg::pixfmt_bgr96 pixfmt;
typedef agg::pixfmt_bgr96_pre pixfmt_pre;
typedef agg::rgba32 color_type;
typedef agg::order_bgr component_order;
typedef agg::gray32 gray_type;

#elif defined(AGG_RGB96)

#include "agg_pixfmt_rgb.h"
const agg::pix_format_e pix_format = agg::pix_format_rgb96;
typedef agg::pixfmt_rgb96 pixfmt;
typedef agg::pixfmt_rgb96_pre pixfmt_pre;
typedef agg::rgba32 color_type;
typedef agg::order_bgr component_order;
typedef agg::gray32 gray_type;

#elif defined(AGG_BGRA32)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_bgra32;
typedef agg::pixfmt_bgra32 pixfmt;
typedef agg::pixfmt_bgra32_pre pixfmt_pre;
typedef agg::rgba8 color_type;
typedef agg::order_bgra component_order;
typedef agg::gray8 gray_type;

#elif defined(AGG_RGBA32)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_rgba32;
typedef agg::pixfmt_rgba32 pixfmt;
typedef agg::pixfmt_rgba32_pre pixfmt_pre;
typedef agg::rgba8 color_type;
typedef agg::order_rgba component_order;
typedef agg::gray8 gray_type;

#elif defined(AGG_ARGB32)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_argb32;
typedef agg::pixfmt_argb32 pixfmt;
typedef agg::pixfmt_argb32_pre pixfmt_pre;
typedef agg::rgba8 color_type;
typedef agg::order_argb component_order;
typedef agg::gray8 gray_type;

#elif defined(AGG_ABGR32)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_abgr32;
typedef agg::pixfmt_abgr32 pixfmt;
typedef agg::pixfmt_abgr32_pre pixfmt_pre;
typedef agg::rgba8 color_type;
typedef agg::order_argb component_order;
typedef agg::gray8 gray_type;

#elif defined(AGG_SBGRA32)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_sbgra32;
typedef agg::pixfmt_sbgra32 pixfmt;
typedef agg::pixfmt_sbgra32_pre pixfmt_pre;
typedef agg::srgba8 color_type;
typedef agg::order_bgra component_order;
typedef agg::sgray8 gray_type;

#elif defined(AGG_SRGBA32)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_srgba32;
typedef agg::pixfmt_srgba32 pixfmt;
typedef agg::pixfmt_srgba32_pre pixfmt_pre;
typedef agg::srgba8 color_type;
typedef agg::order_rgba component_order;
typedef agg::sgray8 gray_type;

#elif defined(AGG_SARGB32)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_sargb32;
typedef agg::pixfmt_sargb32 pixfmt;
typedef agg::pixfmt_sargb32_pre pixfmt_pre;
typedef agg::srgba8 color_type;
typedef agg::order_argb component_order;
typedef agg::sgray8 gray_type;

#elif defined(AGG_SABGR32)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_sabgr32;
typedef agg::pixfmt_sabgr32 pixfmt;
typedef agg::pixfmt_sabgr32_pre pixfmt_pre;
typedef agg::srgba8 color_type;
typedef agg::order_argb component_order;
typedef agg::sgray8 gray_type;

#elif defined(AGG_BGRA64)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_bgra64;
typedef agg::pixfmt_bgra64 pixfmt;
typedef agg::pixfmt_bgra64_pre pixfmt_pre;
typedef agg::rgba16 color_type;
typedef agg::order_bgra component_order;
typedef agg::gray16 gray_type;

#elif defined(AGG_RGBA64)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_rgba64;
typedef agg::pixfmt_rgba64 pixfmt;
typedef agg::pixfmt_rgba64_pre pixfmt_pre;
typedef agg::rgba16 color_type;
typedef agg::order_rgba component_order;
typedef agg::gray16 gray_type;

#elif defined(AGG_ARGB64)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_argb64;
typedef agg::pixfmt_argb64 pixfmt;
typedef agg::pixfmt_argb64_pre pixfmt_pre;
typedef agg::rgba16 color_type;
typedef agg::order_argb component_order;
typedef agg::gray16 gray_type;

#elif defined(AGG_ABGR64)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_abgr64;
typedef agg::pixfmt_abgr64 pixfmt;
typedef agg::pixfmt_abgr64_pre pixfmt_pre;
typedef agg::rgba16 color_type;
typedef agg::order_argb component_order;
typedef agg::gray16 gray_type;

#elif defined(AGG_BGRA128)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_bgra128;
typedef agg::pixfmt_bgra128 pixfmt;
typedef agg::pixfmt_bgra128_pre pixfmt_pre;
typedef agg::rgba32 color_type;
typedef agg::order_bgra component_order;
typedef agg::gray32 gray_type;

#elif defined(AGG_RGBA128)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_rgba128;
typedef agg::pixfmt_rgba128 pixfmt;
typedef agg::pixfmt_rgba128_pre pixfmt_pre;
typedef agg::rgba32 color_type;
typedef agg::order_rgba component_order;
typedef agg::gray32 gray_type;

#elif defined(AGG_ARGB128)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_argb128;
typedef agg::pixfmt_argb128 pixfmt;
typedef agg::pixfmt_argb128_pre pixfmt_pre;
typedef agg::rgba32 color_type;
typedef agg::order_argb component_order;
typedef agg::gray32 gray_type;

#elif defined(AGG_ABGR128)

#include "agg_pixfmt_rgba.h"
const agg::pix_format_e pix_format = agg::pix_format_abgr128;
typedef agg::pixfmt_abgr128 pixfmt;
typedef agg::pixfmt_abgr128_pre pixfmt_pre;
typedef agg::rgba32 color_type;
typedef agg::order_argb component_order;
typedef agg::gray32 gray_type;

#elif defined(AGG_RGB565)

#include "agg_pixfmt_rgb_packed.h"
const agg::pix_format_e pix_format = agg::pix_format_rgb565;
typedef agg::pixfmt_rgb565 pixfmt;
typedef agg::pixfmt_rgb565_pre pixfmt_pre;
typedef agg::rgba8 color_type;
typedef agg::gray8 gray_type;

#elif defined(AGG_RGB555)

#include "agg_pixfmt_rgb_packed.h"
const agg::pix_format_e pix_format = agg::pix_format_rgb555;
typedef agg::pixfmt_rgb555 pixfmt;
typedef agg::pixfmt_rgb555_pre pixfmt_pre;
typedef agg::rgba8 color_type;
typedef agg::gray8 gray_type;

#elif defined(AGG_RGB_AAA)

#include "agg_pixfmt_rgb_packed.h"
const agg::pix_format_e pix_format = agg::pix_format_rgbAAA;
typedef agg::pixfmt_rgbAAA pixfmt;
typedef agg::pixfmt_rgbAAA_pre pixfmt_pre;
typedef agg::rgba16 color_type;
typedef agg::gray16 gray_type;

#elif defined(AGG_BGR_AAA)

#include "agg_pixfmt_rgb_packed.h"
const agg::pix_format_e pix_format = agg::pix_format_bgrAAA;
typedef agg::pixfmt_bgrAAA pixfmt;
typedef agg::pixfmt_bgrAAA_pre pixfmt_pre;
typedef agg::rgba16 color_type;
typedef agg::gray16 gray_type;

#elif defined(AGG_RGB_BBA)

#include "agg_pixfmt_rgb_packed.h"
const agg::pix_format_e pix_format = agg::pix_format_rgbBBA;
typedef agg::pixfmt_rgbBBA pixfmt;
typedef agg::pixfmt_rgbBBA_pre pixfmt_pre;
typedef agg::rgba16 color_type;
typedef agg::gray16 gray_type;

#elif defined(AGG_BGR_ABB)

#include "agg_pixfmt_rgb_packed.h"
const agg::pix_format_e pix_format = agg::pix_format_bgrABB;
typedef agg::pixfmt_bgrABB pixfmt;
typedef agg::pixfmt_bgrABB_pre pixfmt_pre;
typedef agg::rgba16 color_type;
typedef agg::gray16 gray_type;

#else
#error Please define pixel format: AGG_GRAY8, AGG_BGR24, AGG_RGB24, etc. See this file above
#endif

#endif
