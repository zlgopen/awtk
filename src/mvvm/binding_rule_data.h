/**
 * File:   binding_rule_data.c
 * Author: AWTK Develop Team
 * Brief:  data binding rule
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BINDING_RULE_DATA_H
#define TK_BINDING_RULE_DATA_H

#include "tkc/str.h"
#include "tkc/object_default.h"
#include "mvvm/binding_rule.h"

BEGIN_C_DECLS

/**
 * @enum update_model_trigger_t
 * @annotation ["scriptable", "string"]
 * @prefix UPDATE_WHEN_
 * 更新模型的时机。
 */
typedef enum _update_model_trigger_t {
  /**
   * @const UPDATE_WHEN_CHANGED
   * 编辑完成时才触发，即回车会或失去焦点时更新数据到模型。
   */
  UPDATE_WHEN_CHANGED = 0,

  /**
   * @const UPDATE_WHEN_CHANGING
   * 编辑时实时触发，在编辑时实时更新数据到模型。
   */
  UPDATE_WHEN_CHANGING,

  /**
   * @const EUPDATE_WHEN_EXPLICIT
   * 需要显式触发。
   * 一般在命令绑定规则中指定的UpdateModel=True，执行命令前自动调用UpdateModel。
   */
  UPDATE_WHEN_EXPLICIT
} update_model_trigger_t;

/**
 * @enum binding_mode_t
 * @annotation ["scriptable"]
 * @prefix BINDING_
 * 绑定模式。
 */
typedef enum _binding_mode_t {
  /**
   * @const BINDING_TWO_WAY
   * 双向绑定，模型变化时自动更新视图，视图上数据变化时自动更新模型。
   */
  BINDING_TWO_WAY,

  /**
   * @const BINDING_ONE_WAY
   * 单向绑定，模型变化时自动更新视图。
   */
  BINDING_ONE_WAY,

  /**
   * @const BINDING_ONE_WAY_TO_MODEL
   * 单向绑定，视图上数据变化时自动更新模型。
   */
  BINDING_ONE_WAY_TO_MODEL,

  /**
   * @const BINDING_ONCE
   * 单向绑定，仅仅绑定一次。
   */
  BINDING_ONCE
} binding_mode_t;

/**
 * @class binding_rule_data_t
 * @parent binding_rule_t
 * @annotation ["scriptable"]
 * 数据绑定规则。
 *
 */
typedef struct _binding_rule_data_t {
  binding_rule_t binding_rule;
  object_t* props;
  str_t last_error;
} binding_rule_data_t;

binding_rule_t* binding_rule_data_create(void);

const char* binding_rule_data_get_path(binding_rule_t* rule);
const char* binding_rule_data_get_converter(binding_rule_t* rule);
const char* binding_rule_data_get_validator(binding_rule_t* rule);
binding_mode_t binding_rule_data_get_mode(binding_rule_t* rule);
update_model_trigger_t binding_rule_data_get_trigger(binding_rule_t* rule);

END_C_DECLS

#endif /*TK_BINDING_RULE_DATA_H*/
