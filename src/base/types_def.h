/**
 * File:   types_def.h
 * Author: AWTK Develop Team
 * Brief:  basic types definitions.
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * LCD旋转角度(XXX:目前仅支持0度和90度，逆时针方向)。
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
  ICON_AT_BOTTOM = 4
} icon_at_type_t;

/**
 * @enum app_type_t
 * @prefix APP_
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

#ifndef TK_DEFAULT_FONT
#ifdef WITH_MINI_FONT
#define TK_DEFAULT_FONT "default_mini"
#else
#define TK_DEFAULT_FONT "default"
#endif /*WITH_MINI_FONT*/
#endif /*TK_DEFAULT_FONT*/

#ifndef TK_DEFAULT_FONT_SIZE
#define TK_DEFAULT_FONT_SIZE 18
#endif /*TK_DEFAULT_FONT_SIZE*/

#ifndef TK_MAX_FPS
#define TK_MAX_FPS 100
#endif /*TK_MAX_FPS*/

#define TK_OPACITY_ALPHA 0xfa
#define TK_TRANSPARENT_ALPHA 0x02

#define TK_DRAG_THRESHOLD 10
#define TK_ANIMATING_TIME 500

struct _widget_t;
typedef struct _widget_t widget_t;

struct _widget_vtable_t;
typedef struct _widget_vtable_t widget_vtable_t;

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

#ifdef WITH_SDL
#define WITH_WIDGET_TYPE_CHECK 1
#endif /*WITH_SDL*/

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

#ifdef WITH_VGCANVAS
#define WITH_WINDOW_ANIMATORS 1
#endif /*WITH_VGCANVAS*/

#endif /*TK_TYPES_DEF_H*/
