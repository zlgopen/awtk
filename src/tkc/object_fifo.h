/**
 * File:   object_fifo.h
 * Author: AWTK Develop Team
 * Brief:  用于保存 fifo 结构的 object
 *
 * Copyright (c) 2024 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-06-22 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#ifndef TK_OBJECT_FIFO_H
#define TK_OBJECT_FIFO_H

#include "tkc/event.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @enum object_fifo_event_type_t
 * @prefix EVT_OBJECT_FIFO_
 * object_fifo事件类型。
 */
typedef enum _object_fifo_event_type_t {
  /**
   * @const EVT_OBJECT_FIFO_WILL_SET
   * 即将设置元素事件。
   */
  EVT_OBJECT_FIFO_WILL_SET = 0x3000,
  /**
   * @const EVT_OBJECT_FIFO_SET
   * 设置元素事件。
   */
  EVT_OBJECT_FIFO_SET,
  /**
   * @const EVT_OBJECT_FIFO_WILL_PUSH
   * 即将追加元素事件。
   */
  EVT_OBJECT_FIFO_WILL_PUSH,
  /**
   * @const EVT_OBJECT_FIFO_PUSH
   * 追加元素事件。
   */
  EVT_OBJECT_FIFO_PUSH,
  /**
   * @const EVT_OBJECT_FIFO_WILL_PUSH_HEAD
   * 即将在头部插入元素事件。
   */
  EVT_OBJECT_FIFO_WILL_PUSH_HEAD,
  /**
   * @const EVT_OBJECT_FIFO_PUSH_HEAD
   * 在头部插入元素事件。
   */
  EVT_OBJECT_FIFO_PUSH_HEAD,
  /**
   * @const EVT_OBJECT_FIFO_WILL_POP
   * 即将弹出元素事件。
   */
  EVT_OBJECT_FIFO_WILL_POP,
  /**
   * @const EVT_OBJECT_FIFO_POP
   * 弹出元素事件。
   */
  EVT_OBJECT_FIFO_POP,
  /**
   * @const EVT_OBJECT_FIFO_WILL_POP_TAIL
   * 即将从末尾弹出元素事件。
   */
  EVT_OBJECT_FIFO_WILL_POP_TAIL,
  /**
   * @const EVT_OBJECT_FIFO_POP_TAIL
   * 从末尾弹出元素事件。
   */
  EVT_OBJECT_FIFO_POP_TAIL,
  /**
   * @const EVT_OBJECT_FIFO_VALUE_CHANGE
   * 元素改变事件（内部使用，不受fifo的block_event属性影响）。
   */
  EVT_OBJECT_FIFO_VALUE_CHANGE,
} object_fifo_event_type_t;

/**
 * @class object_fifo_set_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 设置元素事件。
 */
typedef struct _object_fifo_set_event_t {
  event_t e;
  /**
   * @property {uint32_t} index
   * @annotation ["readable", "scriptable"]
   * 设置元素时的指定位置。
   */
  uint32_t index;
  /**
   * @property {uint32_t} nr
   * @annotation ["readable", "scriptable"]
   * 设置元素的个数。
   */
  uint32_t nr;
  /**
   * @property {void*} data
   * @annotation ["readable", "scriptable"]
   * 设置数据。
   */
  void* data;

  /*private*/
  void* ctx;
} object_fifo_set_event_t;

/**
 * @class object_fifo_push_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 追加元素事件。
 */
typedef struct _object_fifo_push_event_t {
  event_t e;
  /**
   * @property {uint32_t} nr
   * @annotation ["readable", "scriptable"]
   * 追加元素的个数。
   */
  uint32_t nr;
  /**
   * @property {void*} data
   * @annotation ["readable", "scriptable"]
   * 追加数据。
   */
  void* data;

  /*private*/
  void* ctx;
} object_fifo_push_event_t;

/**
 * @class object_fifo_push_head_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 在头部插入元素事件。
 */
