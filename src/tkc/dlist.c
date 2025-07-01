/**
 * File:   dlist.c
 * Author: AWTK Develop Team
 * Brief:  doubly linked list
 *
 * Copyright (c) 2024 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-09-30 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/dlist.h"

static dlist_node_t* dlist_create_node(dlist_t* dlist, void* data) {
  dlist_node_t* ret = NULL;
  return_value_if_fail(dlist != NULL, NULL);

  if (dlist->node_allocator != NULL) {
    ret = MEM_ALLOCATOR_ALLOC(dlist->node_allocator, sizeof(dlist_node_t));
  } else {
    ret = TKMEM_ALLOC(sizeof(dlist_node_t));
  }
  return_value_if_fail(ret != NULL, NULL);

  memset(ret, 0, sizeof(*ret));

  ret->data = data;

  return ret;
}

static ret_t dlist_destroy_node(dlist_t* dlist, dlist_node_t* node) {
  return_value_if_fail(dlist != NULL && node != NULL, RET_OK);

  if (node->data != NULL) {
    dlist->destroy(node->data);
  }
  if (dlist->node_allocator != NULL) {
    MEM_ALLOCATOR_FREE(dlist->node_allocator, node);
  } else {
    TKMEM_FREE(node);
  }

  return RET_OK;
}

dlist_t* dlist_create(tk_destroy_t destroy, tk_compare_t compare) {
  dlist_t* dlist = TKMEM_ZALLOC(dlist_t);
  return_value_if_fail(dlist != NULL, NULL);

  return dlist_init(dlist, destroy, compare);
}

dlist_t* dlist_init(dlist_t* dlist, tk_destroy_t destroy, tk_compare_t compare) {
  return_value_if_fail(dlist != NULL, NULL);

  memset(dlist, 0, sizeof(dlist_t));

  dlist->destroy = destroy != NULL ? destroy : dummy_destroy;
  dlist->compare = compare != NULL ? compare : pointer_compare;

  return dlist;
}

void* dlist_find_ex(dlist_t* dlist, tk_compare_t compare, void* ctx, bool_t reverse) {
  tk_compare_t real_compare = NULL;
  return_value_if_fail(dlist != NULL, NULL);

  real_compare = (compare != NULL) ? compare : dlist->compare;

  if (!reverse) {
    dlist_node_t* iter = dlist->first;
    while (iter != NULL) {
      if (real_compare(iter->data, ctx) == 0) {
        return iter->data;
      }
      iter = iter->next;
    }
  } else {
    dlist_node_t* iter = dlist->last;
    while (iter != NULL) {
      if (real_compare(iter->data, ctx) == 0) {
        return iter->data;
      }
      iter = iter->prev;
    }
  }

  return NULL;
}

void* dlist_find(dlist_t* dlist, void* ctx) {
  return dlist_find_ex(dlist, NULL, ctx, FALSE);
}

void* dlist_find_last(dlist_t* dlist, void* ctx) {
  return dlist_find_ex(dlist, NULL, ctx, TRUE);
}

static ret_t dlist_insert_node(dlist_t* dlist, dlist_node_t* node, dlist_node_t* next) {
  return_value_if_fail(dlist != NULL && node != NULL, RET_BAD_PARAMS);

  if (dlist->first == NULL || dlist->last == NULL) {
    dlist->first = dlist->last = node;
  } else if (next == NULL) {
    /* append */
    dlist->last->next = node;
    node->prev = dlist->last;
    dlist->last = node;
  } else if (next == dlist->first) {
    /* prepend */
    dlist->first->prev = node;
    node->next = dlist->first;
    dlist->first = node;
  } else {
    node->next = next;
    node->prev = next->prev;

    ENSURE(next->prev != NULL);
    next->prev->next = node;
    next->prev = node;
  }

  dlist->size++;

  return RET_OK;
}

static ret_t dlist_remove_node(dlist_t* dlist, dlist_node_t* node) {
  return_value_if_fail(dlist != NULL && node != NULL, RET_BAD_PARAMS);
  return_value_if_fail(dlist->size > 0, RET_FAIL);

  if (dlist->first == dlist->last) {
    dlist->first = dlist->last = NULL;
  } else if (node == dlist->first) {
    dlist->first = dlist->first->next;
    ENSURE(dlist->first != NULL);
    dlist->first->prev = NULL;
  } else if (node == dlist->last) {
    dlist->last = dlist->last->prev;
    ENSURE(dlist->last != NULL);
    dlist->last->next = NULL;
  } else {
    ENSURE(node->prev != NULL);
    node->prev->next = node->next;
    ENSURE(node->next != NULL);
    node->next->prev = node->prev;
  }

  dlist->size--;

  return RET_OK;
}

