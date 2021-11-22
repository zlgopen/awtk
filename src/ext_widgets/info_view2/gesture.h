


#ifndef TK_CUSTOM_GESTURE_H

#define TK_CUSTOM_GESTURE_H



#include "base/widget.h"

#include "awtk.h"

#include "../info_view/info_view2.h"



BEGIN_C_DECLS



#define EVT_SLIDE_DOWN EVT_USER_START + 1

#define EVT_SLIDE_UP EVT_USER_START + 2

#define EVT_SLIDE_LEFT EVT_USER_START + 3

#define EVT_SLIDE_RIGHT EVT_USER_START + 4

#define EVT_DOUBLE_TAP EVT_USER_START + 5

#define INFO_VIEW_THRESHOLD 5



static info_view2_t *s_info_view2 = NULL;

#define S_WIDGET_H 480

#define S_WIDGET_W 480

info_view2_t *get_info_view2();



typedef struct _gesture_t {

    widget_t widget;



    /**

   * 按下时的点击点

   */

    point_t press_point;



    /**

   * 是否被按下

   */

    bool_t pressed;

    bool_t long_pressed;



    // /**

    //  * @brief  是否忽略输入

    //  * @note   当子控件存在滑动操作时，可通设置这个属性为真以使本控件忽略指针

    //  */

    //bool_t ignore_slide;



    //

    bool_t enable_dt;

    bool_t dt_flag;

    int32_t dt_timer_id;



    /**

   * 长按计时器id

   */

    int32_t timer_id;

    widget_t *grad_widget;

    bool_t child_graded;



    bool_t info_view2_show;

    velocity_t velocity;

    int32_t yoffset;

    bool_t move;



} gesture_t;



widget_t *gesture_create(widget_t *parent, xy_t x, xy_t y, wh_t w, wh_t h);



#define WIDGET_TYPE_GESTURE "gesture"

#define GESTURE(widget) ((gesture_t *)(widget))



info_view2_t *get_info_view2_no_widget();

info_view2_t *get_info_view2(widget_t *widget);

END_C_DECLS



#endif

