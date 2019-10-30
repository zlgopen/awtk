#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/thread.h"

struct _tk_thread_t {
  void* args;
  TaskHandle_t task;
  bool_t running;
  tk_thread_entry_t entry;
  char name[TK_NAME_LEN + 1];
  uint32_t stack_size;
  uint32_t priority;
  EventGroupHandle_t event_sync;
};
const static int TASK_CREATED_BIT = BIT0;

ret_t tk_thread_set_name(tk_thread_t* thread, const char* name) {
  return_value_if_fail(thread != NULL && name != NULL, RET_BAD_PARAMS);

  tk_strncpy(thread->name, name, TK_NAME_LEN);

  return RET_OK;
}

ret_t tk_thread_set_stack_size(tk_thread_t* thread, uint32_t stack_size) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  thread->stack_size = stack_size;

  return RET_OK;
}

ret_t tk_thread_set_priority(tk_thread_t* thread, uint32_t priority) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  thread->priority = priority;

  return RET_OK;
}

void* tk_thread_get_args(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, NULL);

  return thread->args;
}

tk_thread_t* tk_thread_create(tk_thread_entry_t entry, void* args) {
  tk_thread_t* thread = NULL;
  return_value_if_fail(entry != NULL, NULL);

  thread = (tk_thread_t*)TKMEM_ZALLOC(tk_thread_t);
  return_value_if_fail(thread != NULL, NULL);

  thread->args = args;
  thread->entry = entry;
  thread->stack_size = 2048;

  return thread;
}

static void entry(void* arg) {
  tk_thread_t* thread = (tk_thread_t*)arg;
  xEventGroupSetBits(thread->event_sync, TASK_CREATED_BIT);
  thread->entry(thread->args);
  thread->running = FALSE;
  vTaskDelete(thread->task);
}

ret_t tk_thread_start(tk_thread_t* thread) {
  uint32_t priority = 0;
  uint32_t stack_size = 0;
  BaseType_t err = pdPASS;
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);
  thread->event_sync = xEventGroupCreate();
  return_value_if_fail(thread->event_sync != NULL, RET_OOM);
  priority = tk_min(thread->priority, 10);
  stack_size = tk_max(thread->stack_size, 4096);
  err = xTaskCreate(
			entry,
			thread->name,
			stack_size,
			thread,
			priority,
			&(thread->task));

  thread->running = err == pdPASS;
  xEventGroupWaitBits(thread->event_sync, TASK_CREATED_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
  return thread->running ? RET_OK : RET_FAIL;
}

ret_t tk_thread_join(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);
  if (thread->running) {
  }

  return RET_OK;
}

ret_t tk_thread_destroy(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);
  vEventGroupDelete(thread->event_sync);
  memset(thread, 0x00, sizeof(tk_thread_t));
  TKMEM_FREE(thread);
  return RET_OK;
}