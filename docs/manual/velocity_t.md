## velocity\_t
### 概述
velocity。
----------------------------------
### 函数
<p id="velocity_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#velocity_t_velocity_reset">velocity\_reset</a> | 重置状态。 |
| <a href="#velocity_t_velocity_update">velocity\_update</a> | 更新位置。 |
#### velocity\_reset 函数
-----------------------

* 函数功能：

> <p id="velocity_t_velocity_reset">重置状态。

* 函数原型：

```
velocity_t* velocity_reset (velocity_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | velocity\_t* | 返回velocity对象。 |
| v | velocity\_t* | velocity对象。 |
#### velocity\_update 函数
-----------------------

* 函数功能：

> <p id="velocity_t_velocity_update">更新位置。

* 函数原型：

```
ret_t velocity_update (velocity_t* v, uint32_t time, xy_t x, xy_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| v | velocity\_t* | velocity对象。 |
| time | uint32\_t | 时间。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
