/**
 * File:   remote_ui.h
 * Author: AWTK Develop Team
 * Brief:  remote ui client
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-11-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_REMOTE_UI_H
#define TK_REMOTE_UI_H

#include "tkc/buffer.h"
#include "tkc/darray.h"
#include "tkc/iostream.h"
#include "ubjson/ubjson_writer.h"
#include "remote_ui/shared/remote_ui_types_def.h"
#include "service/client.h"

#ifdef TK_IS_PC
#define WITH_FULL_REMOTE_UI 1
#endif/*TK_IS_PC*/

typedef ret_t (*remote_ui_on_log_message_t)(void* ctx, tk_log_level_t level, const char* msg); 

BEGIN_C_DECLS

/**
 * @class remote_ui_t
 * remote ui客户端。 
*/
typedef struct _remote_ui_t {
  tk_client_t client;

  /*private*/
  ubjson_writer_t writer;
  tk_object_t* event_handlers;
  darray_t pending_events;
  darray_t dispatching_events;
  remote_ui_on_log_message_t log_hook;
  void* log_hook_ctx;
} remote_ui_t;

/**
 * @method remote_ui_create
 * 创建remote ui客户端。
 *
 * @param {tk_iostream_t*} io IO流。
 *
 * @return {remote_ui_t*} 返回remote ui客户端对象。
 */
remote_ui_t* remote_ui_create(tk_iostream_t* io);

/**
 * @method remote_ui_login
 * 登录。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} username 用户名。
 * @param {const char*} password 密码。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_login(remote_ui_t* ui, const char* username, const char* password);

/**
 * @method remote_ui_logout
 * 登出。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_logout(remote_ui_t* ui);

/**
 * @method remote_ui_get_dev_info
 * 获取设备信息。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {remote_ui_dev_info_t*} info 设备信息。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_get_dev_info(remote_ui_t* ui, remote_ui_dev_info_t* info);

/**
 * @method remote_ui_reboot
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {remote_ui_reboot_type_t} type 重启类型。
 * 重新启动。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_reboot(remote_ui_t* ui, remote_ui_reboot_type_t type);

/**
 * @method remote_ui_upload_file
 * 上传文件。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} remote_file 远程文件。
 * @param {const char*} local_file 本地文件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_upload_file(remote_ui_t* ui, const char* remote_file, const char* local_file);

/**
 * @method remote_ui_download_file
 * 下载文件。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} remote_file 远程文件。
 * @param {const char*} local_file 本地文件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_download_file(remote_ui_t* ui, const char* remote_file, const char* local_file);

/**
 * @method remote_ui_create_dir
 * 创建目录。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} remote_dir 远程目录。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_create_dir(remote_ui_t* ui, const char* remote_dir);

/**
 * @method remote_ui_remove_dir
 * 删除目录。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} remote_dir 远程目录。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_remove_dir(remote_ui_t* ui, const char* remote_dir);

/**
 * @method remote_ui_remove_file
 * 删除文件。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} remote_file 远程文件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_remove_file(remote_ui_t* ui, const char* remote_file);

/**
 * @method remote_ui_take_snapshot
 * 截屏。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} target 目标控件或窗口。
 * @param {const char*} file 保存截图文件名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_take_snapshot(remote_ui_t* ui, const char* target, const char* file);

/**
 * @method remote_ui_get_manifest
 * 获取manifest。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} file 保存数据的文件名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_get_manifest(remote_ui_t* ui, const char* file);

/**
 * @method remote_ui_get_xml_source
 * 获取当前窗口的XML源码。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} target 目标控件或窗口。
 * @param {const char*} file 保存数据的文件名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_get_xml_source(remote_ui_t* ui, const char* target, const char* file);

/**
 * @method remote_ui_on_event
 * 注册事件。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} target 目标。
 * @param {uint32_t} event 事件。
 * @param {event_func_t} func 事件处理函数。
 * @param {void*} ctx 上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_on_event(remote_ui_t* ui, const char* target, uint32_t event,
                         event_func_t func, void* ctx);

/**
 * @method remote_ui_off_event
 * 注销事件。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} target 目标。
 * @param {uint32_t} event 事件。
 * @param {event_func_t} func 事件处理函数。
 * @param {void*} ctx 上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_off_event(remote_ui_t* ui, const char* target, uint32_t event,
                          event_func_t func, void* ctx);

/**
 * @method remote_ui_send_event
 * 发送事件。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} target 目标。
 * @param {event_t*} event 事件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_send_event(remote_ui_t* ui, const char* target, event_t* event);

/**
 * @method remote_ui_click
 * 点击指定的控件。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} target 目标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_click(remote_ui_t* ui, const char* target);

/**
 * @method remote_ui_key
 * 发送按键给控件。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} target 目标。
 * @param {int32_t} key_code 按键代码。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_key(remote_ui_t* ui, const char* target, int32_t key_code);

/**
 * @method remote_ui_open_window
 * 打开窗口。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} name 窗口名称。
 * @param {const char*} xml 窗口XML。
 * @param {const char*} init_json 初始化JSON。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_open_window(remote_ui_t* ui, const char* name, const char* xml,
                            const char* init_json);

/**
 * @method remote_ui_show_confirm
 * 显示确认对话框。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} title 标题。
 * @param {const char*} content 内容。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_show_confirm(remote_ui_t* ui, const char* title, const char* content);

/**
 * @method remote_ui_show_warn
 * 显示警告对话框。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} title 标题。
 * @param {const char*} content 内容。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_show_warn(remote_ui_t* ui, const char* title, const char* content);

/**
 * @method remote_ui_show_info
 * 显示信息对话框。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} title 标题。
 * @param {const char*} content 内容。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_show_info(remote_ui_t* ui, const char* title, const char* content);
/**
 * @method remote_ui_show_toast
 * 显示信息对话框。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {uint32_t} duration 时长。
 * @param {const char*} content 内容。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_show_toast(remote_ui_t* ui, uint32_t duration, const char* content);
/**
 * @method remote_ui_close_window
 * 关闭窗口。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} name 窗口名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_close_window(remote_ui_t* ui, const char* name);

/**
 * @method remote_ui_back_to_prev
 * 返回上一个窗口。
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_back_to_prev(remote_ui_t* ui);

/**
 * @method remote_ui_back_to_home
 * 返回主窗口。
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
*/
ret_t remote_ui_back_to_home(remote_ui_t* ui);

