#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include <sys/time.h>
#include "tkc/types_def.h"

uint64_t get_time_ms64() {
  struct timeval te;
  gettimeofday(&te, NULL);
  uint64_t milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
  return milliseconds;
}

static void sleep_ms_raw(uint32_t ms) {
  uint32_t count = 0;
  uint64_t start = get_time_ms64();

  while (get_time_ms64() < (start + ms)) {
    count++;
  }
}

void sleep_ms(uint32_t ms) {
  vTaskDelay(1 / portTICK_PERIOD_MS);
}

void delay_ms(uint32_t ms) {
  sleep_ms(ms);
}

void delay_us(uint32_t us) {
  sleep_ms(1);
}
