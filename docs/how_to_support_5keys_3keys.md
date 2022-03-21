# 如何支持极简键盘 (3 键键盘、5 键键盘和旋转按钮）

## 背景

在有的硬件上，没有触摸/鼠标等指针设备，只有极简的键盘，典型情况如下

* 3 键键盘（回车键+上下键），旋转按钮也可以归为此类，左右旋转映射成上下键。有些变种带有数字键和功能键也归于此类。

* 5 键键盘（回车键+上下左右键）。有些变种带有数字键和功能键也归于此类。

对于上面的情况，方向键既要用于导航（切换焦点），又要用于控件输入数据，所以需要引入一种状态（或称为模式）：

* 在移动焦点的状态：方向键用于切换焦点。

* 在非移动焦点的状态：方向键用于修改控件的值。

回车的作用：

* 在按钮等支持用回车键激活的控件上，用于激活按钮，相当于点击事件。

* 在编辑器等控件上，用于切换状态，用于改变方向键的作用。

> 旋转按钮处理方法：左旋触发方向左键，右旋触发方向右键，按下触发回车键。

## 用法

AWTK 对此做了内置的支持，在初始化时指定键盘的类型，代码如下：

```c
ret_t application_init() {
  system_info_set_keyboard_type(system_info(), KEYBOARD_5KEYS);

  return RET_OK;
}
```

键盘类型的定义：

```c
/**
 * @enum keyboard_type_t
 * 键盘的类型
 */
typedef enum _keyboard_type_t {
  /**
   * @const KEYBOARD_NONE
   * 无键盘。
   */
  KEYBOARD_NONE = 0,
  /**
   * @const KEYBOARD_NORMAL
   * 正常键盘。
   */
  KEYBOARD_NORMAL,
  /**
   * @const KEYBOARD_3KEYS
   * 3 键 (RETURN+上下键，附加数字键等）。
   * > RETURN 键用于切换模式：焦点模式下，上下键用于切换焦点，非焦点模式下，上下键用于切修改控件的值。
   */
  KEYBOARD_3KEYS,
  /**
   * @const KEYBOARD_5KEYS
   * 5 键 (RETURN+上下左右键，附加数字键等）。
   * > RETURN 键用于切换模式：焦点模式下，上下键用于切换焦点，非焦点模式下，上下键用于切修改控件的值。
   */
  KEYBOARD_5KEYS
} keyboard_type_t;
```

此外，对想要使用按键切换焦点的控件，需要将控件的 focusable 属性设置为 true，支持焦点停留：

```xml
<button name="button" text="button" focusable="true"/>
```

> 备注：edit及其组合控件（比如：combo_box、combo_box_ex等）默认支持焦点停留，无需设置 focusable 属性。

如果希望软件也支持按键切换焦点，步骤如下：

步骤一：设置软件盘的 grab_keys 属性为 true，代码如下：

```xml
<keyboard theme="keyboard" x="0" y="bottom" w="100%" h="40%" grab_keys="true" name="kb_default">
  ...
</keyboard>
```

步骤二：将软键盘上按钮的 focusable 属性设置为 true，支持焦点停留，代码如下：

```xml
<keyboard theme="keyboard" x="0" y="bottom" w="100%" h="40%" grab_keys="true" name="kb_default">
  ...
      <view x="0" w="100%" h="25%" children_layout="default(r=1,c=10)">
        <button repeat="300" name="q" focusable="true" text="q"/>
        <button repeat="300" name="w" focusable="true" text="w"/>
        ...
      </view>
      ...
</keyboard>
```

## 示例
 * [5keys](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/5keys.c)

## 其他
  如果希望在激活时，控件呈现不同的外观效果，请定义宏 WITH\_STATE\_ACTIVATED。同时在 style 文件中编写 activated状态的参数。如：

```xml
<slider>
  <style>
    <normal bg_color="#a0a0a0" fg_color="blue" border_color="#ffd700"/>
    <pressed bg_color="#a0a0a0" fg_color="blue" border_color="#ffa700"/>
    <over bg_color="#a0a0a0" fg_color="blue" border_color="#ffe700"/>
    <focused bg_color="#a0a0a0" fg_color="blue" border_color="#ffff00"/>
    <activated bg_color="#a0a0a0" fg_color="blue" border_color="#ff0000"/>
  </style>
</slider>
```

请参考：https://github.com/zlgopen/awtk-c-demos/blob/master/design/default/styles/5keys.xml

 

