/**
 * File:   multi_gesture.inc
 * Author: AWTK Develop Team
 * Brief:  multi_gesture
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-11-03 Lou ZhiMing <louzhiming@zlg.cn> created
 *
 */

#include <math.h>
#include "tkc/mem.h"
#include "tkc/rect.h"
#include "tkc/utils.h"
#include "tkc/darray.h"
#include "base/events.h"
#include "base/types_def.h"
#include "main_loop/main_loop_simple.h"
/**
 * @enum multi_event_type_t
 * @annotation ["scriptable"]
 * @prefix EVT_
 * 类型常量定义。
 */
typedef enum _multi_event_type_t {
  /**
   * @const EVT_MULTI_TOUCH_UP
   * 多点触摸抬起事件名(multi_touch_event_t)。
   */
  EVT_MULTI_TOUCH_UP = 0x0,
  /**
   * @const EVT_MULTI_TOUCH_DOWN
   * 多点触摸按下事件名(multi_touch_event_t)。
   */  
  EVT_MULTI_TOUCH_DOWN,
  /**
   * @const EVT_MULTI_TOUCH_MOVE
   * 多点触摸点移动事件名(multi_touch_event_t)。
   */
  EVT_MULTI_TOUCH_MOVE,
} multi_event_type_t;

/**
 * @class multi_touch_point_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 多点触控点信息。
 */
typedef struct _multi_touch_point_event_t {
   /**
   * @property {multi_event_type_t} type
   * @annotation ["readable", "scriptable"]
   * 触摸状态。
   */ 
  multi_event_type_t type;
  /**
   * @property {int64_t} touch_id
   * @annotation ["readable", "scriptable"]
   * touch device id。
   */
  int64_t touch_id;
  /**
   * @property {xy_t} x
   * @annotation ["readable", "scriptable"]
   * x坐标。
   */
  xy_t x;
  /**
   * @property {xy_t} y
   * @annotation ["readable", "scriptable"]
   * y坐标。
   */
  xy_t y;
  /**
   * @property {uint32_t} finger_id
   * @annotation ["readable", "scriptable"]
   * 点击手指 ID
   */
  uint32_t finger_id;
} multi_touch_point_event_t;

/**
 * @class multi_gesture_finger_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 点击手指信息
 */
typedef struct _multi_gesture_finger_t {
  /**
   * @property {int64_t} finger_id
   * @annotation ["readable", "scriptable"]
   * 点击手指 ID
   */
  int64_t finger_id;
  /**
   * @property {xy_t} x
   * @annotation ["readable", "scriptable"]
   * 点击手指 X
   */
  xy_t x;
  /**
   * @property {xy_t} y
   * @annotation ["readable", "scriptable"]
   * 点击手指 Y
   */
  xy_t y;
} multi_gesture_finger_t;

/**
 * @class multi_gesture_touch_fingers_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 可识别手指类型多点触控对象
 */
typedef struct _multi_gesture_touch_fingers_t {
  /**
   * @property {int64_t} touch_id
   * @annotation ["readable", "scriptable"]
   * touch device id。
   */
  int64_t touch_id;
  /**
   * @property {float_t} dtheta
   * @annotation ["readable", "scriptable"]
   * 旋转角度(幅度)增量。（单位弧度）
   */
  float_t dtheta;
  /**
   * @property {float_t} d_dist
   * @annotation ["readable", "scriptable"]
   * 两点间的距离增量。(-1,0)表示缩小，(0-1)表示增加。
   */
  float_t d_dist;
  /* private */
  pointf_t centroid;
  darray_t last_point;
} multi_gesture_touch_fingers_t;

/**
 * @method multi_gesture_gesture_touch_fingers_destroy
 * 释放可识别手指类型的对象
 * @param {multi_gesture_touch_fingers_t*} touch 可识别手指类型的对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t multi_gesture_gesture_touch_fingers_destroy(multi_gesture_touch_fingers_t* touch) {
  return_value_if_fail(touch != NULL, RET_BAD_PARAMS);

  darray_deinit(&(touch->last_point));
  TKMEM_FREE(touch);

  return RET_OK;
}

static int multi_gesture_finger_compare(const void* a, const void* b) {
  int64_t finger_id = *(int64_t*)b;
  multi_gesture_finger_t* touch = (multi_gesture_finger_t*)a;
  return finger_id - touch->finger_id;
}

/**
 * @method multi_gesture_touch_fingers_create
 * 创建可识别手指类型的对象
 * @param {int32_t} finger_size 可识别手指个数。
 *
 * @return {multi_gesture_touch_fingers_t*} 成功返回可识别手指类型的对象，失败返回NULL。
 */
