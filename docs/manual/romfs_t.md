## romfs\_t
### 概述
一个简单的ROMFS(目前用于WEB加载配置文件)
----------------------------------
### 函数
<p id="romfs_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#romfs_t_romfs_deinit">romfs\_deinit</a> | ~初始化。 |
| <a href="#romfs_t_romfs_get">romfs\_get</a> | 获取fs对象。 |
| <a href="#romfs_t_romfs_init">romfs\_init</a> | 初始化。 |
#### romfs\_deinit 函数
-----------------------

* 函数功能：

> <p id="romfs_t_romfs_deinit">~初始化。

* 函数原型：

```
ret_t romfs_deinit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### romfs\_get 函数
-----------------------

* 函数功能：

> <p id="romfs_t_romfs_get">获取fs对象。

* 函数原型：

```
fs_t* romfs_get ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | fs\_t* | 返回fs对象。 |
#### romfs\_init 函数
-----------------------

* 函数功能：

> <p id="romfs_t_romfs_init">初始化。

* 函数原型：

```
ret_t romfs_init (const uint8_t* header_data, uint32_t header_size, const uint8_t* body_data, uint32_t body_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| header\_data | const uint8\_t* | 文件信息数据。 |
| header\_size | uint32\_t | 文件信息数据长度。 |
| body\_data | const uint8\_t* | 文件数据。 |
| body\_size | uint32\_t | 文件数据长度。 |
