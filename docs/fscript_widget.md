# FScript 的 widget 扩展函数

## 1. 介绍

[FScript](fscript.md) 的 widget 扩展函数只能在 widget 的事件处理函数中使用，包括传统的 AWTK 应用程序和 AWTK-MVVM 应用程序的事件处理函数。

FScript 和其它主流脚本 (lua/js) 相比，最大的优势就是体积小巧，而且 AWTK 本身依赖于 fscript，所以使用 fscript 增加额外代码可以忽略不计数。

FScript 并不是要取代 C 或 JS 来开发 AWTK 应用程序，而是一个有益的补充，加快应用程序的开发，在 AWTK 的 UI 文件中直接嵌入 FScript, 有如下优势：

* 1. 在特定情况下，可以极大简化程序的实现。

> 比如下面的代码，点击按钮时打开指定的窗口就非常简洁。

```xml
<button focusable="true" focused="true" on:click="open('basic')" text="Basic" />
```

> 比如下面的代码，点击按钮时关闭当前的窗口也非常简洁。

```xml
<button focusable="true" on:click="close()" text="Close" />
```

* 2. 方便工具（如 AWTK Designer）生成向导代码。

> 一般的 IDE 都有简单的代码生成功能，比如按钮的事件处理函数可以选择打开或关闭窗口等简单的功能。不排除 AWTK Designer 以后增加类似的功能。

* 3. 让 MVVM 应用程序有机会去操作界面。

> 理论上，模型是不能操作界面的，但在特殊情况下，确实有操作界面的需求。在保持视图和模型独立的前提下，FScript 让操作界面成为可能。

* 4. 方便生成自定义的组件。很多情况下，需要对现有的控件进行改进和组合，生成新的控件。

比如，我们需要用 text selector 生成一个日期选择控件，用三个 text selector 控件组合成日期选择控件，分别选择年月日。但是 text selector 之间是独立的，而年月和日则是有依赖关系，比如 1 月的天数是 31 天，2 月的天数与年份有关。当然我们可以用 C 代码来建立三者的关系，而用 FScript，不但实现要简单一些，而且由于 xml 和代码在一起，发布和重用也非常方便。

```xml
<row x="10" y="30" w="100%" h="150" children_layout="default(row=1,col=3)">
  <text_selector name="year" options="2000-2050" selected_index="9">
    <property name="on:value_changed">
      <![CDATA[
      var a = get_days_of_month(widget_get('parent.year', 'value'), widget_get('parent.month', 'value'))
      widget_set('parent.day', 'options', iformat( '1-%d', a) + '%02d')  
       ]]>
    </property>
  </text_selector>
  <text_selector name="month" options="1-12-%02d" selected_index="8" loop_options="true">
    <property name="on:value_changed">
      <![CDATA[
      var a = get_days_of_month(widget_get('parent.year', 'value'), widget_get('parent.month', 'value'))
      widget_set('parent.day', 'options', iformat( '1-%d', a) + '%02d')      
      ]]>
    </property>
  </text_selector>
  <text_selector name="day" options="1-31-%02d" selected_index="9" />
</row>
```

* 5. 不需要重新编译代码。如果资源文件是在文件系统中，直接更新资源即可，而无需编译和下载固件。

## 2. 事件处理函数

添加事件处理函数，实际上是增加一些自定义的属性，属性名以 on: 开头，紧跟事件名称。如：

```xml
<button focusable="true" on:click="close()" text="Close" />
```

为了方便编辑和阅读，对于比较复杂的脚本，可以把事件处理函数独立出来，放到 property 里，并用 CDATA 包装起来。

```xml
<button name="timer" focusable="true" on:click="start_timer(100)" text="Start Timer">
<property name="on:timer"><![CDATA[
        var a = widget_lookup('window', 'bar', true)
        var b = widget_get(a, 'value')
        if(b < 100) {
          widget_set(a, 'value', b + 1)
        } else {
          widget_set(a, 'value', 0)
          stop_timer()
        }
]]></property>
</button>
```

> 如果处理逻辑太复杂，请不要用 FScript 实现，否则不好调试。要善用 FScript 的优点而回避它的缺点。

## 3. 目前支持的事件

* global\_key\_up
* global\_key\_down
* global\_key\_long\_press
* key\_up
* key\_down
* key\_long\_press
* key\_down\_before\_children
* key\_up\_before\_children

