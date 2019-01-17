## system\_info\_t
### 概述
 当前系统的信息。


----------------------------------
### 函数
<p id="system_info_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#system_info_t_system_info">system\_info</a> | 获取system_info对象。 |
| <a href="#system_info_t_system_info_init">system\_info\_init</a> | 初始化system_info对象(PC软件才需调用)。 |
### 属性
<p id="system_info_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#system_info_t_app_name">app\_name</a> | const char* | 应用程序的名称。 |
| <a href="#system_info_t_app_root">app\_root</a> | const char* | 应用程序的根目录，用于定位资源文件。 |
| <a href="#system_info_t_app_type">app\_type</a> | app\_type\_t | 应用程序的类型。 |
| <a href="#system_info_t_device_pixel_ratio">device\_pixel\_ratio</a> | float\_t | 显示屏的密度。 |
| <a href="#system_info_t_lcd_h">lcd\_h</a> | uint32\_t | 显示屏的高度。 |
| <a href="#system_info_t_lcd_orientation">lcd\_orientation</a> | lcd\_orientation\_t | 显示屏的旋转角度。 |
| <a href="#system_info_t_lcd_type">lcd\_type</a> | lcd\_type\_t | 显示屏的类型。 |
| <a href="#system_info_t_lcd_w">lcd\_w</a> | uint32\_t | 显示屏的宽度。 |
#### system\_info 函数
-----------------------

* 函数功能：

> <p id="system_info_t_system_info"> 获取system_info对象。



* 函数原型：

```
system_info_t* system_info ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | system\_info\_t* | 返回system\_info对象。 |
#### system\_info\_init 函数
-----------------------

* 函数功能：

> <p id="system_info_t_system_info_init"> 初始化system_info对象(PC软件才需调用)。




* 函数原型：

```
ret_t system_info_init (app_type_t app_type, const char* app_name, const char* app_root);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| app\_type | app\_type\_t | 应用程序的类型。 |
| app\_name | const char* | 应用程序的名称。 |
| app\_root | const char* | 应用程序的根目录，用于定位资源文件。 |
#### app\_name 属性
-----------------------
> <p id="system_info_t_app_name"> 应用程序的名称。



* 类型：const char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### app\_root 属性
-----------------------
> <p id="system_info_t_app_root"> 应用程序的根目录，用于定位资源文件。



* 类型：const char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### app\_type 属性
-----------------------
> <p id="system_info_t_app_type"> 应用程序的类型。



* 类型：app\_type\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### device\_pixel\_ratio 属性
-----------------------
> <p id="system_info_t_device_pixel_ratio"> 显示屏的密度。



* 类型：float\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### lcd\_h 属性
-----------------------
> <p id="system_info_t_lcd_h"> 显示屏的高度。



* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### lcd\_orientation 属性
-----------------------
> <p id="system_info_t_lcd_orientation"> 显示屏的旋转角度。



* 类型：lcd\_orientation\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### lcd\_type 属性
-----------------------
> <p id="system_info_t_lcd_type"> 显示屏的类型。



* 类型：lcd\_type\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### lcd\_w 属性
-----------------------
> <p id="system_info_t_lcd_w"> 显示屏的宽度。



* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
