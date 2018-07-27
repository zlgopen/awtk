/**
 * File:   types_def.h
 * Author: AWTK Develop Team
 * Brief:  basic types definitions.
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TYPES_DEF_H
#define TYPES_DEF_H

#include <math.h>
#include <ctype.h>
#include <wchar.h>
#include <assert.h>
#include <stdint.h>

#ifdef HAS_STDIO
#include <stdio.h>
#endif /*HAS_STDIO*/

#include <stdlib.h>
#include <string.h>

typedef int32_t xy_t;
typedef int32_t wh_t;

/**
 * @enum ret_t
 * @scriptable
 * 函数返回值常量定义。
 */
typedef enum _ret_t {
  /**
   * @const RET_OK
   * 成功。
   */
  RET_OK = 0,
  /**
   * @const RET_OOM
   * Out of memory。
   */
  RET_OOM,
  /**
   * @const RET_FAIL
   * 失败。
   */
  RET_FAIL,
  /**
   * @const RET_NOT_IMPL
   * 没有实现/不支持。
   */
  RET_NOT_IMPL,
  /**
   * @const RET_QUIT
   * 退出。通常用于主循环。
   */
  RET_QUIT,
  /**
   * @const RET_FOUND
   * 找到。
   */
  RET_FOUND,
  /**
   * @const RET_REMOVE
   * 移出。通常用于定时器。
   */
  RET_REMOVE,
  /**
   * @const RET_REPEAT
   * 重复。通常用于定时器。
   */
  RET_REPEAT,
  /**
   * @const RET_NOT_FOUND
   * 没找到。
   */
  RET_NOT_FOUND,
  /**
   * @const RET_DONE
   * 操作完成。
   */
  RET_DONE,
  /**
   * @const RET_STOP
   * 停止后续操作。
   */
  RET_STOP,
  /**
   * @const RET_CONTINUE
   * 继续后续操作。
   */
  RET_CONTINUE,
  /**
   * @const RET_BAD_PARAMS
   * 无效参数。
   */
  RET_BAD_PARAMS
} ret_t;

/**
 * @enum lcd_orientation_t
 * LCD旋转角度(XXX:目前仅支持0度和90度)。
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

#ifdef WIN32
#include <windows.h>
#define log_debug(format, ...) printf(format, __VA_ARGS__)
#define log_info(format, ...) printf(format, __VA_ARGS__)
#define log_warn(format, ...) printf(format, __VA_ARGS__)
#define log_error(format, ...) printf(format, __VA_ARGS__)
#elif defined(HAS_STDIO)
#define log_debug(format, args...) printf(format, ##args)
#define log_info(format, args...) printf(format, ##args)
#define log_warn(format, args...) printf(format, ##args)
#define log_error(format, args...) printf(format, ##args)
#else
#define log_debug(format, args...)
#define log_info(format, args...)
#define log_warn(format, args...)
#define log_error(format, args...)
#endif

#if !defined(WIN32) && !defined(MAX_PATH)
#define MAX_PATH 255
#endif /*MAX_PATH*/

#ifdef NDEBUG
#define ENSURE(p) p
#define goto_error_if_fail(p) \
  if (!(p)) {                 \
    goto error;               \
  }

#define return_if_fail(p) \
  if (!(p)) {             \
    return;               \
  }

#define break_if_fail(p) \
  if (!(p)) {            \
    break;               \
  }

#define return_value_if_fail(p, value) \
  if (!(p)) {                          \
    return (value);                    \
  }
#else
#define ENSURE(p) assert(p)
#define goto_error_if_fail(p)                           \
  if (!(p)) {                                           \
    log_warn("%s:%d " #p "\n", __FUNCTION__, __LINE__); \
    goto error;                                         \
  }

#define break_if_fail(p)                                \
  if (!(p)) {                                           \
    log_warn("%s:%d " #p "\n", __FUNCTION__, __LINE__); \
    break;                                              \
  }

#define return_if_fail(p)                               \
  if (!(p)) {                                           \
    log_warn("%s:%d " #p "\n", __FUNCTION__, __LINE__); \
    return;                                             \
  }

#define return_value_if_fail(p, value)                  \
  if (!(p)) {                                           \
    log_warn("%s:%d " #p "\n", __FUNCTION__, __LINE__); \
    return (value);                                     \
  }

#endif

#ifdef __cplusplus
#define BEGIN_C_DECLS extern "C" {
#define END_C_DECLS }
#else
#define BEGIN_C_DECLS
#define END_C_DECLS
#endif

#ifndef TRUE
#define TRUE 1
#endif /*TRUE*/

#ifndef FALSE
#define FALSE 0
#endif /*FALSE*/

#ifndef bool_t
#define bool_t uint8_t
#endif /*bool_t*/

typedef void* pointer_t;

#ifndef tk_min
#define tk_min(a, b) ((a) < (b) ? (a) : (b))
#endif /*tk_min*/

#ifndef tk_abs
#define tk_abs(a) ((a) < (0) ? (-a) : (a))
#endif /*tk_abs*/

#ifndef tk_max
#define tk_max(a, b) ((a) > (b) ? (a) : (b))
#endif /*tk_max*/

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif /*ARRAY_SIZE*/

typedef int (*tk_compare_t)(const void* a, const void* b);
typedef ret_t (*tk_visit_t)(void* ctx, void* data);
typedef ret_t (*tk_destroy_t)(void* data);
typedef ret_t (*tk_on_done_t)(void* data);
typedef bool_t (*tk_is_valid_t)(void* data);

/*NAME_LEN+1 must aligned to 4*/
enum { NAME_LEN = 31 };

/**
 * @enum align_v_t
 * @scriptable
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
 * @scriptable
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
 * @scriptable no
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
 * @scriptable no
 * 图标所在的位置。
 */
typedef enum _icon_at_type_t {
  /**
   * @const ICON_AT_NONE
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

typedef float float_t;

#define TK_DEFAULT_FONT "default"
#define TK_DEFAULT_FONT_SIZE 18
#define TK_MAX_FPS 100
#define TK_OPACITY_ALPHA 0xfa
#define TK_TRANSPARENT_ALPHA 0x05
#define TK_INVALID_ID 0

#define tk_str_eq(s1, s2) \
  (((s1) != NULL) && ((s2) != NULL) && *(s1) == *(s2) && strcmp((s1), (s2)) == 0)
#define tk_wstr_eq(s1, s2) \
  (((s1) != NULL) && ((s2) != NULL) && *(s1) == *(s2) && wcscmp((s1), (s2)) == 0)
#define tk_fequal(f1, f2) (fabs((f1) - (f2)) < 0.0000001)

#endif /*TYPES_DEF_H*/
