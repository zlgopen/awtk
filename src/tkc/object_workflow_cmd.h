/**
 * File:   object_workflow_cmd.h
 * Author: AWTK Develop Team
 * Brief:  对象工作流命令
 *
 * Copyright (c) 2026 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2026-08-03 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_OBJECT_WORKFLOW_CMD_H
#define TK_OBJECT_WORKFLOW_CMD_H

#include "tkc/object.h"

BEGIN_C_DECLS

typedef ret_t (*object_workflow_cmd_runner_t)(tk_object_t* obj, const char* args, void* ctx);

/**
 * @class object_workflow_cmd_t
 * @parent tk_object_t
 * 工作流命令对象。
 * 
 * 用于将多个命令组织为一个工作流，可自定义执行顺序与条件（支持整体执行、失败自动回滚、整体撤销（UNDO））。
 * 
 * 示例：
 * 
 *```c
 * // 创建工作流命令对象
 * tk_object_t* obj = object_workflow_cmd_create();
 *
 * // 创建子命令对象
 * tk_object_t* cmd_a = a_cmd_create();
 * tk_object_t* cmd_b = b_cmd_create();
 * tk_object_t* cmd_c = c_cmd_create();
 * 
 * // 添加命令
 * if(RET_OK == tk_object_set_prop_object(obj, "a", cmd_a)) {
 *   tk_object_unref(cmd_a);
 * }
 * if(RET_OK == tk_object_set_prop_object(obj, "b", cmd_b)) {
 *   tk_object_unref(cmd_b);
 * }
 * if(RET_OK == tk_object_set_prop_object(obj, "c", cmd_c)) {
 *   tk_object_unref(cmd_c);
 * }
 * 
 * // 设定工作流（默认为空时顺序执行所有命令）
 * object_workflow_cmd_set_runner(obj, my_workflow_runner, NULL);
 * 
 * // 执行命令（执行失败时会自动回滚已成功执行的子命令）
 * tk_object_exec(obj, TK_OBJECT_CMD_EXEC, NULL);
 * 
 * // 如需撤销执行结果，可调用 UNDO（失败时框架已自动回滚，无需再调用）
 * tk_object_exec(obj, TK_OBJECT_CMD_UNDO, NULL);
 * 
 * // 释放对象
 * TK_OBJECT_UNREF(obj);
 *```
 * 
 *```c
 * ret_t my_workflow_runner(tk_object_t* obj, const char* args, void* ctx) {
 *   tk_object_t* cmd_a = tk_object_get_prop_object(obj, "a");
 *   ret_t ret = tk_object_exec(cmd_a, TK_OBJECT_CMD_EXEC, args);
 *   if (RET_OK == ret) {
 *     tk_object_t* cmd_b = tk_object_get_prop_object(obj, "b");
 *     ret = tk_object_exec(cmd_b, TK_OBJECT_CMD_EXEC, args);
 * 
 *     if (RET_OK == ret) {
 *       // 工作流运行中动态创建命令，并添加到工作流中执行
 *       tk_object_t* cmd_d = d_cmd_create();
 *       ret = tk_object_set_prop_object(obj, "d", cmd_d);
 *       if (RET_OK == ret) {
 *         ret = tk_object_exec(cmd_d, TK_OBJECT_CMD_EXEC, args);
 *       }
 *       tk_object_unref(cmd_d);
 *     }
 *   } else {
 *     tk_object_t* cmd_c = tk_object_get_prop_object(obj, "c");
 *     ret = tk_object_exec(cmd_c, TK_OBJECT_CMD_EXEC, args);
 *   }
 *   // 返回RET_OK表示工作流执行成功，否则表示失败，工作流执行失败时会自动回滚已成功执行的子命令。
 *   return ret;
 * }
 *```
 */
typedef struct _object_workflow_cmd_t object_workflow_cmd_t;

/**
 * @method object_workflow_cmd_create
 * 创建工作流命令对象。
 * @annotation ["constructor", "gc"]
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_workflow_cmd_create(void);

/**
 * @method object_workflow_cmd_set_runner
 * 设置执行工作流。
 * > 为空时则顺序执行所有命令。
 * @param {tk_object_t*} obj workflow_cmd对象。
 * @param {object_workflow_cmd_runner_t} runner 执行工作流的回调函数。
 * @param {void*} ctx 上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_workflow_cmd_set_runner(tk_object_t* obj, object_workflow_cmd_runner_t runner,
                                     void* ctx);

/**
 * @method object_workflow_cmd_cast
 * 转换为object_workflow_cmd对象。
 * @annotation ["cast"]
 * @param {tk_object_t*} obj object_workflow_cmd对象。
 *
 * @return {object_workflow_cmd_t*} object_workflow_cmd对象。
 */
object_workflow_cmd_t* object_workflow_cmd_cast(tk_object_t* obj);

#define OBJECT_WORKFLOW_CMD(obj) object_workflow_cmd_cast(obj)

#define OBJECT_WORKFLOW_CMD_TYPE "object_workflow_cmd"

END_C_DECLS

#endif /*TK_OBJECT_WORKFLOW_CMD_H*/
