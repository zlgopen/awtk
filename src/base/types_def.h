/**
 * File:   types_def.h
 * Author: AWTK Develop Team
 * Brief:  basic types definitions.
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_TYPES_DEF_H
#define TK_TYPES_DEF_H

#include "tkc/types_def.h"

/**
 * @enum lcd_orientation_t
 * LCD旋转角度。
 */
typedef enum _lcd_orientation_t {
  /**
   * @const LCD_ORIENTATION_0
   * LCD没有旋转。
   */
  LCD_ORIENTATION_0 = 0,
  /**
   * @const LCD_ORIENTATION_90
   * LCD旋转90度。
   */
  LCD_ORIENTATION_90 = 90,
  /**
   * @const LCD_ORIENTATION_180
   * LCD旋转180度。
   */
  LCD_ORIENTATION_180 = 180,
  /**
   * @const LCD_ORIENTATION_270
   * LCD旋转270度。
   */
  LCD_ORIENTATION_270 = 270
} lcd_orientation_t;

/**
 * @enum align_v_t
 * @annotation ["scriptable"]
 * 垂直对齐的常量定义。
 */
typedef enum _align_v_t {
  /**
   * @const ALIGN_V_NONE
   * 无效对齐方式。
   */
  ALIGN_V_NONE = 0,
  /**
   * @const ALIGN_V_MIDDLE
   * 居中对齐。
   */
  ALIGN_V_MIDDLE,
  /**
   * @const ALIGN_V_TOP
   * 顶部对齐。
   */
  ALIGN_V_TOP,
  /**
   * @const ALIGN_V_BOTTOM
   * 底部对齐。
   */
  ALIGN_V_BOTTOM
} align_v_t;

/**
 * @enum align_h_t
 * @annotation ["scriptable"]
 * 水平对齐的常量定义。
 */
typedef enum _align_h_t {
  /**
   * @const ALIGN_H_NONE
   * 无效对齐方式。
   */
  ALIGN_H_NONE = 0,
  /**
   * @const ALIGN_H_CENTER
   * 居中对齐。
   */
  ALIGN_H_CENTER,
  /**
   * @const ALIGN_H_LEFT
   * 左边对齐。
   */
  ALIGN_H_LEFT,
  /**
   * @const ALIGN_H_RIGHT
   * 右边对齐。
   */
  ALIGN_H_RIGHT
} align_h_t;

/**
 * @enum border_type_t
 * 控件边框类型。
 */
typedef enum _border_type_t {
  /**
   * @const BORDER_NONE
   * 无边框。
   */
  BORDER_NONE = 0,
  /**
   * @const BORDER_LEFT
   * 左边框线。
   */
  BORDER_LEFT = 1,
  /**
   * @const BORDER_RIGHT
   * 右边框线。
   */
  BORDER_RIGHT = 2,
  /**
   * @const BORDER_TOP
   * 上边框线。
   */
  BORDER_TOP = 4,
  /**
   * @const BORDER_BOTTOM
   * 下边框线。
   */
  BORDER_BOTTOM = 8,
  /**
   * @const BORDER_ALL
   * 全边框线。
   */
  BORDER_ALL = 0xffff
} border_type_t;

/**
 * @enum icon_at_type_t
 * @prefix ICON_AT
 * 图标所在的位置。
 */
typedef enum _icon_at_type_t {
  /**
   * @const ICON_AT_AUTO
   * 自动选择位置。
   */
  ICON_AT_AUTO = 0,
  /**
   * @const ICON_AT_LEFT
   * 水平左边，垂直居中。
   */
  ICON_AT_LEFT = 1,
  /**
   * @const ICON_AT_RIGHT
   * 水平右边，垂直居中(not used now)。
   */
  ICON_AT_RIGHT = 2,
  /**
   * @const ICON_AT_TOP
   * 水平居中，垂直顶部(not used now)。
   */
  ICON_AT_TOP = 3,
  /**
   * @const ICON_AT_BOTTOM
   * 水平居中，垂直低部(not used now)。
   */
  ICON_AT_BOTTOM = 4,
  /**
   * @const ICON_AT_CENTRE
   * 水平居中，垂直居中。
   */
  ICON_AT_CENTRE = 5,
} icon_at_type_t;

/**
 * @enum app_type_t
 * @prefix APP_
 * @annotation ["scriptable"]
 * 应用程序类型。
 */
typedef enum _app_type_t {
  /**
   * @const APP_MOBILE
   * 嵌入式或移动APP
   */
  APP_MOBILE = 0,
  /**
   * @const APP_SIMULATOR
   * 模拟器。
   */
  APP_SIMULATOR,
  /**
   * @const APP_DESKTOP
   * 桌面应用程序。
   */
  APP_DESKTOP
} app_type_t;

#if defined(WITH_STM32_G2D) || defined(WITH_PXP_G2D)
#define WITH_G2D 1
#endif /*WITH_PXP_G2D*/