multi_gesture_touch_fingers_t* multi_gesture_touch_fingers_create(int32_t finger_size) {
  multi_gesture_touch_fingers_t* touch = TKMEM_ZALLOC(multi_gesture_touch_fingers_t);
  return_value_if_fail(touch != NULL, NULL);
  touch->centroid.x = 0;
  touch->centroid.y = 0;
  darray_init(&(touch->last_point), finger_size, default_destroy, multi_gesture_finger_compare);
  return touch;
}

static multi_gesture_finger_t* multi_gesture_get_finger_last_point(multi_gesture_touch_fingers_t* touch, int64_t finger_id) {
  multi_gesture_finger_t* last_point = NULL;
  return_value_if_fail(touch != NULL, NULL);

  last_point = (multi_gesture_finger_t*)darray_find(&(touch->last_point), &finger_id);

  return last_point;
}

static ret_t multi_gesture_remove_finger_last_point(multi_gesture_touch_fingers_t* touch, int64_t finger_id) {
  multi_gesture_finger_t* last_point = NULL;
  return_value_if_fail(touch != NULL, RET_BAD_PARAMS);

  last_point = (multi_gesture_finger_t*)darray_find(&(touch->last_point), &finger_id);
  if (last_point != NULL) {
    return darray_remove(&(touch->last_point), last_point);
  }
  return RET_OK;
}

static ret_t multi_gesture_set_finger_last_point(multi_gesture_touch_fingers_t* touch, int64_t finger_id, xy_t x, xy_t y) {
  multi_gesture_finger_t* last_point = NULL;
  return_value_if_fail(touch != NULL, RET_BAD_PARAMS);

  last_point = (multi_gesture_finger_t*)darray_find(&(touch->last_point), &finger_id);
  if (last_point != NULL) {
    last_point->x = x;
    last_point->y = y;
  } else {
    last_point = TKMEM_ZALLOC(multi_gesture_finger_t);
    last_point->x = x;
    last_point->y = y;
    last_point->finger_id = finger_id;
    darray_push(&(touch->last_point), last_point);
  }
  return RET_OK;
}

