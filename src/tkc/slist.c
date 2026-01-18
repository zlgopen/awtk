/**
 * File:   slist.c
 * Author: AWTK Develop Team
 * Brief:  single link list
 *
 * Copyright (c) 2019 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/slist.h"

static slist_node_t* slist_create_node(slist_t* slist, void* data) {
  slist_node_t* ret = NULL;
  return_value_if_fail(slist != NULL, NULL);

  if (slist->node_allocator != NULL) {
    ret = MEM_ALLOCATOR_ALLOC(slist->node_allocator, sizeof(slist_node_t));
  } else {
    ret = TKMEM_ALLOC(sizeof(slist_node_t));
  }
  return_value_if_fail(ret != NULL, NULL);

  memset(ret, 0, sizeof(*ret));

  ret->data = data;

  return ret;
}

static ret_t slist_destroy_node(slist_t* slist, slist_node_t* node) {
  return_value_if_fail(slist != NULL && node != NULL, RET_OK);

  if (node->data != NULL) {
    slist->destroy(node->data);
  }
  if (slist->node_allocator != NULL) {
    MEM_ALLOCATOR_FREE(slist->node_allocator, node);
  } else {
    TKMEM_FREE(node);
  }

  return RET_OK;
}

inline static slist_node_t* slist_get_node(slist_t* slist, uint32_t index) {
  slist_node_t* iter = NULL;
  return_value_if_fail(slist != NULL, NULL);
  return_value_if_fail(index < slist->size, NULL);

  if (index == slist->size - 1) {
    iter = slist->last;
  } else {
    uint32_t i = 0;
    iter = slist->first;
    while (iter != NULL && i < index) {
      iter = iter->next;
      i++;
    }
  }

  return iter;
}

slist_t* slist_create(tk_destroy_t destroy, tk_compare_t compare) {
  slist_t* slist = TKMEM_ZALLOC(slist_t);
  return_value_if_fail(slist != NULL, NULL);

  return slist_init(slist, destroy, compare);
}

slist_t* slist_init(slist_t* slist, tk_destroy_t destroy, tk_compare_t compare) {
  return_value_if_fail(slist != NULL, NULL);

  memset(slist, 0, sizeof(slist_t));

  slist->destroy = destroy != NULL ? destroy : dummy_destroy;
  slist->compare = compare != NULL ? compare : pointer_compare;

  return slist;
}

void* slist_find_ex(slist_t* slist, tk_compare_t compare, void* ctx) {
  slist_node_t* iter = NULL;
  tk_compare_t real_compare = NULL;
  return_value_if_fail(slist != NULL, NULL);
  if (slist->first == NULL) {
    return NULL;
  }

  real_compare = (compare != NULL) ? compare : slist->compare;

  iter = slist->first;
  while (iter != NULL) {
    if (real_compare(iter->data, ctx) == 0) {
      return iter->data;
    }
    iter = iter->next;
  }

  return NULL;
}

void* slist_find(slist_t* slist, void* ctx) {
  return slist_find_ex(slist, NULL, ctx);
}

static ret_t slist_insert_node(slist_t* slist, slist_node_t* node, slist_node_t* prev) {
  return_value_if_fail(slist != NULL && node != NULL, RET_BAD_PARAMS);

  if (slist->first == NULL || slist->last == NULL) {
    slist->first = slist->last = node;
  } else if (prev == NULL) {
    /* prepend */
    node->next = slist->first;
    slist->first = node;
  } else {
    if (prev == slist->last) {
      /* append */
      slist->last = node;
    }
    node->next = prev->next;
    prev->next = node;
  }

  slist->size++;

  return RET_OK;
}

static ret_t slist_remove_node(slist_t* slist, slist_node_t* node, slist_node_t* prev) {
  return_value_if_fail(slist != NULL && node != NULL, RET_BAD_PARAMS);
  return_value_if_fail(slist->size > 0, RET_FAIL);

  if (slist->first == slist->last) {
    slist->first = slist->last = NULL;
  } else if (prev == NULL || node == slist->first) {
    /* head_pop */
    slist->first = slist->first->next;
  } else if (prev->next == slist->last || node == slist->last) {
    /* tail_pop */
    prev->next = NULL;
    slist->last = prev;
  } else {
    prev->next = prev->next->next;
  }

  slist->size--;

  return RET_OK;
}

