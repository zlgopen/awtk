
# 如何使用多点触控

​	AWTK 提供的多点触控功能主要是为了给用户提供多点触控的基本数据，方便用户开发多点触控的控件或者应用。

### 一，多点触控的类型

​	AWTK 一共提供了两种多点触控的类型，分别是获取手指 ID 的可识别手指类型的多点触控和不可以获取手指 ID 的不可识别手指类型的多点触控，下表为两者的区别：

|                          | 可识别手指类型 | 不可识别手指类型的 |
| ------------------------ | -------------- | ------------------ |
| 是否需要提供手指 ID      | 是             | 否                 |
| 触摸消息是否提供距离增量 | 是             | 是                 |
| 触摸消息是否提供中心点   | 是             | 是                 |
| 触摸消息是否提供旋转角度 | 是             | 否                 |

> 备注：因为某些触摸硬件是无法提供具体的手指 ID 的，所以 AWTK 特别提供一种不需要手指 ID 的多点触控机制，但是这个机制提供出来的消息是无法提供旋转角度的，所以用户需要更加具体情况来旋转使用。

### 二，多点触控的API

#### 	1.可识别手指类型

```h
/**
 * @method multi_gesture_gesture_touch_fingers_destroy
 * 释放可识别手指类型的对象
 * @param {multi_gesture_touch_fingers_t*} touch 可识别手指类型的对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t multi_gesture_gesture_touch_fingers_destroy(multi_gesture_touch_fingers_t* touch);

/**
 * @method multi_gesture_touch_fingers_create
 * 创建可识别手指类型的对象
 * @param {int32_t} finger_size 可识别手指个数。
 *
 * @return {multi_gesture_touch_fingers_t*} 成功返回可识别手指类型的对象，失败返回NULL。
 */
multi_gesture_touch_fingers_t* multi_gesture_touch_fingers_create(int32_t finger_size);

/**
 * @method multi_gesture_post_event_form_fingers
 * 发送多点触控消息
 * @param {main_loop_t*} loop 主循环对象
 * @param {multi_gesture_touch_fingers_t*} touch 可识别手指类型的对象。
 * @param {uint32_t} point_size 手指个数
 * @param {multi_touch_point_event_t*} points 手指数据数组
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t multi_gesture_post_event_form_fingers(main_loop_t* loop, multi_gesture_touch_fingers_t* touch, uint32_t point_size, multi_touch_point_event_t* points);
```

#### 	2.不可识别手指类型

```h
/**
 * @method multi_gesture_gesture_touch_points_destroy
 * 释放不可识别手指类型的对象
 * @param {multi_gesture_touch_points_t*} touch 不可识别手指类型的对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t multi_gesture_gesture_touch_points_destroy(multi_gesture_touch_points_t* touch);

/**
 * @method multi_gesture_touch_points_create
 * 创建不可识别手指类型的对象
 * @param {uint32_t} last_dist_lenght 均值滤波的缓冲长度。
 *
 * @return {multi_gesture_touch_points_t*} 成功返回不可识别手指类型的对象，失败返回NULL。
 */
multi_gesture_touch_points_t* multi_gesture_touch_points_create(uint32_t last_dist_lenght);

/**
 * @method multi_gesture_post_event_form_points
 * 发送多点触控消息
 * @param {main_loop_t*} loop 主循环对象
 * @param {multi_gesture_touch_points_t*} touch 不可识别手指类型的对象。
 * @param {uint32_t} point_size 手指个数
 * @param {multi_touch_point_event_t*} points 手指数据数组
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t multi_gesture_post_event_form_points(main_loop_t* loop, multi_gesture_touch_points_t* touch, uint32_t point_size, multi_touch_point_event_t* points);
```

