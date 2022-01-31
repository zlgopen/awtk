/**
 * File:   plugin_manager.h
 * Author: AWTK Develop Team
 * Brief:  plugin manager
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
 * 2020-10-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_PLUGIN_MANAGER_H
#define TK_PLUGIN_MANAGER_H

#include "tkc/dl.h"
#include "tkc/darray.h"

typedef ret_t (*plugin_get_init_func_name_t)(char func_name[TK_NAME_LEN + 1], const char* name);
typedef ret_t (*plugin_get_deinit_func_name_t)(char func_name[TK_NAME_LEN + 1], const char* name);
/**
 * @class plugin_manager_t
 * 插件管理器。
 */
typedef struct _plugin_manager_t {
  /*private*/
  char* path;
  darray_t plugins;
  plugin_get_init_func_name_t get_init;
  plugin_get_deinit_func_name_t get_deinit;
} plugin_manager_t;

BEGIN_C_DECLS

/**
 * @method plugin_manager_create
 * 创建插件管理器。
 *
 * @param {const char*}  path 插件所在的目录。
 * @param {plugin_get_init_func_name_t} get_init 获取初始化函数的函数名。为NULL时使用"init"。
 * @param {plugin_get_deinit_func_name_t} get_deinit 获取~初始化函数的函数名。为NULL时使用"deinit"。
 *
 * @return {plugin_manager_t*} 动态链接库对象。
 */
plugin_manager_t* plugin_manager_create(const char* path, plugin_get_init_func_name_t get_init,
                                        plugin_get_deinit_func_name_t get_deinit);

/**
 * @method plugin_manager_refresh
 * 重新扫描目录中的插件，加载没有加载的插件。
 *
 * @param {plugin_manager_t*} plugin_manager plugin_manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t plugin_manager_refresh(plugin_manager_t* plugin_manager);

/**
 * @method plugin_manager_reload
 * 卸载所有插件并重新加载。
 *
 * @param {plugin_manager_t*} plugin_manager plugin_manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t plugin_manager_reload(plugin_manager_t* plugin_manager);

/**
 * @method plugin_manager_dump
 * 显示所有的插件。
 *
 * @param {plugin_manager_t*} plugin_manager plugin_manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t plugin_manager_dump(plugin_manager_t* plugin_manager);

/**
 * @method plugin_manager_exist
 * 检查指定的插件是否存在。
 *
 * @param {plugin_manager_t*} plugin_manager plugin_manager对象。
 * @param {const char*} lib_name 动态库的名称。
 *
 * @return {bool_t} 返回TRUE表示存在，否则表示不存在。
 */
bool_t plugin_manager_exist(plugin_manager_t* plugin_manager, const char* lib_name);

/**
 * @method plugin_manager_destroy
 * 卸载全部插件，并销毁插件管理器对象。
 * @param {plugin_manager_t*}    plugin_manager plugin_manager对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t plugin_manager_destroy(plugin_manager_t* plugin_manager);

/*插件初始化的函数名*/
#define TK_PLUGIN_INIT "init"

/*插件~初始化的函数名*/
#define TK_PLUGIN_DEINIT "deinit"

END_C_DECLS

#endif /*TK_PLUGIN_MANAGER_H*/