> 按键事件处理函数可以从 key 变量获取当前的键值。可以从 alt, ctrl, cmd 和 menu 变量中获取相应控制键的状态。如：

```xml
<property name="on:global_key_down">
  print("global keydown:", key, "alt=", alt, "ctrl=", ctrl, "cmd=", cmd, "menu=", menu);
</property>
<property name="on:global_key_up">
  print("global keyup:", key, "alt=", alt, "ctrl=", ctrl, "cmd=", cmd, "menu=", menu);
</property>
<property name="on:global_key_long_press">
  print("global key_long_press:", key, "alt=", alt, "ctrl=", ctrl, "cmd=", cmd, "menu=", menu);
</property>
<property name="on:key_down">
  print("keydown:", key, "alt=", alt, "ctrl=", ctrl, "cmd=", cmd, "menu=", menu);
</property>
<property name="on:key_up">
  print("keyup:", key, "alt=", alt, "ctrl=", ctrl, "cmd=", cmd, "menu=", menu);
</property>
<property name="on:key_long_press">
  print("keyup:", key, "alt=", alt, "ctrl=", ctrl, "cmd=", cmd, "menu=", menu);
</property>  
```

* pointer\_up
* pointer\_down
* pointer\_move
* click

> 指针事件处理函数可以从 x/y 变量获取当前指针的位置。可以从 alt, ctrl, cmd 和 menu 变量中获取相应控制键的状态。如：

```xml
<property name="on:pointer_down">
  print("pointerdown:", x, y, "alt=", alt, "ctrl=", ctrl, "cmd=", cmd, "menu=", menu);
</property>
<property name="on:pointer_up">
  print("pointerup:", x, y, "alt=", alt, "ctrl=", ctrl, "cmd=", cmd, "menu=", menu);
</property>
<property name="on:click">
  print("click:", x, y, "alt=", alt, "ctrl=", ctrl, "cmd=", cmd, "menu=", menu);
</property>
```

* timer

> 定时器，示例：

```xml
<button name="timer" focusable="true" on:click="start_timer(100)" text="Start Timer">
<property name="on:timer"><![CDATA[
  var a = widget_lookup('window', 'bar', true)
  var b = widget_get(a, 'value')
    if(b < 100) {
    widget_set(a, 'value', b + 1)
  } else {
    widget_set(a, 'value', 0)
    stop_timer()
  }
]]></property>
</button>
```

* window\_close
* window\_open
* window\_will\_open

> 窗口事件只适用于窗口。

```xml
<property name="on:window_open">
  print("window_open")
</property>
<property name="on:window_will_open">
  print("window_will_open")
</property>
<property name="on:window_close">
  print("window_close")
</property>
```

* value\_changed

> 控件的值变化事件。

```xml
<progress_bar name="bar" text="" value="10" on:value_changed="print(widget_get('self', 'value'))"/>
```

* focus
* blur

> 控件得到焦点与失去焦点事件，示例：

```xml
<edit name="edit" tips="text edit" on:focus="print(focus)" on:blur="print('blur', widget_get('self', 'value'))"/>
```

* value\_changed\_by\_ui （仅 MVVM 支持）

> 在 MVVM 中，除了支持以上方式编写 FScript，也可以通过 v-on: 和 v-data: 的方式使用 FScript，请参考 MVVM 的文档。

## 4. 事件的返回值

通过 return 返回 RET\_XXX 常量，示例:

```js
return RET_OK;
return RET_FAIL;
return RET_STOP;
return RET_REMOVE;
```

## 5. widget 相关的扩展函数

### 5.1 open

> 打开窗口。
> 相关函数：window_open、window_close_and_open。
----------------------------

#### 原型

```js
open(name, close_current, switch_to_if_exist) => obj_widget
window_open(name, close_current, switch_to_if_exist) => obj_widget
window_close_and_open(name, close_current) => obj_widget;
```

* name 窗口的资源名称(字符串)
* close\_current 可选。为 true 时关闭当前窗口。
* switch\_to\_if\_exist 可选。为 true 时，如果同名窗口存在，直接切换到指定窗口。
* 返回打开的窗口对象。

#### 示例

