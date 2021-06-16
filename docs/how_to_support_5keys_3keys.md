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

## 用法

AWTK 对此做了内置的支持，使用方法如下，在初始化时指定键盘的类型即可。

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
