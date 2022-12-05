## font\_managers\_t
### 概述
在某些情况下，需要多个资源管理器。比如在手表系统里，每个应用或表盘，可能放在独立的资源包中，
此时优先加载应用自己的资源，如果没有就加载系统的资源。
----------------------------------
### 函数
<p id="font_managers_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#font_managers_t_font_managers_ref">font\_managers\_ref</a> | 获取指定小应用程序(applet)的字体管理器。 |
| <a href="#font_managers_t_font_managers_unload_all">font\_managers\_unload\_all</a> | 卸载全部字体。 |
| <a href="#font_managers_t_font_managers_unref">font\_managers\_unref</a> | 释放指定小应用程序(applet)的字体管理器。 |
#### font\_managers\_ref 函数
-----------------------

* 函数功能：

> <p id="font_managers_t_font_managers_ref">获取指定小应用程序(applet)的字体管理器。

* 函数原型：

```
font_manager_t* font_managers_ref (const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | font\_manager\_t* | 返回asset manager对象。 |
| name | const char* | 小应用程序(applet)的名称。 |
#### font\_managers\_unload\_all 函数
-----------------------

* 函数功能：

> <p id="font_managers_t_font_managers_unload_all">卸载全部字体。

* 函数原型：

```
ret_t font_managers_unload_all ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### font\_managers\_unref 函数
-----------------------

* 函数功能：

> <p id="font_managers_t_font_managers_unref">释放指定小应用程序(applet)的字体管理器。

* 函数原型：

```
ret_t font_managers_unref (font_manager_t* imm);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | font\_manager\_t* | 字体管理器对象。 |
