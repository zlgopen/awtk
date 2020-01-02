## 将 AWTK 移植到 STM32f103ze 裸系统

AWTK 的可移植性很高，在移植时只需要实现平台初始化、lcd 和 mainloop 三个方面的东西。本文以 STM32f103ze 为例介绍移植 AWTK 到新平台的过程。

### 一、平台初始化

除了基本的libc函数外，AWTK对平台没有特别要求，实现获取当前时间的函数get\_time\_ms64和sleep_ms函数即可。另外需要给GUI分配一块内存空间，并调用tk\_mem\_init。

我们使用 systick 来实现 get\_time\_ms64 和 sleep\_ms 两个函数，裸系统上只需加入 src/platforms/raw/sys\_tick.c 并初始化 sys\_tick 即可。

> 以下是初始化内存的代码。

```
ret_t platform_prepare(void) {
  static bool_t inited = FALSE;
  static uint32_t s_heam_mem[4000];

  if (!inited) {
    inited = TRUE;
    tk_mem_init(s_heam_mem, sizeof(s_heam_mem));
  }

  return RET_OK;
}
```

> 参考：awtk-port/platform.c

> 以下是初始化 systick 的代码（在板子提供代码上修改而来）。

```
static u8 fac_us = 0; 
static u16 fac_ms = 0; 

void systick_enable_int(void) {
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
  SysTick->LOAD = fac_ms;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Init(void) {
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  fac_us = SystemCoreClock / 8000000;
  fac_ms = (u16)fac_us * 1000;
}

void delay_us(u32 nus) {
  u32 temp = 0;
  SysTick->LOAD = nus * fac_us; 
  SysTick->VAL = 0x00;          
  do {
    temp = SysTick->CTRL;
  } while ((temp & 0x01) && !(temp & (1 << 16))); 
}

void delay_ms(u16 nms) {
  u32 temp = 0;
  SysTick->LOAD = (u32)nms * fac_ms;
  SysTick->VAL = 0x00;              
  do {
    temp = SysTick->CTRL;
  } while ((temp & 0x01) && !(temp & (1 << 16))); 
}
```

> 参考：awtk-port/SysTick.c

### 二、实现 lcd

lcd\_t 接口提供基本的显示功能，AWTK 提供基于*寄存器*、*帧缓冲*和*片段帧缓冲*三种缺省实现，在此基础上实现自己的 lcd\_t 接口非常方便。

stm32f103ze 使用基于*片段帧缓冲*的 lcd 的缺省实现，只需要提供 set\_window\_func 和 write\_data\_func 两个函数/宏即可。这里直接使用了 TFT\_SetWindow 和 TFT\_WriteData 两个函数。

> 直接写寄存器的方式容易闪烁，而帧缓冲又需要大量内存，片段帧缓冲能有效解决低内存平台的闪烁问题。

```
#include "tftlcd.h"
#include "tkc/mem.h"
#include "lcd/lcd_mem_fragment.h"

typedef uint16_t pixel_t;

#define LCD_FORMAT BITMAP_FMT_BGR565
#define pixel_from_rgb(r, g, b)                                                \
  ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))
#define pixel_to_rgba(p)                                                       \
  { (0xff & ((p >> 11) << 3)), (0xff & ((p >> 5) << 2)), (0xff & (p << 3)) }

#define set_window_func LCD_Set_Window
#define write_data_func LCD_WriteData_Color

#include "base/pixel.h"
#include "blend/pixel_ops.inc"
#include "lcd/lcd_mem_fragment.inc"

```

> 参考 awtk-port/lcd\_stm32\_raw.c

### 三、实现 main\_loop

main\_loop 主要负责事件分发和绘制这个不断循环的过程。main\_loop\_raw.inc 里实现了裸系统 main\_loop 的基本功能，在移植时加上输入事件的的分发即可：

```

#include "key.h"
#include "led.h"
#include "rtc.h"
#include "stdlib.h"
#include "tftlcd.h"
#include "touch.h"
#include "usart.h"


#include "base/idle.h"
#include "base/timer.h"
#include "tkc/platform.h"
#include "base/main_loop.h"
#include "base/event_queue.h"
#include "base/font_manager.h"
#include "lcd/lcd_mem_fragment.h"
#include "main_loop/main_loop_simple.h"


ret_t platform_disaptch_input(main_loop_t *l) { return RET_OK; }

static lcd_t *platform_create_lcd(wh_t w, wh_t h) {
  return lcd_mem_fragment_create(w, h);
}

void dispatch_input_events(void) {
  int key = KEY_Scan(0);

  switch (key) {
  case KEY_UP: {
    key = TK_KEY_UP;
    break;
  }
  case KEY_DOWN: {
    key = TK_KEY_DOWN;
    break;
  }
  case KEY_LEFT: {
    key = TK_KEY_RETURN;
    break;
  }
  case KEY_RIGHT: {
    key = TK_KEY_BACK;
    break;
  }
  default: { key = 0; }
  }

  if (key) {
    main_loop_post_key_event(main_loop(), TRUE, key);
  } else {
    main_loop_post_key_event(main_loop(), FALSE, key);
  }

  if (TOUCH_Scan() == 0) {
    main_loop_post_pointer_event(main_loop(), TRUE, TouchData.lcdx,
                                 TouchData.lcdy);
  } else {
    main_loop_post_pointer_event(main_loop(), FALSE, TouchData.lcdx,
                                 TouchData.lcdy);
  }
}

void TIM3_IRQHandler(void) {
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
    dispatch_input_events();
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}

#include "main_loop/main_loop_raw.inc"
```

> 参考 awtk-port/main\_loop\_stm32\_raw.c

四、Keil 设置

* 1. 勾选 c99
* 2. 定义 HAS\_AWTK\_CONFIG
* 3. 增加如下头文件路径：

```
awtk\src
awtk\3rd
awtk-port
```

> 请根据项目文件位置进行调整。

五、其它配置

其它配置请参考 awtk-port/awtk_config.h

注：目前以 [普中科技 STM32F103ZET6 开发实验板](https://item.taobao.com/item.htm?spm=a230r.1.14.1.50a130e8TMKYMC&id=558855281660&ns=1&abbucket=5#detail) 为载体移植，其它开发板应该差不多。

完整项目和源码请参考：[awtk-stm32f103ze-raw](https://github.com/zlgopen/awtk-stm32f103ze-raw)

六、常见问题

 * 1.莫名其妙的崩溃，可能是栈溢出。请修 Stack_Size 的大小。

```
 Stack_Size      EQU     0x00001000
```

* 2.如果出现 wcsxxx 之类的函数没有定义时，请在 awtk-port/awtk\_config.h 中定义 WITH\_WCSXXX。
