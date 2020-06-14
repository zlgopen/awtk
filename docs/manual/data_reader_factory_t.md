## data\_reader\_factory\_t
### 概述
data reader工厂。
----------------------------------
### 函数
<p id="data_reader_factory_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#data_reader_factory_t_data_reader_factory">data\_reader\_factory</a> | 获取缺省的data reader工厂对象。 |
| <a href="#data_reader_factory_t_data_reader_factory_create">data\_reader\_factory\_create</a> | 创建data reader工厂对象。 |
| <a href="#data_reader_factory_t_data_reader_factory_create_reader">data\_reader\_factory\_create\_reader</a> | 创建指定类型的data reader对象。 |
| <a href="#data_reader_factory_t_data_reader_factory_destroy">data\_reader\_factory\_destroy</a> | 析构并释放data reader工厂对象。 |
| <a href="#data_reader_factory_t_data_reader_factory_register">data\_reader\_factory\_register</a> | 注册data reader创建函数。 |
| <a href="#data_reader_factory_t_data_reader_factory_set">data\_reader\_factory\_set</a> | 设置缺省的data reader工厂对象。 |
| <a href="#data_reader_factory_t_data_reader_file_create">data\_reader\_file\_create</a> | 创建基于文件的data reader。 |
#### data\_reader\_factory 函数
-----------------------

* 函数功能：

> <p id="data_reader_factory_t_data_reader_factory">获取缺省的data reader工厂对象。

* 函数原型：

```
data_reader_factory_t* data_reader_factory ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | data\_reader\_factory\_t* | 返回data reader工厂对象。 |
#### data\_reader\_factory\_create 函数
-----------------------

* 函数功能：

> <p id="data_reader_factory_t_data_reader_factory_create">创建data reader工厂对象。

* 函数原型：

```
data_reader_factory_t* data_reader_factory_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | data\_reader\_factory\_t* | 返回data reader工厂对象。 |
#### data\_reader\_factory\_create\_reader 函数
-----------------------

* 函数功能：

> <p id="data_reader_factory_t_data_reader_factory_create_reader">创建指定类型的data reader对象。

* 函数原型：

```
data_reader_t* data_reader_factory_create_reader (data_reader_factory_t* factory, const char* url);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | data\_reader\_t* | 返回data reader对象。 |
| factory | data\_reader\_factory\_t* | data reader工厂对象。 |
| url | const char* | URL。 |
#### data\_reader\_factory\_destroy 函数
-----------------------

* 函数功能：

> <p id="data_reader_factory_t_data_reader_factory_destroy">析构并释放data reader工厂对象。

* 函数原型：

```
ret_t data_reader_factory_destroy (data_reader_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | data\_reader\_factory\_t* | data reader工厂对象。 |
#### data\_reader\_factory\_register 函数
-----------------------

* 函数功能：

> <p id="data_reader_factory_t_data_reader_factory_register">注册data reader创建函数。

* 函数原型：

```
ret_t data_reader_factory_register (data_reader_factory_t* factory, const char* protocol, data_reader_create_t create);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | data\_reader\_factory\_t* | reader工厂对象。 |
| protocol | const char* | 协议(如file)。 |
| create | data\_reader\_create\_t | data reader创建函数。 |
#### data\_reader\_factory\_set 函数
-----------------------

* 函数功能：

> <p id="data_reader_factory_t_data_reader_factory_set">设置缺省的data reader工厂对象。

* 函数原型：

```
ret_t data_reader_factory_set (data_reader_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | data\_reader\_factory\_t* | data reader工厂对象。 |
#### data\_reader\_file\_create 函数
-----------------------

* 函数功能：

> <p id="data_reader_factory_t_data_reader_file_create">创建基于文件的data reader。

* 函数原型：

```
data_reader_t* data_reader_file_create (const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | data\_reader\_t* | 返回data reader对象。 |
| filename | const char* | 文件名。 |
