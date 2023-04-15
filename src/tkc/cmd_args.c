/**
 * File:   cmd_args.c
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

#include "tkc/utils.h"
#include "tkc/tokenizer.h"
#include "tkc/cmd_args.h"

BEGIN_C_DECLS

cmd_args_t* cmd_args_init(cmd_args_t* args, const char* usage, const cmd_arg_desc_t* desc,
                          uint32_t desc_nr, cmd_arg_on_arg_t on_arg, void* on_arg_ctx) {
  return_value_if_fail(desc_nr > 0, NULL);
  return_value_if_fail(args != NULL && desc != NULL && on_arg != NULL, NULL);

  args->desc = desc;
  args->usage = usage;
  args->desc_nr = desc_nr;
  args->on_arg = on_arg;
  args->on_arg_ctx = on_arg_ctx;

  return args;
}

static const cmd_arg_desc_t* cmd_args_find(cmd_args_t* args, const char* name) {
  uint32_t i = 0;
  uint32_t len = 0;
  const char* p = NULL;
  return_value_if_fail(name != NULL, NULL);

  while (*name == '-') {
    name++;
  }

  p = strchr(name, '=');
  if (p != NULL) {
    len = p - name;
  } else {
    len = strlen(name);
  }

  for (i = 0; i < args->desc_nr; i++) {
    const cmd_arg_desc_t* iter = args->desc + i;
    if (tk_str_eq_with_len(name, iter->name, len) ||
        tk_str_eq_with_len(name, iter->short_name, len)) {
      return iter;
    }
  }

  return NULL;
}

ret_t cmd_args_show_help(cmd_args_t* args) {
  uint32_t i = 0;
  log_info("Usage: %s\n", args->usage);
  log_info("  --help/-h show help info.\n");

  for (i = 0; i < args->desc_nr; i++) {
    const cmd_arg_desc_t* iter = args->desc + i;
    log_info("  --%s/-%s %s\n", iter->name, iter->short_name, iter->desc);
  }

  return RET_OK;
}

static ret_t cmd_args_on_arg(cmd_args_t* args, value_type_t type, const char* name,
                             const char* value) {
  if (value == NULL || type == VALUE_TYPE_INVALID) {
    args->on_arg(args->on_arg_ctx, name, NULL);
  } else {
    value_t v;
    switch (type) {
      case VALUE_TYPE_BOOL: {
        value_set_bool(&v, tk_atob(value));
        break;
      }
      case VALUE_TYPE_INT8:
      case VALUE_TYPE_INT16:
      case VALUE_TYPE_INT32:
      case VALUE_TYPE_UINT8:
      case VALUE_TYPE_UINT16:
      case VALUE_TYPE_UINT32: {
        value_set_int(&v, tk_atoi(value));
        break;
      }
      case VALUE_TYPE_FLOAT32:
      case VALUE_TYPE_DOUBLE:
      case VALUE_TYPE_FLOAT: {
        value_set_double(&v, tk_atof(value));
        break;
      }
      default: {
        value_set_str(&v, value);
        break;
      }
    }

    args->on_arg(args->on_arg_ctx, name, &v);
  }

  return RET_OK;
}

ret_t cmd_args_process(cmd_args_t* args, int argc, char* argv[]) {
  uint32_t i = 0;
  const cmd_arg_desc_t* desc = NULL;
  return_value_if_fail(args != NULL && argv != NULL, RET_BAD_PARAMS);

  for (i = 1; i < argc; i++) {
    const char* iter = argv[i];
    if (*iter == '-') {
      if (tk_str_eq(iter, "-h") || tk_str_eq(iter, "--help")) {
        cmd_args_show_help(args);
        return RET_STOP;
      }

      desc = cmd_args_find(args, iter);
      if (desc != NULL && desc->value_type != VALUE_TYPE_INVALID) {
        const char* value = strchr(iter, '=');
        if (value != NULL) {
          value++;
        } else if ((i + 1) < argc) {
          value = argv[++i];
        }
        cmd_args_on_arg(args, desc->value_type, desc->name, value);
      } else {
        if (desc != NULL) {
          cmd_args_on_arg(args, VALUE_TYPE_INVALID, desc->name, NULL);
        } else {
          cmd_args_on_arg(args, VALUE_TYPE_INVALID, iter, NULL);
        }
      }
    } else {
      cmd_args_on_arg(args, VALUE_TYPE_INVALID, iter, NULL);
    }
  }

  return RET_OK;
}

ret_t cmd_args_process_str(cmd_args_t* args, const char* cmd_line) {
  tokenizer_t t;
  const cmd_arg_desc_t* desc = NULL;
  return_value_if_fail(args != NULL && cmd_line != NULL, RET_BAD_PARAMS);

  tokenizer_init(&t, cmd_line, strlen(cmd_line), " \t");
  while (tokenizer_has_more(&t)) {
    const char* iter = tokenizer_next_str(&t);
    if (*iter == '-') {
      if (tk_str_eq(iter, "-h") || tk_str_eq(iter, "--help")) {
        cmd_args_show_help(args);
        return RET_STOP;
      }

      desc = cmd_args_find(args, iter);
      if (desc != NULL && desc->value_type != VALUE_TYPE_INVALID) {
        const char* value = strchr(iter, '=');
        if (value != NULL) {
          value++;
        } else {
          value = tokenizer_next_str(&t);
        }
        cmd_args_on_arg(args, desc->value_type, desc->name, value);
      } else {
        if (desc != NULL) {
          cmd_args_on_arg(args, VALUE_TYPE_INVALID, desc->name, NULL);
        } else {
          cmd_args_on_arg(args, VALUE_TYPE_INVALID, iter, NULL);
        }
      }
    } else {
      cmd_args_on_arg(args, VALUE_TYPE_INVALID, iter, NULL);
    }
  }
  tokenizer_deinit(&t);

  return RET_OK;
}
