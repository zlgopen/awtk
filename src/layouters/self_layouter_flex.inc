/**
 * File:   self_layouter_flex.inc
 * Author: AWTK Develop Team
 * Brief:  self layouter flex
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-06-11 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_SELF_LAYOUTER_FLEX_INC
#define TK_SELF_LAYOUTER_FLEX_INC

#include "layouters/self_layouter_flex.h"

#include "tkc/utils.h"
#include "base/widget.h"

#define SELF_LAYOUTER_FLEX_ATTR_NAME "flex"

static ret_t self_layouter_flex_replace_param(self_layouter_t* layouter, const char* name,
                                              const char* value) {
  self_layouter_flex_t* layout = (self_layouter_flex_t*)layouter;
  self_layouter_t* l = NULL;
  const char* start = NULL;
  char buff[64];
  return_value_if_fail(layout != NULL && TK_STR_IS_NOT_EMPTY(name), RET_BAD_PARAMS);

  l = (self_layouter_t*)layouter;

  tk_snprintf(buff, sizeof(buff) - 1, "%s=", name);
  start = strstr(l->params.str, name);
  if (start != NULL) {
    start += tk_strlen(buff);
    const char* end = strchr(start, ',');
    if (end == NULL) {
      end = strchr(start, ')');
    }
    if (end == NULL) {
      end = l->params.str + l->params.size;
    }
    str_remove(&l->params, start - l->params.str, end - start);
    str_insert(&l->params, start - l->params.str, value);
  } else {
    if (l->params.size == (sizeof(SELF_LAYOUTER_FLEX "()") - 1) &&
        tk_str_eq(l->params.str, SELF_LAYOUTER_FLEX "()")) {
      tk_snprintf(buff, sizeof(buff) - 1, "%s=%s", name, value);
    } else {
      tk_snprintf(buff, sizeof(buff) - 1, "%s=%s,", name, value);
    }
    str_insert(&l->params, sizeof(SELF_LAYOUTER_FLEX "(") - 1, buff);
  }

  return RET_OK;
}

static const char* self_layouter_flex_to_string(self_layouter_t* layouter) {
  str_t* str = &(layouter->params);
  self_layouter_flex_t* layout = (self_layouter_flex_t*)layouter;
  return_value_if_fail(layout != NULL, NULL);

  if (self_layouter_default_vtable() != NULL && self_layouter_default_vtable()->to_string != NULL) {
    self_layouter_default_vtable()->to_string(layouter);
    str_remove(str, 0, sizeof(SELF_LAYOUTER_DEFAULT) - 1);
    str_insert(str, 0, SELF_LAYOUTER_FLEX);
    if (H_ATTR_FLEX == layout->layouter.h_attr) {
      self_layouter_flex_replace_param(layouter, "h", SELF_LAYOUTER_FLEX_ATTR_NAME);
    }
    if (W_ATTR_FLEX == layout->layouter.w_attr) {
      self_layouter_flex_replace_param(layouter, "w", SELF_LAYOUTER_FLEX_ATTR_NAME);
    }
  } else {
    str_set(str, SELF_LAYOUTER_FLEX "(");
    if (W_ATTR_FLEX == layout->layouter.w_attr) {
      str_append(str, "w=" SELF_LAYOUTER_FLEX_ATTR_NAME);
      str_append_char(str, ',');
    }
    if (H_ATTR_FLEX == layout->layouter.h_attr) {
      str_append(str, "h=" SELF_LAYOUTER_FLEX_ATTR_NAME);
    }
    str_append_char(str, ')');
  }

  if (str_end_with(str, ",)")) {
    str_remove(str, str->size - 2, 1);
  }

  return str->str;
}

static ret_t self_layouter_flex_get_param(self_layouter_t* layouter, const char* name, value_t* v) {
  self_layouter_flex_t* l = (self_layouter_flex_t*)layouter;
  return_value_if_fail(l != NULL, RET_BAD_PARAMS);

  if (self_layouter_default_vtable() != NULL && self_layouter_default_vtable()->get_param != NULL) {
    return self_layouter_default_vtable()->get_param(layouter, name, v);
  } else {
    return RET_OK;
  }
}

static ret_t self_layouter_flex_set_param(self_layouter_t* layouter, const char* name,
                                          const value_t* v) {
  ret_t ret = RET_OK;
  self_layouter_flex_t* layout = (self_layouter_flex_t*)layouter;
  return_value_if_fail(layout != NULL, RET_BAD_PARAMS);

  if (self_layouter_default_vtable() != NULL && self_layouter_default_vtable()->set_param != NULL) {
    ret = self_layouter_default_vtable()->set_param(layouter, name, v);
  } else {
    ret = RET_OK;
  }

  if (RET_OK == ret) {
    const char* value = value_str(v);
    switch (*name) {
      case 'w': {
        if (tk_str_eq(value, SELF_LAYOUTER_FLEX_ATTR_NAME)) {
          layout->layouter.w_attr = W_ATTR_FLEX;
        }
      } break;
      case 'h': {
        if (tk_str_eq(value, SELF_LAYOUTER_FLEX_ATTR_NAME)) {
          layout->layouter.h_attr = H_ATTR_FLEX;
        }
      } break;
      default: {
      } break;
    }
  }

  return ret;
}

static ret_t self_layouter_flex_layout(self_layouter_t* layouter, widget_t* widget, rect_t* area) {
  ret_t ret = RET_OK;
  self_layouter_flex_t* layout = (self_layouter_flex_t*)layouter;
  return_value_if_fail(widget != NULL && widget->parent != NULL, RET_BAD_PARAMS);

  if (self_layouter_default_vtable() != NULL && self_layouter_default_vtable()->layout != NULL) {
    ret = self_layouter_default_vtable()->layout(layouter, widget, area);
  } else {
    ret = RET_OK;
  }

  if (RET_OK == ret) {
    if (W_ATTR_FLEX == layout->layouter.w_attr || H_ATTR_FLEX == layout->layouter.h_attr) {
      widget_t* window_manager = widget_get_window_manager(widget);
      wh_t w = widget->w, h = widget->h;

      /* 将容器尺寸改成一个很大的值，等子控件layout好之后再改回来 */
      if (W_ATTR_FLEX == layout->layouter.w_attr) {
        widget->w = window_manager->w;
      }
      if (H_ATTR_FLEX == layout->layouter.h_attr) {
        widget->h = window_manager->h;
      }
      widget_layout_children(widget);
      widget->w = w;
      widget->h = h;

      if (W_ATTR_FLEX == layout->layouter.w_attr) {
        w = 0;
        WIDGET_FOR_EACH_CHILD_BEGIN(widget, child, i)
        w = tk_max(w, child->x + child->w);
        WIDGET_FOR_EACH_CHILD_END()
      }

      if (H_ATTR_FLEX == layout->layouter.h_attr) {
        h = 0;
        WIDGET_FOR_EACH_CHILD_BEGIN(widget, child, i)
        h = tk_max(h, child->y + child->h);
        WIDGET_FOR_EACH_CHILD_END()
      }

      ret = widget_move_resize_ex(widget, widget->x, widget->y, w, h, FALSE);
    }
  }

  return ret;
}

