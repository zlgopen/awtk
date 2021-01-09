
# fscript 的 widget 扩展函数

## 1. 介绍

fscript 的 widget 扩展函数只能在 widget 的事件处理函数中使用，包括传统的 AWTK 应用程序和 AWTK-MVVM 应用程序的事件处理函数。

fscript 和其它主流脚本 (lua/js) 相比，最大的优势就是体积小巧，而且 AWTK 本身依赖于 fscript，所以使用 fscript 增加额外代码可以忽略不计数。在 AWTK 的 UI 文件中直接嵌入 fscript, 有如下优势：

* 1. 在特定情况下，可以极大简化程序的实现。

>比如下面的代码，点击按钮时打开指定的窗口就非常简洁。

```xml
    <button focusable="true" focused="true" on:click="open(basic)" text="Basic" />
```

>比如下面的代码，点击按钮时关闭当前的窗口也非常简洁。

```xml
    <button focusable="true" on:click="close()" text="Close" />
```

* 2. 方便工具（如 designer) 生成向导代码。

> 一般的 IDE 都有简单的代码生成功能，比如按钮的事件处理函数可以选择打开或关闭窗口等简单的功能。
 
* 3. 让 MVVM 应用程序有机会去操作界面。

> 理论上，模型是不能操作界面的，但在特殊情况下，确实有操作界面的需求。在保持视图和模型独立的前提下，fscript 让操作界面成为可能。

## 2. 事件处理函数

添加事件处理函数时，属性名以 on: 开头，紧跟事件名称。如：

```xml
    <button focusable="true" on:click="close()" text="Close" />
```

为了方便编辑和阅读，对于比较复杂的脚本，可以把事件处理函数独立出来，放到 property 里，并用 CDATA 包装起来。

```xml
<button name="timer" focusable="true" on:click="start_timer(100)" text="Start Timer">
<property name="on:timer"><![CDATA[
  a = widget_lookup(window, bar, true)
  b = widget_get(a, "value")
  if(b < 100) {
    widget_set(a, "value", b + 1)
  } else {
    widget_set(a, "value", 0)
    stop_timer()
  }
]]></property>
</button>
```      

> 如果处理逻辑太复杂，请不要用 fscript 实现，否则不好调试。要擅用 fscript 的优点而回避它的缺点。

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
示例：

```xml
<button name="timer" focusable="true" on:click="start_timer(100)" text="Start Timer">
<property name="on:timer"><![CDATA[
  a = widget_lookup(window, bar, true)
  b = widget_get(a, "value")
  if(b < 100) {
    widget_set(a, "value", b + 1)
  } else {
    widget_set(a, "value", 0)
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
``

* value\_changed

```xml
<progress_bar name="bar" text="" value="10" on:value_changed="print(widget_get(self,value))"/>
```

* focus
* blur

示例：

```
<edit name="edit" tips="text edit" on:focus="print(focus)" on:blur="print(blur, widget_get(self,value))"/>
```

* value\_changed\_by\_ui （仅 MVVM 支持）

> 在 MVVM 中，除了支持以上方式编写 fscript，也可以通过 v-on: 和 v-data: 的方式使用 fscript，请参考 MVVM 的文档。

## 4. 事件的返回值

* 事件缺省返回 RET\_OK。

* 如果需要在执行完成后移除事件处理函数，可以设置 RET\_REMOVE 为 true。

* 如果需要在执行完成后移除事件处理函数，停止后续事件处理，可以设置 RET\_STOP 为 true。

## 5. widget 相关的扩展函数

### 5.1 open

> 打开窗口。
----------------------------

#### 原型

```js
open(name, close_current, switch_to_if_exist)
```

* close\_current 可选。为 true 时关闭当前窗口。
* switch\_to\_if\_exist 可选。为 true 时，如果同名窗口存在，直接切换到指定窗口。

### 示例

```
    <button text="Open" focusable="true" focus="true" on:click="open(test_fscript)"/>
    <button text="Close" focusable="true" on:click="open(test_fscript, true)"/>
    <button text="Switch TO" focusable="true" on:click="open(test_fscript, false, true)"/>
```

### 5.2 close

> 关闭窗口。
----------------------------

#### 原型

```js
open(name)
```

* name 可选。缺省关闭当前窗口。

### 示例

```
<button focusable="true" on:click="close()" text="Close" />
```

### 5.3 back

> 退到前一个窗口
----------------------------

#### 原型

```js
back()
```

### 示例

```
<button focusable="true" on:click="back()" text="back" />
```

### 5.4 back_to_home

> 退到主窗口
----------------------------

#### 原型

```js
back_to_home()
```

### 示例

```
<button focusable="true" on:click="back_to_home()" text="home" />
```

### 5.5 quit

> 退出应用程序
----------------------------

#### 原型

```js
quit()
```

### 示例

```
<button focusable="true" on:click="quit()" text="quit" />
```

### 5.6 tr

> 翻译指定字符串
----------------------------

#### 原型

```js
tr(str)
```

### 示例

```
<button focusable="true" on:click="print(tr(OK))" text="tr" />
```