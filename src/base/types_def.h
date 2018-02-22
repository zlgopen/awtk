/**
 * File:   types_def.h 
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  basic types definitions.
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#include <wchar.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int xy_t;
typedef uint32_t id_t;
typedef uint32_t wh_t;

typedef enum _ret_t {
  RET_OK,
  RET_FAIL,
  RET_QUIT,
  RET_FOUND,
  RET_REMOVE,
  RET_REPEAT,
  RET_NOT_FOUND,
  RET_BAD_PARAMS
}ret_t;

#ifdef WIN32
#define log_debug(format, ...) printf(format, __VA_ARGS__)
#define log_info(format, ...) printf(format, __VA_ARGS__)
#define log_warn(format, ...) printf(format, __VA_ARGS__)
#define log_error(format, ...) printf(format, __VA_ARGS__)
#else
#define log_debug(format, args...) printf(format, ##args)
#define log_info(format, args...) printf(format, ##args)
#define log_warn(format, args...) printf(format, ##args)
#define log_error(format, args...) printf(format, ##args)
#endif

#ifdef NDEBUG
#define ENSURE(p) p
#define return_if_fail(p) \
  if (!(p))               \
  {                       \
    return;               \
  }
#define break_if_fail(p) \
  if (!(p))              \
  {                      \
    break;               \
  }
#define return_value_if_fail(p, value) \
  if (!(p))                            \
  {                                    \
    return (value);                    \
  }
#else
#define ENSURE(p) assert(p)
#define break_if_fail(p)                              \
  if (!(p))                                           \
  {                                                   \
    printf("%s:%d " #p "\n", __FUNCTION__, __LINE__); \
    break;                                            \
  }
#define return_if_fail(p)                             \
  if (!(p))                                           \
  {                                                   \
    printf("%s:%d " #p "\n", __FUNCTION__, __LINE__); \
    return;                                           \
  }
#define return_value_if_fail(p, value)                \
  if (!(p))                                           \
  {                                                   \
    printf("%s:%d " #p "\n", __FUNCTION__, __LINE__); \
    return (value);                                   \
  }
#endif

#ifdef __cplusplus
#define BEGIN_C_DECLS extern "C" {
#define END_C_DECLS }
#else
#define BEGIN_C_DECLS
#define END_C_DECLS
#endif

enum _bool_t{
  FALSE = 0,
  TRUE = 1
};

typedef int8_t bool_t;
typedef void* pointer_t;

#ifndef ftk_min
#define ftk_min(a, b) ((a) < (b) ? (a) : (b))
#endif/*ftk_min*/

#ifndef ftk_max
#define ftk_max(a, b) ((a) > (b) ? (a) : (b))
#endif/*ftk_max*/

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#define load_uint32(p, v) (v) = (p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24)); p += 4;
#define save_uint32(p, v) p[0]  = (v & 0xff); p[1] = (v >> 8) & 0xff; p[2] = (v >> 16) & 0xff; p[3] = (v >> 24) & 0xff; p += 4;

#define load_uint16(p, v) (v) = (p[0] | (p[1] << 8)); p += 2;
#define save_uint16(p, v) p[0]  = (v & 0xff); p[1] = (v >> 8) & 0xff; p += 2;

#define load_uint8(p, v) (v) = p[0]; p += 1;
#define save_uint8(p, v) p[0]  = v; p += 1;

typedef int (*compare_t)(const void *a, const void *b);
typedef bool_t (*visit_t)(void *ctx, void *data);

enum {
  NAME_LEN = 15
};

#endif/*TYPES_DEF_H*/