/**
 * @method remote_ui_set_prop
 * 设置属性。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} target 目标。
 * @param {const char*} name 属性名称。
 * @param {const value_t*} value 属性值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_set_prop(remote_ui_t* ui, const char* target, const char* name,
                         const value_t* value);

/**
 * @method remote_ui_get_prop
 * 获取属性。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} target 目标。
 * @param {const char*} name 属性名称。
 * @param {value_t*} value 属性值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_get_prop(remote_ui_t* ui, const char* target, const char* name, value_t* value);

/**
 * @method remote_ui_set_theme
 * 设置主题。
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} theme 主题名称。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
*/
ret_t remote_ui_set_theme(remote_ui_t* ui, const char* theme);

/**
 * @method remote_ui_set_language
 * 设置语言。
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} language 语言名称。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
*/
ret_t remote_ui_set_language(remote_ui_t* ui, const char* language);

/**
 * @method remote_ui_exec_fscript
 * 执行fscript脚本。
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} script 脚本。
 * @param {str_t*} result 执行结果。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_exec_fscript(remote_ui_t* ui, const char* script, str_t* result);

/**
 * @method remote_ui_move_widget
 * 移动控件。
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} target 目标。
 * @param {int32_t} x x坐标。
 * @param {int32_t} y y坐标。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_move_widget(remote_ui_t* ui, const char* target, int32_t x, int32_t y);

/**
 * @method remote_ui_resize_widget
 * 调整控件大小。
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} target 目标。
 * @param {uint32_t} w 宽度。
 * @param {uint32_t} h 高度。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_resize_widget(remote_ui_t* ui, const char* target, uint32_t w, uint32_t h);

/**
 * @method remote_ui_destroy_widget
 * 销毁控件。
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} target 目标。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_destroy_widget(remote_ui_t* ui, const char* target);

/**
 * @method remote_ui_create_widget
 * 创建控件。
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} target 目标。
 * @param {const char*} xml XML。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_create_widget(remote_ui_t* ui, const char* target, const char* xml);

/**
 * @method remote_ui_get_loaded_images_info
 * 获取已经加载的图片。
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} file 保存数据的文件名。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_get_loaded_images_info(remote_ui_t* ui, const char* file);

/**
 * @method remote_ui_get_loaded_assets_info
 * 获取已经加载的资源。
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {const char*} file 保存数据的文件名。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。 
*/
ret_t remote_ui_get_loaded_assets_info(remote_ui_t* ui, const char* file);

/**
 * @method remote_ui_hook_log
 * 设置log钩子函数。
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @param {remote_ui_on_log_message_t} log log hook。
 * @param {void*} ctx 上下文。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_hook_log(remote_ui_t* ui, remote_ui_on_log_message_t log, void* ctx);

/**
 * @method remote_ui_unhook_log
 * 取消log钩子函数。
 * @param {remote_ui_t*} ui remote ui客户端对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_unhook_log(remote_ui_t* ui);

/**
 * @method remote_ui_dispatch
 * 分发事件。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_dispatch(remote_ui_t* ui);

/**
 * @method remote_ui_destroy
 * 销毁remote ui客户端。
 *
 * @param {remote_ui_t*} ui remote ui客户端对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t remote_ui_destroy(remote_ui_t* ui);

END_C_DECLS

#endif /*TK_REMOTE_UI_H*/
