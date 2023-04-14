/**
 * File:   log.h
 * Author: AWTK Develop Team
 * Brief:  log functions
 *
 * Copyright (c) 2019 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-11-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LOG_H
#define TK_LOG_H

BEGIN_C_DECLS

/**
 * @enum log_level_t
 * @prefix LOG_LEVEL_
 * LOG的级别。 
 */
typedef enum _log_level_t {
  /**
   * @const LOG_LEVEL_DEBUG
   * DEBUG
   */
  LOG_LEVEL_DEBUG = 0,
  /**
   * @const LOG_LEVEL_INFO
   * INFO 
   */
  LOG_LEVEL_INFO,
  /**
   * @const LOG_LEVEL_WARN
   * WARN
   */
  LOG_LEVEL_WARN,
  /**
   * @const LOG_LEVEL_ERROR
   * ERROR 
   */
  LOG_LEVEL_ERROR
} log_level_t;

/**
 * @class log_t
 * @annotation ["fake"]
 * log。
 *
 */

/**
 * @method log_get_log_level
 *
 * 获取log的级别。
 *
 * @return {log_level_t} 返回log的级别。
 */
log_level_t log_get_log_level(void);

/**
 * @method log_set_log_level
 *
 * 设置log的级别。
 *
 * @param {log_level_t} log_level log的级别。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t log_set_log_level(log_level_t log_level);

int32_t log_dummy(const char* fmt, ...);

/**
 * @method log_debug
 * @export none
 *
 * 输出调试信息。
 * > 变参函数。
 *
 * ```
 * log_debug("debug message\n");
 * ```
 * @param {const char*} format 格式或信息。
 * @return {void} 无返回值。
 */

/**
 * @method log_info
 * @export none
 *
 * 输出参考信息。
 * > 变参函数。
 *
 * ```
 * log_info("info message\n");
 * ```
 * @param {const char*} format 格式或信息。
 *
 * @return {void} 无返回值。
 */

/**
 * @method log_warn
 * @export none
 *
 * 输出警告信息。
 * > 变参函数。
 *
 * ```
 * log_warn("warn message\n");
 * ```
 * @param {const char*} format 格式或信息。
 *
 * @return {void} 无返回值。
 */

/**
 * @method log_error
 * @export none
 *
 * 输出错误信息。
 * > 变参函数。
 *
 * ```
 * log_error("error message\n");
 * ```
 * @param {const char*} format 格式或信息。
 *
 * @return {void} 无返回值。
 */

