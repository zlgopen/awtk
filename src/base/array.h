/**
 * File:   array.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  dynamic array.
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_ARRAY_H
#define LFTK_ARRAY_H

#include "base/types_def.h"

BEGIN_C_DECLS

typedef struct _array_t
{
  void **elms;
  uint16_t size;
  uint16_t capacity;
} array_t;

array_t *array_create(uint16_t capacity);

array_t *array_init(array_t *array, uint16_t capacity);

void *array_find(array_t *array, compare_t cmp, void *data);
int array_find_index(array_t *array, compare_t cmp, void *data);

bool_t array_remove(array_t *array, compare_t cmp, void *data);
bool_t array_foreach(array_t *array, visit_t visit, void *ctx);

void *array_pop(array_t *array);
bool_t array_push(array_t *array, void *data);

void array_deinit(array_t *array);

void array_destroy(array_t *array);

END_C_DECLS

#endif /*LFTK_ARRAY_H*/