static ret_t self_layouter_flex_destroy(self_layouter_t* layouter) {
  return_value_if_fail(layouter != NULL, RET_BAD_PARAMS);

  if (self_layouter_default_vtable() != NULL && self_layouter_default_vtable()->destroy != NULL) {
    return self_layouter_default_vtable()->destroy(layouter);
  } else {
    return RET_OK;
  }
}

static self_layouter_t* self_layouter_flex_clone(self_layouter_t* layouter) {
  self_layouter_flex_t* l = NULL;

  if (self_layouter_default_vtable() != NULL && self_layouter_default_vtable()->clone != NULL) {
    l = (self_layouter_flex_t*)self_layouter_default_vtable()->clone(layouter);
  } else {
    l = TKMEM_ZALLOC(self_layouter_flex_t);
    memcpy(l, layouter, sizeof(*l));
  }
  return_value_if_fail(l != NULL, NULL);

  return (self_layouter_t*)l;
}

static ret_t self_layouter_flex_init(self_layouter_t* layouter) {
  return_value_if_fail(layouter != NULL, RET_BAD_PARAMS);

  if (self_layouter_default_vtable() != NULL && self_layouter_default_vtable()->init != NULL) {
    return self_layouter_default_vtable()->init(layouter);
  } else {
    return RET_OK;
  }
}

#ifdef __cplusplus
static const struct self_layouter_flex_vtable_t : self_layouter_vtable_t {
  self_layouter_flex_vtable_t() {
    type = SELF_LAYOUTER_FLEX;
    init = self_layouter_flex_init;
    clone = self_layouter_flex_clone;
    to_string = self_layouter_flex_to_string;
    get_param = self_layouter_flex_get_param;
    set_param = self_layouter_flex_set_param;
    layout = self_layouter_flex_layout;
    destroy = self_layouter_flex_destroy;
  }
} s_self_layouter_flex_vtable;
#else
static const self_layouter_vtable_t s_self_layouter_flex_vtable = {
    .type = SELF_LAYOUTER_FLEX,
    .init = self_layouter_flex_init,
    .clone = self_layouter_flex_clone,
    .to_string = self_layouter_flex_to_string,
    .get_param = self_layouter_flex_get_param,
    .set_param = self_layouter_flex_set_param,
    .layout = self_layouter_flex_layout,
    .destroy = self_layouter_flex_destroy,
};
#endif /* __cplusplus */

inline static bool_t self_layouter_flex_is_valid(self_layouter_t* layouter) {
  return layouter && layouter->vt == &s_self_layouter_flex_vtable;
}

/**
 * @method self_layouter_flex_create
 * @annotation ["constructor"]
 * @export none
 *
 * 创建控件自身布局对象。
 *
 * @return {self_layouter_t*} 返回创建控件自身布局对象。
 *
 */
inline static self_layouter_t* self_layouter_flex_create(void) {
  self_layouter_t* l = NULL;
  self_layouter_flex_t* layouter = TKMEM_ZALLOC(self_layouter_flex_t);
  return_value_if_fail(layouter != NULL, NULL);

  l = (self_layouter_t*)layouter;

  l->vt = &s_self_layouter_flex_vtable;
  self_layouter_flex_init(l);

  return l;
}

#endif /*TK_SELF_LAYOUTER_FLEX_INC*/
