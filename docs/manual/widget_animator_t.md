## widget\_animator\_t
### 概述
控件动画接口。
----------------------------------
### 函数
<p id="widget_animator_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#widget_animator_t_widget_animator_destroy">widget\_animator\_destroy</a> | 销毁animator对象。 |
| <a href="#widget_animator_t_widget_animator_init">widget\_animator\_init</a> | 初始化。仅供子类内部使用。 |
| <a href="#widget_animator_t_widget_animator_off">widget\_animator\_off</a> | 注销指定事件的处理函数。 |
| <a href="#widget_animator_t_widget_animator_on">widget\_animator\_on</a> | 注册指定事件的处理函数。 |
| <a href="#widget_animator_t_widget_animator_pause">widget\_animator\_pause</a> | 暂停动画。 |
| <a href="#widget_animator_t_widget_animator_set_destroy_when_done">widget\_animator\_set\_destroy\_when\_done</a> | 设置完成时是否自动销毁动画对象(缺省销毁)。 |
| <a href="#widget_animator_t_widget_animator_set_name">widget\_animator\_set\_name</a> | 设置名称。 |
| <a href="#widget_animator_t_widget_animator_set_repeat">widget\_animator\_set\_repeat</a> | 设置为重复模式。 |
| <a href="#widget_animator_t_widget_animator_set_reversed">widget\_animator\_set\_reversed</a> | 设置为逆向模式。 |
| <a href="#widget_animator_t_widget_animator_set_time_scale">widget\_animator\_set\_time\_scale</a> | 设置时间倍率，用于实现时间加速减速和停滞的功能。 |
| <a href="#widget_animator_t_widget_animator_set_yoyo">widget\_animator\_set\_yoyo</a> | 设置为yoyo模式。 |
| <a href="#widget_animator_t_widget_animator_start">widget\_animator\_start</a> | 启动动画。 |
| <a href="#widget_animator_t_widget_animator_stop">widget\_animator\_stop</a> | 停止动画。 |
### 属性
<p id="widget_animator_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#widget_animator_t_name">name</a> | char* | 名称。 |
#### widget\_animator\_destroy 函数
-----------------------

* 函数功能：

> <p id="widget_animator_t_widget_animator_destroy">销毁animator对象。

* 函数原型：

```
ret_t widget_animator_destroy (widget_animator_t* animator);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象。 |
#### widget\_animator\_init 函数
-----------------------

* 函数功能：

> <p id="widget_animator_t_widget_animator_init">初始化。仅供子类内部使用。

* 函数原型：

```
ret_t widget_animator_init (widget_animator_t* animator, widget_t* widget, uint32_t duration, uint32_t delay, easing_func_t );
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象。 |
| widget | widget\_t* | 控件对象。 |
| duration | uint32\_t | 动画持续时间。 |
| delay | uint32\_t | 动画执行时间。 |
|  | easing\_func\_t | 。 |
#### widget\_animator\_off 函数
-----------------------

* 函数功能：

> <p id="widget_animator_t_widget_animator_off">注销指定事件的处理函数。

* 函数原型：

```
ret_t widget_animator_off (widget_animator_t* animator, uint32_t id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象。 |
| id | uint32\_t | widget\_animator\_on返回的ID。 |
#### widget\_animator\_on 函数
-----------------------

* 函数功能：

> <p id="widget_animator_t_widget_animator_on">注册指定事件的处理函数。
事件类型。目前支持：EVT_ANIM_START,EVT_ANIM_STOP,EVT_ANIM_PAUSE,EVT_ANIM_ONCE和EVT_ANIM_END。

* 函数原型：

```
uint32_t widget_animator_on (widget_animator_t* animator, event_type_t , event_func_t on_event, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回id，用于widget\_animator\_off。 |
| animator | widget\_animator\_t* | 动画对象。 |
|  | event\_type\_t | e |
| on\_event | event\_func\_t | 事件处理函数。 |
| ctx | void* | 事件处理函数上下文。 |
#### widget\_animator\_pause 函数
-----------------------

* 函数功能：

> <p id="widget_animator_t_widget_animator_pause">暂停动画。

* 函数原型：

```
ret_t widget_animator_pause (widget_animator_t* animator);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象。 |
#### widget\_animator\_set\_destroy\_when\_done 函数
-----------------------

* 函数功能：

> <p id="widget_animator_t_widget_animator_set_destroy_when_done">设置完成时是否自动销毁动画对象(缺省销毁)。

* 函数原型：

```
ret_t widget_animator_set_destroy_when_done (widget_animator_t* animator, bool_t destroy_when_done);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象。 |
| destroy\_when\_done | bool\_t | 完成时是否自动销毁动画对象。 |
#### widget\_animator\_set\_name 函数
-----------------------

* 函数功能：

> <p id="widget_animator_t_widget_animator_set_name">设置名称。

* 函数原型：

```
ret_t widget_animator_set_name (widget_animator_t* animator, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象。 |
| name | const char* | 名称。 |
#### widget\_animator\_set\_repeat 函数
-----------------------

* 函数功能：

> <p id="widget_animator_t_widget_animator_set_repeat">设置为重复模式。

* 函数原型：

```
ret_t widget_animator_set_repeat (widget_animator_t* animator, uint32_t repeat_times);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象。 |
| repeat\_times | uint32\_t | 重复的次数。 |
#### widget\_animator\_set\_reversed 函数
-----------------------

* 函数功能：

> <p id="widget_animator_t_widget_animator_set_reversed">设置为逆向模式。

* 函数原型：

```
ret_t widget_animator_set_reversed (widget_animator_t* animator, bool_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象。 |
| value | bool\_t | 是否为逆向模式。 |
#### widget\_animator\_set\_time\_scale 函数
-----------------------

* 函数功能：

> <p id="widget_animator_t_widget_animator_set_time_scale">设置时间倍率，用于实现时间加速减速和停滞的功能。

* 函数原型：

```
ret_t widget_animator_set_time_scale (widget_animator_t* animator, float_t time_scale);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象。 |
| time\_scale | float\_t | 时间倍率。 |
#### widget\_animator\_set\_yoyo 函数
-----------------------

* 函数功能：

> <p id="widget_animator_t_widget_animator_set_yoyo">设置为yoyo模式。

* 函数原型：

```
ret_t widget_animator_set_yoyo (widget_animator_t* animator, uint32_t yoyo_times);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象。 |
| yoyo\_times | uint32\_t | yoyo的次数，往返视为两次。 |
#### widget\_animator\_start 函数
-----------------------

* 函数功能：

> <p id="widget_animator_t_widget_animator_start">启动动画。

* 函数原型：

```
ret_t widget_animator_start (widget_animator_t* animator);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象。 |
#### widget\_animator\_stop 函数
-----------------------

* 函数功能：

> <p id="widget_animator_t_widget_animator_stop">停止动画。

* 函数原型：

```
ret_t widget_animator_stop (widget_animator_t* animator);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| animator | widget\_animator\_t* | 动画对象。 |
#### name 属性
-----------------------
> <p id="widget_animator_t_name">名称。

* 类型：char*