static bool_t multi_gesture_get_event_form_fingers(multi_gesture_touch_fingers_t* touch, multi_touch_point_event_t* point,
                                   multi_gesture_event_t* out_event) {
  pointf_t last_p;
  pointf_t last_centroid;
  float_t dist;
  float_t l_dist;
  float_t dtheta;
  float_t d_dist;
  bool_t is_out_event = FALSE;
  uint32_t type = point->type;
  if (type == EVT_MULTI_TOUCH_MOVE || type == EVT_MULTI_TOUCH_DOWN || type == EVT_MULTI_TOUCH_UP) {
    float_t x = point->x;
    float_t y = point->y;
    uint32_t finger_id = point->finger_id;
    uint32_t num_down_fingers = touch->last_point.size;

    /* Finger Up */
    if (type == EVT_MULTI_TOUCH_UP && num_down_fingers > 0) {

      multi_gesture_remove_finger_last_point(touch, finger_id);
      num_down_fingers = touch->last_point.size;
      /* touch->gestureLast[j] = touch->gestureLast[touch->num_down_fingers]; */
      if (num_down_fingers > 0) {
        touch->centroid.x = (touch->centroid.x * (num_down_fingers + 1) - x) / num_down_fingers;
        touch->centroid.y = (touch->centroid.y * (num_down_fingers + 1) - y) / num_down_fingers;
      } else {
        touch->centroid.x = 0;
        touch->centroid.y = 0;
      }
    } else if (type == EVT_MULTI_TOUCH_MOVE && num_down_fingers > 1) {
      multi_gesture_finger_t* finger = multi_gesture_get_finger_last_point(touch, finger_id);
      return_value_if_fail(finger != NULL, FALSE);
      float_t delta_theta = 0.0f;
      float_t dx = x - finger->x;
      float_t dy = y - finger->y;

      last_p.x = x - dx;
      last_p.y = y - dy;
      last_centroid = touch->centroid;

      touch->centroid.x += dx / num_down_fingers;
      touch->centroid.y += dy / num_down_fingers;
      // printf("Centrid : (%f,%f), dx:%f, dy:%f \r\n",touch->centroid.x,touch->centroid.y,dx,dy);
      if (num_down_fingers > 1) {
        pointf_t lv; /* Vector from centroid to last x,y position */
        pointf_t v;  /* Vector from centroid to current x,y position */
        /* lv = touch->gestureLast[j].cv; */
        lv.x = last_p.x - last_centroid.x;
        lv.y = last_p.y - last_centroid.y;
        l_dist = sqrtf(lv.x * lv.x + lv.y * lv.y);
        /* printf("l_dist = %f\n",l_dist); */
        v.x = x - touch->centroid.x;
        v.y = y - touch->centroid.y;
        /* touch->gestureLast[j].cv = v; */
        dist = sqrtf(v.x * v.x + v.y * v.y);
        /* SDL_cos(dTheta) = (v . lv)/(|v| * |lv|) */

        /* Normalize Vectors to simplify angle calculation */
        lv.x /= l_dist;
        lv.y /= l_dist;
        v.x /= dist;
        v.y /= dist;
        dtheta = atan2f(lv.x * v.y - lv.y * v.x, lv.x * v.x + lv.y * v.y);

        d_dist = (dist - l_dist);
        if (l_dist == 0) {
          d_dist = 0;
          dtheta = 0;
        } /* To avoid impossible values */
        delta_theta = tk_abs(touch->dtheta - dtheta);
        if (delta_theta < 0.01f || delta_theta > M_PI / 2) {
          dtheta = 0;
        }

        /* touch->gestureLast[j].d_dist = d_dist;
                touch->gestureLast[j].dtheta = dtheta;

                printf("d_dist = %f, dTheta = %f\n",d_dist,dtheta);
                gdtheta = gdtheta*.9 + dtheta*.1;
                gd_dist  =  gd_dist*.9 +  d_dist*.1
                knob.r += d_dist/num_down_fingers;
                knob.ang += dtheta;
                printf("thetaSum = %f, distSum = %f\n",gdtheta,gd_dist);
                printf("id: %i dTheta = %f, d_dist = %f\n",j,dtheta,d_dist); */

        // SDL_SendGestureMulti(touch,dtheta,d_dist);--------------------------------------------------------------触发消息
        touch->dtheta = dtheta;
        touch->d_dist = d_dist;
        is_out_event = TRUE;
      }
    } else if (type == EVT_MULTI_TOUCH_DOWN) {
      multi_gesture_finger_t* last_point = (multi_gesture_finger_t*)darray_find(&(touch->last_point), &finger_id);
      if (last_point == NULL) {
        num_down_fingers++;
        touch->centroid.x = (touch->centroid.x * (num_down_fingers - 1) + x) / num_down_fingers;
        touch->centroid.y = (touch->centroid.y * (num_down_fingers - 1) + y) / num_down_fingers;
      }
    }
    if (type != EVT_MULTI_TOUCH_UP) {
      multi_gesture_set_finger_last_point(touch, finger_id, x, y);
    }
  }

  return is_out_event;
}

/**
 * @method multi_gesture_post_event_from_fingers
 * 发送多点触控消息
 * @param {main_loop_t*} loop 主循环对象
 * @param {multi_gesture_touch_fingers_t*} touch 可识别手指类型的对象。
 * @param {uint32_t} point_size 手指个数
 * @param {multi_touch_point_event_t*} points 手指数据数组
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t multi_gesture_post_event_from_fingers(main_loop_t* loop, multi_gesture_touch_fingers_t* touch, uint32_t point_size, multi_touch_point_event_t* points) {
  uint32_t i = 0;
  multi_gesture_event_t multi_gesture_event;
  return_value_if_fail(loop != NULL && touch != NULL && points != NULL, RET_BAD_PARAMS);
  // printf("-------------------------------------------- \r\n");
  for (; i < point_size; i++) {
    multi_touch_point_event_t* point = &(points[i]);
    // printf("point:(%d, %d), type:%d \r\n", point->x, point->y, point->type);
    if (multi_gesture_get_event_form_fingers(touch, point, &multi_gesture_event)) {
	    // printf("centroid:(%f, %f), dtheta:%f, d_dist:%f, num_down_fingers:%d \r\n", touch->centroid.x, touch->centroid.y, touch->dtheta, touch->d_dist, touch->last_point.size);
      multi_gesture_event_init(&multi_gesture_event, window_manager(), touch->centroid.x, touch->centroid.y,
                              touch->dtheta, touch->d_dist);
      main_loop_post_multi_gesture_event(loop, &multi_gesture_event);
    }
  }
  if (point_size == 0 || (point_size > 0 && touch->last_point.size == 0)) {
    multi_gesture_event_init(&multi_gesture_event, window_manager(), 0, 0, 0, 0);
    main_loop_post_multi_gesture_event(loop, &multi_gesture_event);
    if (point_size == 0) {
      touch->centroid.x = 0;
      touch->centroid.y = 0;
      darray_clear(&(touch->last_point));
    }
  }
  return RET_OK;
}



#ifndef MULTI_GESTURE_DIST_MIN_RANGE
#define MULTI_GESTURE_DIST_MIN_RANGE  1.0f
#endif

/**
 * @class multi_gesture_touch_fingers_t
 * @annotation ["scriptable"]
 * @parent event_t
 * 不可识别手指类型多点触控对象
 */
