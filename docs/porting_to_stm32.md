## 将AWTK移植到STM32裸系统

AWTK的可移植性很高，在移植时需要实现平台初始化、lcd和mainloop三个方面的东西。本文以STM32为例介绍移植AWTK到新平台的过程。

### 一、平台初始化

除了基本的libc函数外，AWTK对平台没有特别要求，在stm32上需要实现一个获取当前时间的函数get\_time\_ms。

```
 31 #include "stdlib.h"
 32 #include "sys.h"
 33 #include "tim.h"
 34 #include "touch.h"
 35 #include "usart.h"
 36 
 37 ret_t platform_prepare() {
 38   delay_init();
 39   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 40   led_init();
 41   button_init();
 42   TFT_Init();
 43   TFT_ClearScreen(BLACK);
 44   FLASH_Init();
 45   TOUCH_Init();
 46 
 47   TIM3_Init(50, 7199);
 48   rtc_init();
 49   
 50   timer_init(get_time_ms);
 51   
 52   return RET_OK;
 53 }
 54 
 55 uint32_t get_time_ms() {
 56   return RTC_GetCounter();
 57 }
```

> 参考：platforms/stm32f103ze\_raw.c

### 二、实现lcd

lcd\_t接口提供基本的显示功能，实现lcd_t接口是很容易的。AWTK提供基于寄存器和基于framebuffer两种缺省实现，在此基础上实现自己的lcd\_t接口就更方便了。stm32使用基于寄存器的lcd的缺省实现，只需要提供set\_window\_func和write\_data\_func两个函数/宏即可。这里直接使用TFT\_SetWindow和TFT\_WriteData两个函数。

```
#include "gui.h"
#include "lcd_driver.h"
#include "lcd/lcd_reg.h"

#define set_window_func TFT_SetWindow
#define write_data_func TFT_WriteData

#include "base/color.h"
#include "base/mem.h"
#include "lcd/rgb565.h"
#include "lcd/lcd_reg.inc"
```
> 参考 lcd/lcd\_stm32\_raw.c

### 三、实现main\_loop

main\_loop主要负责事件分发和绘制这个不断循环的过程，一般来说先处理定时器，再分发事件，最后绘制。

```
static ret_t main_loop_stm32_raw_run(main_loop_t* l) {
  main_loop_stm32_raw_t* loop = (main_loop_stm32_raw_t*)l;

  while (l->running) {
    timer_check();
    main_loop_stm32_raw_dispatch(loop);
    main_loop_stm32_raw_paint(loop);
    delay_ms(100);
  }

  return RET_OK;
}
```

触屏事件在终端中获取，转成pointer\_event\_t之后放入事件队列：

```
static void dispatch_touch_events(bool_t pressed, xy_t x, xy_t y) {
  event_all_t any;
  pointer_event_t event;

  loop.touch_x = x;
  loop.touch_y = y;

  event.button = 0;
  event.x = loop.touch_x;
  event.y = loop.touch_y;

  if (pressed) {
    if (loop.pressed) {
      event.e.type = EVT_POINTER_MOVE;
    } else {
      event.e.type = EVT_POINTER_DOWN;
    }
    loop.pressed = TRUE;
    event.pressed = loop.pressed;

    any.e.pointer_event = event;
    event_queue_send(loop.queue, &any);
  } else {
    if (loop.pressed) {
      loop.pressed = FALSE;
      event.e.type = EVT_POINTER_UP;
      event.pressed = loop.pressed;

      any.e.pointer_event = event;
      event_queue_send(loop.queue, &any);
    }
  }
}

void TIM3_IRQHandler(void) {
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    if (TOUCH_Scan() == 0) {
      dispatch_touch_events(TRUE, TouchData.lcdx, TouchData.lcdy);
    } else {
      dispatch_touch_events(FALSE, TouchData.lcdx, TouchData.lcdy);
    }
  }
}
```

键盘则是直接扫描并分发:

```
static ret_t main_loop_stm32_raw_dispatch(main_loop_stm32_raw_t* loop) {
  event_all_t event;
  widget_t* widget = loop->wm;
  uint8_t key = keyscan(0);
  /*TODO dispatch key*/
  while (event_queue_recv(loop->queue, &event) == RET_OK) {
    switch (event.e.event.type) {
      case EVT_POINTER_DOWN:
        widget_on_pointer_down(widget, &(event.e.pointer_event));
        break;
      case EVT_POINTER_MOVE:
        widget_on_pointer_move(widget, &(event.e.pointer_event));
        break;
      case EVT_POINTER_UP:
        widget_on_pointer_up(widget, &(event.e.pointer_event));
        break;
      default:
        break;
    }
  }

  return RET_OK;
}
```

> 参考 main_loop/main\_loop\_stm32\_raw.c


注：目前以[普中科技STM32F103ZET6开发实验板](https://item.taobao.com/item.htm?spm=a230r.1.14.1.50a130e8TMKYMC&id=558855281660&ns=1&abbucket=5#detail) 为载体移植(该开发板性价比不错，推荐一下:))。

