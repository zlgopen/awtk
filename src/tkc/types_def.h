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

#ifndef TYPES_DEF_H
#define TYPES_DEF_H

#include <math.h>
#include <ctype.h>
#include <wchar.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAS_STDIO
#include <stdio.h>
#else
#define STBI_NO_STDIO
#endif /*HAS_STDIO*/

#ifndef TRUE
#define TRUE 1
#endif /*TRUE*/

#ifndef FALSE
#define FALSE 0
#endif /*FALSE*/

#if defined(__GNUC__) && !defined(__cplusplus)
typedef _Bool bool_t;
#else
typedef uint8_t bool_t;
#endif

typedef int32_t xy_t;
typedef int32_t wh_t;
typedef float float_t;
typedef void* pointer_t;
typedef uint16_t font_size_t;

struct _value_t;
typedef struct _value_t value_t;

struct _object_t;
typedef struct _object_t object_t;

/**
 * @enum ret_t
 * @annotation ["scriptable"]
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
   * @const RET_BUSY
   * 对象忙。
   */
  RET_BUSY,
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
   * @const RET_OBJECT_CHANGED
   * 对象属性变化。
   */
  RET_OBJECT_CHANGED,
  /**
   * @const RET_ITEMS_CHANGED
   * 集合数目变化。
   */
  RET_ITEMS_CHANGED,
  /**
   * @const RET_BAD_PARAMS
   * 无效参数。
   */
  RET_BAD_PARAMS
} ret_t;

#ifdef WIN32
#include <windows.h>
#define random rand
#define srandom srand
#define strcasecmp stricmp
#define log_debug(format, ...) printf(format, __VA_ARGS__)
#define log_info(format, ...) printf(format, __VA_ARGS__)
#define log_warn(format, ...) printf(format, __VA_ARGS__)
#define log_error(format, ...) printf(format, __VA_ARGS__)
#define snprintf _snprintf
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

#if defined(WIN32)
#define PATH_SEP '\\'
#else
#define PATH_SEP '/'
#endif /*PATH_SEP*/

#if defined(NDEBUG) || defined(SYLIXOS)
#define ENSURE(p) (void)(p)
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

#define tk_min(a, b) ((a) < (b) ? (a) : (b))
#define tk_abs(a) ((a) < (0) ? (-a) : (a))
#define tk_max(a, b) ((a) > (b) ? (a) : (b))
#define tk_roundi(a) (int32_t)(((a) >= 0) ? ((a) + 0.5f) : ((a)-0.5f))
#define tk_clampi(a, mn, mx) ((a) < (mn) ? (mn) : ((a) > (mx) ? (mx) : (a)))

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif /*ARRAY_SIZE*/

typedef void* (*tk_create_t)(void);
typedef ret_t (*tk_destroy_t)(void* data);
typedef ret_t (*tk_on_done_t)(void* data);
typedef bool_t (*tk_is_valid_t)(void* data);
typedef int (*tk_compare_t)(const void* a, const void* b);
typedef ret_t (*tk_visit_t)(void* ctx, const void* data);

/*TK_NAME_LEN+1 must aligned to 4*/
enum { TK_NAME_LEN = 31 };

#ifdef WITH_CPPCHECK
#define tk_str_eq strcmp
#define tk_str_eq strcasecmp
#else
#define tk_str_eq(s1, s2) \
  (((s1) != NULL) && ((s2) != NULL) && *(s1) == *(s2) && strcmp((s1), (s2)) == 0)
#define tk_str_ieq(s1, s2) (((s1) != NULL) && ((s2) != NULL) && strcasecmp((s1), (s2)) == 0)

#define tk_wstr_eq(s1, s2) \
  (((s1) != NULL) && ((s2) != NULL) && *(s1) == *(s2) && wcscmp((s1), (s2)) == 0)
#endif /*WITH_CPPCHECK*/

#define tk_fequal(f1, f2) (fabs((f1) - (f2)) < 0.0000001)

#define TK_ROUND_TO(size, round_size) ((((size) + round_size - 1) / round_size) * round_size)

#ifndef M_PI
#define M_PI 3.1415926f
#endif /*M_PI*/

#define TK_INVALID_ID 0
#define TK_NUM_MAX_LEN 31
#define TK_UINT32_MAX 0xffffffff
#define TK_LOCALE_MAGIC "$locale$"

#define TK_D2R(d) (((d)*M_PI) / 180)
#define TK_R2D(r) (((r)*180) / M_PI)

#if defined(HAS_AWTK_CONFIG)
#include "awtk_config.h"
#endif /*HAS_AWTK_CONFIG*/

#ifdef _MSC_VER
#define TK_CONST_DATA_ALIGN(v) __declspec(align(8)) v
#else
#define TK_CONST_DATA_ALIGN(v) v __attribute__((aligned(8)))
#endif /*_MSC_VER*/

#endif /*TYPES_DEF_H*/
