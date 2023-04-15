/**
 * File:   cmd_args.h
 * Author: AWTK Develop Team
 * Brief:  command line args parser 
 *
 * Copyright (c) 2020 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-04-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CMD_ARGS_H
#define TK_CMD_ARGS_H

#include "tkc/str.h"

BEGIN_C_DECLS

/**
 * @class cmd_arg_desc_t
 * 参数描述。
 */
typedef struct _cmd_arg_desc_t {
  /**
   * @property {const char*} name
   * @annotation ["readable"]
   * 名称。
   */
  const char* name;
  /**
   * @property {const char*} desc
   * @annotation ["readable"]
   * 描述。
   */
  const char* desc;
  /**
   * @property {const char*} short_name
   * @annotation ["readable"]
   * 缩写。
   */
  const char* short_name;
  /**
   * @property {value_type_t} value_type
   * @annotation ["readable"]
   * 数据类型。
   */
  value_type_t value_type;
} cmd_arg_desc_t;

typedef ret_t (*cmd_arg_on_arg_t)(void* ctx, const char* name, const value_t* v);

/**
 * @class cmd_args_t
 * 命令行处理。
 */
typedef struct _cmd_args_t {
  /*private*/
  const char* usage;
  void* on_arg_ctx;
  cmd_arg_on_arg_t on_arg;
  const cmd_arg_desc_t* desc;
  uint32_t desc_nr;
} cmd_args_t;

/**
 * @method cmd_args_init
 * 初始化。
 *
 * @param {cmd_args_t*} args args对象。
 * @param {const char*} usage 帮助信息。
 * @param {const cmd_arg_desc_t*}  desc 参数描述。
 * @param {uint32_t} desc_nr 参数描述个数。
 * @param {cmd_arg_on_arg_t} on_arg 参数处理回调函数。
 * @param {void*} on_arg_ctx 参数处理回调函数的上下文。
 *
 * @return {cmd_args_t*} 返回CMD_ARGS对象。 
 */
cmd_args_t* cmd_args_init(cmd_args_t* args, const char* usage, const cmd_arg_desc_t* desc,
                          uint32_t desc_nr, cmd_arg_on_arg_t on_arg, void* on_arg_ctx);

/**
 * @method cmd_args_show_help
 * 显示帮助信息。
 *
 * @param {cmd_args_t*} args args对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t cmd_args_show_help(cmd_args_t* args);

/**
 * @method cmd_args_process
 * 处理。
 *
 * @param {cmd_args_t*} args args对象。
 * @param {int}  argc 参数个数。
 * @param {char**}  argv 参数数组。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t cmd_args_process(cmd_args_t* args, int argc, char* argv[]);

/**
 * @method cmd_args_process_str
 * 处理。
 *
 * @param {cmd_args_t*} args args对象。
 * @param {const char*}  cmd_line 字符串格式的参数(适用于Windows)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t cmd_args_process_str(cmd_args_t* args, const char* cmd_line);

END_C_DECLS

#endif /*TK_CMD_ARGS_H*/

