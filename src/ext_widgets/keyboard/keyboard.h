/**
 * File:   keyboard.h
 * Author: AWTK Develop Team
 * Brief:  keyboard
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
 * 2018-06-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_KEYBOARD_H
#define TK_KEYBOARD_H

#include "base/window_base.h"

BEGIN_C_DECLS

/**
 * @class keyboard_t
 * @parent window_base_t
 * @annotation ["design","widget","window", "scriptable"]
 * 软键盘。
 *
 * 软键盘是一个特殊的窗口，由编辑器通过输入法自动打开和关闭。
 *
 * 这里介绍一下定制软键盘的方法：
 *
 *  编辑器输入类型和软键盘UI资源文件的对应关系:
 *
 *| 输入类型       | 软键盘UI资源文件|
 *|----------------|:---------------:|
 *| INPUT\_PHONE    | kb\_phone.xml    |
 *| INPUT\_INT      | kb\_int.xml      |
 *| INPUT\_FLOAT    | kb\_float.xml    |
 *| INPUT\_UINT     | kb\_uint.xml     |
 *| INPUT\_UFLOAT   | kb\_ufloat.xml   |
 *| INPUT\_HEX      | kb\_hex.xml      |
 *| INPUT\_EMAIL    | kb\_ascii.xml    |
 *| INPUT\_PASSWORD | kb\_ascii.xml    |
 *| INPUT\_CUSTOM   | 使用自定义的键盘 |
 *| 其它            | kb\_default.xml  |
 *
 * keyboard中按钮子控件的名称有些特殊要求：
 *
 *|  名称          | 功能            |
 *|----------------|:---------------:|
 *| return         | 回车键          |
 *| action         | 定制按钮        |
 *| backspace      | 删除键          |
 *| tab            | tab键           |
 *| space          | 空格键          |
 *| close          | 关闭软键盘       |
 *| back           | 关闭当前窗口     |
 *| back_to_home   | 返回home窗口    |
 *| 前缀key:        | 键值           |
 *| 前缀hard_key:   | 模拟物理键盘    |
 *| 前缀page:       | 切换到页面      |
 *| 前缀opt:        | 多个字符选择一个，点击切换到下一个，超时提交字符(用于实现九宫格输入) |
 *
 * 示例：
 * 
 * * 按键"a"，提交输入法处理。
 * 
 * ```xml
 * <button repeat="300" name="key:a" text="a"/>
 * ```
 * 
 * * 字符"a"，直接提交到编辑器。
 * 
 * ```xml
 * <button repeat="300" name="a" text="a"/>
 * ```
 * 
 * * 模拟物理键盘数字"1"，触发key down/up事件（可以用来选择候选字）。
 * 
 * ```xml
 * <button repeat="300" name="hard_key:1" text="1"/>
 * ```
 
 * * 九宫格输入
 * 
 * ```xml
 *   <button repeat="300" name="opt:._@/#" text="._@/#"/>
 *   <button repeat="300" name="opt:abc" text="abc"/>
 *   <button repeat="300" name="opt:def" text="def"/>
 * ```
 *
 *
 * * 输入语言切换
 *
 *  有的输入法，同时支持输入多种语言。
 *  比如T9，可以同时支持中文和英文输入，配合软键盘随时切换输入的语言。
 *
 * 可以在pages的页面里指定lang属性，切换到该页面时会设置输入法的语言。如：
 *
 * ```xml
 * <pages x="0" y="bottom" w="100%" h="-28" active="2">
 *   <view name="lower" lang="en_us"
 *     x="0" y="0" w="100%" h="100%" children_layout="default(r=4,c=4,s=2,m=2)">
 *     ...
 *    </view>
 *    <view name="chinese" lang="zh_cn"
 *     x="0" y="0" w="100%" h="100%" children_layout="default(r=4,c=4,s=2,m=2)">
 *     ...
 *    </view>
 * </pages>
 * ```
 *
 * > 更多用法请参考：
 * [kb_default](https://github.com/zlgopen/awtk/blob/master/design/default/ui/kb_default.xml)
 *
 */

typedef struct _keyboard_t {
  window_base_t window;

  /*private*/
  widget_t* last_click_button;
  darray_t action_buttons;
  uint32_t action_info_id;
  uint32_t timer_id;
  str_t temp;
  int key_down;
} keyboard_t;

/**
 * @method keyboard_create
 * 创建keyboard对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* keyboard_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method keyboard_close
 * 关闭keyboard窗口。
 * @annotation ["deconstructor"]
 * @param {widget_t*} parent keyboard对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t keyboard_close(widget_t* parent);

/**
 * @method keyboard_cast
 * 转换为keyboard对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget keyboard对象。
 *
 * @return {widget_t*} keyboard对象。
 */
widget_t* keyboard_cast(widget_t* widget);

#define KEYBOARD(widget) ((keyboard_t*)(keyboard_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(keyboard);

END_C_DECLS

#endif /*TK_KEYBOARD_H*/
