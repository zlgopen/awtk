
#ifdef WITH_FREE_RTOS_DIR

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#else
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#endif/*WITH_FREE_RTOS_DIR*/