ret_t dlist_remove_ex(dlist_t* dlist, tk_compare_t compare, void* ctx, int32_t remove_size,
                      bool_t reverse) {
  int32_t n = remove_size;
  return_value_if_fail(dlist != NULL, RET_BAD_PARAMS);

  if (!reverse) {
    dlist_node_t* iter = dlist->first;
    while (iter != NULL) {
      if (compare(iter->data, ctx) == 0) {
        dlist_node_t* next = iter->next;
        dlist_remove_node(dlist, iter);
        dlist_destroy_node(dlist, iter);
        iter = next;
        if (--n == 0) {
          return RET_OK;
        }
      } else {
        iter = iter->next;
      }
    }
  } else {
    dlist_node_t* iter = dlist->last;
    while (iter != NULL) {
      if (compare(iter->data, ctx) == 0) {
        dlist_node_t* prev = iter->prev;
        dlist_remove_node(dlist, iter);
        dlist_destroy_node(dlist, iter);
        iter = prev;
        if (--n == 0) {
          return RET_OK;
        }
      } else {
        iter = iter->prev;
      }
    }
  }

  return remove_size == n ? RET_NOT_FOUND : RET_OK;
}

ret_t dlist_remove(dlist_t* dlist, void* ctx) {
  return dlist_remove_ex(dlist, dlist->compare, ctx, 1, FALSE);
}

ret_t dlist_remove_last(dlist_t* dlist, void* ctx) {
  return dlist_remove_ex(dlist, dlist->compare, ctx, 1, TRUE);
}

ret_t dlist_append(dlist_t* dlist, void* data) {
  dlist_node_t* node = NULL;
  return_value_if_fail(dlist != NULL, RET_BAD_PARAMS);

  node = dlist_create_node(dlist, data);
  return_value_if_fail(node != NULL, RET_OOM);

  return dlist_insert_node(dlist, node, NULL);
}

ret_t dlist_prepend(dlist_t* dlist, void* data) {
  dlist_node_t* node = NULL;
  return_value_if_fail(dlist != NULL, RET_BAD_PARAMS);

  node = dlist_create_node(dlist, data);
  return_value_if_fail(node != NULL, RET_OOM);

  return dlist_insert_node(dlist, node, dlist->first);
}

ret_t dlist_foreach(dlist_t* dlist, tk_visit_t visit, void* ctx) {
  ret_t ret = RET_OK;
  dlist_node_t* iter = NULL;
  return_value_if_fail(dlist != NULL && visit != NULL, RET_BAD_PARAMS);

  iter = dlist->first;
  while (iter != NULL) {
    ret = visit(ctx, iter->data);
    if (ret == RET_REMOVE) {
      dlist_node_t* next = iter->next;
      dlist_remove_node(dlist, iter);
      dlist_destroy_node(dlist, iter);
      iter = next;
      continue;
    } else if (ret != RET_OK) {
      break;
    }
    iter = iter->next;
  }

  return RET_OK;
}

ret_t dlist_foreach_reverse(dlist_t* dlist, tk_visit_t visit, void* ctx) {
  ret_t ret = RET_OK;
  dlist_node_t* iter = NULL;
  return_value_if_fail(dlist != NULL && visit != NULL, RET_BAD_PARAMS);

  iter = dlist->last;
  while (iter != NULL) {
    ret = visit(ctx, iter->data);
    if (ret == RET_REMOVE) {
      dlist_node_t* prev = iter->prev;
      dlist_remove_node(dlist, iter);
      dlist_destroy_node(dlist, iter);
      iter = prev;
      continue;
    } else if (ret != RET_OK) {
      break;
    }
    iter = iter->prev;
  }

  return RET_OK;
}

static void* dlist_pop(dlist_t* dlist, bool_t head) {
  void* data = NULL;
  dlist_node_t* iter = NULL;
  return_value_if_fail(dlist != NULL, NULL);

  iter = head ? dlist->first : dlist->last;
  if (iter == NULL) {
    return NULL;
  }

  dlist_remove_node(dlist, iter);

  data = iter->data;
  iter->data = NULL;
  dlist_destroy_node(dlist, iter);

  return data;
}

void* dlist_tail_pop(dlist_t* dlist) {
  return dlist_pop(dlist, FALSE);
}

void* dlist_head_pop(dlist_t* dlist) {
  return dlist_pop(dlist, TRUE);
}