typedef struct _multi_gesture_touch_points_t {
  /**
   * @property {int64_t} touch_id
   * @annotation ["readable", "scriptable"]
   * touch device id。
   */
  int64_t touch_id;
  /* private */
  float_t last_dist;
  uint32_t last_dist_length;
  slist_t last_dist_list;
  uint32_t num_down_fingers;

  pointf_t last_vector;
} multi_gesture_touch_points_t;

/**
 * @method multi_gesture_gesture_touch_points_destroy
 * 释放不可识别手指类型的对象
 * @param {multi_gesture_touch_points_t*} touch 不可识别手指类型的对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t multi_gesture_gesture_touch_points_destroy(multi_gesture_touch_points_t* touch) {
  return_value_if_fail(touch != NULL, RET_BAD_PARAMS);

  slist_deinit(&(touch->last_dist_list));
  TKMEM_FREE(touch);

  return RET_OK;
}

/**
 * @method multi_gesture_touch_points_create
 * 创建不可识别手指类型的对象
 * @param {uint32_t} last_dist_length 缩放比例均值滤波的缓冲长度。
 *
 * @return {multi_gesture_touch_points_t*} 成功返回不可识别手指类型的对象，失败返回NULL。
 */
multi_gesture_touch_points_t* multi_gesture_touch_points_create(uint32_t last_dist_length) {
  multi_gesture_touch_points_t* touch = TKMEM_ZALLOC(multi_gesture_touch_points_t);
  return_value_if_fail(touch != NULL, NULL);
  touch->last_dist_length = last_dist_length;
  slist_init(&(touch->last_dist_list), default_destroy, NULL);
  return touch;
}

static float_t multi_gesture_touch_get_last_d_dists(multi_gesture_touch_points_t* touch, float_t dists) {
  uint32_t size = 0;
  slist_t* slist = NULL;
  float_t d_dists = 0.0f;
  slist_node_t* iter = NULL;
  float_t* tmp = TKMEM_ZALLOC(float_t);
  return_value_if_fail(tmp != NULL, RET_OOM);
  return_value_if_fail(touch != NULL, RET_BAD_PARAMS);

  *tmp = dists;
  slist = &(touch->last_dist_list);
  if (touch->last_dist_length <= slist_size(slist)) {
    void* data = slist_head_pop(slist);
    TKMEM_FREE(data);
  }
  slist_append(slist, tmp);

  iter = slist->first;
  while (iter != NULL) {
    d_dists += *((float_t*)(iter->data));
    iter = iter->next;
    size++;
  }
  // printf("size:%d \r\n", size);
  return d_dists / size;
}

static float_t multi_gesture_touch_get_dtheta(multi_gesture_touch_points_t* touch, point_t point1, point_t point2, float_t dist) {
  float_t dtheta = 0.0f;
  pointf_t v1 = {(point1.x - point2.x) / dist, (point1.y - point2.y) / dist};
  pointf_t v2 = {(point2.x - point1.x) / dist, (point2.y - point1.y) / dist};
  return_value_if_fail(touch != NULL, RET_BAD_PARAMS);
  if (touch->last_vector.x == 0 && touch->last_vector.y == 0 || dist == 0) {
    touch->last_vector.x = v1.x;
    touch->last_vector.y = v1.y;
  } else {
    float_t dtheta1 = atan2f(touch->last_vector.x * v1.y - touch->last_vector.y * v1.x, touch->last_vector.x * v1.x + touch->last_vector.y * v1.y);
    float_t dtheta2 = atan2f(touch->last_vector.x * v2.y - touch->last_vector.y * v2.x, touch->last_vector.x * v2.x + touch->last_vector.y * v2.y);

    if (tk_abs(dtheta1) > tk_abs(dtheta2)) {
      dtheta = dtheta2;
      touch->last_vector.x = v2.x;
      touch->last_vector.y = v2.y;
    } else {
      dtheta = dtheta1;
      touch->last_vector.x = v1.x;
      touch->last_vector.y = v1.y;
    }
  }
  return dtheta;
}

static ret_t multi_gesture_touch_points_reset(multi_gesture_touch_points_t* touch) {
  return_value_if_fail(touch != NULL, RET_BAD_PARAMS);
  touch->last_dist = 0;
  touch->last_vector.x = 0;
  touch->last_vector.y = 0;
  slist_remove_all(&(touch->last_dist_list));
  return RET_OK;
}

