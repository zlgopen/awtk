/**
 * File:   vpage.h
 * Author: AWTK Develop Team
 * Brief:  虚拟页面(根据情况自动加载/卸载页面, 并提供入场/出场动画)。
 *
 * Copyright (c) 2021 - 2021 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-06-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VPAGE_H
#define TK_VPAGE_H

#include "base/widget.h"

BEGIN_C_DECLS
/**
 * @class vpage_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 虚拟页面(根据情况自动加载/卸载页面，并提供入场/出场动画)。
 *
 * > 虚拟页面只能作为pages的直接子控件使用。
 *
 * 如果指定了ui_asset:
 * 
 *  * 当页面切换到后台时自动卸载，并触发EVT\_VPAGE\_CLOSE消息。
 *  * 当页面切换到前台时自动加载，在动画前出发EVT\_VPAGE\_WILL\_OPEN消息，在动画完成时触发 EVT\_VPAGE\_CLOSE消息。 
 * 
 * vpage\_t也可以当作普通view嵌入到pages中，让tab控件在切换时具有动画效果。
 * 
 * 在xml中使用"vpage"标签创建控件。如：
 *
 * ```xml
 * <!-- ui -->
 * <vpage x="c" y="50" w="100" h="100" ui_asset="mypage"/>
 * ```
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等(一般无需指定)。如：
 * 
 * ```xml
 * <!-- style -->
 * <vpage>
 *   <style name="default">
 *     <normal />
 *   </style>
 * </vpage>
 * ```
 */
typedef struct _vpage_t {
  widget_t widget;

  /**
   * @property {char*} ui_asset
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * UI资源名称。
   */
  char* ui_asset;

  /**
   * @property {char*} anim_hint
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 动画类型(目前支持：vtranslate: 垂直平移，htranslate: 水平平移)。
   */
  char* anim_hint;
} vpage_t;

/**
 * @event {event_t} EVT_VPAGE_WILL_OPEN
 * 页面即将打开(动画前)。
 */

/**
 * @event {event_t} EVT_VPAGE_OPEN
 * 页面打开完成(动画后)。
 */

/**
 * @event {event_t} EVT_VPAGE_CLOSE
 * 页面已经关闭(动画后)。
 */

/**
 * @method vpage_create
 * @annotation ["constructor", "scriptable"]
 * 创建vpage对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} vpage对象。
 */
widget_t* vpage_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method vpage_cast
 * 转换为vpage对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget vpage对象。
 *
 * @return {widget_t*} vpage对象。
 */
widget_t* vpage_cast(widget_t* widget);

/**
 * @method vpage_set_ui_asset
 * 设置 UI资源名称。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} ui_asset UI资源名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vpage_set_ui_asset(widget_t* widget, const char* ui_asset);

/**
 * @method vpage_set_anim_hint
 * 设置动画类型(vtranslate: 垂直平移，htranslate: 水平平移)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} anim_hint 动画类型。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vpage_set_anim_hint(widget_t* widget, const char* anim_hint);

/**
 * @enum vpage_event_t
 * @annotation ["scriptable"]
 * @prefix EVT_
 * 虚拟页面的事件。
 */
typedef enum _vpage_event_t {
  /**
   * @const EVT_VPAGE_WILL_OPEN
   * 页面即将打开(动画前)。
   */
  EVT_VPAGE_WILL_OPEN = EVT_USER_START + 100,
  /**
   * @const EVT_VPAGE_OPEN
   * 页面打开完成(动画后)。
   */
  EVT_VPAGE_OPEN,
  /**
   * @const EVT_VPAGE_CLOSE
   * 页面已经关闭(动画后)。
   */
  EVT_VPAGE_CLOSE
} vpage_event_t;

#define VPAGE_PROP_UI_ASSET "ui_asset"

#define VPAGE_ANIM_VTRANSLATE "vtranslate"
#define VPAGE_ANIM_HTRANSLATE "htranslate"

#define WIDGET_TYPE_VPAGE "vpage"

#define VPAGE(widget) ((vpage_t*)(vpage_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(vpage);

END_C_DECLS

#endif /*TK_VPAGE_H*/