```xml
<button text="Open" focusable="true" focus="true" on:click="open('test_fscript')"/>
<button text="Close" focusable="true" on:click="open('test_fscript', true)"/>
<button text="Switch TO" focusable="true" on:click="open('test_fscript', false, true)"/>
```

### 5.2 close

> 关闭窗口。
> 相关函数：window_close。
----------------------------

#### 原型

```js
close(name) => bool
window_close(name)=> bool
```

* name 窗口的名称(字符串)，可选。缺省关闭当前窗口。

#### 示例

```xml
<button focusable="true" on:click="close()" text="Close" />
```

### 5.3 back

> 退到前一个窗口
> 相关函数：window_back。
----------------------------

#### 原型

```js
back()
window_back()
```

#### 示例

```xml
<button focusable="true" on:click="back()" text="back" />
```

### 5.4 back\_to\_home

> 退到主窗口
> 相关函数：window_back_to_home。
----------------------------

#### 原型

```js
back_to_home()
window_back_to_home()
```

#### 示例

```xml
<button focusable="true" on:click="back_to_home()" text="home" />
```

### 5.5 quit

> 退出应用程序
> 相关函数：window_quit。
----------------------------

#### 原型

```js
quit()
window_quit()
```

#### 示例

```xml
<button focusable="true" on:click="quit()" text="quit" />
```

### 5.6 tr

> 翻译指定字符串
> 相关函数：widget_tr。
----------------------------

#### 原型

```js
tr(widget, before_tr_str) => after_tr_str
widget_tr(widget, before_tr_str) => after_tr_str
tr(before_tr_str) => after_tr_str
widget_tr(before_tr_str) => after_tr_str
```

* widget 用作锚点，后面的路径相对于该 widget。'self' 表示当前控件，'parent' 表示当前控件的父控件，'window' 表示当前的窗口，'window\_manager' 表示窗口管理器。
* 若不传入 widget，则默认为 'self'。
* 返回翻译后的字符串。

#### 示例

```xml
<button focusable="true" on:click="print(tr('OK'))" text="tr" />
```

### 5.7 widget_lookup

> 查找指定的控件
----------------------------

#### 原型

```js
widget_lookup(widget, path) => obj_widget
widget_lookup(widget, name, recursive) => obj_widget
```

* widget 用作锚点，后面的路径相对于该 widget。'self' 表示当前控件，'parent' 表示当前控件的父控件，'window' 表示当前的窗口，'window\_manager' 表示窗口管理器。
* 返回查找到的控件对象。

#### 示例

> 查找窗口下名为 view 控件下的名为 bar 的控件。

```js
var a = widget_lookup('window', 'view.bar')
```

> 递归查找窗口下名为 bar 的控件。

```js
var a = widget_lookup('window', 'bar', true)
```

> 递归查找当前控件下名为 bar 的控件。

```js
var a = widget_lookup('self', 'bar', true)
```

> 递归查找当前控件的父控件下名为 bar 的控件。

```js
var a = widget_lookup('self', 'bar', true)
```

### 5.8 widget_get

> 获取控件的属性
----------------------------

#### 原型

```js
widget_get(widget, prop) => v
widget_get(widget, path.prop) => v
```

* widget 用作锚点，后面的路径相对于该 widget。'self' 表示当前控件，'parent' 表示当前控件的父控件，'window' 表示当前的窗口，'window\_manager' 表示窗口管理器。
* prop 可以是简单的属性命名，也可以是 widget 路径+属性名。
* 支持 widget.prop 方式访问控件属性。
* 返回获取的控件属性值。

#### 示例

> 获取当前控件的 value

```js
var a = widget_get('self', 'value')
```

> 获取当前控件下名为 bar 控件的 value

```js
var a = widget_get('self', 'bar.value')
```

> 获取当前控件的父控件下名为 bar 控件的 value

```js
var a = widget_get('self', 'bar.value')
```

> 获取当前控件的父控件下名为 bar 控件的 value

```js
var a = widget_get('parent', 'bar.value')
```

> 获取当前窗口下名为 bar 控件的 value

```js
var a = widget_get('window', 'bar.value')
```

> 获取当前窗口下名为 view 控件下名为 bar 控件的 value

```js
var a = widget_get('window', 'view.bar.value')
```

>对于控件对象，可以直接访问其属性

