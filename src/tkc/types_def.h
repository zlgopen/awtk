/**
 * File:   types_def.h
 * Author: AWTK Develop Team
 * Brief:  basic types definitions.
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifdef __cplusplus
#define BEGIN_C_DECLS extern "C" {
#define END_C_DECLS }
#else
#define BEGIN_C_DECLS
#define END_C_DECLS
#endif

#if TK_DISABLE_DEPRECATE_WARNINGS
#define TK_DEPRECATED(message)
#else
#if ((__GNUC__ * 100 + __GNUC_MINOR__) >= 405) || defined(__clang__)
#define TK_DEPRECATED(message) __attribute__((deprecated(message)))
#elif ((__GNUC__ * 100 + __GNUC_MINOR__) >= 301)
#define TK_DEPRECATED(message) __attribute__((deprecated))
#elif defined(_MSC_VER)
#define TK_DEPRECATED(message) __declspec(deprecated(message))
#else
/* Not support TK_DEPRECATED */
#define TK_DEPRECATED(message)
#endif
#endif /* TK_DISABLE_DEPRECATE_WARNINGS */

#if defined(__GNUC__)
#define TK_MAYBE_UNUSED __attribute__((unused))
#else
#define TK_MAYBE_UNUSED
#endif

#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <inttypes.h>

#if defined(HAS_AWTK_CONFIG) || defined(CONFIG_HAS_AWTK_CONFIG)
#include "awtk_config.h"
#endif /*HAS_AWTK_CONFIG*/

#if defined(WIN32) || defined(LINUX) || defined(MACOS) || defined(ANDROID) || defined(IOS)
#define WITH_SOCKET 1
#endif /*WIN32 || MACOS || LINUX || IOS || ANDROID*/

#ifndef TK_IS_PC
#if defined(WITH_SDL) && !defined(ANDROID) && !defined(IOS)
#define TK_IS_PC 1
#endif
#endif /*TK_IS_PC*/

#ifndef WITH_WASM
#include <stddef.h>
#include <math.h>
#include <time.h>
#include <wchar.h>
#include <assert.h>
#include <wctype.h>
#ifdef WITH_SOCKET
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <winsock2.h>
typedef int socklen_t;
#elif defined(WITH_LWIP)
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#define perror(s)
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#endif /*WIN32*/
#endif /*WITH_SOCKET*/

#else
#include "tkc/wasm_helper.h"
#endif /*WITH_WASM*/

BEGIN_C_DECLS
int32_t tk_strcmp(const char* a, const char* b);
int32_t tk_strncmp(const char* a, const char* b, size_t n);
int32_t tk_stricmp(const char* a, const char* b);
int32_t tk_strnicmp(const char* a, const char* b, size_t n);
int32_t tk_wstrcmp(const wchar_t* a, const wchar_t* b);
int32_t tk_wstricmp(const wchar_t* a, const wchar_t* b);
END_C_DECLS

#if defined(__MINGW32__) || defined(__MINGW64__)
#ifndef MINGW
#define MINGW 1
#endif
#endif

#if defined(HAS_STDIO) || defined(AWTK_WEB)
#include <stdio.h>
#else
#define STBI_NO_STDIO
#endif /*HAS_STDIO*/

#ifndef TK_WEAK
#if defined(__CC_ARM) || \
    (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) /* ARM Compiler */
#define TK_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__) /* for IAR Compiler */
#define TK_WEAK __weak
#elif defined(__GNUC__) && !defined(__MINGW32__) && !defined(__MINGW64__) /* GNU GCC Compiler */
#define TK_WEAK __attribute__((weak))
#elif defined(__ADSPBLACKFIN__) /* for VisualDSP++ Compiler */
#define TK_WEAK __attribute__((weak))
#elif defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__) /* Windows */
#define TK_WEAK
#elif defined(__TI_COMPILER_VERSION__)
#define TK_WEAK
#else
#define TK_WEAK
#endif
#endif

#ifndef TRUE
#define TRUE 1
#endif /*TRUE*/

#ifndef FALSE
#define FALSE 0
#endif /*FALSE*/

#define _TK_STRINGIZE_(symbol) #symbol
#define TK_STRINGIZE(macro) _TK_STRINGIZE_(macro)

#define _TK_CONCAT_(a, b) a##b
#define TK_CONCAT(a, b) _TK_CONCAT_(a, b)

