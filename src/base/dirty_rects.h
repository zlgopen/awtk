/**
 * File:   dirty_rects.h
 * Author: AWTK Develop Team
 * Brief:  multiple dirty rects
 *
 * Copyright (c) 2021 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-06-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DIRTY_RECTS_H
#define TK_DIRTY_RECTS_H

#include "tkc/rect.h"

BEGIN_C_DECLS

#ifndef TK_MAX_DIRTY_RECT_NR
#define TK_MAX_DIRTY_RECT_NR 10
#endif /*TK_MAX_DIRTY_RECT_NR*/

/**
 * @class dirty_rects_t
 * 支持多个脏矩形。
 * > 在通常情况下，脏矩形的个数并不多，而且一般都是不重叠的，
 * > 所以为了降低计算开销、避免内存分配和简化实现。这里采用下列措施：
 *
 * * 如果新的脏矩形是独立的，直接加入进来。
 * * 如果新的脏矩形与某个脏矩形有重叠，则合并到该脏矩形。
 * * 如果脏矩形的个数超出最大个数则进行合并。
 * 
 */
typedef struct _dirty_rects_t {
  /**
   * @property {uint32_t} nr
   * @annotation ["readable"]
   * 个数。
   */
  uint32_t nr;

  /**
   * @property {rect_t} max
   * @annotation ["readable"]
   * 最大脏矩形。
   */
  rect_t max;

  /**
   * @property {rect_t} rects
   * @annotation ["readable"]
   * 脏矩形。
   */
  rect_t rects[TK_MAX_DIRTY_RECT_NR];
  /*private*/
  bool_t debug;
  bool_t profile;
  bool_t disable_multiple;
} dirty_rects_t;

/**
 * @method dirty_rects_reset
 * 重置dirty_rects对象。
 * @export none
 * @param {dirty_rects_t*} dirty_rects dirty_rects对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t dirty_rects_reset(dirty_rects_t* dirty_rects) {
  return_value_if_fail(dirty_rects != NULL, RET_BAD_PARAMS);
  memset(dirty_rects, 0x00, sizeof(dirty_rects_t));
  /*
  dirty_rects->debug = TRUE;
  dirty_rects->profile = TRUE;
  dirty_rects->disable_multiple = TRUE;
*/

  return RET_OK;
}

/**
 * @method dirty_rects_init
 * @export none
 * 初始化dirty_rects对象。
 * @param {dirty_rects_t*} dirty_rects dirty_rects对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t dirty_rects_init(dirty_rects_t* dirty_rects) {
  return dirty_rects_reset(dirty_rects);
}

/**
 * @method dirty_rects_deinit
 * @export none
 * 释放dirty_rects对象。
 * @param {dirty_rects_t*} dirty_rects dirty_rects对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t dirty_rects_deinit(dirty_rects_t* dirty_rects) {
  return dirty_rects_reset(dirty_rects);
}

/**
 * @method dirty_rects_remove
 * @export none
 * 删除指定序数的矩形。
 * @param {dirty_rects_t*} dirty_rects dirty_rects对象。
 * @param {uint32_t} index 序数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t dirty_rects_remove(dirty_rects_t* dirty_rects, uint32_t index) {
  uint32_t i = 0;
  return_value_if_fail(dirty_rects != NULL, RET_BAD_PARAMS);
  return_value_if_fail(index < dirty_rects->nr, RET_BAD_PARAMS);

  for (i = index; (i + 1) < dirty_rects->nr; i++) {
    dirty_rects->rects[i] = dirty_rects->rects[i + 1];
  }
  dirty_rects->nr--;

  return RET_OK;
}

/**
 * @method dirty_rects_fix
 * @export none
 * 脏矩形之间可能重叠，合并重叠的脏矩形。
 * @param {dirty_rects_t*} dirty_rects dirty_rects对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t dirty_rects_fix(dirty_rects_t* dirty_rects) {
  uint32_t i = 0;
  return_value_if_fail(dirty_rects != NULL, RET_BAD_PARAMS);

  if (dirty_rects->nr > 1) {
    for (i = 1; i < dirty_rects->nr; i++) {
      rect_t* iter = dirty_rects->rects + i;
      if (rect_has_intersect(dirty_rects->rects, iter)) {
        rect_merge(dirty_rects->rects, iter);
        dirty_rects_remove(dirty_rects, i);

        return dirty_rects_fix(dirty_rects);
      }
    }
  }

  return RET_OK;
}

/**
 * @method dirty_rects_add
 * @export none
 * 增加脏矩形。
 * @param {dirty_rects_t*} dirty_rects dirty_rects对象。
 * @param {rect_t*} r 脏矩形。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t dirty_rects_add(dirty_rects_t* dirty_rects, const rect_t* r) {
  uint32_t i = 0;
  return_value_if_fail(dirty_rects != NULL && r != NULL, RET_BAD_PARAMS);

  if (r->w == 0 || r->h == 0) {
    return RET_OK;
  }

  if (dirty_rects->nr >= TK_MAX_DIRTY_RECT_NR) {
    for (i = 1; i < dirty_rects->nr; i++) {
      rect_merge(dirty_rects->rects, dirty_rects->rects + i);
    }
    dirty_rects->nr = 1;
  }

  rect_merge(&(dirty_rects->max), r);

  for (i = 0; i < dirty_rects->nr; i++) {
    rect_t* iter = dirty_rects->rects + i;
    if (rect_has_intersect(iter, r)) {
      rect_merge(iter, r);
      return dirty_rects_fix(dirty_rects);
    }
  }

  dirty_rects->rects[dirty_rects->nr++] = *r;

  return RET_OK;
}

/**
 * @method dirty_rects_set_debug
 * @export none
 * 设置是否开启调试模式。
 * @param {dirty_rects_t*} dirty_rects dirty_rects对象。
 * @param {bool_t} debug 是否开启调试模式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t dirty_rects_set_debug(dirty_rects_t* dirty_rects, bool_t debug) {
  return_value_if_fail(dirty_rects != NULL, RET_BAD_PARAMS);
  dirty_rects->debug = debug;
  return RET_OK;
}

/**
 * @method dirty_rects_set_profile
 * @export none
 * 设置是否开启profile模式。
 * @param {dirty_rects_t*} dirty_rects dirty_rects对象。
 * @param {bool_t} profile 是否开启profile模式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t dirty_rects_set_profile(dirty_rects_t* dirty_rects, bool_t profile) {
  return_value_if_fail(dirty_rects != NULL, RET_BAD_PARAMS);
  dirty_rects->profile = profile;
  return RET_OK;
}

/**
 * @method dirty_rects_set_disable_multiple
 * @export none
 * 设置是否关闭多脏矩形模式(方便对比测试和调试)。
 * @param {dirty_rects_t*} dirty_rects dirty_rects对象。
 * @param {bool_t} disable_multiple 是否关闭多脏矩形模式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
static inline ret_t dirty_rects_set_disable_multiple(dirty_rects_t* dirty_rects,
                                                     bool_t disable_multiple) {
  return_value_if_fail(dirty_rects != NULL, RET_BAD_PARAMS);
  dirty_rects->disable_multiple = disable_multiple;
  return RET_OK;
}

END_C_DECLS

#endif /*TK_DIRTY_RECTS_H*/
