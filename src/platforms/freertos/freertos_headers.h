
#ifdef WITH_FREE_RTOS_DIR

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#elif defined(WITH_OPEN_RTOS_DIR)

#include "openrtos/FreeRTOS.h"
#include "openrtos/task.h"
#include "openrtos/semphr.h"


#else
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#endif/*WITH_FREE_RTOS_DIR*/