#if defined(WITH_NANOVG_AGGE) || defined(WITH_NANOVG_AGG)
#ifndef WITH_NANOVG_SOFT
#define WITH_NANOVG_SOFT
#endif /*WITH_NANOVG_SOFT*/
#endif /*defined(WITH_NANOVG_AGGE) || defined(WITH_NANOVG_AGG)*/

#if defined(WITH_NANOVG_SOFT) || defined(WITH_NANOVG_GPU)
#define WITH_NANOVG 1
#define WITH_VGCANVAS 1
#endif /*defined(WITH_NANOVG_SOFT) || defined(WITH_NANOVG_GPU)*/

#ifndef TK_DEFAULT_FONT_SIZE
#define TK_DEFAULT_FONT_SIZE 18
#endif /*TK_DEFAULT_FONT_SIZE*/

#ifndef TK_MAX_FPS
#if defined(LINUX) || defined(MACOS) || defined(WIN32)
#define TK_MAX_FPS 60
#else
#define TK_MAX_FPS 100
#endif
#endif /*TK_MAX_FPS*/

/* alpha 大于 TK_OPACITY_ALPHA 的颜色认为是不透明颜色，不进行alpha混合。*/
#define TK_OPACITY_ALPHA 0xfa

/* alpha 小于 TK_OPACITY_ALPHA 的颜色认为是透明颜色，不进行alpha混合，直接丢弃。*/
#define TK_TRANSPARENT_ALPHA 0x02

#define TK_DRAG_THRESHOLD 10
#define TK_CLICK_TOLERANCE 10
#define TK_ANIMATING_TIME 500
#define TK_INPUTING_TIMEOUT 3000

struct _widget_t;
typedef struct _widget_t widget_t;

struct _widget_vtable_t;
typedef struct _widget_vtable_t widget_vtable_t;

struct _widget_animator_manager_t;
typedef struct _widget_animator_manager_t widget_animator_manager_t;

struct _widget_animator_t;
typedef struct _widget_animator_t widget_animator_t;

#define fix_xywh(x, y, w, h) \
  if (w < 0) {               \
    w = -w;                  \
    x = x - w + 1;           \
  }                          \
  if (h < 0) {               \
    h = -h;                  \
    y = y - h + 1;           \
  }

#define TK_LONG_PRESS_TIME 1000

#ifdef WITH_WIDGET_TYPE_CHECK
#define TK_REF_VTABLE(vt) &(g_##vt##_vtable)
#define TK_PARENT_VTABLE(vt) TK_REF_VTABLE(vt)
#define TK_DECL_VTABLE(vt) const widget_vtable_t g_##vt##_vtable
#define TK_EXTERN_VTABLE(vt) extern const widget_vtable_t g_##vt##_vtable
#else
#define TK_REF_VTABLE(vt) &(s_##vt##_vtable)
#define TK_PARENT_VTABLE(vt) NULL
#define TK_DECL_VTABLE(vt) static const widget_vtable_t s_##vt##_vtable
#define TK_EXTERN_VTABLE(vt)
#endif /*WITH_WIDGET_TYPE_CHECK*/

#if defined(WITH_VGCANVAS)
#define WITH_WINDOW_ANIMATORS 1
#endif /*WITH_VGCANVAS*/

#ifndef TK_KEY_MOVE_FOCUS_NEXT
#define TK_KEY_MOVE_FOCUS_NEXT "tab"
#endif /*TK_KEY_MOVE_FOCUS_NEXT*/

#ifndef TK_KEY_MOVE_FOCUS_PREV
#define TK_KEY_MOVE_FOCUS_PREV "shift+tab"
#endif /*TK_KEY_MOVE_FOCUS_PREV*/

struct _locale_info_t;
typedef struct _locale_info_t locale_info_t;

struct _assets_manager_t;
typedef struct _assets_manager_t assets_manager_t;

struct _image_manager_t;
typedef struct _image_manager_t image_manager_t;

struct _system_info_t;
typedef struct _system_info_t system_info_t;

#define STR_SCHEMA_FILE "file://"
#define STR_SCHEMA_HTTP "http://"
#define STR_SCHEMA_HTTPS "https://"

#if defined(WITH_NANOVG_GL3) || defined(WITH_NANOVG_GL2) || defined(WITH_NANOVG_GLES3) || \
    defined(WITH_NANOVG_GLES2)
#define WITH_NANOVG_GL 1
#endif

#ifndef TK_GLYPH_CACHE_NR
#ifdef WITH_SDL
#define TK_GLYPH_CACHE_NR 4096
#else
#define TK_GLYPH_CACHE_NR 256
#endif /*WITH_SDL*/

#endif /*TK_GLYPH_CACHE_NR*/

#if TK_GLYPH_CACHE_NR <= 0
#error " TK_GLYPH_CACHE_NR must > 0 "
#endif

#if defined(WITH_STB_FONT) || defined(WITH_FT_FONT)
#define WITH_TRUETYPE_FONT 1
#endif /*WITH_STB_FONT or WITH_FT_FONT*/

