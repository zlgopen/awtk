## children\_layouter\_factory\_t
### 概述
子控件布局算法工厂。
----------------------------------
### 函数
<p id="children_layouter_factory_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#children_layouter_factory_t_children_layouter_factory">children\_layouter\_factory</a> | 获取缺省的子控件布局算法工厂对象。 |
| <a href="#children_layouter_factory_t_children_layouter_factory_create">children\_layouter\_factory\_create</a> | 创建子控件布局算法工厂对象。 |
| <a href="#children_layouter_factory_t_children_layouter_factory_create_layouter">children\_layouter\_factory\_create\_layouter</a> | 创建指定类型的子控件布局算法对象。 |
| <a href="#children_layouter_factory_t_children_layouter_factory_destroy">children\_layouter\_factory\_destroy</a> | 析构并释放子控件布局算法工厂对象。 |
| <a href="#children_layouter_factory_t_children_layouter_factory_register">children\_layouter\_factory\_register</a> | 注册子控件布局算法创建函数。 |
| <a href="#children_layouter_factory_t_children_layouter_factory_set">children\_layouter\_factory\_set</a> | 设置缺省的子控件布局算法工厂对象。 |
#### children\_layouter\_factory 函数
-----------------------

* 函数功能：

> <p id="children_layouter_factory_t_children_layouter_factory">获取缺省的子控件布局算法工厂对象。

* 函数原型：

```
children_layouter_factory_t* children_layouter_factory ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | children\_layouter\_factory\_t* | 返回子控件布局算法工厂对象。 |
#### children\_layouter\_factory\_create 函数
-----------------------

* 函数功能：

> <p id="children_layouter_factory_t_children_layouter_factory_create">创建子控件布局算法工厂对象。

* 函数原型：

```
children_layouter_factory_t* children_layouter_factory_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | children\_layouter\_factory\_t* | 返回子控件布局算法工厂对象。 |
#### children\_layouter\_factory\_create\_layouter 函数
-----------------------

* 函数功能：

> <p id="children_layouter_factory_t_children_layouter_factory_create_layouter">创建指定类型的子控件布局算法对象。

* 函数原型：

```
children_layouter_t* children_layouter_factory_create_layouter (children_layouter_factory_t* factory, const char* type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | children\_layouter\_t* | 返回子控件布局算法对象。 |
| factory | children\_layouter\_factory\_t* | 子控件布局算法工厂对象。 |
| type | const char* | 类型。 |
#### children\_layouter\_factory\_destroy 函数
-----------------------

* 函数功能：

> <p id="children_layouter_factory_t_children_layouter_factory_destroy">析构并释放子控件布局算法工厂对象。

* 函数原型：

```
ret_t children_layouter_factory_destroy (children_layouter_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | children\_layouter\_factory\_t* | 子控件布局算法工厂对象。 |
#### children\_layouter\_factory\_register 函数
-----------------------

* 函数功能：

> <p id="children_layouter_factory_t_children_layouter_factory_register">注册子控件布局算法创建函数。

* 函数原型：

```
ret_t children_layouter_factory_register (children_layouter_factory_t* factory, const char* type, children_layouter_create_t create);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | children\_layouter\_factory\_t* | layouter工厂对象。 |
| type | const char* | 子控件布局算法类型。 |
| create | children\_layouter\_create\_t | 子控件布局算法创建函数。 |
#### children\_layouter\_factory\_set 函数
-----------------------

* 函数功能：

> <p id="children_layouter_factory_t_children_layouter_factory_set">设置缺省的子控件布局算法工厂对象。

* 函数原型：

```
ret_t children_layouter_factory_set (children_layouter_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | children\_layouter\_factory\_t* | 子控件布局算法工厂对象。 |
