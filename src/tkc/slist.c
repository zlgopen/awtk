/**
 * File:   slist.c
 * Author: AWTK Develop Team
 * Brief:  single link list
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static slist_node_t* slist_node_create(void* data) {
  slist_node_t* node = TKMEM_ZALLOC(slist_node_t);
  return_value_if_fail(node != NULL, NULL);

  node->data = data;

  return node;
}

static ret_t slist_node_destroy(slist_node_t* node, tk_destroy_t destroy) {
  return_value_if_fail(node != NULL && destroy != NULL, RET_OK);

  destroy(node->data);
  TKMEM_FREE(node);

  return RET_OK;
}

slist_t* slist_create(tk_destroy_t destroy, tk_compare_t compare) {
  slist_t* slist = TKMEM_ZALLOC(slist_t);
  return_value_if_fail(slist != NULL, NULL);

  return slist_init(slist, destroy, compare);
}

slist_t* slist_init(slist_t* slist, tk_destroy_t destroy, tk_compare_t compare) {
  return_value_if_fail(slist != NULL, NULL);
  slist->first = NULL;
  slist->destroy = destroy != NULL ? destroy : dummy_destroy;
  slist->compare = compare != NULL ? compare : pointer_compare;

  return slist;
}

void* slist_find(slist_t* slist, void* ctx) {
  slist_node_t* iter = NULL;
  return_value_if_fail(slist != NULL && slist->first != NULL, NULL);

  iter = slist->first;
  while (iter != NULL) {
    if (slist->compare(iter->data, ctx) == 0) {
      return iter->data;
    }
    iter = iter->next;
  }

  return NULL;
}

ret_t slist_remove(slist_t* slist, void* ctx) {
  slist_node_t* iter = NULL;
  slist_node_t* prev = NULL;
  return_value_if_fail(slist != NULL, RET_BAD_PARAMS);

  iter = slist->first;
  prev = slist->first;
  while (iter != NULL) {
    if (slist->compare(iter->data, ctx) == 0) {
      if (iter == slist->first) {
        slist->first = slist->first->next;
      } else {
        prev->next = iter->next;
      }
      slist_node_destroy(iter, slist->destroy);

      return RET_OK;
    }
    prev = iter;
    iter = iter->next;
  }

  return RET_NOT_FOUND;
}

ret_t slist_append(slist_t* slist, void* data) {
  slist_node_t* iter = NULL;
  slist_node_t* node = NULL;
  return_value_if_fail(slist != NULL, RET_BAD_PARAMS);
  node = slist_node_create(data);
  return_value_if_fail(node != NULL, RET_OOM);

  iter = slist->first;
  if (iter == NULL) {
    slist->first = node;
  } else {
    while (iter->next != NULL) {
      iter = iter->next;
    }
    iter->next = node;
  }

  return RET_OK;
}

ret_t slist_prepend(slist_t* slist, void* data) {
  slist_node_t* node = NULL;
  return_value_if_fail(slist != NULL, RET_BAD_PARAMS);

  node = slist_node_create(data);
  return_value_if_fail(node != NULL, RET_OOM);

  node->next = slist->first;
  slist->first = node;

  return RET_OK;
}

ret_t slist_foreach(slist_t* slist, tk_visit_t visit, void* ctx) {
  ret_t ret = RET_OK;
  slist_node_t* iter = NULL;
  return_value_if_fail(slist != NULL && visit != NULL, RET_BAD_PARAMS);

  iter = slist->first;
  while (iter != NULL) {
    ret = visit(ctx, iter->data);
    if (ret != RET_OK) {
      break;
    }

    iter = iter->next;
  }

  return RET_OK;
}

void* slist_tail_pop(slist_t* slist) {
  void* data = NULL;
  slist_node_t* iter = NULL;
  slist_node_t* last_iter = NULL;
  return_value_if_fail(slist != NULL, NULL);

  iter = slist->first;
  return_value_if_fail(iter != NULL, NULL);

  if (iter->next == NULL) {
    slist->first = NULL;
  } else {
    while (iter->next != NULL) {
      last_iter = iter;
      iter = iter->next;
    }

    last_iter->next = NULL;
  }

  data = iter->data;
  TKMEM_FREE(iter);

  return data;
}

void* slist_head_pop(slist_t* slist) {
  void* data = NULL;
  slist_node_t* iter = NULL;
  return_value_if_fail(slist != NULL, NULL);

  iter = slist->first;
  return_value_if_fail(iter != NULL, NULL);
  
  slist->first = iter->next;

  data = iter->data;
  TKMEM_FREE(iter);

  return data;
}

int32_t slist_size(slist_t* slist) {
  int32_t size = 0;
  slist_node_t* iter = NULL;
  return_value_if_fail(slist != NULL, 0);

  iter = slist->first;
  while (iter != NULL) {
    size++;
    iter = iter->next;
  }

  return size;
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
  return slist_remove_all(slist);
}

ret_t slist_remove_all(slist_t* slist) {
  return_value_if_fail(slist != NULL, RET_BAD_PARAMS);
  if (slist->first != NULL) {
    slist_node_t* iter = slist->first;

    while (iter != NULL) {
      slist_node_t* next = iter->next;
      slist_node_destroy(iter, slist->destroy);

      iter = next;
    }
    slist->first = NULL;
  }

  return RET_OK;
}

ret_t slist_destroy(slist_t* slist) {
  return_value_if_fail(slist != NULL, RET_BAD_PARAMS);
  slist_deinit(slist);
  TKMEM_FREE(slist);

  return RET_OK;
}