typedef struct _object_fifo_push_head_event_t {
  event_t e;
  /**
   * @property {uint32_t} nr
   * @annotation ["readable", "scriptable"]
   * 插入元素的个数。
   */
  uint32_t nr;
  /**
   * @property {void*} data
   * @annotation ["readable", "scriptable"]
   * 插入数据。
   */
  void* data;

  /*private*/
  void* ctx;
} object_fifo_push_head_event_t;

/**
 * @class object_fifo_pop_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 弹出元素事件。
 */
typedef struct _object_fifo_pop_event_t {
  event_t e;
  /**
   * @property {uint32_t} nr
   * @annotation ["readable", "scriptable"]
   * 弹出元素的个数。
   */
  uint32_t nr;

  /*private*/
  void* ctx;
} object_fifo_pop_event_t;

/**
 * @class object_fifo_pop_tail_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 从末尾弹出元素事件。
 */
typedef struct _object_fifo_pop_tail_event_t {
  event_t e;
  /**
   * @property {uint32_t} nr
   * @annotation ["readable", "scriptable"]
   * 弹出元素的个数。
   */
  uint32_t nr;

  /*private*/
  void* ctx;
} object_fifo_pop_tail_event_t;

/**
 * @class object_fifo_value_change_event_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 值改变事件。
 */
typedef struct _object_fifo_value_change_event_t {
  event_t e;
  /**
   * @property {uint32_t} type
   * @annotation ["readable", "scriptable"]
   * 具体的事件类型。
   */
  uint32_t type;

  union {
    object_fifo_set_event_t set_event;
    object_fifo_push_event_t push_event;
    object_fifo_push_head_event_t push_head_event;
    object_fifo_pop_event_t pop_event;
    object_fifo_pop_tail_event_t pop_tail_event;
  } fifo_event;

  /*private*/
  void* ctx;
} object_fifo_value_change_event_t;

/**
 * @method object_fifo_set_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转object_fifo_event_set_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {object_fifo_set_event_t*} event对象。
 */
object_fifo_set_event_t* object_fifo_set_event_cast(event_t* event);

/**
 * @method object_fifo_set_event_init
 * 初始化object_fifo_set_event_t事件。
 * @param {object_fifo_set_event_t*} event event对象。
 * @param {uint32_t} etype 事件类型。
 * @param {void*} target 事件目标。
 *
 * @return {event_t*} event对象。
 */
event_t* object_fifo_set_event_init(object_fifo_set_event_t* event, uint32_t etype, void* target);

/**
 * @method object_fifo_push_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转object_fifo_push_event_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {object_fifo_push_event_t*} event对象。
 */
object_fifo_push_event_t* object_fifo_push_event_cast(event_t* event);

/**
 * @method object_fifo_push_event_init
 * 初始化object_fifo_push_event_init事件。
 * @param {object_fifo_push_event_t*} event event对象。
 * @param {uint32_t} etype 事件类型。
 * @param {void*} target 事件目标。
 *
 * @return {event_t*} event对象。
 */
event_t* object_fifo_push_event_init(object_fifo_push_event_t* event, uint32_t etype, void* target);

/**
 * @method object_fifo_push_head_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转object_fifo_push_head_event_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {object_fifo_push_head_event_t*} event对象。
 */
object_fifo_push_head_event_t* object_fifo_push_head_event_cast(event_t* event);

/**
 * @method object_fifo_push_head_event_init
 * 初始化object_fifo_push_head_event_t事件。
 * @param {object_fifo_push_head_event_t*} event event对象。
 * @param {uint32_t} etype 事件类型。
 * @param {void*} target 事件目标。
 *
 * @return {event_t*} event对象。
 */
event_t* object_fifo_push_head_event_init(object_fifo_push_head_event_t* event, uint32_t etype,
                                          void* target);

/**
 * @method object_fifo_pop_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转object_fifo_pop_event_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {object_fifo_pop_event_t*} event对象。
 */
object_fifo_pop_event_t* object_fifo_pop_event_cast(event_t* event);

