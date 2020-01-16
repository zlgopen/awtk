## 将AWTK移植到AWorksOS平台

AWTK的可移植性很高，在移植时只需要实现平台初始化、lcd和mainloop三个方面的东西。本文以aworks-rt1052平台为例介绍移植AWTK到新平台的过程。

### 一、平台初始化

除了基本的libc函数外，AWTK对平台没有特别要求，实现获取当前时间的函数get\_time\_ms64和sleep_ms函数即可。另外需要给GUI分配一块内存空间，并调用tk\_mem\_init。

```
#include "aw_system.h"
#include "aw_delay.h"
#include "base/mem.h"
#include "base/timer.h"

uint32_t get_time_ms64() {
  return aw_sys_tick_get();
}

void sleep_ms(uint32_t ms) {
  aw_mdelay(ms);
}

#define TK_MEM_SIZE 4 * 1024 * 1024

ret_t platform_prepare(void) {
  uint32_t* mem = (uint32_t*) aw_mem_alloc(TK_MEM_SIZE);
  return_value_if_fail(mem != NULL, RET_OOM);

  tk_mem_init(mem, TK_MEM_SIZE);

  return RET_OK;
}
```
 
> 参考：awtk-port/platform.c

### 二、实现lcd

AWorksOS使用基于framebuffer的lcd的缺省实现，只需要调用函数lcd\_mem\_create\_double\_fb即可。


```
#include "aw_emwin_fb.h"
#include "aw_sem.h"
#include "aw_cache.h"
#include "aw_delay.h"
#include "aw_vdebug.h"
#include <string.h>

#include "base/types_def.h"
static uint32_t *s_frame_buffer = 0;
static uint32_t *s_offline_frame_buffer = 0;
static aw_emwin_fb_info_t *s_awtk_fb = NULL;

uint32_t* aworks_get_online_fb(void) {
  return s_frame_buffer;
}

uint32_t* aworks_get_offline_fb(void) {
  return s_offline_frame_buffer;
}

aw_emwin_fb_info_t* aworks_lcd_init(void) {
  int fb_size = 0;
  s_awtk_fb = aw_emwin_fb_open("imx1050_emwin_fb", 0); 
  return_value_if_fail(s_awtk_fb != NULL, NULL);

  fb_size = s_awtk_fb->x_res * s_awtk_fb->y_res * 2;

  s_frame_buffer = (uint32_t*) aw_cache_dma_malloc(fb_size);
  return_value_if_fail(s_frame_buffer != NULL, NULL);
  memset(s_frame_buffer, 0x00, fb_size);

  s_offline_frame_buffer = (uint32_t*) aw_cache_dma_malloc(fb_size);
  return_value_if_fail(s_offline_frame_buffer != NULL, NULL);
  memset(s_offline_frame_buffer, 0x00, fb_size);

  return_value_if_fail(
      AW_OK == aw_emwin_fb_init(s_awtk_fb, s_frame_buffer, AW_FB_VERTICAL_SCREEN),
      NULL);

  aw_emwin_fb_run(s_awtk_fb);

  aw_emwin_fb_backlight(s_awtk_fb, 100);

  return s_awtk_fb;
}
```

> 参考 awtk-port/lcd\_aworks.c

### 三、实现main\_loop

main\_loop主要负责事件分发和绘制这个不断循环的过程。main\_loop\_raw.inc里实现了裸系统main\_loop的基本功能，在移植时加上输入事件的的分发即可：

```
static aw_ts_id ts_app_init(void) {
  aw_ts_id sys_ts = sys_ts = aw_ts_serv_id_get("480x272", 0, 0); 
  return_value_if_fail(sys_ts != NULL, NULL);
  return_value_if_fail(aw_ts_calc_data_read(sys_ts) == AW_OK, NULL);

  return sys_ts;
}

ret_t platform_disaptch_input(main_loop_t* loop) {
  int tsret = 0;
  static aw_ts_id ts_id = NULL;
  struct aw_ts_state ts_state;

  memset(&ts_state, 0x00, sizeof(ts_state));
  if (ts_id == NULL) {
    ts_id = ts_app_init();
  }

  return_value_if_fail(ts_id != NULL, RET_FAIL);

  tsret = aw_ts_exec(ts_id, &ts_state, 1); 
  if (tsret >= 0) {
    if (ts_state.pressed) {
      main_loop_post_pointer_event(loop, ts_state.pressed, ts_state.x,
          ts_state.y);
    } else {
      main_loop_post_pointer_event(loop, ts_state.pressed, ts_state.x,
          ts_state.y);
    }   
  }

  return RET_OK;
}

extern uint32_t* aworks_get_online_fb(void);
extern uint32_t* aworks_get_offline_fb(void);

lcd_t* platform_create_lcd(wh_t w, wh_t h) {
  lcd_t* lcd = lcd_mem_rgb565_create_double_fb(w, h, (uint8_t*) aworks_get_online_fb(),
      (uint8_t*) aworks_get_offline_fb());

  return lcd;
}

#include "main_loop/main\_loop_raw.inc"
```

> 参考 awtk-port/main\_loop\_aworks.c

完整项目和源码请参考：[awtk-aworks-rt1052](https://github.com/zlgopen/awtk-aworks-rt1052)

