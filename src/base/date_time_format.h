/**
 * File:   date_time_format.h
 * Author: AWTK Develop Team
 * Brief:  date_time_format
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-01-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DATE_TIME_FORMAT_H
#define TK_DATE_TIME_FORMAT_H

#include "tkc/str.h"
#include "tkc/wstr.h"
#include "tkc/types_def.h"
#include "tkc/date_time.h"

BEGIN_C_DECLS

/**
 * @class date_time_format_t
 * @annotation ["fake"]
 * 格式化日期时间。
 *
 * 格式规则：
 * * Y 代表年(完整显示)
 * * M 代表月(1-12)
 * * D 代表日(1-31)
 * * h 代表时(0-23)
 * * m 代表分(0-59)
 * * s 代表秒(0-59)
 * * w 代表星期(0-6)
 * * W 代表星期的英文缩写(支持翻译)
 * * YY 代表年(只显示末两位)
 * * MM 代表月(01-12)
 * * DD 代表日(01-31)
 * * hh 代表时(00-23)
 * * mm 代表分(00-59)
 * * ss 代表秒(00-59)
 * * MMM 代表月的英文缩写(支持翻译)
 *
 * 如 日期时间为：2018/11/12 9:10:20
 * * "Y/M/D"显示为"2018/11/12"
 * * "Y-M-D"显示为"2018-11-12"
 * * "Y-M-D h:m:s"显示为"2018-11-12 9:10:20"
 * * "Y-M-D hh:mm:ss"显示为"2018-11-12 09:10:20"
 */

/**
 * @method wstr_format_time
 * 格式化time。
 * @param {wstr_t*} str 输出字符串。
 * @param {const char*} format 格式。
 * @param {uint64_t} time 时间。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wstr_format_time(wstr_t* str, const char* format, uint64_t time);

/**
 * @method wstr_format_date_time
 * 格式化date_time。
 * @param {wstr_t*} str 输出字符串。
 * @param {const char*} format 格式。
 * @param {date_time_t*} dt 时间。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t wstr_format_date_time(wstr_t* str, const char* format, const date_time_t* dt);

END_C_DECLS

#endif /*TK_DATE_TIME_FORMAT_H*/