/**
 * @method object_fifo_pop_event_init
 * 初始化object_fifo_pop_event_t事件。
 * @param {object_fifo_pop_event_t*} event event对象。
 * @param {uint32_t} etype 事件类型。
 * @param {void*} target 事件目标。
 *
 * @return {event_t*} event对象。
 */
event_t* object_fifo_pop_event_init(object_fifo_pop_event_t* event, uint32_t etype, void* target);

/**
 * @method object_fifo_pop_tail_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转object_fifo_pop_tail_event_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {object_fifo_pop_tail_event_t*} event对象。
 */
object_fifo_pop_tail_event_t* object_fifo_pop_tail_event_cast(event_t* event);

/**
 * @method object_fifo_pop_tail_event_init
 * 初始化object_fifo_pop_tail_event_t事件。
 * @param {object_fifo_pop_tail_event_t*} event event对象。
 * @param {uint32_t} etype 事件类型。
 * @param {void*} target 事件目标。
 *
 * @return {event_t*} event对象。
 */
event_t* object_fifo_pop_tail_event_init(object_fifo_pop_tail_event_t* event, uint32_t etype,
                                         void* target);

/**
 * @method object_fifo_value_change_event_cast
 * @annotation ["cast", "scriptable"]
 * 把event对象转object_fifo_value_change_event_t对象，主要给脚本语言使用。
 * @param {event_t*} event event对象。
 *
 * @return {object_fifo_value_change_event_t*} event对象。
 */
object_fifo_value_change_event_t* object_fifo_value_change_event_cast(event_t* event);

/**
 * @method object_fifo_value_change_event_init
 * 初始化object_fifo_value_change_event_t事件。
 * @param {object_fifo_value_change_event_t*} event event对象。
 * @param {uint32_t} etype 事件类型。
 * @param {void*} target 事件目标。
 *
 * @return {event_t*} event对象。
 */
event_t* object_fifo_value_change_event_init(object_fifo_value_change_event_t* event,
                                             uint32_t etype, void* target);

struct _object_fifo_t;
typedef struct _object_fifo_t object_fifo_t;

typedef void* (*object_fifo_get_t)(object_t* obj, uint32_t index);
typedef ret_t (*object_fifo_set_t)(object_t* obj, uint32_t index, const void* data, uint32_t nr);
typedef int (*object_fifo_compare_t)(object_t* obj, const void* a, const void* b);
typedef object_t* (*object_fifo_part_clone_t)(object_t* obj, uint32_t index, uint32_t nr);
typedef ret_t (*object_fifo_set_capacity_t)(object_t* obj, uint32_t capacity);

typedef struct _object_fifo_vtable_t {
  object_fifo_get_t get;
  object_fifo_set_t set;
  object_fifo_compare_t compare;
  object_fifo_part_clone_t part_clone;
  object_fifo_set_capacity_t set_capacity;
} object_fifo_vtable_t;

/**
 * @class object_fifo_t
 * @parent object_t
 *
 * FIFO，先进先出队列，环形缓存。
 * 可以使用 object_fifo_default_t 实例化该类。
 *
 */
struct _object_fifo_t {
  object_t obj;

  /**
   * @property {bool_t} block_event
   * @annotation ["readable","writable"]
   * 阻止分发 object_fifo_event 事件。
   */
  bool_t block_event;
  /**
   * @property {const object_fifo_vtable_t*} vt
   * @annotation ["readable"]
   * 虚函数表。
   */
  const object_fifo_vtable_t* vt;
};

/**
 * @method object_fifo_part_clone
 * clone部分。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {uint32_t} index 被clone元素在FIFO中的位置。
 * @param {uint32_t} nr 被clone元素的数量。
 *
 * @return {object_t*} 返回clone的对象。
 */
object_t* object_fifo_part_clone(object_t* obj, uint32_t index, uint32_t nr);

/**
 * @method object_fifo_get
 * 返回特定位置的元素。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {uint32_t} index 元素在FIFO中的位置。
 *
 * @return {void*} 如果找到，返回特定位置的元素，否则返回NULL。
 */