void* dlist_tail(dlist_t* dlist) {
  dlist_node_t* iter = NULL;
  return_value_if_fail(dlist != NULL, NULL);

  iter = dlist->last;
  if (iter == NULL) {
    return NULL;
  }

  return iter->data;
}

void* dlist_head(dlist_t* dlist) {
  dlist_node_t* iter = NULL;
  return_value_if_fail(dlist != NULL, NULL);

  iter = dlist->first;
  if (iter == NULL) {
    return NULL;
  }

  return iter->data;
}

bool_t dlist_is_empty(dlist_t* dlist) {
  return_value_if_fail(dlist != NULL, TRUE);
  return dlist->size == 0;
}

int32_t dlist_size(dlist_t* dlist) {
  return_value_if_fail(dlist != NULL, 0);
  return dlist->size;
}

int32_t dlist_count(dlist_t* dlist, void* ctx) {
  int32_t size = 0;
  dlist_node_t* iter = NULL;
  return_value_if_fail(dlist != NULL, 0);

  iter = dlist->first;
  while (iter != NULL) {
    if (dlist->compare(iter->data, ctx) == 0) {
      size++;
    }
    iter = iter->next;
  }

  return size;
}

ret_t dlist_deinit(dlist_t* dlist) {
  ret_t ret = RET_OK;
  return_value_if_fail(dlist != NULL, RET_BAD_PARAMS);

  ret = dlist_remove_all(dlist);
  if (RET_OK == ret) {
    dlist_set_node_allocator(dlist, NULL);
  }
  return ret;
}

ret_t dlist_remove_all(dlist_t* dlist) {
  return_value_if_fail(dlist != NULL, RET_BAD_PARAMS);
  if (dlist->first != NULL) {
    dlist_node_t* iter = dlist->first;

    while (iter != NULL) {
      dlist_node_t* next = iter->next;
      dlist_destroy_node(dlist, iter);

      iter = next;
    }
    dlist->first = NULL;
    dlist->last = NULL;
    dlist->size = 0;
  }

  return RET_OK;
}

ret_t dlist_destroy(dlist_t* dlist) {
  return_value_if_fail(dlist != NULL, RET_BAD_PARAMS);
  dlist_deinit(dlist);
  TKMEM_FREE(dlist);

  return RET_OK;
}

ret_t dlist_insert(dlist_t* dlist, uint32_t index, void* data) {
  dlist_node_t* node = NULL;
  dlist_node_t* iter = NULL;
  return_value_if_fail(dlist != NULL, RET_BAD_PARAMS);

  iter = dlist->first;
  while (iter != NULL && index > 0) {
    index--;
    iter = iter->next;
  }

  node = dlist_create_node(dlist, data);
  return_value_if_fail(node != NULL, RET_OOM);

  return dlist_insert_node(dlist, node, iter);
}

ret_t dlist_reverse(dlist_t* dlist) {
  dlist_node_t* iter = NULL;
  return_value_if_fail(dlist != NULL, RET_BAD_PARAMS);

  iter = dlist->first;
  while (iter != NULL) {
    tk_swap(iter->prev, iter->next, dlist_node_t*);
    iter = iter->prev;
  }
  tk_swap(dlist->first, dlist->last, dlist_node_t*);

  return RET_OK;
}

inline static ret_t dlist_set_node_allocator_impl(dlist_t* dlist, mem_allocator_t* allocator,
                                                  bool_t shared) {
  return_value_if_fail(dlist != NULL, RET_BAD_PARAMS);

  if (dlist->node_allocator != allocator) {
    return_value_if_fail(dlist_is_empty(dlist), RET_FAIL);

    if (dlist->node_allocator != NULL && !dlist->node_allocator_is_shared) {
      mem_allocator_destroy(dlist->node_allocator);
    }
    dlist->node_allocator = allocator;
  }
  dlist->node_allocator_is_shared = shared;

  return RET_OK;
}

ret_t dlist_set_node_allocator(dlist_t* dlist, mem_allocator_t* allocator) {
  return dlist_set_node_allocator_impl(dlist, allocator, FALSE);
}

ret_t dlist_set_shared_node_allocator(dlist_t* dlist, mem_allocator_t* allocator) {
  return dlist_set_node_allocator_impl(dlist, allocator, TRUE);
}

void* dlist_get(dlist_t* dlist, uint32_t index) {
  uint32_t i = 0;
  dlist_node_t* iter = NULL;
  return_value_if_fail(dlist != NULL, NULL);
  return_value_if_fail(index < dlist->size, NULL);

  iter = dlist->first;
  while (iter != NULL && i < index) {
    iter = iter->next;
    i++;
  }

  assert(iter != NULL);
  return iter->data;
}