/**
 * @method multi_gesture_post_event_from_points
 * 发送多点触控消息
 * @param {main_loop_t*} loop 主循环对象
 * @param {multi_gesture_touch_points_t*} touch 不可识别手指类型的对象。
 * @param {uint32_t} point_size 手指个数
 * @param {multi_touch_point_event_t*} points 手指数据数组
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t multi_gesture_post_event_from_points(main_loop_t* loop, multi_gesture_touch_points_t* touch, uint32_t point_size, multi_touch_point_event_t* points) {
  uint32_t i = 0;
  float_t t_dist = 0.0f;
  float_t d_dist = 0.0f;
  float_t d_theta = 0.0f;
  point_t* point_list = NULL;
  uint32_t num_down_fingers = 0;
  pointf_t centroid = { 0.0f, 0.0f };
  multi_gesture_event_t multi_gesture_event;
  return_value_if_fail(loop != NULL && points != NULL, RET_BAD_PARAMS);

  point_list = TKMEM_ZALLOCN(point_t, point_size);
  return_value_if_fail(point_list != NULL, RET_OOM);

  if (point_size < 2) {
    TKMEM_FREE(point_list);
    multi_gesture_touch_points_reset(touch);
    return RET_OK;
  }

  // printf("---------------------------------------\r\n");
  for (i = 0; i < point_size; i++) {
    multi_touch_point_event_t* point = &(points[i]);
    uint32_t type = point->type;
    if (type == EVT_MULTI_TOUCH_MOVE) {
      // printf("x:%d, y:%d \r\n", point->x, point->y);
      point_list[num_down_fingers].x = point->x;
      point_list[num_down_fingers].y = point->y;
      num_down_fingers++;
    }
  }
  
  if (num_down_fingers > 1 && touch->num_down_fingers == num_down_fingers) {
    if (num_down_fingers == 2) {
      int32_t x = point_list[0].x;
      int32_t y = point_list[0].y;
      centroid.x = x;
      centroid.y = y;
      for (i = 1; i < num_down_fingers; i++) {
        x -= point_list[i].x;
        y -= point_list[i].y;
        centroid.x = (centroid.x * (i - 1) + point_list[i].x) / i;
        centroid.y = (centroid.y * (i - 1) + point_list[i].y) / i;
      }
      t_dist = sqrtf(x * x + y * y);
      d_theta = multi_gesture_touch_get_dtheta(touch, point_list[0], point_list[1], t_dist);
    } else {
      touch->last_vector.x = 0;
      touch->last_vector.y = 0;
      centroid.x = point_list[i].x;
      centroid.y = point_list[i].y;
      for (i = 1; i < num_down_fingers; i++) {
        centroid.x = (centroid.x * (i - 1) + point_list[i].x) / i;
        centroid.y = (centroid.y * (i - 1) + point_list[i].y) / i;
      }
      for (i = 0; i < num_down_fingers; i++) {
        float_t x = point_list[i].x - centroid.x;
        float_t y = point_list[i].y - centroid.y;
        t_dist += sqrtf(x * x + y * y);
      }
    }

    t_dist = multi_gesture_touch_get_last_d_dists(touch, t_dist);
    if (touch->last_dist != 0) {
      d_dist = t_dist - touch->last_dist;
      if (tk_abs(d_dist) <= MULTI_GESTURE_DIST_MIN_RANGE) {
        d_dist = 0.0f;
        t_dist = touch->last_dist;
      }
    }
    
    // printf("last_dist:%f, t_dist:%f, d_dist:%f, d_theta:%f, f:%d \r\n", touch->last_dist, t_dist, d_dist, d_theta, num_down_fingers);
    multi_gesture_event_init(&multi_gesture_event, window_manager(), centroid.x, centroid.y, d_theta, d_dist);
    main_loop_post_multi_gesture_event(loop, &multi_gesture_event);
  } else {
    multi_gesture_touch_points_reset(touch);
  }
  touch->num_down_fingers = num_down_fingers;

  if (num_down_fingers == 0 && touch->last_dist != 0) {
    multi_gesture_event_init(&multi_gesture_event, window_manager(), 0, 0, 0, 0);
    main_loop_post_multi_gesture_event(loop, &multi_gesture_event);
  }

  touch->last_dist = t_dist;
  if (t_dist == 0) {
    slist_remove_all(&(touch->last_dist_list));
  }
  TKMEM_FREE(point_list);
  return RET_OK;
}


