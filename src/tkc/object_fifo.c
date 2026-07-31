/**
 * File:   object_fifo.c
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

#include "tkc/mem.h"
#include "object_fifo.h"

object_fifo_set_event_t* object_fifo_set_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type >= EVT_OBJECT_FIFO_WILL_SET, NULL);
  return_value_if_fail(event->type <= EVT_OBJECT_FIFO_SET, NULL);
  return_value_if_fail(event->size == sizeof(object_fifo_set_event_t), NULL);

  return (object_fifo_set_event_t*)event;
}

event_t* object_fifo_set_event_init(object_fifo_set_event_t* event, uint32_t etype, void* target) {
  return_value_if_fail(event != NULL, NULL);

  memset(event, 0x00, sizeof(*event));
  event->e = event_init(etype, target);
  event->e.size = sizeof(*event);

  return (event_t*)event;
}

object_fifo_push_event_t* object_fifo_push_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type >= EVT_OBJECT_FIFO_WILL_PUSH, NULL);
  return_value_if_fail(event->type <= EVT_OBJECT_FIFO_PUSH, NULL);
  return_value_if_fail(event->size == sizeof(object_fifo_push_event_t), NULL);

  return (object_fifo_push_event_t*)event;
}

event_t* object_fifo_push_event_init(object_fifo_push_event_t* event, uint32_t etype,
                                     void* target) {
  return_value_if_fail(event != NULL, NULL);

  memset(event, 0x00, sizeof(*event));
  event->e = event_init(etype, target);
  event->e.size = sizeof(*event);

  return (event_t*)event;
}

object_fifo_push_head_event_t* object_fifo_push_head_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type >= EVT_OBJECT_FIFO_WILL_PUSH_HEAD, NULL);
  return_value_if_fail(event->type <= EVT_OBJECT_FIFO_PUSH_HEAD, NULL);
  return_value_if_fail(event->size == sizeof(object_fifo_push_head_event_t), NULL);

  return (object_fifo_push_head_event_t*)event;
}

event_t* object_fifo_push_head_event_init(object_fifo_push_head_event_t* event, uint32_t etype,
                                          void* target) {
  return_value_if_fail(event != NULL, NULL);

  memset(event, 0x00, sizeof(*event));
  event->e = event_init(etype, target);
  event->e.size = sizeof(*event);

  return (event_t*)event;
}

object_fifo_pop_event_t* object_fifo_pop_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type >= EVT_OBJECT_FIFO_WILL_POP, NULL);
  return_value_if_fail(event->type <= EVT_OBJECT_FIFO_POP, NULL);
  return_value_if_fail(event->size == sizeof(object_fifo_pop_event_t), NULL);

  return (object_fifo_pop_event_t*)event;
}

event_t* object_fifo_pop_event_init(object_fifo_pop_event_t* event, uint32_t etype, void* target) {
  return_value_if_fail(event != NULL, NULL);

  memset(event, 0x00, sizeof(*event));
  event->e = event_init(etype, target);
  event->e.size = sizeof(*event);

  return (event_t*)event;
}

object_fifo_pop_tail_event_t* object_fifo_pop_tail_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type >= EVT_OBJECT_FIFO_WILL_POP_TAIL, NULL);
  return_value_if_fail(event->type <= EVT_OBJECT_FIFO_POP_TAIL, NULL);
  return_value_if_fail(event->size == sizeof(object_fifo_pop_tail_event_t), NULL);

  return (object_fifo_pop_tail_event_t*)event;
}

event_t* object_fifo_pop_tail_event_init(object_fifo_pop_tail_event_t* event, uint32_t etype,
                                         void* target) {
  return_value_if_fail(event != NULL, NULL);

  memset(event, 0x00, sizeof(*event));
  event->e = event_init(etype, target);
  event->e.size = sizeof(*event);

  return (event_t*)event;
}

object_fifo_value_change_event_t* object_fifo_value_change_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type == EVT_OBJECT_FIFO_VALUE_CHANGE, NULL);
  return_value_if_fail(event->size == sizeof(object_fifo_value_change_event_t), NULL);

  return (object_fifo_value_change_event_t*)event;
}

event_t* object_fifo_value_change_event_init(object_fifo_value_change_event_t* event,
                                             uint32_t etype, void* target) {
  return_value_if_fail(event != NULL, NULL);

  memset(event, 0x00, sizeof(*event));
  event->e = event_init(etype, target);
  event->e.size = sizeof(*event);

  return (event_t*)event;
}

static bool_t object_fifo_is_valid(object_t* obj, uint32_t index) {
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  return_value_if_fail(object_fifo != NULL && object_fifo->vt != NULL, FALSE);
  return index >= 0 && index < OBJECT_FIFO_GET_SIZE(obj);
}

static uint32_t object_fifo_to_abs_index(object_t* obj, uint32_t index) {
  return_value_if_fail(obj != NULL, 0);

  uint32_t size = OBJECT_FIFO_GET_SIZE(obj);
  uint32_t cursor = OBJECT_FIFO_GET_CURSOR(obj);
  uint32_t capacity = OBJECT_FIFO_GET_CAPACITY(obj);

  return (cursor - size + 1 + capacity + index) % capacity;
}

void* object_fifo_get(object_t* obj, uint32_t index) {
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  return_value_if_fail(object_fifo_is_valid(obj, index), NULL);

  if (OBJECT_FIFO_GET_SIZE(obj) == 0) {
    return NULL;
  }

  if (object_fifo->vt->get) {
    uint32_t abs_index = object_fifo_to_abs_index(obj, index);
    return object_fifo->vt->get(obj, abs_index);
  }

  return NULL;
}

int object_fifo_compare(object_t* obj, const void* a, const void* b) {
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  return_value_if_fail(object_fifo != NULL && object_fifo->vt != NULL, -1);

  if (object_fifo->vt->compare) {
    return object_fifo->vt->compare(obj, a, b);
  }

  return -1;
}

object_t* object_fifo_part_clone(object_t* obj, uint32_t index, uint32_t nr) {
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  return_value_if_fail(object_fifo_is_valid(obj, index), NULL);
  return_value_if_fail(object_fifo_is_valid(obj, index + nr - 1), NULL);

  if (object_fifo->vt->part_clone) {
    uint32_t abs_index = object_fifo_to_abs_index(obj, index);
    return object_fifo->vt->part_clone(obj, abs_index, nr);
  }

  return NULL;
}

static ret_t object_fifo_dispatch_set_event(object_fifo_t* object_fifo, uint32_t type,
                                            uint32_t index, const void* data, uint32_t nr) {
  object_fifo_set_event_t evt;
  event_t* e = object_fifo_set_event_init(&evt, type, (void*)object_fifo);

  evt.index = index;
  evt.data = (void*)data;
  evt.nr = nr;
  if (!object_fifo->block_event) {
    return emitter_dispatch(EMITTER(object_fifo), e);
  } else {
    object_fifo_value_change_event_t value_change_evt;
    event_t* value_change_e;
    value_change_e = object_fifo_value_change_event_init(
        &value_change_evt, EVT_OBJECT_FIFO_VALUE_CHANGE, (void*)object_fifo);
    value_change_evt.fifo_event.set_event = evt;
    value_change_evt.type = type;
    return emitter_dispatch(EMITTER(object_fifo), value_change_e);
  }

  return RET_OK;
}

static ret_t object_fifo_dispatch_push_event(object_fifo_t* object_fifo, uint32_t type,
                                             const void* data, uint32_t nr) {
  object_fifo_push_event_t evt;
  event_t* e = object_fifo_push_event_init(&evt, type, (void*)object_fifo);

  evt.nr = nr;
  evt.data = (void*)data;
  if (!object_fifo->block_event) {
    return emitter_dispatch(EMITTER(object_fifo), e);
  } else {
    object_fifo_value_change_event_t value_change_evt;
    event_t* value_change_e;
    value_change_e = object_fifo_value_change_event_init(
        &value_change_evt, EVT_OBJECT_FIFO_VALUE_CHANGE, (void*)object_fifo);
    value_change_evt.fifo_event.push_event = evt;
    value_change_evt.type = type;
    return emitter_dispatch(EMITTER(object_fifo), value_change_e);
  }

  return RET_OK;
}

static ret_t object_fifo_dispatch_pop_event(object_fifo_t* object_fifo, uint32_t type,
                                            uint32_t nr) {
  object_fifo_pop_event_t evt;
  event_t* e = object_fifo_pop_event_init(&evt, type, (void*)object_fifo);

  evt.nr = nr;
  if (!object_fifo->block_event) {
    return emitter_dispatch(EMITTER(object_fifo), e);
  } else {
    object_fifo_value_change_event_t value_change_evt;
    event_t* value_change_e;
    value_change_e = object_fifo_value_change_event_init(
        &value_change_evt, EVT_OBJECT_FIFO_VALUE_CHANGE, (void*)object_fifo);
    value_change_evt.fifo_event.pop_event = evt;
    value_change_evt.type = type;
    return emitter_dispatch(EMITTER(object_fifo), value_change_e);
  }

  return RET_OK;
}

static ret_t object_fifo_dispatch_push_head_event(object_fifo_t* object_fifo, uint32_t type,
                                                  const void* data, uint32_t nr) {
  object_fifo_push_head_event_t evt;
  event_t* e = object_fifo_push_head_event_init(&evt, type, (void*)object_fifo);

  evt.nr = nr;
  evt.data = (void*)data;
  if (!object_fifo->block_event) {
    return emitter_dispatch(EMITTER(object_fifo), e);
  } else {
    object_fifo_value_change_event_t value_change_evt;
    event_t* value_change_e;
    value_change_e = object_fifo_value_change_event_init(
        &value_change_evt, EVT_OBJECT_FIFO_VALUE_CHANGE, (void*)object_fifo);
    value_change_evt.fifo_event.push_head_event = evt;
    value_change_evt.type = type;
    return emitter_dispatch(EMITTER(object_fifo), value_change_e);
  }

  return RET_OK;
}

static ret_t object_fifo_dispatch_pop_tail_event(object_fifo_t* object_fifo, uint32_t type,
                                                 uint32_t nr) {
  object_fifo_pop_tail_event_t evt;
  event_t* e = object_fifo_pop_tail_event_init(&evt, type, (void*)object_fifo);

  evt.nr = nr;
  if (!object_fifo->block_event) {
    return emitter_dispatch(EMITTER(object_fifo), e);
  } else {
    object_fifo_value_change_event_t value_change_evt;
    event_t* value_change_e;
    value_change_e = object_fifo_value_change_event_init(
        &value_change_evt, EVT_OBJECT_FIFO_VALUE_CHANGE, (void*)object_fifo);
    value_change_evt.fifo_event.pop_tail_event = evt;
    value_change_evt.type = type;
    return emitter_dispatch(EMITTER(object_fifo), value_change_e);
  }

  return RET_OK;
}

static void object_fifo_quick_sort_impl(object_t* obj, size_t left, size_t right, tk_compare_t cmp,
                                        uint8_t* pivot) {
  size_t save_left = left;
  size_t save_right = right;
  int32_t result = 0;
  uint32_t unit_size = OBJECT_FIFO_GET_UNIT_SIZE(obj);
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  void* y = NULL;

  memcpy(pivot, object_fifo->vt->get(obj, left), unit_size);

  while (left < right) {
    y = object_fifo->vt->get(obj, right);
    if (cmp) {
      result = cmp(y, pivot);
    } else {
      result = object_fifo->vt->compare(obj, y, pivot);
    }

    while (result >= 0 && left < right) {
      right--;

      y = object_fifo->vt->get(obj, right);
      if (cmp) {
        result = cmp(y, pivot);
      } else {
        result = object_fifo->vt->compare(obj, y, pivot);
      }
    }

    if (left != right) {
      object_fifo->vt->set(obj, left, y, 1);
      left++;
    }

    y = object_fifo->vt->get(obj, left);
    if (cmp) {
      result = cmp(y, pivot);
    } else {
      result = object_fifo->vt->compare(obj, y, pivot);
    }

    while (result <= 0 && left < right) {
      left++;

      y = object_fifo->vt->get(obj, left);
      if (cmp) {
        result = cmp(y, pivot);
      } else {
        result = object_fifo->vt->compare(obj, y, pivot);
      }
    }

    if (left != right) {
      object_fifo->vt->set(obj, right, y, 1);
      right--;
    }
  }

  object_fifo->vt->set(obj, left, pivot, 1);

  if (save_left < left) {
    object_fifo_quick_sort_impl(obj, save_left, left - 1, cmp, pivot);
  }

  if (save_right > left) {
    object_fifo_quick_sort_impl(obj, left + 1, save_right, cmp, pivot);
  }

  return;
}

ret_t object_fifo_sort(object_t* obj, tk_compare_t cmp) {
  ret_t ret = RET_OK;
  uint32_t size = 0;
  uint32_t unit_size = 0;
  uint8_t* pivot = NULL;
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  return_value_if_fail(object_fifo != NULL && object_fifo->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(object_fifo->vt->get != NULL, RET_BAD_PARAMS);
  return_value_if_fail(object_fifo->vt->set != NULL, RET_BAD_PARAMS);
  return_value_if_fail(object_fifo->vt->compare != NULL || cmp != NULL, RET_BAD_PARAMS);

  size = OBJECT_FIFO_GET_SIZE(obj);
  if (size > 1) {
    void* buffer = OBJECT_FIFO_GET_BUFFER(obj);
    unit_size = OBJECT_FIFO_GET_UNIT_SIZE(obj);
    pivot = TKMEM_ALLOC(unit_size);
    return_value_if_fail(pivot != NULL, RET_OOM);

    object_fifo_dispatch_set_event(object_fifo, EVT_OBJECT_FIFO_WILL_SET, 0, buffer, size);
    object_fifo_quick_sort_impl(obj, 0, size - 1, cmp, pivot);
    object_set_prop_uint32(obj, OBJECT_FIFO_PROP_CURSOR, size - 1);
    object_fifo_dispatch_set_event(object_fifo, EVT_OBJECT_FIFO_SET, 0, buffer, size);
    TKMEM_FREE(pivot);
  }

  return RET_OK;
}

ret_t object_fifo_set(object_t* obj, uint32_t index, const void* data, uint32_t nr) {
  ret_t ret = RET_NOT_IMPL;
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  uint32_t size, capacity, unit_size;
  return_value_if_fail(object_fifo_is_valid(obj, index) || index == 0, RET_BAD_PARAMS);

  if (object_fifo->vt->set) {
    size = OBJECT_FIFO_GET_SIZE(obj);
    capacity = OBJECT_FIFO_GET_CAPACITY(obj);
    unit_size = OBJECT_FIFO_GET_UNIT_SIZE(obj);

    /* 去除多余数据 */
    if (nr >= capacity) {
      index = 0;
      data = (void*)((uint8_t*)data + unit_size * (nr - capacity));
      nr = capacity;
    }

    ret = object_fifo_dispatch_set_event(object_fifo, EVT_OBJECT_FIFO_WILL_SET, index, data, nr);

    if (ret == RET_STOP) {
      ret = RET_OK;
    } else {
      uint32_t abs_index = 0;

      /* 使用空元素占位 */
      if (index + nr > size) {
        bool_t save = object_fifo->block_event;
        object_fifo_set_block_event(obj, TRUE);
        object_fifo_npush(obj, NULL, index + nr - size);
        object_fifo_set_block_event(obj, save);
      }

      abs_index = object_fifo_to_abs_index(obj, index);
      ret = object_fifo->vt->set(obj, abs_index, data, nr);
      object_fifo_dispatch_set_event(object_fifo, EVT_OBJECT_FIFO_SET, index, data, nr);
    }
  }

  return ret;
}