```js
var a = widget_lookup('window', 'bar', true)
if(a.value <= 90) {
  a.value = a.value + 10
}
```

### 5.9 widget_set

> 设置控件的属性
----------------------------

#### 原型

```js
widget_set(widget, prop, value) => bool
widget_set(widget, path.prop, value) => bool
```

* widget 用作锚点，后面的路径相对于该 widget。'self' 表示当前控件，'parent' 表示当前控件的父控件，'window' 表示当前的窗口，'window\_manager' 表示窗口管理器。
* prop 可以是简单的属性命名，也可以是 widget 路径+属性名。
* 支持 widget.prop 方式设置控件属性。

#### 示例

> 设置当前控件的 value

```js
widget_set('self', 'value', 12)
```

> 设置当前控件下名为 bar 控件的 value

```js
widget_set('self', 'bar.value', 12)
```

> 设置当前控件的父控件下名为 bar 控件的 value

```js
widget_set('parent', 'bar.value', 1.2)
```

> 设置当前窗口下名为 bar 控件的 value

```js
widget_set('window', 'bar.value', 'hello')
```

> 设置当前窗口下名为 view 控件下名为 bar 控件的 value

```js
widget_set('window', 'view.bar.value', 12)
```

>对于控件对象，可以直接访问其属性

```js
var a = widget_lookup('window', 'bar', true)
if(a.value <= 90) {
  a.value = a.value + 10
}
```

### 5.10 widget_create

> 创建控件
----------------------------

#### 原型

```js
widget_create(type, parent, x, y, w, h) => obj_widget
```

* type 为控件的字符串名称。
* parent 为父控件。可以是 widget 对象，也可以是 widget 的路径。
* 返回创建的控件对象。

#### 示例

```js
var a = widget_lookup('window', 'foobar', true)
if(value_is_null(a)) {
  a = widget_create('label', 'window.view', 0, 0, 0, 0)
  assert(!value_is_null(a))

  widget_set(a, 'text', 'Dynamic created')
  assert(widget_get(a, 'text') == 'Dynamic created')
  widget_set(a, 'name', 'foobar')
  assert(widget_get(a, 'name') == 'foobar')
} else {
  print("foobar exist");
}
```

### 5.10 widget_destroy

> 销毁控件
----------------------------

#### 原型

```js
widget_destroy(widget) => bool
```

* widget 可以是 widget 对象，也可以是 widget 的路径。

#### 示例

```js
var a = widget_lookup('window', 'foobar', true)
if(!value_is_null(a)) {
  widget_destroy(a)
} else {
  print('not found foobar');
}s
```

```js
widget_destroy('self.bar')
widget_destroy('window.view.bar')
```

### 5.11 start_timer

> 开启一个定时器
----------------------------

#### 原型

```js
start_timer(duration) => uint32_t
start_timer(widget, duration) => uint32_t
```

* 不指定 widget 是启动当前控件的定时器。
* widget 可以是 widget 对象，也可以是 widget 的路径。
* duration 为定时器时间间隔。
* 一个控件只能开启一个定时器，如果定时器存在，自动先移除之前的定时器。
* 定时器时间到了之后，会触发控件的 timer 事件，所以对应的控件需要处理 timer 事件。
* 返回定时器ID。

#### 示例

```xml
<button name="timer" focusable="true" on:click="start_timer(100)" text="Start Timer">
  <property name="on:timer">
<![CDATA[
  var a = widget_lookup('window', 'bar', true)
  var b = widget_get(a, 'value')
  if(b < 100) {
    widget_set(a, 'value', b + 1)
  } else {
    widget_set(a, 'value', 0)
    stop_timer()
  }
]]>
  </property>
</button>
```

### 5.12 stop_timer

> 停止指定控件的定时器
----------------------------

#### 原型

```js
stop_timer() => bool
stop_timer(widget) => bool
```

* 不指定 widget 是停止当前控件的定时。
* widget 可以是 widget 对象，也可以是 widget 的路径。

#### 示例

```js
stop_timer('parent.timer')
```

### 5.13 reset_timer

> 重置指定的timer，重置之后定时器重新开始计时。
----------------------------

#### 原型

```js
reset_timer() => bool
reset_timer(widget) => bool
```

* 不指定 widget 是重置当前控件的定时。
* widget 可以是 widget 对象，也可以是 widget 的路径。