/**
 * @method log_notify_debugger
 *
 * 用于debugger拦截日志，发送给客户端。
 * > 变参函数。
 *
 * @param {const char*} format 格式或信息。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t log_notify_debugger(const char* format, ...);

typedef ret_t (*tk_debugger_log_t)(void* ctx, const char* msg);

/**
 * @method log_set_debugger_hook
 *
 * 设置调试器的钩子函数。
 *
 * @param {tk_debugger_log_t} log 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t log_set_debugger_hook(tk_debugger_log_t log, void* ctx);

#ifdef ANDROID
#include "android/log.h"
#define log_debug(...)                                           \
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) {                  \
    log_notify_debugger("AWTK", __VA_ARGS__);                    \
    __android_log_print(ANDROID_LOG_DEBUG, "AWTK", __VA_ARGS__); \
  }
#define log_info(...)                                           \
  if (log_get_log_level() <= LOG_LEVEL_INFO) {                  \
    log_notify_debugger("AWTK", __VA_ARGS__);                   \
    __android_log_print(ANDROID_LOG_INFO, "AWTK", __VA_ARGS__); \
  }
#define log_warn(...)                                           \
  if (log_get_log_level() <= LOG_LEVEL_WARN) {                  \
    log_notify_debugger("AWTK", __VA_ARGS__);                   \
    __android_log_print(ANDROID_LOG_WARN, "AWTK", __VA_ARGS__); \
  }
#define log_error(...)                                           \
  if (log_get_log_level() <= LOG_LEVEL_ERROR) {                  \
    log_notify_debugger("AWTK", __VA_ARGS__);                    \
    __android_log_print(ANDROID_LOG_ERROR, "AWTK", __VA_ARGS__); \
  }
#elif defined(IOS)
void awtk_ios_log(const char* message, ...);
#define log_debug(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) { \
    log_notify_debugger(format, ##args);        \
    awtk_ios_log(format, ##args);               \
  }
#define log_info(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_INFO) { \
    log_notify_debugger(format, ##args);       \
    awtk_ios_log(format, ##args);              \
  }
#define log_warn(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_WARN) { \
    log_notify_debugger(format, ##args);       \
    awtk_ios_log(format, ##args);              \
  }
#define log_error(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_ERROR) { \
    log_notify_debugger(format, ##args);        \
    awtk_ios_log(format, ##args);               \
  }
#elif defined(WIN32)
#include <windows.h>
#if defined(__GNUC__)
/*MINGW*/
#define log_debug(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) { \
    log_notify_debugger(format, ##args);        \
    printf(format, ##args);                     \
  }
#define log_info(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_INFO) { \
    log_notify_debugger(format, ##args);       \
    printf(format, ##args);                    \
  }
#define log_warn(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_WARN) { \
    log_notify_debugger(format, ##args);       \
    printf(format, ##args);                    \
  }
#define log_error(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_ERROR) { \
    log_notify_debugger(format, ##args);        \
    printf(format, ##args);                     \
  }
#else
/*MSVC*/
#define log_debug(format, ...)                  \
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) { \
    log_notify_debugger(format, __VA_ARGS__);   \
    printf(format, __VA_ARGS__);                \
    fflush(stdout);                             \
  }
#define log_info(format, ...)                  \
  if (log_get_log_level() <= LOG_LEVEL_INFO) { \
    log_notify_debugger(format, __VA_ARGS__);  \
    printf(format, __VA_ARGS__);               \
    fflush(stdout);                            \
  }
#define log_warn(format, ...)                  \
  if (log_get_log_level() <= LOG_LEVEL_WARN) { \
    log_notify_debugger(format, __VA_ARGS__);  \
    printf(format, __VA_ARGS__);               \
    fflush(stdout);                            \
  }
#define log_error(format, ...)                  \
  if (log_get_log_level() <= LOG_LEVEL_ERROR) { \
    log_notify_debugger(format, __VA_ARGS__);   \
    printf(format, __VA_ARGS__);                \
    fflush(stdout);                             \
  }
#endif
#elif defined(HAS_STDIO) || defined(AWTK_WEB)
#include <stdio.h>
#define log_debug(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) { \
    log_notify_debugger(format, ##args);        \
    printf(format, ##args);                     \
    fflush(stdout);                             \
  }
#define log_info(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_INFO) { \
    log_notify_debugger(format, ##args);       \
    printf(format, ##args);                    \
    fflush(stdout);                            \
  }
#define log_warn(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_WARN) { \
    log_notify_debugger(format, ##args);       \
    printf(format, ##args);                    \
    fflush(stdout);                            \
  }
#define log_error(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_ERROR) { \
    log_notify_debugger(format, ##args);        \
    printf(format, ##args);                     \
    fflush(stdout);                             \
  }
#elif defined(log_impl)
#define log_debug(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) { \
    log_notify_debugger(format, ##args);        \
    log_impl(format, ##args);                   \
  }
#define log_info(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_INFO) { \
    log_notify_debugger(format, ##args);       \
    log_impl(format, ##args);                  \
  }
#define log_warn(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_WARN) { \
    log_notify_debugger(format, ##args);       \
    log_impl(format, ##args);                  \
  }
#define log_error(format, args...)              \
  if (log_get_log_level() <= LOG_LEVEL_ERROR) { \
    log_notify_debugger(format, ##args);        \
    log_impl(format, ##args);                   \
  }
#else
#define log_debug(format, args...)
#define log_info(format, args...)
#define log_warn(format, args...)
#define log_error(format, args...)
#endif

END_C_DECLS

#endif /*TK_LOG_H*/
