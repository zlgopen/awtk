/**
 * File:   nvgp_darray.c
 * Author: AWTK Develop Team
 * Brief:  nanovg darry.
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
 * 2021-10-27 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#include "nvgp_darray.h"

nvgp_darray_t* nvgp_darray_init(nvgp_darray_t* darray, uint32_t capacity, uint32_t data_type_size) {
  if (darray->elms != NULL) {
    return NULL;
  }
  darray->size = 0;
  darray->data_type_size = 0;
  darray->capacity = capacity;
  darray->elms = NVGP_ZALLOCN(void*, capacity);
  if (data_type_size > 0) {
    uint32_t i = 0;
    darray->data_type_size = data_type_size;
    for (i = 0; i < capacity; i++) {
      darray->elms[i] = NVGP_MALLOC(data_type_size);
    }
  }
  return darray;
}

void nvgp_darray_deinit(nvgp_darray_t* darray) {
  if (darray == NULL) {
    return;
  }
  if (darray->elms != NULL) {
    if (darray->data_type_size > 0) {
      for (uint32_t i = 0; i < darray->capacity; i++) {
        NVGP_FREE(darray->elms[i]);
      }
    }
    NVGP_FREE(darray->elms);
  }
  NVGP_MEMSET(darray, 0x0, sizeof(nvgp_darray_t));
}

nvgp_darray_t* nvgp_darray_create(uint32_t capacity, uint32_t data_type_size) {
  nvgp_darray_t* darray = NVGP_ZALLOC(nvgp_darray_t);
  if (darray == NULL) {
    return NULL;
  }
  NVGP_MEMSET(darray, 0x0, sizeof(nvgp_darray_t));
  return nvgp_darray_init(darray, capacity, data_type_size);
}

void nvgp_darray_destroy(nvgp_darray_t* darray) {
  if (darray == NULL) {
    return;
  }
  nvgp_darray_deinit(darray);
  NVGP_FREE(darray);
}

void* nvgp_darray_get_empty_data_by_tail(nvgp_darray_t* darray) {
  if (darray == NULL) {
    return NULL;
  }
  if (darray->size >= darray->capacity) {
    uint32_t n = nvgp_max(darray->size + 1, 4) + darray->capacity / 2;
    darray->elms = NVGP_REALLOCT(void*, darray->elms, n);
    if (darray->elms == NULL) {
      return NULL;
    }
    darray->capacity = n;
    if (darray->data_type_size > 0) {
      for (uint32_t i = darray->size; i < darray->capacity; i++) {
        darray->elms[i] = NVGP_MALLOC(darray->data_type_size);
      }
    } else {
      for (uint32_t i = darray->size; i < darray->capacity; i++) {
        darray->elms[i] = NULL;
      }
    }
  }
  return darray->elms[darray->size++];
}


void* nvgp_darray_get_empty_data_ptr_by_tail(nvgp_darray_t* darray) {
  if (darray == NULL && darray->data_type_size == 0) {
    return NULL;
  }
  if (darray->size >= darray->capacity) {
    uint32_t n = nvgp_max(darray->size + 1, 4) + darray->capacity / 2;
    darray->elms = NVGP_REALLOCT(void*, darray->elms, n);
    if (darray->elms == NULL) {
      return NULL;
    }
    darray->capacity = n;
    for (uint32_t i = darray->size; i < darray->capacity; i++) {
      darray->elms[i] = NULL;
    }
  }
  return &(darray->elms[darray->size++]);
}

nvgp_error_t nvgp_darray_push(nvgp_darray_t* darray, void* data) {
  if (darray == NULL || data == NULL) {
    return NVGP_BAD_PARAMS;
  }
  if (darray->size >= darray->capacity) {
    uint32_t n = nvgp_max(darray->size + 1, 4) + darray->capacity / 2; 
    darray->elms = NVGP_REALLOCT(void*, darray->elms, n);
    if (darray->elms == NULL) {
      return NVGP_OOM;
    }
    darray->capacity = n;
    if (darray->data_type_size > 0) {
      for (uint32_t i = darray->size; i < darray->capacity; i++) {
        darray->elms[i] = NVGP_MALLOC(darray->data_type_size);
      }
    } else {
      for (uint32_t i = darray->size; i < darray->capacity; i++) {
        darray->elms[i] = NULL;
      }
    }
  }
  if (darray->data_type_size > 0) {
    NVGP_MEMCPY(&(darray->elms[darray->size++]), data, darray->data_type_size);
  } else {
    darray->elms[darray->size++] = data;
  }
  return NVGP_OK;
}

nvgp_error_t nvgp_darray_pop(nvgp_darray_t* darray) {
  int32_t n = 0;
  if (darray == NULL) {
    return NVGP_BAD_PARAMS;
  }
  n = darray->size - 1;
  if (0 <= n && n < darray->capacity) {
    NVGP_MEMSET(darray->elms[--darray->size], 0x0, darray->data_type_size);
    return NVGP_OK;
  }
  return NVGP_FAIL;
}

static void nvgp_darray_memset_elms(void* data, void* ctx) {
  uint32_t data_type_size = *(uint32_t*)ctx;
  NVGP_MEMSET(data, 0x0, data_type_size);
}

nvgp_error_t nvgp_darray_remove(nvgp_darray_t* darray, uint32_t index, nvgp_destroy_t destroy_func, void* ctx) {
  int32_t i = 0;
  int32_t size = 0;
  if (darray == NULL || darray->elms == NULL || darray->size <= index ||
      !(darray->data_type_size > 0 && destroy_func == NULL)) {
    return NVGP_BAD_PARAMS;
  }
  
  if (darray->data_type_size == 0 && destroy_func != NULL) {
    destroy_func(darray->elms[index], ctx);
  }

  if (darray->data_type_size == 0) {
    for (size = darray->size - 1, i = index; i < size; i++) {
      darray->elms[i] = darray->elms[i + 1];
    }
    darray->elms[i] = NULL;
  } else {
    for (size = darray->size - 1, i = index; i < size; i++) {
      NVGP_MEMCPY(darray->elms[i], darray->elms[i + 1], darray->data_type_size);
    }
    NVGP_MEMSET(darray->elms[i], 0x0, darray->data_type_size);
  }
  darray->size--;
  return NVGP_OK;
}

nvgp_error_t nvgp_darray_clear(nvgp_darray_t* darray) {
  return nvgp_darray_clear_by_destroy_function(darray, nvgp_darray_memset_elms, &darray->data_type_size);
}

nvgp_error_t nvgp_darray_clear_by_destroy_function(nvgp_darray_t* darray, nvgp_destroy_t destroy_func, void* ctx) {
  int32_t i = 0;
  if (darray == NULL || darray->elms == NULL || destroy_func == NULL) {
    return NVGP_BAD_PARAMS;
  }
  for (i = 0; i < darray->size; i++) {
    destroy_func(darray->elms[i], ctx);
  }
  if (darray->data_type_size == 0) {
    for (i = 0; i < darray->size; i++) {
      darray->elms[i] = NULL;
    }
  }
  darray->size = 0;
  return NVGP_OK;
}

nvgp_error_t nvgp_darray_remove_empty_cache(nvgp_darray_t* darray) {
  int32_t n = 0;
  if (darray == NULL || darray->elms == NULL) {
    return NVGP_BAD_PARAMS;
  }
  n = darray->capacity - darray->size;
  if (n > 0) {
    if (darray->data_type_size > 0) {
      for (uint32_t i = darray->size; i < darray->capacity; i++) {
        NVGP_FREE(darray->elms[i]);
        darray->elms[i] = NULL;
      }
    }
    darray->elms = NVGP_REALLOCT(void*, darray->elms, n);
    darray->capacity = n;
  }
  return NVGP_OK;
}

nvgp_error_t nvgp_darray_reverse(nvgp_darray_t* darray) {
  int32_t i = 0, j = 0;
  void* tmp = NULL;
  if (darray == NULL || darray->elms == NULL) {
    return NVGP_BAD_PARAMS;
  }
  j = darray->size - 1;
  while (i < j) {
    tmp = darray->elms[i];
    darray->elms[i] = darray->elms[j];
    darray->elms[j] = tmp;
    i++;
    j--;
  }
  return NVGP_OK;
}