#### 示例

```js
reset_timer('parent.timer')
```

### 5.14 modify_timer

> 修改指定的timer的duration，修改之后定时器重新开始计时。
----------------------------

#### 原型

```js
modify_timer(duration) => bool
modify_timer(widget, duration) => bool
```

* 不指定 widget 是设置当前控件的定时器。
* widget 可以是 widget 对象，也可以是 widget 的路径。
* duration 为定时器时间间隔。

#### 示例

```js
modify_timer('parent.timer')
```

### 5.15 suspend_timer

> 挂起指定的timer，一般用于不断循环触发的计时器。
----------------------------

#### 原型

```js
suspend_timer() => bool
suspend_timer(widget) => bool
```

* 不指定 widget 是挂起当前控件的定时。
* widget 可以是 widget 对象，也可以是 widget 的路径。

#### 示例

```js
suspend_timer('parent.timer')
```

### 5.16 resume_timer

> 唤醒挂起指定的timer，并且重置定时器重新开始计时。
----------------------------

#### 原型

```js
resume_timer() => bool
resume_timer(widget) => bool
```

* 不指定 widget 是唤醒当前控件的定时。
* widget 可以是 widget 对象，也可以是 widget 的路径。

#### 示例

```js
resume_timer('parent.timer')
```

### 5.17 send_key

> 向指定控件发生按键事件
> 相关函数：widget_send_key。
----------------------------

#### 原型

```js
send_key(widget, key_name) => bool
widget_send_key(widget, key_name) => bool
```

* widget 可以是 widget 对象，也可以是 widget 的路径。

#### 示例

```js
 <button text="Backspace" on:click="send_key('window.edit', 'backspace')"/>
 <button text="Char" on:click="send_key('window.edit', 'a')"/>
```

### 5.18 widget\_eval

> 有时，几个事件处理函数的代码是重复的，我们可以把代码放到控件的属性中，通过widget\_eval来执行。
----------------------------

#### 原型
```js
widget_eval(path.prop)
widget_eval(widget, prop)
widget_eval(widget, path.prop)
```

* widget 用作锚点，后面的路径相对于该 widget。'self' 表示当前控件，'parent' 表示当前控件的父控件，'window' 表示当前的窗口，'window\_manager' 表示窗口管理器。
* prop 可以是简单的属性命名，也可以是 widget 路径+属性名。

#### 示例

下面两个事件处理函数的代码是相同。

```xml
<row x="10" y="30" w="100%" h="150" children_layout="default(row=1,col=3)">
  <text_selector name="year" options="2000-2050" selected_index="9">
    <property name="on:value_changed">
      <![CDATA[
      var a = get_days_of_month(widget_get('parent.year', 'value'), widget_get('parent.month', 'value'))
      widget_set('parent.day', 'options', iformat( '1-%d', a) + '%02d')
       ]]>
    </property>
  </text_selector>
  <text_selector name="month" options="1-12-%02d" selected_index="8" loop_options="true">
    <property name="on:value_changed">
      <![CDATA[
      var a = get_days_of_month(widget_get('parent.year', 'value'), widget_get('parent.month', 'value'))
      widget_set('parent.day', 'options', iformat( '1-%d', a) + '%02d')
      ]]>
    </property>
  </text_selector>
  <text_selector name="day" options="1-31-%02d" selected_index="9" />
</row>
```

我们可以改造成这样：

```xml
<row x="10" y="bottom" w="100%" h="150" children_layout="default(row=1,col=3)">
  <property name="handle_value_changed">
    <![CDATA[
    var a = get_days_of_month(widget_get('parent.year', 'value'), widget_get('parent.month', 'value'))
    widget_set('parent.day', 'options', iformat( '1-%d', a) + '%02d')
     ]]>
  </property>
  <text_selector name="year" options="2000-2050" selected_index="9"
    on:value_changed="widget_eval('parent.handle_value_changed')" />
  <text_selector name="month" options="1-12-%02d" selected_index="8" loop_options="true"
    on:value_changed="widget_eval('parent.handle_value_changed')" />
  <text_selector name="day" options="1-31-%02d" selected_index="9" />
</row>
```

### 5.19 locale\_get

> 获取本地化信息(国家和语言)
> 相关函数：widget_locale_get。
----------------------------

