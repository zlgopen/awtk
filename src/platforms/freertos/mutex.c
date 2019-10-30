#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "tkc/mem.h"
#include "tkc/mutex.h"

struct _tk_mutex_t {
  SemaphoreHandle_t mutex;
};

tk_mutex_t* tk_mutex_create() {
  tk_mutex_t* mutex = TKMEM_ZALLOC(tk_mutex_t);
  return_value_if_fail(mutex != NULL, NULL);

  mutex->mutex = xSemaphoreCreateMutex();
  if (mutex->mutex == NULL)
  {
    return NULL;
  }
  return mutex;
}

ret_t tk_mutex_lock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  return_value_if_fail(xSemaphoreTake(mutex->mutex, portMAX_DELAY) == pdPASS, RET_FAIL);
  return RET_OK;
}

ret_t tk_mutex_unlock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);
  return_value_if_fail(xSemaphoreGive(mutex->mutex) == pdPASS, RET_FAIL);

  return RET_OK;
}

ret_t tk_mutex_destroy(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  vSemaphoreDelete(mutex->mutex);
  return RET_OK;
}
