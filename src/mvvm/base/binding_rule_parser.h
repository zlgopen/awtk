/**
 * File:   binding_rule.h
 * Author: AWTK Develop Team
 * Brief:  binding rule parser
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
 * 2019-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BINDING_RULE_PARSER_H
#define TK_BINDING_RULE_PARSER_H

#include "mvvm/base/binding_rule.h"

BEGIN_C_DECLS

binding_rule_t* binding_rule_parse(const char* name, const char* value);

END_C_DECLS

#endif /*TK_BINDING_RULE_PARSER_H*/
