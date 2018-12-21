## paint\_event\_t
### 概述
![image](images/paint_event_t_0.png)

 绘制事件。

### 函数
<p id="paint_event_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#paint_event_t_paint_event_cast">paint\_event\_cast</a> |  把event对象转paint_event_t对象。主要给脚本语言使用。 |
### 属性
<p id="paint_event_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#paint_event_t_c">c</a> | canvas_t* |  canvas。 |
### 事件
<p id="paint_event_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### paint\_event\_cast 函数
* 函数原型：

```
paint_event_t* paint_event_cast (event_t* event);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | paint\_event\_t* | 对象。 |
| event | event\_t* | event对象。 |
* 函数功能：

> <p id="paint_event_t_paint_event_cast"> 把event对象转paint_event_t对象。主要给脚本语言使用。




#### c 属性
-----------------------
> <p id="paint_event_t_c"> canvas。



* 类型：canvas\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 否 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 否 |
| 可在XML中设置 | 否 |
| 支通过widget_get_prop读取 | 否 |
| 支通过widget_set_prop修改 | 否 |
