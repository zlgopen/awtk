## tk\_dl\_t
### 概述
动态链接库对象。
----------------------------------
### 函数
<p id="tk_dl_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_dl_t_tk_dl_close">tk\_dl\_close</a> | 关闭dl对象。 |
| <a href="#tk_dl_t_tk_dl_open">tk\_dl\_open</a> | 打开动态链接库。 |
| <a href="#tk_dl_t_tk_dl_sym">tk\_dl\_sym</a> | 获取指定名称函数的地址。 |
#### tk\_dl\_close 函数
-----------------------

* 函数功能：

> <p id="tk_dl_t_tk_dl_close">关闭dl对象。

* 函数原型：

```
ret_t tk_dl_close (tk_dl_t* dl);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dl | tk\_dl\_t* | dl对象。 |
#### tk\_dl\_open 函数
-----------------------

* 函数功能：

> <p id="tk_dl_t_tk_dl_open">打开动态链接库。

* 函数原型：

```
tk_dl_t* tk_dl_open (const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_dl\_t* | 动态链接库对象。 |
| filename | const char* | 文件名。 |
#### tk\_dl\_sym 函数
-----------------------

* 函数功能：

> <p id="tk_dl_t_tk_dl_sym">获取指定名称函数的地址。

* 函数原型：

```
void* tk_dl_sym (tk_dl_t* dl, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 返回函数的地址。 |
| dl | tk\_dl\_t* | dl对象。 |
| name | const char* | 函数名。 |
