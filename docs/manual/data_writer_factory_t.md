## data\_writer\_factory\_t
### 概述
data writer工厂。
----------------------------------
### 函数
<p id="data_writer_factory_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#data_writer_factory_t_data_writer_factory">data\_writer\_factory</a> | 获取缺省的data writer工厂对象。 |
| <a href="#data_writer_factory_t_data_writer_factory_create">data\_writer\_factory\_create</a> | 创建data writer工厂对象。 |
| <a href="#data_writer_factory_t_data_writer_factory_create_writer">data\_writer\_factory\_create\_writer</a> | 创建指定类型的data writer对象。 |
| <a href="#data_writer_factory_t_data_writer_factory_destroy">data\_writer\_factory\_destroy</a> | 析构并释放data writer工厂对象。 |
| <a href="#data_writer_factory_t_data_writer_factory_register">data\_writer\_factory\_register</a> | 注册data writer创建函数。 |
| <a href="#data_writer_factory_t_data_writer_factory_set">data\_writer\_factory\_set</a> | 设置缺省的data writer工厂对象。 |
| <a href="#data_writer_factory_t_data_writer_file_create">data\_writer\_file\_create</a> | 创建基于文件的data writer。 |
#### data\_writer\_factory 函数
-----------------------

* 函数功能：

> <p id="data_writer_factory_t_data_writer_factory">获取缺省的data writer工厂对象。

* 函数原型：

```
data_writer_factory_t* data_writer_factory ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | data\_writer\_factory\_t* | 返回data writer工厂对象。 |
#### data\_writer\_factory\_create 函数
-----------------------

* 函数功能：

> <p id="data_writer_factory_t_data_writer_factory_create">创建data writer工厂对象。

* 函数原型：

```
data_writer_factory_t* data_writer_factory_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | data\_writer\_factory\_t* | 返回data writer工厂对象。 |
#### data\_writer\_factory\_create\_writer 函数
-----------------------

* 函数功能：

> <p id="data_writer_factory_t_data_writer_factory_create_writer">创建指定类型的data writer对象。

* 函数原型：

```
data_writer_t* data_writer_factory_create_writer (data_writer_factory_t* factory, const char* url);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | data\_writer\_t* | 返回data writer对象。 |
| factory | data\_writer\_factory\_t* | data writer工厂对象。 |
| url | const char* | URL。 |
#### data\_writer\_factory\_destroy 函数
-----------------------

* 函数功能：

> <p id="data_writer_factory_t_data_writer_factory_destroy">析构并释放data writer工厂对象。

* 函数原型：

```
ret_t data_writer_factory_destroy (data_writer_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | data\_writer\_factory\_t* | data writer工厂对象。 |
#### data\_writer\_factory\_register 函数
-----------------------

* 函数功能：

> <p id="data_writer_factory_t_data_writer_factory_register">注册data writer创建函数。

* 函数原型：

```
ret_t data_writer_factory_register (data_writer_factory_t* factory, const char* protocol, data_writer_create_t create);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | data\_writer\_factory\_t* | writer工厂对象。 |
| protocol | const char* | 协议(如file)。 |
| create | data\_writer\_create\_t | data writer创建函数。 |
#### data\_writer\_factory\_set 函数
-----------------------

* 函数功能：

> <p id="data_writer_factory_t_data_writer_factory_set">设置缺省的data writer工厂对象。

* 函数原型：

```
ret_t data_writer_factory_set (data_writer_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | data\_writer\_factory\_t* | data writer工厂对象。 |
#### data\_writer\_file\_create 函数
-----------------------

* 函数功能：

> <p id="data_writer_factory_t_data_writer_file_create">创建基于文件的data writer。

* 函数原型：

```
data_writer_t* data_writer_file_create (const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | data\_writer\_t* | 返回data writer对象。 |
| filename | const char* | 文件名。 |
