/**
 * File:   shortcut.c
 * Author: AWTK Develop Team
 * Brief:  shortcut
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-05-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <tkc/mem.h>
#include "tkc/tokenizer.h"

#include "base/enums.h"
#include "base/shortcut.h"

shortcut_t* shortcut_init(shortcut_t* shortcut, uint32_t key) {
  return_value_if_fail(shortcut != NULL, NULL);
  memset(shortcut, 0x00, sizeof(shortcut_t));

  shortcut->key = key;
  shortcut->is_valid = TRUE;

  return shortcut;
}

shortcut_t* shortcut_init_with_str(shortcut_t* shortcut, const char* str) {
  tokenizer_t t;
  const char* k = NULL;
  return_value_if_fail(shortcut != NULL && str != NULL, NULL);
  return_value_if_fail(tokenizer_init_ex(&t, str, strlen(str), " +", NULL) != NULL, NULL);

  memset(shortcut, 0, sizeof(shortcut_t));
  while (tokenizer_has_more(&t)) {
    k = tokenizer_next(&t);
    if (tk_str_ieq(k, "ctrl")) {
      shortcut->ctrl = TRUE;
      shortcut->lctrl = TRUE;
      shortcut->rctrl = TRUE;
    } else if (tk_str_ieq(k, "alt")) {
      shortcut->alt = TRUE;
      shortcut->lalt = TRUE;
      shortcut->ralt = TRUE;
    } else if (tk_str_ieq(k, "shift")) {
      shortcut->shift = TRUE;
      shortcut->lshift = TRUE;
      shortcut->rshift = TRUE;
    } else if (tk_str_ieq(k, "lctrl")) {
      shortcut->lctrl = TRUE;
    } else if (tk_str_ieq(k, "lalt")) {
      shortcut->lalt = TRUE;
    } else if (tk_str_ieq(k, "lshift")) {
      shortcut->lshift = TRUE;
    } else if (tk_str_ieq(k, "rctrl")) {
      shortcut->rctrl = TRUE;
    } else if (tk_str_ieq(k, "ralt")) {
      shortcut->ralt = TRUE;
    } else if (tk_str_ieq(k, "rshift")) {
      shortcut->rshift = TRUE;
    } else if (tk_str_ieq(k, "cmd")) {
      shortcut->cmd = TRUE;
    } else if (tk_str_ieq(k, "menu")) {
      shortcut->menu = TRUE;
    } else {
      const key_type_value_t* kv = keys_type_find(k);
      if (kv != NULL) {
        shortcut->key = kv->value;
      } else {
        log_warn("%s not supported\n", k);
      }
    }
    shortcut->is_valid = TRUE;
  }
  tokenizer_deinit(&t);

  return shortcut;
}

bool_t shortcut_equal(shortcut_t* shortcut1, shortcut_t* shortcut2) {
  return_value_if_fail(shortcut1 != NULL && shortcut2 != NULL, FALSE);

  return memcmp(shortcut1, shortcut2, sizeof(shortcut_t)) == 0;
}

bool_t shortcut_match(shortcut_t* filter, shortcut_t* event) {
  return_value_if_fail(filter != NULL && event != NULL, FALSE);

  if (filter->key != event->key) {
    return FALSE;
  }

  if (filter->cmd != event->cmd) {
    return FALSE;
  }

  if (filter->menu != event->menu) {
    return FALSE;
  }

  if (filter->ctrl) {
    if (!event->lctrl && !event->rctrl) {
      return FALSE;
    }
  } else {
    if (filter->lctrl != event->lctrl) {
      return FALSE;
    }

    if (filter->rctrl != event->rctrl) {
      return FALSE;
    }
  }

  if (filter->alt) {
    if (!event->lalt && !event->ralt) {
      return FALSE;
    }
  } else {
    if (filter->lalt != event->lalt) {
      return FALSE;
    }

    if (filter->ralt != event->ralt) {
      return FALSE;
    }
  }

  if (filter->shift) {
    if (!event->lshift && !event->rshift) {
      return FALSE;
    }
  } else {
    if (filter->lshift != event->lshift) {
      return FALSE;
    }

    if (filter->rshift != event->rshift) {
      return FALSE;
    }
  }

  return TRUE;
}
