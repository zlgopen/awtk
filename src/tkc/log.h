/**
 * File:   log.h
 * Author: AWTK Develop Team
 * Brief:  log functions
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifdef ANDROID
#include "android/log.h"
#define log_debug(...)                        \
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) \
  __android_log_print(ANDROID_LOG_DEBUG, "AWTK", __VA_ARGS__)
#define log_info(...)                        \
  if (log_get_log_level() <= LOG_LEVEL_INFO) \
  __android_log_print(ANDROID_LOG_INFO, "AWTK", __VA_ARGS__)
#define log_warn(...)                        \
  if (log_get_log_level() <= LOG_LEVEL_WARN) \
  __android_log_print(ANDROID_LOG_WARN, "AWTK", __VA_ARGS__)
#define log_error(...)                        \
  if (log_get_log_level() <= LOG_LEVEL_ERROR) \
  __android_log_print(ANDROID_LOG_ERROR, "AWTK", __VA_ARGS__)
#elif defined(IOS)
void awtk_ios_log(const char* message, ...);
#define log_debug(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) awtk_ios_log(format, ##args)
#define log_info(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_INFO) awtk_ios_log(format, ##args)
#define log_warn(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_WARN) awtk_ios_log(format, ##args)
#define log_error(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_ERROR) awtk_ios_log(format, ##args)
#elif defined(WIN32)
#include <windows.h>
#if defined(__GNUC__)
/*MINGW*/
#define log_debug(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) printf(format, ##args)
#define log_info(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_INFO) printf(format, ##args)
#define log_warn(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_WARN) printf(format, ##args)
#define log_error(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_ERROR) printf(format, ##args)
#else
/*MSVC*/
#define log_debug(format, ...) \
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) printf(format, __VA_ARGS__)
#define log_info(format, ...) \
  if (log_get_log_level() <= LOG_LEVEL_INFO) printf(format, __VA_ARGS__)
#define log_warn(format, ...) \
  if (log_get_log_level() <= LOG_LEVEL_WARN) printf(format, __VA_ARGS__)
#define log_error(format, ...) \
  if (log_get_log_level() <= LOG_LEVEL_ERROR) printf(format, __VA_ARGS__)
#endif
#elif defined(HAS_STDIO) || defined(AWTK_WEB)
#include <stdio.h>
#define log_debug(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) printf(format, ##args)
#define log_info(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_INFO) printf(format, ##args)
#define log_warn(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_WARN) printf(format, ##args)
#define log_error(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_ERROR) printf(format, ##args)
#else
#define log_debug(format, args...)
#define log_info(format, args...)
#define log_warn(format, args...)
#define log_error(format, args...)
#endif

END_C_DECLS

#endif /*TK_LOG_H*/