void* object_fifo_get(object_t* obj, uint32_t index);

/**
 * @method object_fifo_sort
 * 排序。
 * @param {object_t*} obj object_fifo对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL则使用内置的比较函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_fifo_sort(object_t* obj, tk_compare_t cmp);

/**
 * @method object_fifo_set
 * 设置特定位置开始的多个元素。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {uint32_t} index 元素在FIFO中的位置。
 * @param {const void*} data 元素数据。
 * @param {uint32_t} nr 元素数量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_fifo_set(object_t* obj, uint32_t index, const void* data, uint32_t nr);

/**
 * @method object_fifo_set_reverse
 * 设置特定位置开始的多个元素（反向）。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {uint32_t} index 元素在FIFO中的位置。
 * @param {const void*} data 元素数据。
 * @param {uint32_t} nr 元素数量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_fifo_set_reverse(object_t* obj, uint32_t index, const void* data, uint32_t nr);

/**
 * @method object_fifo_compare
 * 比较两个元素。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {const void*} a 元素a。
 * @param {const void*} b 元素b。
 *
 * @return {int} 两元素相等返回0。
 */
int object_fifo_compare(object_t* obj, const void* a, const void* b);

/**
 * @method object_fifo_find
 * 查找第一个满足条件的元素。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {void*} 如果找到，返回满足条件的对象，否则返回NULL。
 */
void* object_fifo_find(object_t* obj, void* ctx);

/**
 * @method object_fifo_find_index
 * 查找第一个满足条件的元素，并返回位置。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {int} 如果找到，返回满足条件的对象的位置，否则返回-1。
 */
int object_fifo_find_index(object_t* obj, void* ctx);

/**
 * @method object_fifo_bsearch_index
 * 二分查找(确保object_fifo有序)。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL则使用内置的比较函数。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {int32_t} 如果找到，返回满足条件的对象的位置，否则返回-1。
 */
int32_t object_fifo_bsearch_index(object_t* obj, tk_compare_t cmp, void* ctx);

/**
 * @method object_fifo_bsearch_index_ex
 * 二分查找(确保object_fifo有序)。元素不存在时，返回low索引。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL则使用内置的比较函数。
 * @param {void*} ctx 比较函数的上下文。
 * @param {int32_t*} ret_low low索引。
 *
 * @return {int32_t} 如果找到，返回满足条件的对象的位置，否则返回-1。
 */
int32_t object_fifo_bsearch_index_ex(object_t* obj, tk_compare_t cmp, void* ctx, int32_t* ret_low);

/**
 * @method object_fifo_bsearch
 * 二分查找(确保object_fifo有序)。。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {tk_compare_t} cmp 比较函数，为NULL则使用内置的比较函数。
 * @param {void*} ctx 比较函数的上下文。
 *
 * @return {void*} 如果找到，返回满足条件的对象，否则返回NULL。
 */
void* object_fifo_bsearch(object_t* obj, tk_compare_t cmp, void* ctx);

/**
 * @method object_fifo_push
 * 在尾巴追加一个元素。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {const void*} data 待追加的元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_fifo_push(object_t* obj, const void* data);

/**
 * @method object_fifo_pop
 * 弹出第一个元素。
 *
 * @param {object_t*} obj object_fifo对象。
 *
 * @return {void*} 成功返回第一个元素，失败返回NULL。
 */
void* object_fifo_pop(object_t* obj);

