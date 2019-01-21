## tk\_t
### 概述
 TK全局对象。


----------------------------------
### 函数
<p id="tk_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_t_tk_init">tk\_init</a> | 初始化TK。 |
| <a href="#tk_t_tk_quit">tk\_quit</a> | 退出TK事件主循环。 |
| <a href="#tk_t_tk_run">tk\_run</a> | 进入TK事件主循环。 |
| <a href="#tk_t_tk_set_lcd_orientation">tk\_set\_lcd\_orientation</a> | 设置屏幕的旋转方向(XXX:目前仅支持0度和90度)。 |
#### tk\_init 函数
-----------------------

* 函数功能：

> <p id="tk_t_tk_init"> 初始化TK。




* 函数原型：

```
ret_t tk_init (wh_t w, wh_t h, app_type_t app_type, const char* app_name, const char* app_root);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| w | wh\_t | LCD宽度。 |
| h | wh\_t | LCD高度。 |
| app\_type | app\_type\_t | 应用程序的类型。 |
| app\_name | const char* | 应用程序的名称(必须为常量字符串)。 |
| app\_root | const char* | 应用程序的根目录，用于定位资源文件(必须为常量字符串)。 |
#### tk\_quit 函数
-----------------------

* 函数功能：

> <p id="tk_t_tk_quit"> 退出TK事件主循环。




* 函数原型：

```
ret_t tk_quit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### tk\_run 函数
-----------------------

* 函数功能：

> <p id="tk_t_tk_run"> 进入TK事件主循环。




* 函数原型：

```
ret_t tk_run ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### tk\_set\_lcd\_orientation 函数
-----------------------

* 函数功能：

> <p id="tk_t_tk_set_lcd_orientation"> 设置屏幕的旋转方向(XXX:目前仅支持0度和90度)。




* 函数原型：

```
ret_t tk_set_lcd_orientation (int orientation);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| orientation | int | 旋转方向。 |