#### 原型

```js
locale_get() => object
locale_get(widget) => object
widget_locale_get() => object
widget_locale_get(widget) => object
```

* widget 参数：'self' 表示当前控件，'parent' 表示当前控件的父控件，'window' 表示当前的窗口，'window\_manager' 表示窗口管理器。
* 不传入 widget 参数时，默认使用 'self' 当前控件。

返回的本地化信息对象包含以下属性：

| 属性名称 | 属性类型 | 说明               |
| -------- | -------- | ------------------ |
| language | string   | 语言。如：zh       |
| country  | string   | 国家或地区。如：CN |

#### 示例

> 获取当前控件的本地化信息

```js
obj = locale_get()
print(object_get(obj, 'country'))
print(object_get(obj, 'language'))
```

> 获取窗口管理器的本地化信息

```js
obj = locale_get('window_manager')
print(object_get(obj, 'country'))
print(object_get(obj, 'language'))
```

获取当前控件的父控件或当前窗口的本地化信息用法类似，此处不多赘述。

### 5.20 locale\_set

> 设置本地化信息(国家和语言)
> 相关函数：widget_locale_set。
----------------------------

#### 原型

```js
locale_set(language, country) => bool
locale_set(widget, language, country) => bool
widget_locale_set(language, country) => bool
widget_locale_set(widget, language, country) => bool
```

* widget 参数：'self' 表示当前控件，'parent' 表示当前控件的父控件，'window' 表示当前的窗口，'window\_manager' 表示窗口管理器。
* 不传入 widget 参数时，默认使用 'self' 当前控件。

#### 示例

> 设置当前控件使用 "zh_CN" 语言

```js
locale_set('zh', 'CN')
```

> 设置窗口管理器使用 "en_US" 语言

```js
locale_set('window_manager', 'en', 'US')
```

设置当前控件的父控件或当前窗口的本地化信息用法类似，此处不多赘述。

### 5.21 theme\_get

> 获取当前主题
> 相关函数：widget_theme_get。
----------------------------

#### 原型

```js
theme_get() => string
theme_get(widget) => string
widget_theme_get() => string
widget_theme_get(widget) => string
```

* widget 参数：'self' 表示当前控件，'parent' 表示当前控件的父控件，'window' 表示当前的窗口，'window\_manager' 表示窗口管理器。
* 不传入 widget 参数时，默认使用 'self' 当前控件。

#### 示例

> 获取当前控件的主题

```js
print(theme_get())
```

> 获取窗口管理器的主题

```js
print(theme_get('window_manager'))
```

获取当前控件的父控件或当前窗口的主题用法类似，此处不多赘述。

### 5.22 theme\_set

> 设置本地化信息(国家和语言)
> 相关函数：widget_theme_set。
----------------------------

#### 原型

```js
theme_set(theme_name) => bool
theme_set(widget, theme_name) => bool
widget_theme_set(theme_name) => bool
widget_theme_set(widget, theme_name) => bool
```

* widget 参数：'self' 表示当前控件，'parent' 表示当前控件的父控件，'window' 表示当前的窗口，'window\_manager' 表示窗口管理器。
* 不传入 widget 参数时，默认使用 'self' 当前控件。

#### 示例

> 设置当前控件使用 "dark" 主题

```js
theme_set('dark')
```

> 设置窗口管理器使用 "dark" 主题

```js
theme_set('window_manager', 'dark')
```

设置当前控件的父控件或当前窗口的主题用法类似，此处不多赘述。

### 5.23 dialog_info

> 显示info提示对话框。
----------------------------

#### 原型

```js
dialog_info(title, content) => bool
```

* title 标题
* content 内容

#### 示例

```js
dialog_info('info', 'Done');
```

### 5.24 dialog_warn

> 显示warn提示对话框。
----------------------------

#### 原型

```js
dialog_warn(title, content) => bool
```

* title 标题
* content 内容

#### 示例

```js
dialog_warn('info', 'Done');
```

### 5.25 dialog_confirm

> 显示confirm提示对话框。
----------------------------

#### 原型

```js
dialog_confirm(title, content) => bool
```

* title 标题
* content 内容

#### 示例

```js
dialog_confirm('info', 'Are you sure?');
```

### 5.26 dialog_toast

> 显示toast提示对话框。
----------------------------

