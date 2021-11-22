#include "gesture.h"

#include "base/timer.h"

#include "base/widget_vtable.h"

#include "common/log.h"



static ret_t gesture_remove_timer(widget_t *widget) {

    gesture_t *gesture = GESTURE(widget);

    if (gesture->timer_id != TK_INVALID_ID) {

        timer_remove(gesture->timer_id);

        gesture->timer_id = TK_INVALID_ID;

    }



    return RET_OK;

}



static ret_t gesture_pointer_up_cleanup(widget_t *widget) {

    gesture_t *gesture = GESTURE(widget);

    gesture->move = FALSE;

    gesture->yoffset = 0;

    gesture->pressed = FALSE;

    get_info_view2(WIDGET(gesture)->parent)->yoffset = 0;



    return RET_OK;

}



static ret_t gesture_on_long_press(const timer_info_t *info) {

    LOG("gesture_on_long_press\r\n");

    widget_t *widget = WIDGET(info->ctx);

    gesture_t *gesture = GESTURE(widget);

    widget_t *window = widget_get_window(widget);



    if (gesture->pressed) {

        pointer_event_t evt;

        pointer_event_init(&evt, EVT_LONG_PRESS, widget, 0, 0);

        LOG("loog press enable");



        gesture_pointer_up_cleanup(widget);

        widget_dispatch(widget, (event_t *)&evt);



        gesture->long_pressed = TRUE;

        pointer_event_init(&evt, EVT_POINTER_DOWN_ABORT, window, 0, 0);

        widget_dispatch(window, (event_t *)&evt);

    }



    return RET_REMOVE;

}



static widget_t *gesture_find_grabed_target(widget_t *widget) {

    widget_t *grabed = NULL;

    return_value_if_fail(widget != NULL, NULL);



    while (widget->grab_widget != NULL) {

        grabed = widget->grab_widget;

        widget = widget->grab_widget;

    }

    return grabed;

}



static ret_t gesture_on_pointer_up(gesture_t *gesture, pointer_event_t *e) {

    LOG("into gesture_on_pointer_up");



    int32_t yoffset_end = 0;

    int32_t yoffset = gesture->yoffset;

    int32_t xoffset_end = 0;

    int32_t xoffset = 0;

    uint32_t v_threshhold = 100;

    widget_t *widget = WIDGET(gesture);

    velocity_t *v = &(gesture->velocity);



    velocity_update(v, e->e.time, e->x, e->y);



    int32_t h = widget->h;

    int32_t yv = tk_abs(v->yv);

    bool_t rollback = (yv < v_threshhold) && (tk_abs(yoffset) < h / 2);

    LOG("yoffset is %d  roolback is %d", yoffset, rollback);



    if (tk_abs(yoffset) < TK_DRAG_THRESHOLD) {

        rollback = TRUE;

    }



    if (!rollback) {

        yoffset_end = yoffset > 0 ? h : -h;

    }

    LOG("yoffset is %d  roolback is %d", yoffset, rollback);

    if (yoffset > 0 && gesture->info_view2_show == TRUE) {

        gesture->yoffset = 0;

        get_info_view2(WIDGET(gesture)->parent)->yoffset = 0;

        widget->dirty = FALSE;

        widget_invalidate(WIDGET(s_info_view2), NULL);

        gesture->info_view2_show = FALSE;

        LOG("通知栏不能下拉了");

        return RET_OK;

    } else if (yoffset < 0 && gesture->info_view2_show == FALSE) {

        gesture->yoffset = 0;

        get_info_view2(WIDGET(gesture)->parent)->yoffset = 0;

        widget->dirty = FALSE;

        widget_invalidate(s_info_view2, NULL);

        gesture->info_view2_show = FALSE;

        LOG("界面不能上拉了");

        return RET_OK;

    }

    if (gesture->yoffset > 0 && gesture->press_point.y > INFO_VIEW_THRESHOLD) {

        LOG("指针抬起，但是向下拉切初始点不在INFO_VIEW_THRESHOLD范围内，啥也不做，直接返回");

        return RET_OK;

    }

    //TODO添加通知栏根据yoffest进行展开或者回退

    //return gesture_animate_to(gesture, xoffset, yoffset, xoffset_end, yoffset_end);

    return RET_OK;

}



