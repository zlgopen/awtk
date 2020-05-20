## API 注释格式

AWTK 中的 API 注释，除了作为 API 的文档之外，还有以下用途：

* 提取 JSON 格式的 IDL，用于生成各种语言的绑定代码。
* 用于设计器 (designer) 获取各个控件的元信息。
* 生成动态库的导出符号表。

这里采用了类似于 [jsduck](https://github.com/senchalabs/jsduck) 的 API 注释格式，但是 jsduck 并不支持 C 语言的数据类型，所以没有办法完全兼容 jsduck 的格式。

### 一、类的注释

@class 表示类定义。

示例：

```
/**
 * @class progress_bar_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 进度条控件。
 */
```

里面说明了类的名称、基类的名称和该类型是否可以脚本化。对于类，annotation 的取值有：

* scriptable 该类可以被脚本化。
* fake 该类是 fake 的，并不真实存在。
* widget 表示该类是 widget 的子类。
* window 表示该类是窗口的子类。
* design 表示可以在 UI 设计器中使用。

### 二、属性注释

@property 表示属性定义。

示例：

```
  /** 
   * @property {uint8_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 进度条的值 [0-100]。
   */
```

里面说明了成员变量的类型、名称和是否只读等信息。对于 property，annotation 的取值有：

* set\_prop 是否可以通过 widget\_set\_prop 来设置该属性。
* get\_prop 是否可以通过 widget\_get\_prop 来获取该属性。
* readable 该属性是否可以直接读取。
* writable 该属性是否可以直接修改。
* persitent 该属性是否需要持久化。
* design   该属性可以在设计器中设置。
* scriptable 该属性是否支持脚本化。

### 三、函数的注释

@method 表示函数定义。

示例：

```
/**
 * @method progress_bar_create
 * @annotation ["constructor", "scriptable"]
 * 创建 progress_bar 对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x 坐标
 * @param {xy_t} y y 坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* progress_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h); 

/**
 * @method progress_bar_cast
 * 转换为 progress_bar 对象（供脚本语言使用）。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget progress_bar 对象。
 *
 * @return {widget_t*} progress_bar 对象。
 */
widget_t* progress_bar_cast(widget_t* widget);

/**
 * @method progress_bar_set_value
 * 设置进度条的进度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint8_t}  value 进度
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t progress_bar_set_value(widget_t* widget, uint8_t value);
```

里面说明了函数的名称、参数和返回值。对于 property，annotation 的取值有：

* global 是否是全局函数。除了指明为全局函数，函数是当前类的成员函数。
* cast 类型转换函数。
* constructor 构造函数
* deconstructor 析构函数
* scriptable 是否可以脚本化。对于特殊函数（通常有回调函数作为参数）不方便直接产生代码，可以指定为 scriptable:custom，使用定制的绑定代码。

### 四、枚举的注释

@enum 表示枚举定义。

示例：

```
/**
 * @enum align_v_t
 * @annotation ["scriptable"]
 * 垂直对齐的常量定义。
 */
typedef enum _align_v_t {
  /**
   * @const ALIGN_V_NONE
   * 无效对齐方式。
   */
  ALIGN_V_NONE= 0,
  /**
   * @const ALIGN_V_MIDDLE
   * 居中对齐。
   */
  ALIGN_V_MIDDLE,
  /**
   * @const ALIGN_V_TOP
   * 顶部对齐。
   */
  ALIGN_V_TOP,
  /** 
   * @const ALIGN_V_BOTTOM
   * 底部对齐。
   */
  ALIGN_V_BOTTOM
}align_v_t;
```

里面定义了枚举的名称和各个枚举值。对于枚举，annotation 的取值有：

* scriptable 该类可以被脚本化。

### 五、事件的注释

@event 表示事件定义。

示例：

```c
/**
 * @event {pointer_event_t} EVT_CLICK
 * 点击事件。
 */

/**
 * @event {pointer_event_t} EVT_LONG_PRESS
 * 长按事件。
 */
```
