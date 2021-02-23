## mmap\_t
### 概述
把文件内容映射到内存。
----------------------------------
### 函数
<p id="mmap_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#mmap_t_mmap_create">mmap\_create</a> | 初始化mmap对象。 |
| <a href="#mmap_t_mmap_destroy">mmap\_destroy</a> | 销毁mmap。 |
### 属性
<p id="mmap_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#mmap_t_data">data</a> | void* | 内存地址。 |
| <a href="#mmap_t_size">size</a> | uint32\_t | 数据长度。 |
#### mmap\_create 函数
-----------------------

* 函数功能：

> <p id="mmap_t_mmap_create">初始化mmap对象。

* 函数原型：

```
mmap_t* mmap_create (const char* filename, bool_t writable, bool_t shared);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | mmap\_t* | mmap对象本身。 |
| filename | const char* | 文件名。 |
| writable | bool\_t | 是否可写。 |
| shared | bool\_t | 是否共享。 |
#### mmap\_destroy 函数
-----------------------

* 函数功能：

> <p id="mmap_t_mmap_destroy">销毁mmap。

* 函数原型：

```
ret_t mmap_destroy (mmap_t* mmap);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| mmap | mmap\_t* | mmap对象。 |
#### data 属性
-----------------------
> <p id="mmap_t_data">内存地址。

* 类型：void*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### size 属性
-----------------------
> <p id="mmap_t_size">数据长度。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