static ret_t gesture_on_pointer_move(gesture_t *gesture, pointer_event_t *e) {

    //LOG("into info_view_on_pointer_move");



    velocity_t *v = &(gesture->velocity);

    velocity_update(v, e->e.time, e->x, e->y);

    uint32_t ydistance = (e->y - gesture->press_point.y);



    gesture->yoffset = ydistance;

    get_info_view2(WIDGET(gesture)->parent)->yoffset = ydistance;



    return RET_OK;

}



static ret_t gesture_on_event(widget_t *widget, event_t *e) {

    uint16_t type = e->type;

    gesture_t *gesture = GESTURE(widget);



    switch (type) {

    case EVT_POINTER_DOWN: {

        LOG("down");

        gesture->pressed = TRUE;

        pointer_event_t *pointer_event = (pointer_event_t *)e;

        gesture->press_point.x = pointer_event->x;

        gesture->press_point.y = pointer_event->y;

        velocity_t *v = &(gesture->velocity);

        velocity_reset(v);

        velocity_update(v, pointer_event->e.time, pointer_event->x, pointer_event->y);



        break;

    }

    case EVT_POINTER_DOWN_ABORT: {

        LOG("down_abort");

        if (gesture->pressed) {

            LOG("EVT_POINTER_DOWN_ABORT");

            gesture_pointer_up_cleanup(widget);

        }

        break;

    }

    case EVT_POINTER_MOVE: {

        point_t *press_point = &(gesture->press_point);

        pointer_event_t *evt = (pointer_event_t *)e;

        int32_t delta = 0;

        delta = evt->y - gesture->press_point.y;



        if (delta > 0 && gesture->pressed == TRUE) {

            LOG("往下划,初始点必须是最上面的INFO_VIEW_THRESHOLD个像素");

            if (press_point->y > INFO_VIEW_THRESHOLD) {

                //不在初始的INFO_VIEW_THRESHOLD个像素点，就不理会直接返回，仅记录位置

                LOG("不在初始的INFO_VIEW_THRESHOLD个像素点");

                gesture->yoffset = delta;



            } else {

                LOG("在初始的INFO_VIEW_THRESHOLD个像素点");

                //在最初始的INFO_VIEW_THRESHOLD个像素，进行info_view的下拉绘制

                if (gesture->move) {

                    //更新info_view的位置

                    gesture_on_pointer_move(gesture, evt);

                    widget_invalidate(s_info_view2, NULL);

                } else if (evt->pressed && gesture->pressed) {

                    if (tk_abs(delta) > TK_DRAG_THRESHOLD) {

                        pointer_event_t abort;

                        pointer_event_init(&abort, EVT_POINTER_DOWN_ABORT, widget, evt->x, evt->y);

                        widget_dispatch_event_to_target_recursive(widget, (event_t *)(&abort));

                        gesture->move = TRUE;

                        gesture_on_pointer_move(gesture, evt);

                        widget_invalidate(s_info_view2, NULL);

                    }

                }

            }

        } else if (delta < 0 && gesture->pressed == TRUE) {

            LOG("往上划，不对初始点进行要求");

            if (gesture->move) {

                //更新info_view的位置

                gesture_on_pointer_move(gesture, evt);

                widget_invalidate(s_info_view2, NULL);

            } else if (evt->pressed && gesture->pressed) {

                if (tk_abs(delta) > TK_DRAG_THRESHOLD) {

                    pointer_event_t abort;

                    pointer_event_init(&abort, EVT_POINTER_DOWN_ABORT, widget, evt->x, evt->y);

                    widget_dispatch_event_to_target_recursive(widget, (event_t *)(&abort));

                    gesture->move = TRUE;

                    gesture_on_pointer_move(gesture, evt);

                    widget_invalidate(s_info_view2, NULL);

                }

            }

        }



        break;

    }

    case EVT_POINTER_UP: {

        LOG("up");

        if (gesture->pressed) {

            if (gesture->move) {

                //指针抬起后要么对info_view进行后退，要么直接画出来

                gesture_on_pointer_up(gesture, (pointer_event_t *)e);

            }

            gesture_pointer_up_cleanup(widget);

        }

        break;

    }

    }

    return RET_OK;

}