#define TK_STATIC_ASSERT(p)                                                                  \
  typedef struct {                                                                           \
    TK_MAYBE_UNUSED char TK_CONCAT(Static_Assert_Failed_at_Line_, __LINE__)[!!(p) ? 1 : -1]; \
  } TK_CONCAT(_sTATIC_aSSERT_, __LINE__)

#if defined(__GNUC__) && !defined(__cplusplus)
typedef _Bool bool_t;
#else
typedef uint8_t bool_t;
#endif

typedef int32_t xy_t;
typedef int32_t wh_t;
typedef uint16_t font_size_t;

/* 
 * 如果使用了 mingw 或者 32 位 pc 的 linux 出现 “conflicting declaration 'typedef float float_t'” 的错误提示的话，
 * 因为 awtk 的 float_t 类型为 float，所以请定义 __FLT_EVAL_METHOD__ 宏并且宏为 0 （#define __FLT_EVAL_METHOD__ 0 ）
 */
#if defined(WITH_DOUBLE_FLOAT)
typedef long double float_t;
#else
typedef float float_t;
#endif /*WITH_DOUBLE_FLOAT*/

#if !defined(ANDROID) && !defined(IOS) && !defined(LINUX) && !defined(MACOS) && !defined(WIN32)
#ifndef time_t
#define time_t uint64_t
#endif /*time_t*/
#endif

struct _value_t;
typedef struct _value_t value_t;

struct _tk_object_t;
typedef struct _tk_object_t tk_object_t;

#ifndef USE_TK_PREFIX
#define object_t tk_object_t
#endif /*USE_TK_PREFIX*/

/**
 * @enum ret_t
 * @prefix RET_
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
   * @const RET_SKIP
   * 跳过当前项。
   */
  RET_SKIP,
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
  RET_BAD_PARAMS,
  /**
   * @const RET_TIMEOUT
   * 超时。
   */
  RET_TIMEOUT,
  /**
   * @const RET_CRC
   * CRC错误。
   */
  RET_CRC,
  /**
   * @const RET_IO
   * IO错误。
   */
  RET_IO,
  /**
   * @const RET_EOS
   * End of Stream
   */
  RET_EOS,
  /**
   * @const RET_NOT_MODIFIED
   * 没有改变。
   */
  RET_NOT_MODIFIED,
  /**
   * @const RET_NO_PERMISSION
   * 没有权限。
   */
  RET_NO_PERMISSION,
  /**
   * @const RET_INVALID_ADDR
   * 无效地址。
   */
  RET_INVALID_ADDR,
  /**
   * @const RET_EXCEED_RANGE
   * 超出范围。
   */
  RET_EXCEED_RANGE,
  /**
   * @const RET_MAX_NR
   * 最大值。
   */
  RET_MAX_NR
} ret_t;

#include "tkc/log.h"

#if defined(WIN32) || defined(__ARMCC_VERSION) || defined(MINGW)
#define random rand
#define srandom srand
#endif /*WIN32||__ARMCC_VERSION*/

#if !defined(WIN32) && !defined(MAX_PATH)
#define MAX_PATH 255
#endif /*MAX_PATH*/

#if defined(WIN32)
#define TK_PATH_SEP '\\'
#else
#define TK_PATH_SEP '/'
#endif /*TK_PATH_SEP*/

#if defined(WIN32) && !defined(MINGW)
#ifndef snprintf
#define snprintf _snprintf
#endif /*snprintf*/

#define strcasecmp stricmp
#define strncasecmp strnicmp
#define wcscasecmp wcsicmp

#endif /*TK_PATH_SEP*/

#define log_if_fail(p)                                                       \
  do {                                                                       \
    if (!(p)) {                                                              \
      log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
    }                                                                        \
  } while (0)

#if defined(NDEBUG) || defined(SYLIXOS)
#ifdef WITH_INFERCHECK
#define __INFER_ENSURE__(p) (void)(!!(p) || (exit(0), 0))
#define ENSURE(p) __INFER_ENSURE__(p)
#else
#define ENSURE(p) (void)(p)
#endif
#define goto_error_if_fail(p) \
  do {                        \
    if (!(p)) {               \
      goto error;             \
    }                         \
  } while (0)

#define goto_error_if_fail_ex(p, sentence) \
  do {                                     \
    if (!(p)) {                            \
      sentence;                            \
      goto error;                          \
    }                                      \
  } while (0)

