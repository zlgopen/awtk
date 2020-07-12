/**
 * File:   date_time.h
 * Author: AWTK Develop Team
 * Brief:  date time
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
 * 2018-11-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DATE_TIME_H
#define TK_DATE_TIME_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class date_time_t
 * @annotation ["scriptable"]
 * 日期时间。
 *
 *> 在嵌入式平台中，在系统初始时，需要调用date\_time\_global\_init设置实际获取/设置系统时间的函数。
 *
 */
typedef struct _date_time_t {
  /**
   * @property {int32_t} second
   * @annotation ["readable", "scriptable"]
   * 秒(0 - 59)。
   */
  int32_t second;
  /**
   * @property {int32_t} minute
   * @annotation ["readable", "scriptable"]
   * 分(0 - 59)。
   */
  int32_t minute;
  /**
   * @property {int32_t} hour
   * @annotation ["readable", "scriptable"]
   * 时(0 - 23)。
   */
  int32_t hour; /* hours (0 - 23) */
  /**
   * @property {int32_t} day
   * @annotation ["readable", "scriptable"]
   * 日(1-31)。
   */
  int32_t day;
  /**
   * @property {int32_t} wday
   * @annotation ["readable", "scriptable"]
   * 星期几(0-6, Sunday = 0)。
   */
  int32_t wday;
  /**
   * @property {int32_t} month
   * @annotation ["readable", "scriptable"]
   * 月(1-12)。
   */
  int32_t month;
  /**
   * @property {int32_t} year
   * @annotation ["readable", "scriptable"]
   * 年。
   */
  int32_t year;
} date_time_t;

/**
 * @method date_time_create
 * 创建date_time对象，并初始为当前日期和时间(一般供脚本语言中使用)。
 * @annotation ["constructor", "scriptable", "gc"]
 *
 * @return {date_time_t*} 返回date_time对象。
 */
date_time_t* date_time_create(void);

/**
 * @method date_time_init
 * 初始为当前日期和时间。
 * @param {date_time_t*} dt date_time对象。
 *
 * @return {date_time_t*} 返回date_time对象。
 */
date_time_t* date_time_init(date_time_t* dt);

/**
 * @method date_time_set
 * 设置当前时间。
 *
 * @annotation ["scriptable"]
 * @param {date_time_t*} dt date_time对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t date_time_set(date_time_t* dt);

/**
 * @method date_time_from_time
 * 从time转换而来。
 *
 * @annotation ["scriptable"]
 * @param {date_time_t*} dt date_time对象。
 * @param {uint64_t} time 时间。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t date_time_from_time(date_time_t* dt, uint64_t time);

/**
 * @method date_time_is_leap
 * 是否是闰年。
 *
 * @annotation ["scriptable", "static"]
 * @param {uint32_t} year 年份。
 *
 * @return {bool_t} 返回TRUE表示是，否则表示否。
 */
bool_t date_time_is_leap(uint32_t year);

/**
 * @method date_time_get_days
 * 获取指定年份月份的天数。
 *
 * @annotation ["scriptable", "static"]
 * @param {uint32_t} year 年份。
 * @param {uint32_t} montn 月份(1-12)。
 *
 * @return {int32_t} 返回大于0表示天数，否则表示失败。
 */
int32_t date_time_get_days(uint32_t year, uint32_t month);

/**
 * @method date_time_get_wday
 * 获取指定日期是周几(0-6, Sunday = 0)。。
 *
 * @annotation ["scriptable", "static"]
 * @param {uint32_t} year 年份。
 * @param {uint32_t} montn 月份(1-12)。
 * @param {uint32_t} day 日(1-31)。
 *
 * @return {int32_t} 返回大于等于0表示周几(0-6)，否则表示失败。
 */
int32_t date_time_get_wday(uint32_t year, uint32_t month, uint32_t day);

/**
 * @method date_time_get_month_name
 * 获取指定月份的英文名称(简写)。
 *
 * @annotation ["scriptable", "static"]
 * @param {uint32_t} montn 月份(1-12)。
 *
 * @return {const char*} 返回指定月份的英文名称(简写)。
 */
const char* date_time_get_month_name(uint32_t month);

/**
 * @method date_time_get_wday_name
 * 获取周几的英文名称(简写)。
 *
 * @annotation ["scriptable", "static"]
 * @param {uint32_t} wday 星期几(0-6, Sunday = 0)。
 *
 * @return {const char*} 返回指定周几的英文名称(简写)。
 */
const char* date_time_get_wday_name(uint32_t wday);

/**
 * @method date_time_destroy
 * 销毁date_time对象(一般供脚本语言中使用)。
 *
 * @annotation ["deconstructor", "scriptable", "gc"]
 * @param {date_time_t*} dt date_time对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t date_time_destroy(date_time_t* dt);

typedef ret_t (*date_time_get_now_t)(date_time_t* dt);
typedef ret_t (*date_time_set_now_t)(date_time_t* dt);
typedef ret_t (*date_time_from_time_t)(date_time_t* dt, uint64_t time);

typedef struct _date_time_vtable_t {
  date_time_get_now_t get_now;
  date_time_set_now_t set_now;
  date_time_from_time_t from_time;
} date_time_vtable_t;

/**
 * @method date_time_global_init_ex
 * 时间日期全局初始化。
 *
 * > 嵌入式平台需要提供并设置获取当前日期和时间的函数，否则相关的功能(如时钟控件)将无法正常工作。
 *
 * @param {date_time_vtable_t} vt 日期和时间的相关函数的实现。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t date_time_global_init_ex(const date_time_vtable_t* vt);

/*deprecated*/
ret_t date_time_set_impl(date_time_get_now_t date_time_get_now);
ret_t date_time_global_init(date_time_get_now_t get, date_time_set_now_t set);
END_C_DECLS

#endif /*TK_DATE_TIME_H*/