> 具体用法请参考
>
> [multi\_gesture.inc](https://github.com/zlgopen/awtk/blob/master/demos/multi_gesture.inc)

### 三，如何使用

​	本文使用 github 中 [stm32f429 项目](https://github.com/zlgopen/awtk-stm32f429igtx-raw)为例子，来讲述如何使用 awtk 提供的多点触控方案。

#### 1.适配层

​	stm32f429 项目中的适配层，把触摸数据发送到 AWTK 消息循环中。

```c
/* main_loop_stm32_raw.c */
/* 省略了一部分无关代码 */

#include "base/multi_gesture.inc"
static uint8_t last_touch_number = 0;
static multi_gesture_touch_points_t* touch_points = NULL;
static multi_gesture_touch_fingers_t* touch_fingers = NULL;
static multi_touch_point_event_t s_points[CT_MAX_TOUCH];

extern u32* ltdc_framebuf[2];
#define online_fb_addr (uint8_t*)ltdc_framebuf[0]
#define offline_fb_addr (uint8_t*)ltdc_framebuf[1]


uint8_t platform_disaptch_input(main_loop_t* loop) {
  int x = 0;
  int y = 0;
  int32_t i = 0;
  int32_t touch_id = 0;
  uint8_t key = KEY_Scan(0);
	
  uint8_t touch_number = 0;
  uint8_t num_down_fingers = 0;
	
  tp_dev.scan(0);

  x = tp_dev.x[0];
  y = tp_dev.y[0];

  y = lcdltdc.pheight - tp_dev.x[0];
  x = tp_dev.y[0];
  /* 计算当前有多少只手指按下 */
  for(i = 0; i < CT_MAX_TOUCH; i++) {
      if (tp_dev.sta & (1<<i)) {
          touch_number++;
      }
  }
  /* 由于有手指弹起，所以清空多点触控的转态，并且给 AWTK 发送多点触控弹起的消息 */
  if (touch_number < last_touch_number) {
      for(i = 0; i < CT_MAX_TOUCH; i++) {
          s_points[i].type = EVT_MULTI_TOUCH_UP;
      }
      /* 使用可识别手指类型发送多点触控消息 */
      multi_gesture_post_event_form_fingers(loop, touch_fingers, last_touch_number, s_points);
      /* 使用不可识别手指类型发送多点触控消息 */
      //multi_gesture_post_event_form_points(loop, touch_points, last_touch_number, s_points);
  }
  /* 把手指数据转换为 multi_touch_point_event_t 类型 */
  for(i = 0; i < CT_MAX_TOUCH; i++) {
      s_points[i].touch_id = touch_id;
      s_points[i].finger_id = i;
      if (tp_dev.sta & (1<<i) && (!(tp_dev.y[i] == 0 && tp_dev.x[i] == 0))) {
          num_down_fingers++;
          if (!(tp_dev.y[i] == 0 && tp_dev.x[i] == 0)) {
              s_points[i].x = tp_dev.y[i];
              s_points[i].y = lcdltdc.pheight - tp_dev.x[i];
          }
          s_points[i].type = (s_points[i].type == EVT_MULTI_TOUCH_UP) ? EVT_MULTI_TOUCH_DOWN : EVT_MULTI_TOUCH_MOVE;
      } else {
          if (s_points[i].type == EVT_MULTI_TOUCH_MOVE || s_points[i].type == EVT_MULTI_TOUCH_DOWN) {
              num_down_fingers++;
          }
          s_points[i].x = 0;
          s_points[i].y = 0;
          s_points[i].type = EVT_MULTI_TOUCH_UP;
      }
   }

  /* 使用可识别手指类型发送多点触控消息 */
  multi_gesture_post_event_form_fingers(loop, touch_fingers, num_down_fingers, s_points);
  /* 使用不可识别手指类型发送多点触控消息 */
  //multi_gesture_post_event_form_points(loop, touch_points, num_down_fingers, s_points);

  /* 当只有一个手指的时候转为单点触控 */
  if (touch_number == 1) {
      if (tp_dev.sta & TP_PRES_DOWN) {
          main_loop_post_pointer_event(loop, TRUE, x, y);
      } else {
          main_loop_post_pointer_event(loop, FALSE, x, y);
      }
  } else if (last_touch_number == 1) {
      /* 当由单点触控转为多点触控的时候，要给控件发送弹起消息，以免导致消息混乱 */
      main_loop_post_pointer_event(loop, FALSE, x, y);
  }
  last_touch_number = touch_number;
  return 0;
}

extern lcd_t* stm32f429_create_lcd(wh_t w, wh_t h);

lcd_t* platform_create_lcd(wh_t w, wh_t h) {
  /* 创建不可识别手指类型的多点触控句柄 */
  touch_points = multi_gesture_touch_points_create(15);
  /* 创建可识别手指类型的多点触控句柄 */
  touch_fingers = multi_gesture_touch_fingers_create(CT_MAX_TOUCH);
  memset(s_points, 0x0, sizeof(multi_touch_point_event_t) * CT_MAX_TOUCH);
  return stm32f429_create_lcd(w, h);
}

#include "main_loop/main_loop_raw.inc"

```

> 备注：
>
> 1. 上面的代码有两个 for 循环，一个是计算当前按下的手指个数，一个是把硬件的手指坐标数据转换为 multi_touch_point_event_t 类型。
> 2. 只需要把上面 multi_gesture_post_event_form_points 函数取消注释，然后把 multi_gesture_post_event_form_fingers 函数加上注释就可以实现使用不可识别手指类型的多点触控，本例子默认为可识别手指类型的多点触控。

#### 2.应用层

​	由于多点触控无法通过手指点击坐标来确定具体的控件，所以 AWTK 的多点触控默认会发送给当前选中的窗口上面，所以用户想获取具体的消息，需要通过给当前的窗口注册多点触控消息事件（EVT_MULTI_GESTURE 事件）来获取对应消息，如下代码：

```c
static ret_t multi_gesture_view_on_event_multi_gesture(void* ctx, event_t* e) {
  multi_gesture_event_t* event = (multi_gesture_event_t*)e;
  return_value_if_fail(multi_gesture_view != NULL && e != NULL, RET_BAD_PARAMS);
  /* centroid 为中心点
   * distance为两点间的距离增量，(-1,0)表示缩小，(0-1)表示增加。
   * rotation为旋转角度(幅度)增量，（单位弧度）。
   */
  printf("centroid:(%d, %d), distance:%f, rotation:%f \r\n", event->x, event->y, event->distance, event->rotation);
  return RET_OK;
}

static ret_t multi_gesture_view_init_multi_gesture_event(widget_t* widget) {
  widget_t* win = NULL;
  multi_gesture_view_t* multi_gesture_view = MULTI_GESTURE_VIEW(widget);
  return_value_if_fail(widget != NULL && multi_gesture_view != NULL, RET_BAD_PARAMS);

  if (widget->parent != NULL) {
    /* 获取控件的父集窗口，当焦点在控件上面时，父集窗口肯定也是选中的 */
    win = widget_get_window(widget);
  }

  if (win != NULL && multi_gesture_view->event_win != win) {
    if (multi_gesture_view->event_id != TK_INVALID_ID) {
      /* 切换父集窗口的时候，移除消息，以免父集窗口不一样，导致出错 */
      multi_gesture_view_deint_multi_gesture_event(widget);
    }

    multi_gesture_view->event_win = win;
    /* 注册多点触控消息事件 */
    multi_gesture_view->event_id = widget_on(win, EVT_MULTI_GESTURE, multi_gesture_view_on_event_multi_gesture, widget);
    return multi_gesture_view->event_id != TK_INVALID_ID ? RET_OK : RET_FAIL;
  }
  return RET_FAIL;
}
```

> 备注：用户可以自行根据需求自行写上述代码来获取多点触控消息。