ret_t object_fifo_set_reverse(object_t* obj, uint32_t index, const void* data, uint32_t nr) {
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  uint32_t size = OBJECT_FIFO_GET_SIZE(obj);
  index = size >= index + 1 ? (size - index - 1) : 0;

  return object_fifo_set(obj, index, data, nr);
}

void* object_fifo_find(object_t* obj, void* ctx) {
  uint32_t i = 0;
  return_value_if_fail(obj != NULL, NULL);

  for (i = 0; i < OBJECT_FIFO_GET_SIZE(obj); i++) {
    void* iter = object_fifo_get(obj, i);
    if (object_fifo_compare(obj, iter, ctx) == 0) {
      return iter;
    }
  }

  return NULL;
}

int object_fifo_find_index(object_t* obj, void* ctx) {
  uint32_t i = 0;
  return_value_if_fail(obj != NULL, -1);

  for (i = 0; i < OBJECT_FIFO_GET_SIZE(obj); i++) {
    void* iter = object_fifo_get(obj, i);
    if (object_fifo_compare(obj, iter, ctx) == 0) {
      return i;
    }
  }

  return -1;
}

int32_t object_fifo_bsearch_index_ex(object_t* obj, tk_compare_t cmp, void* ctx, int32_t* ret_low) {
  int32_t low = 0;
  int32_t mid = 0;
  int32_t high = 0;
  int32_t result = 0;
  uint32_t size = 0;
  void* iter = NULL;
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  return_value_if_fail(object_fifo != NULL && object_fifo->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(object_fifo->vt->get != NULL, RET_BAD_PARAMS);

  size = OBJECT_FIFO_GET_SIZE(obj);
  if (size == 0) {
    return -1;
  }

  high = size - 1;
  while (low <= high) {
    mid = low + ((high - low) >> 1);
    iter = object_fifo->vt->get(obj, object_fifo_to_abs_index(obj, mid));

    if (cmp != NULL) {
      result = cmp(iter, ctx);
    } else if (object_fifo->vt->compare != NULL) {
      result = object_fifo->vt->compare(obj, iter, ctx);
    } else {
      result = 0;
    }

    if (result == 0) {
      return mid;
    } else if (result < 0) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  if (ret_low != NULL) {
    *ret_low = low;
  }

  return -1;
}

int32_t object_fifo_bsearch_index(object_t* obj, tk_compare_t cmp, void* ctx) {
  return object_fifo_bsearch_index_ex(obj, cmp, ctx, NULL);
}

void* object_fifo_bsearch(object_t* obj, tk_compare_t cmp, void* ctx) {
  int32_t index = object_fifo_bsearch_index(obj, cmp, ctx);

  if (index >= 0) {
    return object_fifo_get(obj, index);
  } else {
    return NULL;
  }
}

ret_t object_fifo_push(object_t* obj, const void* data) {
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  return object_fifo_npush(obj, data, 1);
}

void* object_fifo_pop(object_t* obj) {
  void* data = NULL;
  return_value_if_fail(obj != NULL, NULL);

  data = object_fifo_get(obj, 0);
  return_value_if_fail(data != NULL, NULL);

  object_fifo_npop(obj, 1);

  return data;
}

ret_t object_fifo_npush(object_t* obj, const void* data, uint32_t nr) {
  ret_t ret = RET_NOT_IMPL;
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  return_value_if_fail(object_fifo != NULL && object_fifo->vt != NULL, RET_BAD_PARAMS);

  if (object_fifo->vt->set) {
    uint32_t head_index;
    uint32_t size = OBJECT_FIFO_GET_SIZE(obj);
    uint32_t cursor = OBJECT_FIFO_GET_CURSOR(obj);
    uint32_t capacity = OBJECT_FIFO_GET_CAPACITY(obj);

    ret = object_fifo_dispatch_push_event(object_fifo, EVT_OBJECT_FIFO_WILL_PUSH, data, nr);
    if (ret == RET_STOP) {
      ret = RET_OK;
    } else {
      if (size) {
        cursor = (cursor + nr) % capacity;
      } else {
        cursor = (cursor + nr - 1) % capacity;
      }

      size = tk_min(size + nr, capacity);
      object_set_prop_uint32(obj, OBJECT_FIFO_PROP_SIZE, size);
      object_set_prop_uint32(obj, OBJECT_FIFO_PROP_CURSOR, cursor);

      head_index = (cursor - tk_min(nr, capacity) + 1 + capacity) % capacity;
      ret = object_fifo->vt->set(obj, head_index, data, nr);
      object_fifo_dispatch_push_event(object_fifo, EVT_OBJECT_FIFO_PUSH, data, nr);
    }
  }

  return ret;
}

ret_t object_fifo_push_head(object_t* obj, const void* data) {
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  return object_fifo_npush_head(obj, data, 1);
}

ret_t object_fifo_npush_head(object_t* obj, const void* data, uint32_t nr) {
  ret_t ret = RET_OK;
  uint32_t size = 0;
  uint32_t capacity = 0;
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  return_value_if_fail(object_fifo != NULL && object_fifo->vt != NULL, RET_BAD_PARAMS);

  size = OBJECT_FIFO_GET_SIZE(obj);
  capacity = OBJECT_FIFO_GET_CAPACITY(obj);

  ret = object_fifo_dispatch_push_head_event(object_fifo, EVT_OBJECT_FIFO_WILL_PUSH_HEAD, data, nr);
  if (ret == RET_STOP) {
    ret = RET_OK;
  } else {
    size = tk_min(size + nr, capacity);
    object_set_prop_uint32(obj, OBJECT_FIFO_PROP_SIZE, size);

    object_fifo_set(obj, 0, data, nr);
    object_fifo_dispatch_push_head_event(object_fifo, EVT_OBJECT_FIFO_PUSH_HEAD, data, nr);
  }

  return ret;
}

ret_t object_fifo_npop(object_t* obj, uint32_t nr) {
  ret_t ret = RET_OK;
  uint32_t size = 0;
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  return_value_if_fail(object_fifo != NULL && object_fifo->vt != NULL, RET_BAD_PARAMS);

  size = OBJECT_FIFO_GET_SIZE(obj);
  return_value_if_fail(nr > 0 && nr <= size, RET_BAD_PARAMS);

  ret = object_fifo_dispatch_pop_event(object_fifo, EVT_OBJECT_FIFO_WILL_POP, nr);
  if (ret == RET_STOP) {
    ret = RET_OK;
  } else {
    object_set_prop_uint32(obj, OBJECT_FIFO_PROP_SIZE, size - nr);
    object_fifo_dispatch_pop_event(object_fifo, EVT_OBJECT_FIFO_POP, nr);
  }

  return ret;
}

void* object_fifo_pop_tail(object_t* obj) {
  void* data = NULL;
  uint32_t size = 0;
  return_value_if_fail(obj != NULL, NULL);

  size = OBJECT_FIFO_GET_SIZE(obj);
  return_value_if_fail(size > 0, NULL);

  data = object_fifo_get(obj, size - 1);
  return_value_if_fail(data != NULL, NULL);

  object_fifo_npop_tail(obj, 1);

  return data;
}

ret_t object_fifo_npop_tail(object_t* obj, uint32_t nr) {
  ret_t ret = RET_OK;
  uint32_t size = 0;
  uint32_t capacity = 0;
  uint32_t cursor = 0;
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  return_value_if_fail(object_fifo != NULL && object_fifo->vt != NULL, RET_BAD_PARAMS);

  size = OBJECT_FIFO_GET_SIZE(obj);
  capacity = OBJECT_FIFO_GET_CAPACITY(obj);
  cursor = OBJECT_FIFO_GET_CURSOR(obj);
  return_value_if_fail(nr > 0 && nr <= size, RET_BAD_PARAMS);

  ret = object_fifo_dispatch_pop_tail_event(object_fifo, EVT_OBJECT_FIFO_WILL_POP_TAIL, nr);
  if (ret == RET_STOP) {
    ret = RET_OK;
  } else {
    uint32_t new_size = size - nr;
    uint32_t new_cursor = 0;

    if (new_size > 0) {
      new_cursor = (cursor - nr + capacity) % capacity;
    } else {
      new_cursor = 0;
    }

    object_set_prop_uint32(obj, OBJECT_FIFO_PROP_SIZE, new_size);
    object_set_prop_uint32(obj, OBJECT_FIFO_PROP_CURSOR, new_cursor);
    object_fifo_dispatch_pop_tail_event(object_fifo, EVT_OBJECT_FIFO_POP_TAIL, nr);
  }

  return ret;
}

ret_t object_fifo_clear(object_t* obj) {
  ret_t ret = RET_OK;
  uint32_t size = 0;
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  return_value_if_fail(object_fifo != NULL && object_fifo->vt != NULL, RET_BAD_PARAMS);

  size = OBJECT_FIFO_GET_SIZE(obj);
  if (size == 0) {
    return RET_OK;
  }

  ret = object_fifo_dispatch_pop_event(object_fifo, EVT_OBJECT_FIFO_WILL_POP, size);
  if (ret == RET_STOP) {
    ret = RET_OK;
  } else {
    object_set_prop_uint32(obj, OBJECT_FIFO_PROP_SIZE, 0);
    object_set_prop_uint32(obj, OBJECT_FIFO_PROP_CURSOR, 0);
    object_fifo_dispatch_pop_event(object_fifo, EVT_OBJECT_FIFO_POP, size);
  }

  return ret;
}

ret_t object_fifo_set_capacity(object_t* obj, uint32_t capacity) {
  ret_t ret = RET_NOT_IMPL;
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  return_value_if_fail(object_fifo != NULL && object_fifo->vt != NULL, RET_BAD_PARAMS);

  if (object_fifo->vt->set_capacity) {
    ret = object_fifo->vt->set_capacity(obj, capacity);
  }

  return ret;
}

ret_t object_fifo_set_block_event(object_t* obj, bool_t block_event) {
  object_fifo_t* object_fifo = OBJECT_FIFO(obj);
  return_value_if_fail(object_fifo != NULL, RET_BAD_PARAMS);

  object_fifo->block_event = block_event;

  return RET_OK;
}

uint32_t object_fifo_get_capacity(tk_object_t* obj) {
  return object_get_prop_uint32(obj, OBJECT_FIFO_PROP_CAPACITY, 0);
}

uint32_t object_fifo_get_size(tk_object_t* obj) {
  return object_get_prop_uint32(obj, OBJECT_FIFO_PROP_SIZE, 0);
}

uint32_t object_fifo_get_cursor(tk_object_t* obj) {
  return object_get_prop_uint32(obj, OBJECT_FIFO_PROP_CURSOR, 0);
}

uint32_t object_fifo_get_unit_size(tk_object_t* obj) {
  return object_get_prop_uint32(obj, OBJECT_FIFO_PROP_UNIT_SIZE, 0);
}

uint8_t* object_fifo_get_buffer(tk_object_t* obj) {
  return object_get_prop_pointer(obj, OBJECT_FIFO_PROP_BUFFER);
}
