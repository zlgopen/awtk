#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "tkc/mem.h"
#include "tkc/cond_var.h"

struct _tk_cond_var_t {
    EventGroupHandle_t event;
};

tk_cond_var_t* tk_cond_var_create(void) {
  tk_cond_var_t* cond_var = TKMEM_ZALLOC(tk_cond_var_t);
  return_value_if_fail(cond_var != NULL, NULL);
  cond_var->event = xEventGroupCreate();
  if (cond_var->event == NULL) {
    TKMEM_FREE(cond_var);
  }

  return cond_var;
}

ret_t tk_cond_var_wait(tk_cond_var_t* cond_var, uint32_t timeout_ms) {
  return_value_if_fail(cond_var != NULL, RET_BAD_PARAMS);
  EventBits_t uxBits = xEventGroupWaitBits(cond_var->event, BIT(0), pdTRUE, pdFALSE, timeout_ms);
  if ((uxBits & BIT0) == BIT0) {
     return RET_OK;
  } else {
    return RET_FAIL;
  }
}

ret_t tk_cond_var_awake(tk_cond_var_t* cond_var) {
  return_value_if_fail(cond_var != NULL, RET_BAD_PARAMS);
  EventBits_t uxBits = xEventGroupSetBits(cond_var->event, BIT(0));
  if ((uxBits & BIT0) == BIT0) {
     return RET_OK;
  } else {
    return RET_FAIL;
  }
}

ret_t tk_cond_var_destroy(tk_cond_var_t* cond_var) {
  return_value_if_fail(cond_var != NULL, RET_BAD_PARAMS);

  vEventGroupDelete(cond_var->event);
  memset(cond_var, 0x00, sizeof(tk_cond_var_t));
  TKMEM_FREE(cond_var);

  return RET_OK;
}
