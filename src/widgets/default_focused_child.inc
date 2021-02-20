/**
 * File:   default_focused_child.inc
 * Author: AWTK Develop Team
 * Brief:  default_focused_child function
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-02-05 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/tokenizer.h"

#define DEFAULT_FOCUSED_CHILD_SAVE_TARGET_TAG_LENGT 16
#define DEFAULT_FOCUSED_CHILD_FIND_SAVE_TARGET_NUMBER 32
#define DEFAULT_FOCUSED_CHILD_FIND_SAVE_TARGET_SEPARTOR '/'

#define DEFAULT_FOCUSED_CHILD_SAVE_TARGET_STRING "save_target"

typedef bool_t (*is_target_type_t)(widget_t* target);

static widget_t* default_focused_child_find_child(widget_t* widget, uint32_t v) {
  int32_t i = 0;
  widget_t* child = NULL;
  return_value_if_fail(widget != NULL && v != 0, NULL);
  for (i = 0; i < widget->children->size; i++) {
    if (v == tk_pointer_to_int(darray_get(widget->children, i))) {
      child = WIDGET(darray_get(widget->children, i));
      break;
    }
  }
  return child;
}

static widget_t* default_focused_child_save_target_from_string(str_t* tag, widget_t* parent) {
  tokenizer_t tokenizer;
  tokenizer_t* t = NULL;
  char separtor[2] = {0};
  widget_t* save_target = NULL;
  return_value_if_fail(tag != NULL && parent != NULL, NULL);
  memset(&separtor, 0x0, sizeof(separtor));
  separtor[0] = DEFAULT_FOCUSED_CHILD_FIND_SAVE_TARGET_SEPARTOR;
  t = tokenizer_init(&tokenizer, tag->str, tag->size, (const char*)separtor);
  return_value_if_fail(t != NULL, NULL);

  save_target = parent;
  while (tokenizer_has_more(t)) {
    const char* str_ptr = tokenizer_next(t);
    widget_t* tmp = default_focused_child_find_child(save_target, tk_atoi(str_ptr));
    if (tmp == NULL) {
      break;
    }
    save_target = tmp;
  }
  tokenizer_deinit(t);

  return save_target;
}

static str_t* default_focused_child_save_target_to_string(str_t* tag, widget_t* target, widget_t* view) {
  int32_t i = 0;
  darray_t ptrs;
  widget_t* parent = NULL;
  return_value_if_fail(tag != NULL && target != NULL && view != NULL, NULL);
  str_clear(tag);
  darray_init(&ptrs, DEFAULT_FOCUSED_CHILD_FIND_SAVE_TARGET_NUMBER, NULL, NULL);

  parent = target;
  while (parent != NULL && parent != view) {
    darray_push(&ptrs, parent);
    parent = parent->parent;
  }

  for (i = ptrs.size - 1; i >= 0; i--) {
    int32_t v = tk_pointer_to_int(darray_get(&ptrs, i));
    str_append_int(tag, v);
    str_append_char(tag, DEFAULT_FOCUSED_CHILD_FIND_SAVE_TARGET_SEPARTOR);
  }
  str_append_char(tag, '\0');
  darray_deinit(&ptrs);
  return tag;
}

static widget_t* default_focused_child_get_save_target(widget_t* widget, widget_t* active_view) {
  str_t str_tag;
  widget_t* target = NULL;
  return_value_if_fail(widget != NULL && active_view != NULL, NULL);
  str_init(&str_tag, DEFAULT_FOCUSED_CHILD_SAVE_TARGET_TAG_LENGT);
  const char* str_target = widget_get_prop_str(active_view, DEFAULT_FOCUSED_CHILD_SAVE_TARGET_STRING, NULL);
  if (str_target != NULL) {
    str_set(&str_tag, str_target);
    target = default_focused_child_save_target_from_string(&str_tag, widget);
  }
  str_reset(&str_tag);

  if (target == NULL || target->parent == NULL) {
    const char* default_focused_child =
        widget_get_prop_str(active_view, WIDGET_PROP_DEFAULT_FOCUSED_CHILD, NULL);
    if (default_focused_child != NULL) {
      target = widget_lookup(active_view, default_focused_child, TRUE);
      if (target == NULL) {
        target = widget_lookup_by_type(active_view, default_focused_child, TRUE);
      }
    }
  }

  return target;
}

static ret_t default_focused_child_set_save_target(widget_t* widget, widget_t* active_view, is_target_type_t is_target_type) {
  widget_t* target = active_view;
  return_value_if_fail(widget != NULL && active_view != NULL, RET_BAD_PARAMS);
  while (target->key_target != NULL) {
    target = target->key_target;
    if (is_target_type(target)) {
      break;
    }
  }

  if (target != NULL && active_view != target) {
    str_t str_target;
    str_init(&str_target, DEFAULT_FOCUSED_CHILD_SAVE_TARGET_TAG_LENGT);
    default_focused_child_save_target_to_string(&str_target, target, widget);
    widget_set_prop_str(active_view, DEFAULT_FOCUSED_CHILD_SAVE_TARGET_STRING, str_target.str);
    str_reset(&str_target);
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static ret_t default_focused_child_set_target_focused(str_t* tag, widget_t* widget) {
  widget_t* target = NULL;
  widget_t* save_target = NULL;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  save_target = target = default_focused_child_save_target_from_string(tag, widget);
  if (target != NULL) {
    while (target->parent != NULL) {
      if (target == target->parent->target) {
        break;
      }
      target->parent->target = target;
      target = target->parent;
    }

    widget_set_focused(save_target, TRUE);
  }
  return RET_OK;
}
