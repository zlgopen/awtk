/**
 * File:   object_orchestrator.h
 * Author: AWTK Develop Team
 * Brief:  对象编排器
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

#ifndef TK_OBJECT_ORCHESTRATOR_H
#define TK_OBJECT_ORCHESTRATOR_H

#include "tkc/object.h"

BEGIN_C_DECLS

typedef ret_t (*object_orchestrator_exec_workflow_t)(tk_object_t* cmds, const char* args,
                                                     void* ctx);

/**
 * @class object_orchestrator_t
 * @parent tk_object_t
 * 编排器对象。
 */
typedef struct _object_orchestrator_t object_orchestrator_t;

/**
 * @method object_orchestrator_create
 * 创建编排器对象。
 * @annotation ["constructor", "gc"]
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_orchestrator_create(void);

/**
 * @method object_orchestrator_add_cmd
 * 添加指令对象。
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj orchestrator对象。
 * @param {const char*} name 指令名称。
 * @param {tk_object_t*} cmd 指令对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_orchestrator_add_cmd(tk_object_t* obj, const char* name, tk_object_t* cmd);

/**
 * @method object_orchestrator_set_workflow
 * 设置工作流。
 * @param {tk_object_t*} obj orchestrator对象。
 * @param {object_orchestrator_exec_workflow_t} exec_workflow 执行工作流函数。
 * @param {void*} ctx 上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_orchestrator_set_workflow(tk_object_t* obj,
                                       object_orchestrator_exec_workflow_t exec_workflow,
                                       void* ctx);

/**
 * @method object_orchestrator_cast
 *
 * 转换为object_orchestrator对象。
 *
 * @annotation ["cast"]
 * @param {tk_object_t*} obj object_orchestrator对象。
 *
 * @return {object_orchestrator_t*} object_orchestrator对象。
 */
object_orchestrator_t* object_orchestrator_cast(tk_object_t* obj);

#define OBJECT_ORCHESTRATOR(obj) object_orchestrator_cast(obj)

#define OBJECT_ORCHESTRATOR_TYPE "object_orchestrator"

END_C_DECLS

#endif /*TK_OBJECT_ORCHESTRATOR_H*/
