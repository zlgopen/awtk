/**
 * File:   binding_rule.h
 * Author: AWTK Develop Team
 * Brief:  binding rule
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

#ifndef TK_BINDING_RULE_H
#define TK_BINDING_RULE_H

#include "tkc/object.h"
#include "mvvm/base/types_def.h"

BEGIN_C_DECLS

/**
 * @enum binding_rule_type_t
 * @annotation ["scriptable", "string"]
 * @prefix BINDING_RULE_
 * 绑定规则类型。
 */

/**
 * @const BINDING_RULE_DATA
 * 数据绑定规则。
 */
#define BINDING_RULE_DATA "data"

/**
 * @const BINDING_RULE_COMMAND
 * 命令绑定规则。
 */
#define BINDING_RULE_COMMAND "command"

/**
 * @class binding_rule_t
 * @parent object_t
 * @annotation ["scriptable"]
 * 绑定规则。
 *
 */
typedef struct _binding_rule_t {
  object_t object;

  void* widget;
  binding_context_t* binding_context;
} binding_rule_t;

#define BINDING_RULE(rule) ((binding_rule_t*)(rule))

#define BINDING_RULE_DATA_PREFIX "v-data"
#define BINDING_RULE_COMMAND_PREFIX "v-command"

END_C_DECLS

#endif /*TK_BINDING_RULE_H*/
