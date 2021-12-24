/**
 * File:   object_date_time.h
 * Author: AWTK Develop Team
 * Brief:  wrap date time to an object.
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-01-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OBJECT_DATE_TIME_H
#define TK_OBJECT_DATE_TIME_H

#include "tkc/object.h"
#include "tkc/date_time.h"

BEGIN_C_DECLS

/**
 * @class object_date_time_t
 * @parent tk_object_t
 *
 * 将date_time包装成object。
 *
 */
typedef struct _object_date_time_t {
  tk_object_t object;

  /**
   * @property {date_time_t*} dt
   * @annotation ["readable"]
   * date time对象。
   *
   */
  date_time_t* dt;
} object_date_time_t;

/**
 * @method object_date_time_create
 *
 * 创建对象。
 *
 * @annotation ["constructor"]
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_date_time_create(void);

/**
 * @method object_date_time_cast
 * 转换为object_date_time对象。
 * @annotation ["cast"]
 * @param {tk_object_t*} obj object_date_time对象。
 *
 * @return {object_date_time_t*} object_date_time对象。
 */
object_date_time_t* object_date_time_cast(tk_object_t* obj);
#define OBJECT_DATE_TIME(obj) object_date_time_cast(obj)

END_C_DECLS

#endif /*TK_OBJECT_DATE_TIME_H*/