#if defined(WITH_LCD_MONO)
#undef WITH_VGCANVAS
#undef WITH_WINDOW_ANIMATORS
#define WITH_BITMAP_FONT 1
#endif /*WITH_LCD_MONO*/

#ifdef AWTK_LITE
#define WITH_NULL_IM 1
#define WITHOUT_LAYOUT 1
#define WITHOUT_CLIPBOARD 1
#define WITHOUT_EXT_WIDGETS 1
#define WITHOUT_INPUT_METHOD 1
#define WITHOUT_WINDOW_ANIMATORS
#define WITHOUT_WIDGET_ANIMATORS 1
#define WITHOUT_DIALOG_HIGHLIGHTER 1
#endif /*AWTK_LITE*/

/**
 * @enum bitmap_format_t
 * @prefix BITMAP_FMT_
 * @annotation ["scriptable"]
 * 位图格式常量定义。
 */
typedef enum _bitmap_format_t {
  /**
   * @const BITMAP_FMT_NONE
   * 无效格式。
   */
  BITMAP_FMT_NONE = 0,
  /**
   * @const BITMAP_FMT_RGBA8888
   * 一个像素占用4个字节，RGBA占一个字节，按内存地址递增。
   */
  BITMAP_FMT_RGBA8888,
  /**
   * @const BITMAP_FMT_ABGR8888
   * 一个像素占用4个字节，ABGR占一个字节，按内存地址递增。
   */
  BITMAP_FMT_ABGR8888,
  /**
   * @const BITMAP_FMT_BGRA8888
   * 一个像素占用4个字节，BGRA占一个字节，按内存地址递增。
   */
  BITMAP_FMT_BGRA8888,
  /**
   * @const BITMAP_FMT_ARGB8888
   * 一个像素占用4个字节，ARGB占一个字节，按内存地址递增。
   */
  BITMAP_FMT_ARGB8888,
  /**
   * @const BITMAP_FMT_RGB565
   * 一个像素占用2个字节，RGB分别占用5,6,5位, 按内存地址递增。
   */
  BITMAP_FMT_RGB565,
  /**
   * @const BITMAP_FMT_BGR565
   * 一个像素占用2个字节，BGR分别占用5,6,5位, 按内存地址递增。
   */
  BITMAP_FMT_BGR565,
  /**
   * @const BITMAP_FMT_RGB888
   * 一个像素占用3个字节，RGB占一个字节，按内存地址递增。
   */
  BITMAP_FMT_RGB888,
  /**
   * @const BITMAP_FMT_BGR888
   * 一个像素占用3个字节，RGB占一个字节，按内存地址递增。
   */
  BITMAP_FMT_BGR888,
  /**
   * @const BITMAP_FMT_GRAY
   * 一个像素占用1个字节。
   */
  BITMAP_FMT_GRAY,
  /**
   * @const BITMAP_FMT_MONO
   * 一个像素占用1比特。
   */
  BITMAP_FMT_MONO,
} bitmap_format_t;

/**
 * @enum bitmap_flag_t
 * @annotation ["scriptable"]
 * @prefix BITMAP_FLAG_
 * 位图标志常量定义。
 */
typedef enum _bitmap_flag_t {
  /**
   * @const BITMAP_FLAG_NONE
   * 无特殊标志。
   */
  BITMAP_FLAG_NONE = 0,
  /**
   * @const BITMAP_FLAG_OPAQUE
   * 不透明图片。
   */
  BITMAP_FLAG_OPAQUE = 1,
  /**
   * @const BITMAP_FLAG_IMMUTABLE
   * 图片内容不会变化。
   */
  BITMAP_FLAG_IMMUTABLE = 2,
  /**
   * @const BITMAP_FLAG_TEXTURE
   * OpenGL Texture, bitmap的id是有效的texture id。
   */
  BITMAP_FLAG_TEXTURE = 4,
  /**
   * @const BITMAP_FLAG_CHANGED
   * 如果是MUTABLE的图片，更新时需要设置此标志，底层可能会做特殊处理，比如更新图片到GPU。
   */
  BITMAP_FLAG_CHANGED = 8,
  /**
   * @const BITMAP_FLAG_PREMULTI_ALPHA
   * 预乘alpha。
   */
  BITMAP_FLAG_PREMULTI_ALPHA = 16
} bitmap_flag_t;

#ifndef BITMAP_ALIGN_SIZE
#define BITMAP_ALIGN_SIZE 32
#endif /*BITMAP_ALIGN_SIZE*/

#ifndef WITH_ASSET_LOADER
#ifdef WITH_FS_RES
#define WITH_ASSET_LOADER 1
#endif /*WITH_FS_RES*/
#endif /*WITH_ASSET_LOADER*/

struct _bitmap_t;
typedef struct _bitmap_t bitmap_t;

struct _input_method_t;
typedef struct _input_method_t input_method_t;

struct _input_engine_t;
typedef struct _input_engine_t input_engine_t;

#endif /*TK_TYPES_DEF_H*/
