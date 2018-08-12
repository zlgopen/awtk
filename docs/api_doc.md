## 注释格式

AWTK中的API注释，除了作为API的文档之外，还充当以下重要的作用：

* 提取JSON格式的IDL，用于生成各种语言的绑定代码。
* 用于设计器(designer)获取各个控件而元信息。

我采用了类似于[jsduck](https://github.com/senchalabs/jsduck)的API注释格式，但是jsduck并不支持C语言的数据类型，所以没有办法完全兼容jsduck的格式。

### 一、类的注释

示例：

```
/**
 * @class progress_bar_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 进度条控件。
 */
```

里面说明了类的名称、基类的名称和该类型是否可以脚本化。对于类，annotation的取值有：

* scriptable 该类可以被脚本化。
* fake 该类是fake的，并不真实存在。

### 二、类成员变量注释

示例：

```
  /** 
   * @property {uint8_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 进度条的值[0-100]。
   */
```

里面说明了成员变量的类型、名称和是否只读等信息。

### 三、函数的注释

示例：

```
/**
 * @method progress_bar_set_value
 * 设置进度条的进度。
 * @param {widget_t*} widget 控件对象。
 * @param {uint8_t}  value 进度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
```

里面说明了函数的名称、参数和返回值。缺省为前一个类的成员函数，也可以用@global把它标识为一个全局函数。构造函数和析构可能会有不同的处理，所以用@constructor和deconstructor分别标识出来。有的函数自动产生绑定代码比较困难，此时可以用@custom标识为手写绑定代码。

### 四、枚举的注释

示例：

```
/**
 * @enum align_v_t
 * @scriptable
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

里面定义了枚举的名称和各个枚举值。

