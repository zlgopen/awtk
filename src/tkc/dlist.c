/**
 * File:   dlist.c
 * Author: AWTK Develop Team
 * Brief:  doubly linked list
 *
 * Copyright (c) 2024 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static dlist_node_t* dlist_node_create(void* data) {
  dlist_node_t* node = TKMEM_ZALLOC(dlist_node_t);
  return_value_if_fail(node != NULL, NULL);

  node->data = data;

  return node;
}

static ret_t dlist_node_destroy(dlist_node_t* node, tk_destroy_t destroy) {
  return_value_if_fail(node != NULL && destroy != NULL, RET_OK);

  destroy(node->data);
  TKMEM_FREE(node);

  return RET_OK;
}

dlist_t* dlist_create(tk_destroy_t destroy, tk_compare_t compare) {
  dlist_t* dlist = TKMEM_ZALLOC(dlist_t);
  return_value_if_fail(dlist != NULL, NULL);

  return dlist_init(dlist, destroy, compare);
}

dlist_t* dlist_init(dlist_t* dlist, tk_destroy_t destroy, tk_compare_t compare) {
  return_value_if_fail(dlist != NULL, NULL);
  dlist->first = NULL;
  dlist->last = NULL;
  dlist->destroy = destroy != NULL ? destroy : dummy_destroy;
  dlist->compare = compare != NULL ? compare : pointer_compare;

  return dlist;
}

void* dlist_find_ex(dlist_t* dlist, tk_compare_t compare, void* ctx) {
  dlist_node_t* iter = NULL;
  tk_compare_t real_compare = NULL;
  return_value_if_fail(dlist != NULL, NULL);
  if (dlist->first == NULL) {
    return NULL;
  }

  real_compare = (compare != NULL) ? compare : dlist->compare;

  iter = dlist->first;
  while (iter != NULL) {
    if (real_compare(iter->data, ctx) == 0) {
      return iter->data;
    }
    iter = iter->next;
  }

  return NULL;
}

void* dlist_find(dlist_t* dlist, void* ctx) {
  return dlist_find_ex(dlist, NULL, ctx);
}

static ret_t dlist_insert_node(dlist_t* dlist, dlist_node_t* node, dlist_node_t* iter) {
  return_value_if_fail(dlist != NULL && node != NULL, RET_BAD_PARAMS);

  if (dlist->first == NULL || dlist->last == NULL) {
    dlist->first = dlist->last = node;
  } else if (iter == NULL) {
    /* append */
    dlist->last->next = node;
    node->prev = dlist->last;
    dlist->last = node;
  } else if (iter == dlist->first) {
    /* prepend */
    dlist->first->prev = node;
    node->next = dlist->first;
    dlist->first = node;
  } else {
    node->next = iter;
    node->prev = iter->prev;

    if (iter->prev != NULL) {
      iter->prev->next = node;
    }
    iter->prev = node;
  }

  return RET_OK;
}

static ret_t dlist_remove_node(dlist_t* dlist, dlist_node_t* node) {
  return_value_if_fail(dlist != NULL && node != NULL, RET_BAD_PARAMS);

  if (dlist->first == dlist->last) {
    dlist->first = dlist->last = NULL;
  } else if (node == dlist->first) {
    dlist->first = dlist->first->next;
    if (dlist->first != NULL) {
      dlist->first->prev = NULL;
    }
  } else if (node == dlist->last) {
    dlist->last = dlist->last->prev;
    if (dlist->last != NULL) {
      dlist->last->next = NULL;
    }
  } else {
    if (node->prev != NULL) {
      node->prev->next = node->next;
    }
    if (node->next != NULL) {
      node->next->prev = node->prev;
    }
  }

  return RET_OK;
}

ret_t dlist_remove_ex(dlist_t* dlist, tk_compare_t compare, void* ctx, int32_t remove_size) {
  int32_t n = remove_size;
  dlist_node_t* iter = NULL;
  return_value_if_fail(dlist != NULL, RET_BAD_PARAMS);

  iter = dlist->first;
  while (iter != NULL) {
    if (compare(iter->data, ctx) == 0) {
      dlist_node_t* next = iter->next;
      dlist_remove_node(dlist, iter);
      dlist_node_destroy(iter, dlist->destroy);
      iter = next;
      if (--n == 0) {
        return RET_OK;
      }
    } else {
      iter = iter->next;
    }
  }

  return remove_size == n ? RET_NOT_FOUND : RET_OK;
}

ret_t dlist_remove(dlist_t* dlist, void* ctx) {
  return dlist_remove_ex(dlist, dlist->compare, ctx, 1);
}

ret_t dlist_append(dlist_t* dlist, void* data) {
  dlist_node_t* node = NULL;
  return_value_if_fail(dlist != NULL, RET_BAD_PARAMS);

  node = dlist_node_create(data);
  return_value_if_fail(node != NULL, RET_OOM);

  return dlist_insert_node(dlist, node, NULL);
}

ret_t dlist_prepend(dlist_t* dlist, void* data) {
  dlist_node_t* node = NULL;
  return_value_if_fail(dlist != NULL, RET_BAD_PARAMS);

  node = dlist_node_create(data);
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
      dlist_node_destroy(iter, dlist->destroy);
      iter = next;
      continue;
    } else if (ret != RET_OK) {
      break;
    }
    iter = iter->next;
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
  TKMEM_FREE(iter);

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
  return dlist->first == NULL;
}

int32_t dlist_size(dlist_t* dlist) {
  int32_t size = 0;
  dlist_node_t* iter = NULL;
  return_value_if_fail(dlist != NULL, 0);

  iter = dlist->first;
  while (iter != NULL) {
    size++;
    iter = iter->next;
  }

  return size;
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
  return dlist_remove_all(dlist);
}

ret_t dlist_remove_all(dlist_t* dlist) {
  return_value_if_fail(dlist != NULL, RET_BAD_PARAMS);
  if (dlist->first != NULL) {
    dlist_node_t* iter = dlist->first;

    while (iter != NULL) {
      dlist_node_t* next = iter->next;
      dlist_node_destroy(iter, dlist->destroy);

      iter = next;
    }
    dlist->first = NULL;
    dlist->last = NULL;
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

  node = dlist_node_create(data);
  dlist_insert_node(dlist, node, iter);

  return RET_OK;
}
