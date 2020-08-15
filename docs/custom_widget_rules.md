# 自定义控件规范

为了自定义控件可以在 Designer 中使用，需要将自定义控件编译成动态库。

编译脚本请参考：https://github.com/zlgopen/awtk-widget-number-label/blob/master/src/SConscript

## 1. 目录结构

* design 存放示例代码的原始资源，用 Designer 打开后可进行编辑。

* res 存放示例代码运行时需要的资源，由 Designer 打包 或者 scripts/update_res.py 脚本生成。

* bin 存放动态库和可执行文件。

* lib 存放静态库文件。

* demos 存放示例代码。

* idl 存放生成的 IDL 文件。

* scripts 存放资源生成的脚本文件。

* src 存放控件源码，每个控件放在独立的目录。

* tests 用于存放测试代码。

示例：

```
├─design
│  └─default
├─res
│  └─assets
│       └─default
├─bin
├─lib
├─demos
├─docs
├─idl
├─scripts
├─src
│  └─number_label
└─tests
```

> 完整示例参考 [number-label](https://github.com/zlgopen/awtk-widget-number-label)

## 2. 命名规范

### 2.1 控件类型名

控件类型名使用小写的英文单词，多个单词之间用下划线连接。

如：

```
#define WIDGET_TYPE_NUMBER_LABEL "number_label"
```

控件类型名用于注册控件，以便在 XML UI 文件中使用。

> 控件名要求能展示控件的功能。

### 2.2 子目录名

在 src 下的子目录名与控件类型名保持一致。

### 2.3 源文件名

* 头文件名：控件类型名 + ".h"
* 源文件名：控件类型名 + ".c"

### 2.4 类名

控件类型名 + "_t"

如：

```c
typedef struct _number_label_t {
  widget_t widget;
  ...
} number_label_t;  
```

> 自定义控件只能以 widget_t 作为父类。

### 2.2 函数名

成员函数以控件类型名为前缀，以下划线连接，全部使用小写单词。

如：
```
/**
 * @method number_label_set_format
 * 设置格式字符串（用于将浮点数转换为字符串）。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*} format 格式字符串。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t number_label_set_format(widget_t* widget, const char* format);
```

### 2.3 属性名

属性名使用小写的英文单词，多个单词之间用下划线连接。

> 属性名必须与 XML UI 文件中的属性名保持一致。

示例

```c
  /**
   * @property {char*} format
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   *
   * 格式字符串。
   *
   */
  char* format;
```

### 2.4 事件名

如果控件有自定义的事件，事件名以"EVT"为前缀，使用大写的英文单词，单词之间用下划线连接。

如：

```c
/**
 * @event {pointer_event_t} EVT_LONG_PRESS
 * 长按事件。
 */
```

### 2.5 动态库名

如果动态库中只有一个控件，动态库的名称使用控件类型名。如果动态库中有多个控件，可以自行考虑名称。

### 2.6 控件注册函数名

控件注册函数负责注册动态库中的控件。

控件注册函数名必须以动态库名为前缀，加上"_register"。注册函数无参数，并返回 ret_t。

如：

```c
/**
 * @method number_label_register
 * 注册数值文本控件。
 *
 * @annotation ["global"]
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t number_label_register(void);
```

### 2.7 获取支持的渲染模式的函数名

目前可选的渲染模式有OpenGL、AGGE-BGR565、AGGE-BGRA8888、AGGE-MONO；默认支持全部模式，模式之间以“|”间隔，可根据实际情况修改。

获取支持的渲染模式的函数名必须以动态库名为前缀，加上"_supported_render_mode"。该函数无参数，并返回 const char*。

如：

```c
/**
 * @method number_label_supported_render_mode
 * 获取支持的渲染模式。
 *
 * @annotation ["global"]
 *
 * @return {const char*} 返回 RET_OK 表示成功，否则表示失败。
 */
const char* number_label_supported_render_mode(void);
```

## 3. 注释格式

自定义控件需要使用 AWTK 标准注释。请参考 [API 注释格式](api_doc.md)

## 4. 辅助工具

awtk 提供了一些工具用于生成 IDL 和动态库导出符号表。

* awtk/tools/idl\_gen/index.js 用于生成 IDL。

如：

```
node ../awtk/tools/idl_gen/index.js idl/idl.json src/
```

* awtk/tools/dll\_def\_gen/index.js 用于生成动态库导出符号表。 

如：
```
node ../awtk/tools/dll_def_gen/index.js idl/idl.json src/number_label.def
```

> 使用这些工具需要安装 [nodejs](https://nodejs.org/zh-cn/)

## 5. 依赖的 AWTK 版本

可以在自定义控件项目的 project.json 文件中设置该动态库依赖的 AWTK 版本。

如：
```json
{
  ...
  "usesSdk": {
    "awtk:minSdkVersion": "20070",
    "awtk:maxSdkVersion": "20090",
    "awtk:targetSdkVersion": "20090"
  }
  ...
}
```

* 参数 awtk:minSdkVersion 是可选的，表示兼容的最低版本。

* 参数 awtk:maxSdkVersion 是可选的，表示兼容的最高版本。

* 参数 awtk:targetSdkVersion 是可选的，表示最佳版本。

> 上述版本号对应发布的 AWTK 中 component.json 文件中的 "release_id"
> 注意：如果没有显式设置，则认为兼容所有版本。

## 6. Designer新建控件的初始状态

默认情况下，从Designer的控件列表的自定义分组中拖出一个控件，其属性为控件 create 时的初值，样式为全透明。

如果需要指定新建控件的初始状态，可以在控件的class注释上补充如下格式的注释。

```c
/**
...
 * ```xml
 * <!-- ui -->
 * 控件初始属性的xml描述（如果描述中包含子控件，会同时创建）
 * ```
...
 * ```xml
 * <!-- style -->
 * 控件默认样式的xml描述（如果描述中包含其它控件的样式，会同时添加到default.xml样式文件）
 * ```
...
 */
```

如：

```c
/**
 * @class number_label_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 数值文本控件。
 *
 * 在xml中使用"number\_label"标签创建数值文本控件。如：
 *
 * ```xml
 * <!-- ui -->
 * <number_label x="c" y="50" w="24" h="100" value="40" format="%.4lf" decimal_font_size_scale="0.5"/>
 * ```
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 * 
 * ```xml
 * <!-- style -->
 * <number_label>
 *   <style name="default" font_size="32">
 *     <normal text_color="black" />
 *   </style>
 *   <style name="green" font_name="led" font_size="32">
 *     <normal text_color="green" />
 *   </style>
 * </number_label>
 * ```
 */
```

> Designer新建控件时会根据上述描述初始化控件的属性及样式，但会忽略x、y属性。