/**
 * @method object_fifo_npush
 * 在尾巴追加多个元素。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {const void*} data 待追加的元素。
 * @param {uint32_t} nr 待追加的元素个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_fifo_npush(object_t* obj, const void* data, uint32_t nr);

/**
 * @method object_fifo_push_head
 * 在头部插入一个元素。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {const void*} data 待插入的元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_fifo_push_head(object_t* obj, const void* data);

/**
 * @method object_fifo_npush_head
 * 在头部插入多个元素。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {const void*} data 待插入的元素。
 * @param {uint32_t} nr 待插入的元素个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_fifo_npush_head(object_t* obj, const void* data, uint32_t nr);

/**
 * @method object_fifo_npop
 * 弹出开头多个元素。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {uint32_t} nr 待弹出的元素个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_fifo_npop(object_t* obj, uint32_t nr);

/**
 * @method object_fifo_pop_tail
 * 弹出最后一个元素。
 *
 * @param {object_t*} obj object_fifo对象。
 *
 * @return {void*} 成功返回最后一个元素，失败返回NULL。
 */
void* object_fifo_pop_tail(object_t* obj);

/**
 * @method object_fifo_npop_tail
 * 弹出末尾多个元素。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {uint32_t} nr 待弹出的元素个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_fifo_npop_tail(object_t* obj, uint32_t nr);

/**
 * @method object_fifo_clear
 * 清除全部元素。
 *
 * @param {object_t*} obj object_fifo对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_fifo_clear(object_t* obj);

/**
 * @method object_fifo_set_capacity
 * 设置FIFO容量（会导致FIFO被清空）。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {uint32_t} capacity 容量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_fifo_set_capacity(object_t* obj, uint32_t capacity);

/**
 * @method object_fifo_set_block_event
 * 设置 block_event 属性。
 *
 * @param {object_t*} obj object_fifo对象。
 * @param {bool_t} block_event 是否阻止分发事件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_fifo_set_block_event(object_t* obj, bool_t block_event);

/**
 * @method object_fifo_get_capacity
 * 获取容量。
 * @param {tk_object_t*} obj object_fifo对象。
 *
 * @return {uint32_t} 返回结果。
 */
uint32_t object_fifo_get_capacity(tk_object_t* obj);

/**
 * @method object_fifo_get_size
 * 获取大小。
 * @param {tk_object_t*} obj object_fifo对象。
 *
 * @return {uint32_t} 返回结果。
 */
uint32_t object_fifo_get_size(tk_object_t* obj);

/**
 * @method object_fifo_get_cursor
 * 获取索引。
 * @param {tk_object_t*} obj object_fifo对象。
 *
 * @return {uint32_t} 返回结果。
 */
uint32_t object_fifo_get_cursor(tk_object_t* obj);

/**
 * @method object_fifo_get_unit_size
 * 获取单位大小。
 * @param {tk_object_t*} obj object_fifo对象。
 *
 * @return {uint32_t} 返回结果。
 */
uint32_t object_fifo_get_unit_size(tk_object_t* obj);

/**
 * @method object_fifo_get_buffer
 * 获取缓冲区。
 * @param {tk_object_t*} obj object_fifo对象。
 *
 * @return {uint8_t*} 返回结果。
 */
uint8_t* object_fifo_get_buffer(tk_object_t* obj);

#define OBJECT_FIFO_GET_CAPACITY(obj) object_fifo_get_capacity(OBJECT(obj))
#define OBJECT_FIFO_GET_SIZE(obj) object_fifo_get_size(OBJECT(obj))
#define OBJECT_FIFO_GET_CURSOR(obj) object_fifo_get_cursor(OBJECT(obj))
#define OBJECT_FIFO_GET_UNIT_SIZE(obj) object_fifo_get_unit_size(OBJECT(obj))
#define OBJECT_FIFO_GET_BUFFER(obj) object_fifo_get_buffer(OBJECT(obj))

#define OBJECT_FIFO(obj) ((object_fifo_t*)(obj))

#define OBJECT_FIFO_PROP_CAPACITY "capacity"
#define OBJECT_FIFO_PROP_SIZE "size"
#define OBJECT_FIFO_PROP_CURSOR "cursor"
#define OBJECT_FIFO_PROP_UNIT_SIZE "unit_size"
#define OBJECT_FIFO_PROP_BUFFER "buffer"

END_C_DECLS

#endif /*TK_OBJECT_FIFO_H*/