ret_t slist_remove_ex(slist_t* slist, tk_compare_t compare, void* ctx, int32_t remove_size) {
  int32_t n = remove_size;
  slist_node_t* iter = NULL;
  slist_node_t* prev = NULL;
  return_value_if_fail(slist != NULL, RET_BAD_PARAMS);

  iter = slist->first;
  while (iter != NULL) {
    if (compare(iter->data, ctx) == 0) {
      slist_node_t* next = iter->next;
      slist_remove_node(slist, iter, prev);
      slist_destroy_node(slist, iter);
      iter = next;
      if (--n == 0) {
        return RET_OK;
      }
    } else {
      prev = iter;
      iter = iter->next;
    }
  }

  return remove_size == n ? RET_NOT_FOUND : RET_OK;
}

ret_t slist_remove(slist_t* slist, void* ctx) {
  return slist_remove_ex(slist, slist->compare, ctx, 1);
}

ret_t slist_append(slist_t* slist, void* data) {
  slist_node_t* node = NULL;
  return_value_if_fail(slist != NULL, RET_BAD_PARAMS);

  node = slist_create_node(slist, data);
  return_value_if_fail(node != NULL, RET_OOM);

  return slist_insert_node(slist, node, slist->last);
}

ret_t slist_prepend(slist_t* slist, void* data) {
  slist_node_t* node = NULL;
  return_value_if_fail(slist != NULL, RET_BAD_PARAMS);

  node = slist_create_node(slist, data);
  return_value_if_fail(node != NULL, RET_OOM);

  return slist_insert_node(slist, node, NULL);
}

ret_t slist_foreach(slist_t* slist, tk_visit_t visit, void* ctx) {
  ret_t ret = RET_OK;
  slist_node_t* iter = NULL;
  slist_node_t* prev = NULL;
  return_value_if_fail(slist != NULL && visit != NULL, RET_BAD_PARAMS);

  iter = slist->first;
  while (iter != NULL) {
    ret = visit(ctx, iter->data);
    TK_FOREACH_VISIT_RESULT_PROCESSING(ret, slist_node_t* next = iter->next;
                                       slist_remove_node(slist, iter, prev);
                                       slist_destroy_node(slist, iter); iter = next);
    prev = iter;
    iter = iter->next;
  }

  return ret;
}

void* slist_tail_pop(slist_t* slist) {
  void* data = NULL;
  slist_node_t* iter = NULL;
  slist_node_t* prev = NULL;
  return_value_if_fail(slist != NULL, NULL);

  iter = slist->first;
  if (iter == NULL) {
    return NULL;
  }

  if (slist->size >= 2) {
    prev = slist_get_node(slist, slist->size - 2);
    ENSURE(prev != NULL);
    if (prev != NULL) {
      iter = prev->next;
    }
  }
  slist_remove_node(slist, iter, prev);

  data = iter->data;
  iter->data = NULL;
  slist_destroy_node(slist, iter);

  return data;
}

void* slist_head_pop(slist_t* slist) {
  void* data = NULL;
  slist_node_t* iter = NULL;
  return_value_if_fail(slist != NULL, NULL);

  iter = slist->first;
  if (iter == NULL) {
    return NULL;
  }

  slist_remove_node(slist, iter, NULL);

  data = iter->data;
  iter->data = NULL;
  slist_destroy_node(slist, iter);

  return data;
}

void* slist_tail(slist_t* slist) {
  slist_node_t* iter = NULL;
  return_value_if_fail(slist != NULL, NULL);

  iter = slist->last;
  if (iter == NULL) {
    return NULL;
  }

  return iter->data;
}

void* slist_head(slist_t* slist) {
  slist_node_t* iter = NULL;
  return_value_if_fail(slist != NULL, NULL);

  iter = slist->first;
  if (iter == NULL) {
    return NULL;
  }

  return iter->data;
}

