/**
 * File:   app_conf.h
 * Author: AWTK Develop Team
 * Brief:  config for application
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-05-31 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_APP_CONF_H
#define TK_APP_CONF_H

#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @class app_conf_t
 * @annotation ["fake", "scriptable"]
 * 
 * 应用程序的配置信息。
 *
 * 底层实现可以是任何格式，比如INI，XML，JSON和UBJSON。
 *
 * 对于树状的文档，key可以是多级的，用.分隔。如network.ip。
 * 
 */

/**
 * @method app_conf_set_instance
 * 设置app_conf实例。
 *
 * @annotation ["static"]
 * 
 * @param {object_t*} 配置对象。由app_conf_deinit释放。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_set_instance(object_t* obj);

/**
 * @method app_conf_get_instance
 * 获取app_conf实例。
 *
 * @annotation ["static"]
 * 
 * @return {object_t*} 返回app_conf实例。
 */
object_t* app_conf_get_instance(void);

/**
 * @method app_conf_save
 * 持久保存配置。
 *
 * @annotation ["static", "scriptable"]
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_save(void);

/**
 * @method app_conf_reload
 * 重新加载配置(内存中的配置丢失)。
 *
 * @annotation ["static", "scriptable"]
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_reload(void);

/**
 * @method app_conf_on_changed
 * 注册配置变化事件。
 *
 * @annotation ["static"]
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数上下文。
 * @return {uint32_t} 返回id，用于app_conf_off_changed。
 */
uint32_t app_conf_on_changed(event_func_t on_event, void* ctx);

/**
 * @method app_conf_off_changed
 * 注销配置变化事件。
 *
 * @annotation ["static"]
 * @param {uint32_t} id app_conf_on_changed返回的ID。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_off_changed(uint32_t id);

/**
 * @method app_conf_off_changed_by_ctx
 * 注销配置变化事件。
 *
 * @annotation ["static"]
 * @param {void*} ctx 事件处理函数上下文。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_off_changed_by_ctx(void* ctx);

/**
 * @method app_conf_deinit
 * 释放conf对象。
 *
 * @annotation ["static", "scriptable"]
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_deinit(void);

/**
 * @method app_conf_set
 * 设置配置项的值。
 *
 * @annotation ["static"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {const value_t*} v 配置项的值。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_set(const char* key, const value_t* v);

/**
 * @method app_conf_get
 * 获取配置项的值。
 *
 * @annotation ["static"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {value_t*} v 配置项的值(用于返回)。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_get(const char* key, value_t* v);

/**
 * @method app_conf_exist
 * 检查配置项是否存在。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * 
 * @return {bool_t} 返回TRUE表示存在，FALSE表示不存在。
 */
bool_t app_conf_exist(const char* key);

/**
 * @method app_conf_set_int
 * 设置整数类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {int32_t} v 配置项的值。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_set_int(const char* key, int32_t v);

/**
 * @method app_conf_set_int64
 * 设置64位整数类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {int64_t} v 配置项的值。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_set_int64(const char* key, int64_t v);

/**
 * @method app_conf_set_bool
 * 设置bool类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {bool_t} v 配置项的值。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_set_bool(const char* key, bool_t v);

/**
 * @method app_conf_set_double
 * 设置双精度类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {double} v 配置项的值。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_set_double(const char* key, double v);

/**
 * @method app_conf_set_str
 * 设置字符串类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {const char*} v 配置项的值。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_set_str(const char* key, const char* v);

/**
 * @method app_conf_set_wstr
 * 设置宽字符串类型配置项的值。
 *
 * @annotation ["static"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {const wchar_t*} v 配置项的值。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_set_wstr(const char* key, const wchar_t* v);

/**
 * @method app_conf_get_int
 * 获取整数类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {int32_t} defval 缺省值。
 * 
 * @return {int32_t} 返回配置项的值（如果不存在返回缺省值）。
 */
int32_t app_conf_get_int(const char* key, int32_t defval);

/**
 * @method app_conf_get_int64
 * 获取64位整数类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {int64_t} defval 缺省值。
 * 
 * @return {int64_t} 返回配置项的值（如果不存在返回缺省值）。
 */
int64_t app_conf_get_int64(const char* key, int64_t defval);

/**
 * @method app_conf_get_bool
 * 获取bool类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {bool_t} defval 缺省值。
 * 
 * @return {bool_t} 返回配置项的值（如果不存在返回缺省值）。
 */
bool_t app_conf_get_bool(const char* key, bool_t defval);

/**
 * @method app_conf_get_double
 * 获取单精度浮点数类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {double} defval 缺省值。
 * 
 * @return {double} 返回配置项的值（如果不存在返回缺省值）。
 */
double app_conf_get_double(const char* key, double defval);

/**
 * @method app_conf_get_str
 * 获取字符串类型配置项的值。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {const char*} defval 缺省值。
 * 
 * @return {const char*} 返回配置项的值（如果不存在返回缺省值）。
 */
const char* app_conf_get_str(const char* key, const char* defval);

/**
 * @method app_conf_get_wstr
 * 获取宽字符串类型配置项的值。
 *
 * @annotation ["static"]
 *
 * @param {const char*} key 配置项的名称。
 * @param {const wchar_t*} str 返回值。
 * @param {uint32_t} max_size 最大长度(字符数)。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_get_wstr(const char* key, wchar_t* str, uint32_t max_size);

/**
 * @method app_conf_remove
 * 删除配置项。
 *
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} key 配置项的名称。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_remove(const char* key);

END_C_DECLS

#endif /*TK_APP_CONF_H*/