static ret_t gesture_on_destroy(widget_t *widget) {

    return gesture_remove_timer(widget);

}



static ret_t gesture_on_pointer_up_before_children(void *ctx, event_t *e) {

    widget_t *widget = WIDGET(ctx);

    gesture_t *gesture = GESTURE(widget);

    widget_t *grad_widget = gesture_find_grabed_target(widget);



    if (gesture->grad_widget != NULL && gesture->grad_widget == grad_widget) {

        pointer_event_t *evt = (pointer_event_t *)e;

        bool_t point_in = widget_is_point_in(grad_widget, evt->x, evt->y, FALSE);



        if (!tk_str_eq(grad_widget->name, "profile")) {

            return RET_OK;

        }



        if (!point_in) {

            return RET_OK;

        }



        if (!grad_widget->vt->inputable && !tk_str_eq(widget_get_type(grad_widget), WIDGET_TYPE_BUTTON)) {

            return RET_OK;

        }



        gesture->child_graded = TRUE;

    }



    return RET_OK;

}



static const widget_vtable_t s_gesture_vtable = {.size = sizeof(gesture_t),

                                                 .type = WIDGET_TYPE_GESTURE,

                                                 .create = gesture_create,

                                                 .on_paint_self = widget_on_paint_self_default,

                                                 .on_event = gesture_on_event,

                                                 .on_destroy = gesture_on_destroy};



widget_t *gesture_create(widget_t *parent, xy_t x, xy_t y, wh_t w, wh_t h) {

    gesture_t *gesture = TKMEM_ZALLOC(gesture_t);

    widget_t *widget = WIDGET(gesture);

    return_value_if_fail(gesture != NULL, NULL);



    widget_init(widget, parent, &s_gesture_vtable, x, y, w, h);



    gesture->pressed = FALSE;

    gesture->enable_dt = TRUE;

    gesture->dt_flag = FALSE;

    gesture->dt_timer_id = TK_INVALID_ID;

    gesture->long_pressed = FALSE;

    gesture->info_view2_show = FALSE;



    widget_on(widget, EVT_POINTER_UP_BEFORE_CHILDREN, gesture_on_pointer_up_before_children, widget);



    return widget;

}



/*

这里开始是关于info_view的部分

*/



static ret_t init_info_view2(widget_t *widget) {

    if (s_info_view2 != NULL) {

        LOG("s_info_view2 不是空的，不能进行初始化");

        return RET_FAIL;

    }

    s_info_view2 = info_view2_create(NULL, 0, -widget->h, widget->w, widget->h);

    s_info_view2->prev_window = widget;

    widget_t *button = button_create(s_info_view2, 240, 240, 200, 80);

    widget_set_text(button, L"test");

    return RET_OK;

}



info_view2_t *get_info_view2(widget_t *widget) {

    if (s_info_view2 == NULL) {

        LOG("s_info_view2为空，进行初始化");

        init_info_view2(widget);

        //widget_layout(s_info_view2);

    }

    return s_info_view2;

}



info_view2_t *get_info_view2_no_widget() {

    if (s_info_view2 == NULL) {

        LOG("s_info_view2为空");

    }

    return s_info_view2;

}