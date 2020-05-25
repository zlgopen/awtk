## global\_t
### 概述
TK全局对象。
----------------------------------
### 函数
<p id="global_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#global_t_asset_loader_default_create">asset\_loader\_default\_create</a> | 创建缺省的资源加载器。 |
| <a href="#global_t_asset_loader_zip_create">asset\_loader\_zip\_create</a> | 创建zip资源加载器。 |
| <a href="#global_t_asset_loader_zip_create_with_reader">asset\_loader\_zip\_create\_with\_reader</a> | 创建zip资源加载器。 |
| <a href="#global_t_tk_deinit_internal">tk\_deinit\_internal</a> | deinit。 |
| <a href="#global_t_tk_exit">tk\_exit</a> | public for web |
| <a href="#global_t_tk_get_pointer_x">tk\_get\_pointer\_x</a> | 获取全局指针的X坐标。 |
| <a href="#global_t_tk_get_pointer_y">tk\_get\_pointer\_y</a> | 获取全局指针的Y坐标。 |
| <a href="#global_t_tk_init">tk\_init</a> | 初始化TK。 |
| <a href="#global_t_tk_init_assets">tk\_init\_assets</a> | 初始化资源。 |
| <a href="#global_t_tk_init_internal">tk\_init\_internal</a> | init。 |
| <a href="#global_t_tk_is_pointer_pressed">tk\_is\_pointer\_pressed</a> | 获取全局指针是否按下。 |
| <a href="#global_t_tk_quit">tk\_quit</a> | 退出TK事件主循环。 |
| <a href="#global_t_tk_run">tk\_run</a> | 进入TK事件主循环。 |
| <a href="#global_t_tk_set_lcd_orientation">tk\_set\_lcd\_orientation</a> | 设置屏幕的旋转方向(XXX:目前仅支持0度和90度)。 |
#### asset\_loader\_default\_create 函数
-----------------------

* 函数功能：

> <p id="global_t_asset_loader_default_create">创建缺省的资源加载器。

* 函数原型：

```
asset_loader_t* asset_loader_default_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | asset\_loader\_t* | 返回loader对象。 |
#### asset\_loader\_zip\_create 函数
-----------------------

* 函数功能：

> <p id="global_t_asset_loader_zip_create">创建zip资源加载器。

* 函数原型：

```
asset_loader_t* asset_loader_zip_create (const char* zipfile);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | asset\_loader\_t* | 返回loader对象。 |
| zipfile | const char* | zip文件名。 |
#### asset\_loader\_zip\_create\_with\_reader 函数
-----------------------

* 函数功能：

> <p id="global_t_asset_loader_zip_create_with_reader">创建zip资源加载器。

* 函数原型：

```
asset_loader_t* asset_loader_zip_create_with_reader (data_reader_t* reader, bool_t own_the_reader);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | asset\_loader\_t* | 返回loader对象。 |
| reader | data\_reader\_t* | reader对象。 |
| own\_the\_reader | bool\_t | 如果希望销毁loader时销毁reader，请设置为TRUE。 |
#### tk\_deinit\_internal 函数
-----------------------

* 函数功能：

> <p id="global_t_tk_deinit_internal">deinit。
> public for test program

* 函数原型：

```
ret_t tk_deinit_internal ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### tk\_exit 函数
-----------------------

* 函数功能：

> <p id="global_t_tk_exit">public for web

* 函数原型：

```
ret_t tk_exit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### tk\_get\_pointer\_x 函数
-----------------------

* 函数功能：

> <p id="global_t_tk_get_pointer_x">获取全局指针的X坐标。

* 函数原型：

```
int32_t tk_get_pointer_x ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回全局指针的X坐标。 |
#### tk\_get\_pointer\_y 函数
-----------------------

* 函数功能：

> <p id="global_t_tk_get_pointer_y">获取全局指针的Y坐标。

* 函数原型：

```
int32_t tk_get_pointer_y ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回全局指针的X坐标。 |
#### tk\_init 函数
-----------------------

* 函数功能：

> <p id="global_t_tk_init">初始化TK。

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
#### tk\_init\_assets 函数
-----------------------

* 函数功能：

> <p id="global_t_tk_init_assets">初始化资源。

* 函数原型：

```
ret_t tk_init_assets ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### tk\_init\_internal 函数
-----------------------

* 函数功能：

> <p id="global_t_tk_init_internal">init。
> public for test program

* 函数原型：

```
ret_t tk_init_internal ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### tk\_is\_pointer\_pressed 函数
-----------------------

* 函数功能：

> <p id="global_t_tk_is_pointer_pressed">获取全局指针是否按下。

* 函数原型：

```
bool_t tk_is_pointer_pressed ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回全局指针是否按下。 |
#### tk\_quit 函数
-----------------------

* 函数功能：

> <p id="global_t_tk_quit">退出TK事件主循环。

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

> <p id="global_t_tk_run">进入TK事件主循环。

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

> <p id="global_t_tk_set_lcd_orientation">设置屏幕的旋转方向(XXX:目前仅支持0度和90度)。

* 函数原型：

```
ret_t tk_set_lcd_orientation (int orientation);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| orientation | int | 旋转方向。 |
