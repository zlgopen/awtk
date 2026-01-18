/**
 * File:   conf_utils.h
 * Author: AWTK Develop Team
 * Brief:  utils struct and utils functions.
 *
 * Copyright (c) 2025 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * @annotation ["fake", "scriptable"]
 * 工具类。
 *
 */

/**
 * @method object_load_conf
 * 加载配置文件到对象中。
 * @annotation ["static", "scriptable"]
 *
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} url 配置文件路径。
 * @param {const char*} type 配置文件类型, 如果为NULL，则自动检测。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_load_conf(tk_object_t* obj, const char* url, const char* type);

/**
 * @method object_to_json
 * 将对象转换成json字符串。
 * @annotation ["static"]
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
 * @annotation ["static"]
 *
 * @param {tk_object_t*} obj object对象, 用于返回结果。
 * @param {const char*} str json字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_from_json(tk_object_t* obj, const char* str);

END_C_DECLS

#endif /*TK_CONF_UTILS_H*/