bool_t slist_is_empty(slist_t* slist) {
  return_value_if_fail(slist != NULL, TRUE);
  return slist->size == 0;
}

int32_t slist_size(slist_t* slist) {
  return_value_if_fail(slist != NULL, 0);
  return slist->size;
}

int32_t slist_count(slist_t* slist, void* ctx) {
  int32_t size = 0;
  slist_node_t* iter = NULL;
  return_value_if_fail(slist != NULL, 0);

  iter = slist->first;
  while (iter != NULL) {
    if (slist->compare(iter->data, ctx) == 0) {
      size++;
    }
    iter = iter->next;
  }

  return size;
}

ret_t slist_deinit(slist_t* slist) {
  ret_t ret = RET_OK;
  return_value_if_fail(slist != NULL, RET_BAD_PARAMS);

  ret = slist_remove_all(slist);
  if (RET_OK == ret) {
    slist_set_node_allocator(slist, NULL);
  }
  return ret;
}

ret_t slist_remove_all(slist_t* slist) {
  return_value_if_fail(slist != NULL, RET_BAD_PARAMS);
  if (slist->first != NULL) {
    slist_node_t* iter = slist->first;

    while (iter != NULL) {
      slist_node_t* next = iter->next;
      slist_destroy_node(slist, iter);

      iter = next;
    }
    slist->first = NULL;
    slist->last = NULL;
    slist->size = 0;
  }

  return RET_OK;
}

ret_t slist_destroy(slist_t* slist) {
  return_value_if_fail(slist != NULL, RET_BAD_PARAMS);
  slist_deinit(slist);
  TKMEM_FREE(slist);

  return RET_OK;
}

ret_t slist_insert(slist_t* slist, uint32_t index, void* data) {
  slist_node_t* node = NULL;
  slist_node_t* prev = NULL;
  return_value_if_fail(slist != NULL, RET_BAD_PARAMS);

  node = slist_create_node(slist, data);
  return_value_if_fail(node != NULL, RET_OOM);

  if (index > 0) {
    uint32_t i = tk_min(index, slist->size);
    prev = slist_get_node(slist, i - 1);
    ENSURE(prev != NULL);
  }

  return slist_insert_node(slist, node, prev);
}

ret_t slist_reverse(slist_t* slist) {
  slist_node_t* iter = NULL;
  slist_node_t* prev = NULL;
  return_value_if_fail(slist != NULL, RET_BAD_PARAMS);

  slist->last = slist->first;
  iter = slist->first;
  while (iter != NULL) {
    slist_node_t* next = iter->next;
    iter->next = prev;
    prev = iter;
    iter = next;
  }
  slist->first = prev;

  return RET_OK;
}

inline static ret_t slist_set_node_allocator_impl(slist_t* slist, mem_allocator_t* allocator,
                                                  bool_t shared) {
  return_value_if_fail(slist != NULL, RET_BAD_PARAMS);

  if (slist->node_allocator != allocator) {
    return_value_if_fail(slist_is_empty(slist), RET_FAIL);

    if (slist->node_allocator != NULL && !slist->node_allocator_is_shared) {
      mem_allocator_destroy(slist->node_allocator);
    }
    slist->node_allocator = allocator;
  }
  slist->node_allocator_is_shared = shared;

  return RET_OK;
}

ret_t slist_set_node_allocator(slist_t* slist, mem_allocator_t* allocator) {
  return slist_set_node_allocator_impl(slist, allocator, FALSE);
}

ret_t slist_set_shared_node_allocator(slist_t* slist, mem_allocator_t* allocator) {
  return slist_set_node_allocator_impl(slist, allocator, TRUE);
}

void* slist_get(slist_t* slist, uint32_t index) {
  slist_node_t* node = NULL;
  return_value_if_fail(slist != NULL, NULL);
  return_value_if_fail(index < slist->size, NULL);

  node = slist_get_node(slist, index);
  ENSURE(node != NULL);

  return node->data;
}
