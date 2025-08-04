/**
 * File:   conf_utils.h
 * Author: AWTK Develop Team
 * Brief:  utils struct and utils functions.
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-08-04 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_CONF_UTILS_H
#define TK_CONF_UTILS_H

#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @class conf_utils_t
 * @annotation ["fake"]
 * 工具类。
 *
 */

/**
 * @method object_to_json
 * 将对象转换成json字符串。
 *
 * @param {tk_object_t*} obj object对象。
 * @param {str_t*} str 用于返回结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_to_json(tk_object_t* obj, str_t* str);

/**
 * @method object_from_json
 * 将json字符串转换成对象。
 *
 * @param {tk_object_t*} obj object对象, 用于返回结果。
 * @param {const char*} str json字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_from_json(tk_object_t* obj, const char* str);

END_C_DECLS

#endif /*TK_CONF_UTILS_H*/