#### 原型

```js
dialog_toast(content, time) => bool
```

* title 标题
* content 内容

#### 示例

```js
dialog_toast('done', 3000);
```

### 5.27 choose_files

> 显示选择文件对话框。
----------------------------

#### 原型

```js
choose_files(filters, init_dir) => array
```

* filters 过滤条件(文件扩展名，如：.png.jpg.gif)
* init_dir 初始目录。
* 返回文件列表，列表项为文件名字符串。

#### 示例

```js
var files = choose_files('.png.jpg.gif', './')
print(files.size)
```

### 5.28 choose_file

> 显示选择文件对话框。
----------------------------

#### 原型

```js
choose_file(filters, init_dir) => str
```

* filters 过滤条件(文件扩展名，如：.png.jpg.gif)
* init_dir 初始目录。
* 返回文件名字符串。

#### 示例

```js
var filename = choose_files('.png.jpg.gif', './')
print(filename)
```

### 5.29 choose_folder

> 显示选择目录对话框。
----------------------------

#### 原型

```js
choose_folder(init_dir) => str
```

* init_dir 初始目录。
* 返回目录名称字符串。

#### 示例

```js
var foldername = choose_folder('./')
print(foldername)
```

### 5.30 choose\_file\_for\_save

> 选择保存文件对话框。
----------------------------

#### 原型

```js
choose_file_for_save(filters, init_dir) => str
```

* filters 过滤条件(文件扩展名，如：.png.jpg.gif)
* init_dir 初始目录。
* 返回文件名称字符串。

#### 示例

```js
var filename = choose_file_for_save('.png.jpg.gif', './')
print(filename)
```

### 5.31 widget_clone

> 克隆控件
----------------------------

#### 原型

```js
widget_clone(widget) => obj_widget
```

* widget 可以是 widget 对象，也可以是 widget 的路径。
* 返回克隆的控件对象。

#### 示例

```js
var a = widget_lookup('window', 'foobar', true)
if(!value_is_null(a)) {
  widget_clone(a)
} else {
  print('not found foobar');
}
```

```js
var a = widget_clone('self.bar')
var b = widget_clone('window.view.bar')
```

### 5.32 widget\_destroy\_children

> 销毁控件的全部子控件。
----------------------------

#### 原型

```js
widget_destroy_children(widget) => bool
```

* widget 可以是 widget 对象，也可以是 widget 的路径。

#### 示例

```js
var a = widget_lookup('window', 'foobar', true)
if(!value_is_null(a)) {
  widget_destroy_children(a)
} else {
  print('not found foobar');
}
```

```js
widget_destroy_children('self.bar')
widget_destroy_children('window.view.bar')
```

### 5.33 widget\_add\_value

> 增加控件的值。
----------------------------

#### 原型

```js
widget_add_value(widget, value) => bool
```

* widget 可以是 widget 对象，也可以是 widget 的路径。
* value 是控件值的增量。

#### 示例

```js
widget_add_value('self', 30)
```

### 5.34 notify\_model\_changed

> 通知模型改变。
----------------------------

#### 原型

```js
notify_model_changed(name, change_type, model) => bool
```

* name 是模型名称。
* change_type 是变化类型（update/add/remove）。
* model 可以是模型对象（object），也可以是字符串（用于查找对应的模型对象）。

#### 示例

```js
notify_model_changed("model_name", "update", obj_model)
notify_model_changed("model_name", "update", "global")
```

### 5.35 show\_fps

> 设置是否显示FPS。
----------------------------

#### 原型

```js
show_fps(is_show_fps) => bool
```

#### 示例

```js
show_fps(true)
show_fps(false)
```

### 示例参考

* https://github.com/zlgopen/awtk/blob/master/design/default/ui/main_fscript.xml
* https://github.com/zlgopen/awtk/blob/master/design/default/ui/text_selector_fscript.xml
* https://github.com/zlgopen/awtk/blob/master/design/default/ui/button_fscript.xml
* https://github.com/zlgopen/awtk/blob/master/design/default/ui/test_fscript.xml
* https://github.com/zlgopen/awtk/blob/master/design/default/ui/basic_fscript.xml

> 可以用 preview_ui 直接运行。如：

```bash
./bin/preview_ui.exe design/default/ui/main_fscript.xml
```