#define return_if_fail(p) \
  do {                    \
    if (!(p)) {           \
      return;             \
    }                     \
  } while (0)

#define break_if_fail(p) \
  {                      \
    if (!(p)) {          \
      break;             \
    }                    \
  }

#define break_if_fail_ex(p, sentence) \
  {                                   \
    if (!(p)) {                       \
      sentence;                       \
      break;                          \
    }                                 \
  }

#define return_value_if_fail(p, value) \
  do {                                 \
    if (!(p)) {                        \
      return (value);                  \
    }                                  \
  } while (0)

#define return_ret_if_fail(p)                    \
  do {                                           \
    ret_t TK_CONCAT(__ret_, __LINE__) = p;       \
    if (TK_CONCAT(__ret_, __LINE__) != RET_OK) { \
      return TK_CONCAT(__ret_, __LINE__);        \
    }                                            \
  } while (0)
#else
#define ENSURE(p) assert(p)
#define goto_error_if_fail(p)                                                \
  do {                                                                       \
    if (!(p)) {                                                              \
      log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
      goto error;                                                            \
    }                                                                        \
  } while (0)

#define goto_error_if_fail_ex(p, sentence)                                   \
  do {                                                                       \
    if (!(p)) {                                                              \
      log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
      sentence;                                                              \
      goto error;                                                            \
    }                                                                        \
  } while (0)

#define break_if_fail(p)                                                     \
  {                                                                          \
    if (!(p)) {                                                              \
      log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
      break;                                                                 \
    }                                                                        \
  }

#define break_if_fail_ex(p, sentence)                                        \
  {                                                                          \
    if (!(p)) {                                                              \
      log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
      sentence;                                                              \
      break;                                                                 \
    }                                                                        \
  }

#define return_if_fail(p)                                                    \
  do {                                                                       \
    if (!(p)) {                                                              \
      log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
      return;                                                                \
    }                                                                        \
  } while (0)

#define return_value_if_fail(p, value)                                       \
  do {                                                                       \
    if (!(p)) {                                                              \
      log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
      return (value);                                                        \
    }                                                                        \
  } while (0)

