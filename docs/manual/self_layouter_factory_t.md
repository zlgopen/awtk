## self\_layouter\_factory\_t
### 概述
控件自身布局算法工厂。
----------------------------------
### 函数
<p id="self_layouter_factory_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#self_layouter_factory_t_self_layouter_factory">self\_layouter\_factory</a> | 获取缺省的控件自身布局算法工厂对象。 |
| <a href="#self_layouter_factory_t_self_layouter_factory_create">self\_layouter\_factory\_create</a> | 创建控件自身布局算法工厂对象。 |
| <a href="#self_layouter_factory_t_self_layouter_factory_create_layouter">self\_layouter\_factory\_create\_layouter</a> | 创建指定类型的控件自身布局算法对象。 |
| <a href="#self_layouter_factory_t_self_layouter_factory_destroy">self\_layouter\_factory\_destroy</a> | 析构并释放控件自身布局算法工厂对象。 |
| <a href="#self_layouter_factory_t_self_layouter_factory_register">self\_layouter\_factory\_register</a> | 注册控件自身布局算法创建函数。 |
| <a href="#self_layouter_factory_t_self_layouter_factory_set">self\_layouter\_factory\_set</a> | 设置缺省的控件自身布局算法工厂对象。 |
#### self\_layouter\_factory 函数
-----------------------

* 函数功能：

> <p id="self_layouter_factory_t_self_layouter_factory">获取缺省的控件自身布局算法工厂对象。

* 函数原型：

```
self_layouter_factory_t* self_layouter_factory ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | self\_layouter\_factory\_t* | 返回控件自身布局算法工厂对象。 |
#### self\_layouter\_factory\_create 函数
-----------------------

* 函数功能：

> <p id="self_layouter_factory_t_self_layouter_factory_create">创建控件自身布局算法工厂对象。

* 函数原型：

```
self_layouter_factory_t* self_layouter_factory_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | self\_layouter\_factory\_t* | 返回控件自身布局算法工厂对象。 |
#### self\_layouter\_factory\_create\_layouter 函数
-----------------------

* 函数功能：

> <p id="self_layouter_factory_t_self_layouter_factory_create_layouter">创建指定类型的控件自身布局算法对象。

* 函数原型：

```
self_layouter_t* self_layouter_factory_create_layouter (self_layouter_factory_t* factory, const char* type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | self\_layouter\_t* | 返回控件自身布局算法对象。 |
| factory | self\_layouter\_factory\_t* | 控件自身布局算法工厂对象。 |
| type | const char* | 类型。 |
#### self\_layouter\_factory\_destroy 函数
-----------------------

* 函数功能：

> <p id="self_layouter_factory_t_self_layouter_factory_destroy">析构并释放控件自身布局算法工厂对象。

* 函数原型：

```
ret_t self_layouter_factory_destroy (self_layouter_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | self\_layouter\_factory\_t* | 控件自身布局算法工厂对象。 |
#### self\_layouter\_factory\_register 函数
-----------------------

* 函数功能：

> <p id="self_layouter_factory_t_self_layouter_factory_register">注册控件自身布局算法创建函数。

* 函数原型：

```
ret_t self_layouter_factory_register (self_layouter_factory_t* factory, const char* type, self_layouter_create_t create);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | self\_layouter\_factory\_t* | layouter工厂对象。 |
| type | const char* | 控件自身布局算法类型。 |
| create | self\_layouter\_create\_t | 控件自身布局算法创建函数。 |
#### self\_layouter\_factory\_set 函数
-----------------------

* 函数功能：

> <p id="self_layouter_factory_t_self_layouter_factory_set">设置缺省的控件自身布局算法工厂对象。

* 函数原型：

```
ret_t self_layouter_factory_set (self_layouter_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | self\_layouter\_factory\_t* | 控件自身布局算法工厂对象。 |