#define return_ret_if_fail(p)                                        \
  do {                                                               \
    ret_t TK_CONCAT(__ret_, __LINE__) = p;                           \
    if (TK_CONCAT(__ret_, __LINE__) != RET_OK) {                     \
      log_warn("%s:%d " #p " != RET_OK!\n", __FUNCTION__, __LINE__); \
      return TK_CONCAT(__ret_, __LINE__);                            \
    }                                                                \
  } while (0)
#endif

#define return_value_if_equal(p, value) \
  do {                                  \
    if ((p) == value) {                 \
      return (value);                   \
    }                                   \
  } while (0)

#define tk_min(a, b) ((a) < (b) ? (a) : (b))
#define tk_abs(a) ((a) < (0) ? (-(a)) : (a))
#define tk_max(a, b) ((a) > (b) ? (a) : (b))
#define tk_roundi(a) (int32_t)(((a) >= 0) ? ((a) + 0.5f) : ((a) - 0.5f))
#define tk_roundi64(a) (int64_t)(((a) >= 0) ? ((a) + 0.5f) : ((a) - 0.5f))
#define tk_clamp(a, mn, mx) ((a) < (mn) ? (mn) : ((a) > (mx) ? (mx) : (a)))
#define tk_clampi(a, mn, mx) (int32_t)((a) < (mn) ? (mn) : ((a) > (mx) ? (mx) : (a)))
#define tk_swap(a, b, type)               \
  do {                                    \
    type TK_CONCAT(_tEMP_, __LINE__) = a; \
    a = b;                                \
    b = TK_CONCAT(_tEMP_, __LINE__);      \
  } while (0)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif /*ARRAY_SIZE*/

typedef void* (*tk_create_t)(void);
typedef ret_t (*tk_destroy_t)(void* data);
typedef ret_t (*tk_on_done_t)(void* data);
typedef ret_t (*tk_on_result_t)(void* ctx, const void* data);
typedef bool_t (*tk_is_valid_t)(void* data);

/*TRUE 保留，FALSE 忽略*/
typedef bool_t (*tk_filter_t)(void* ctx, const void* data);
typedef int (*tk_compare_t)(const void* a, const void* b);
typedef ret_t (*tk_visit_t)(void* ctx, const void* data);
typedef uint32_t (*tk_hash_t)(const void* data);
typedef ret_t (*tk_callback_t)(void* ctx);

/*TK_NAME_LEN+1 must aligned to 4*/
enum { TK_NAME_LEN = 31, TK_FUNC_NAME_LEN = 63 };

#ifdef WITH_CPPCHECK
int __cppcheck__strcmp(const char* s1, const char* s2);
int __cppcheck__strcasecmp(const char* s1, const char* s2);
int __cppcheck__strncmp(const char* s1, const char* s2, size_t n);
#define tk_str_eq(s1, s2) (__cppcheck__strcmp((s1), (s2)) == 0)
#define tk_str_ieq(s1, s2) (__cppcheck__strcasecmp((s1), (s2)) == 0)
#define tk_str_eq_with_len(s1, s2, len) (__cppcheck__strncmp((s1), (s2), len) == 0)
#else
#define tk_str_eq(s1, s2)                                                           \
  (((s1) == NULL && (s2) == NULL) ||                                                \
   (((s1) != NULL) && ((s2) != NULL) && *((const char*)s1) == *((const char*)s2) && \
    tk_strcmp((s1), (s2)) == 0))
#define tk_str_eq_with_len(s1, s2, len)                                            \
  (((s1) != NULL) && ((s2) != NULL) && *((const char*)s1) == *((const char*)s2) && \
   tk_strncmp((s1), (s2), len) == 0)

#define tk_str_ieq(s1, s2)           \
  (((s1) == NULL && (s2) == NULL) || \
   (((s1) != NULL) && ((s2) != NULL) && tk_stricmp((s1), (s2)) == 0))
#define tk_str_ieq_with_len(s1, s2, len) \
  (((s1) != NULL) && ((s2) != NULL) && tk_strnicmp((s1), (s2), len) == 0)

#define tk_wstr_eq(s1, s2)                                                                \
  (((s1) == NULL && (s2) == NULL) ||                                                      \
   (((s1) != NULL) && ((s2) != NULL) && *((const wchar_t*)s1) == *((const wchar_t*)s2) && \
    tk_wstrcmp((s1), (s2)) == 0))
#define tk_wstr_ieq(s1, s2)          \
  (((s1) == NULL && (s2) == NULL) || \
   (((s1) != NULL) && ((s2) != NULL) && tk_wstricmp((s1), (s2)) == 0))
#endif /*WITH_CPPCHECK*/

#define tk_fequal(f1, f2) (fabs((f1) - (f2)) < 0.0001)
#define tk_lfequal(f1, f2) (fabs((f1) - (f2)) < 0.000001)

#ifndef M_PI
#define M_PI 3.1415926f
#endif /*M_PI*/

#ifndef M_E
#define M_E 2.71828f
#endif /*M_E*/

#ifndef M_SQRT2
#define M_SQRT2 1.4142135f
#endif /*M_SQRT2*/

#define TK_INVALID_ID 0u
#define TK_NUM_MAX_LEN 31u
#define TK_UINT32_MAX 0xffffffffu
#define TK_LOCALE_MAGIC "$locale$"
#define TK_OBJECT_PROP_NAME_MAX_LEN 64u

#define TK_D2R(d) (((d) * M_PI) / 180)
#define TK_R2D(r) (((r) * 180) / M_PI)

#ifdef _MSC_VER
#define TK_CONST_DATA_ALIGN(v) __declspec(align(8)) v
#else
#define TK_CONST_DATA_ALIGN(v) v __attribute__((aligned(8)))
#endif /*_MSC_VER*/

typedef uint64_t (*tk_get_time_t)(void);
typedef uint64_t (*tk_get_time_ms_t)(void);
typedef void (*tk_sleep_ms_t)(uint32_t ms);

#if defined(WIN32) && !defined(NDEBUG) && defined(_CONSOLE)
#define TK_ENABLE_CONSOLE()                   \
  do {                                        \
    AllocConsole();                           \
    FILE* fp = NULL;                          \
    freopen_s(&fp, "CONOUT$", "w+t", stdout); \
    system("chcp 65001");                     \
  } while (0)
#else
#define TK_ENABLE_CONSOLE() \
  do {                      \
  } while (0)
#endif /*WIN32 && !NDEBUG*/

struct _event_source_t;
typedef struct _event_source_t event_source_t;

struct _event_source_manager_t;
typedef struct _event_source_manager_t event_source_manager_t;

#ifndef EAGAIN
#define EAGAIN 11
#endif /*EAGAIN*/

#define TK_SET_NULL(p) (p) = NULL
#define TK_ROUND_TO4(size) ((((size) + 3) >> 2) << 2)
#define TK_ROUND_TO8(size) ((((size) + 7) >> 3) << 3)
#define TK_ROUND_TO_MACH(size) ((sizeof(void*) == 4) ? TK_ROUND_TO4(size) : TK_ROUND_TO8(size))
#define TK_ROUND_TO(size, round_size) ((((size) + (round_size) - 1) / (round_size)) * (round_size))

#define TK_SET_BIT(v, n) ((v) |= 1UL << (n))
#define TK_CLEAR_BIT(v, n) ((v) &= ~(1UL << (n)))
#define TK_TOGGLE_BIT(v, n) ((v) ^= (1UL << (n)))
#define TK_TEST_BIT(v, n) (((v) >> (n)) & 1U)
#define TK_SET_OR_CLEAR_BIT(v, n, b) ((b) ? TK_SET_BIT(v, n) : TK_CLEAR_BIT(v, n))

#ifndef TK_DEFAULT_WAIT_TIME
#define TK_DEFAULT_WAIT_TIME 16
#endif /*TK_DEFAULT_WAIT_TIME*/

/*from cairo/cairo/cairoint.h*/
#if _XOPEN_SOURCE >= 600 || defined(_ISOC99_SOURCE)
#define TK_ISFINITE(x) isfinite(x)
#else
#define TK_ISFINITE(x) ((x) * (x) >= 0.) /* check for NaNs */
#endif

#define tk_isspace(c) ((0 <= (int)(c)) && ((int)(c) < 128) && isspace(c))
#define tk_isdigit(c) ((int)(c) >= '0' && (int)(c) <= '9')
#define tk_isxdigit(c) ((0 <= (int)(c)) && ((int)(c) < 128) && isxdigit(c))
#define tk_isprint(c) ((0 <= (int)(c)) && ((int)(c) < 128) && isprint(c))
#define tk_isalpha(c) ((0 <= (int)(c)) && ((int)(c) < 128) && isalpha(c))
#define tk_islower(c) ((0 <= (int)(c)) && ((int)(c) < 128) && islower(c))
#define tk_isupper(c) ((0 <= (int)(c)) && ((int)(c) < 128) && isupper(c))
#define tk_isalnum(c) ((0 <= (int)(c)) && ((int)(c) < 128) && isalnum(c))

#define STR_SCHEMA_TCP "tcp://"
#define STR_SCHEMA_UDP "udp://"
#define STR_SCHEMA_SERIAL "serial://"
#define STR_SCHEMA_FILE "file://"
#define STR_SCHEMA_FTP "ftp://"
#define STR_SCHEMA_HTTP "http://"
#define STR_SCHEMA_HTTPS "https://"
#define STR_SCHEMA_MEM "mem://"

#define TK_STR_IS_EMPTY(s) (s == NULL || *s == '\0')
#define TK_STR_IS_NOT_EMPTY(s) ((s != NULL) && *s)

#ifdef WIN32
#define popen _popen
#define pclose _pclose
#endif /*WIN32*/

#define TK_VALUE_UNDEFINED "undefined"

#define TK_ADR_EQ(p1, p2) (((uint8_t*)(p1)) - ((uint8_t*)(p2)) == 0)

typedef struct _key_type_value_t {
  char* name;
  uint32_t type;
  uint32_t value;
} key_type_value_t;

#define TK_FOREACH_VISIT_RESULT_PROCESSING(result, remove_sentence)         \
  {                                                                         \
    if (RET_CONTINUE == result) {                                           \
      result = RET_OK;                                                      \
    } else if (RET_SKIP == result) {                                        \
      /* 如果在树形结构中，需要自行实现跳过遍历子结点的逻辑。 */            \
      /* 如 tree_foreach、widget_foreach、fs_foreach。 */                   \
      result = RET_OK;                                                      \
    } else if (RET_REMOVE == result) {                                      \
      remove_sentence;                                                      \
      result = RET_OK;                                                      \
      continue;                                                             \
    } else if (RET_REPEAT == result) {                                      \
      log_warn("%s: result type REPEAT is not supported!\n", __FUNCTION__); \
      result = RET_OK;                                                      \
    } else if (result != RET_OK) {                                          \
      break;                                                                \
    }                                                                       \
  }

#endif /*TYPES_DEF_H*/
