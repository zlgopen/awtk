## dialog\_highlighter\_factory\_t
### 概述
对话框高亮策略工厂。
----------------------------------
### 函数
<p id="dialog_highlighter_factory_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#dialog_highlighter_factory_t_dialog_highlighter_factory">dialog\_highlighter\_factory</a> | 获取缺省的对话框高亮策略工厂对象。 |
| <a href="#dialog_highlighter_factory_t_dialog_highlighter_factory_create">dialog\_highlighter\_factory\_create</a> | 创建对话框高亮策略工厂对象。 |
| <a href="#dialog_highlighter_factory_t_dialog_highlighter_factory_create_highlighter">dialog\_highlighter\_factory\_create\_highlighter</a> | 创建指定类型的对话框高亮策略对象。 |
| <a href="#dialog_highlighter_factory_t_dialog_highlighter_factory_destroy">dialog\_highlighter\_factory\_destroy</a> | 析构并释放对话框高亮策略工厂对象。 |
| <a href="#dialog_highlighter_factory_t_dialog_highlighter_factory_register">dialog\_highlighter\_factory\_register</a> | 注册对话框高亮策略创建函数。 |
| <a href="#dialog_highlighter_factory_t_dialog_highlighter_factory_set">dialog\_highlighter\_factory\_set</a> | 设置缺省的对话框高亮策略工厂对象。 |
#### dialog\_highlighter\_factory 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_factory_t_dialog_highlighter_factory">获取缺省的对话框高亮策略工厂对象。

* 函数原型：

```
dialog_highlighter_factory_t* dialog_highlighter_factory ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | dialog\_highlighter\_factory\_t* | 返回对话框高亮策略工厂对象。 |
#### dialog\_highlighter\_factory\_create 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_factory_t_dialog_highlighter_factory_create">创建对话框高亮策略工厂对象。

* 函数原型：

```
dialog_highlighter_factory_t* dialog_highlighter_factory_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | dialog\_highlighter\_factory\_t* | 返回对话框高亮策略工厂对象。 |
#### dialog\_highlighter\_factory\_create\_highlighter 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_factory_t_dialog_highlighter_factory_create_highlighter">创建指定类型的对话框高亮策略对象。

* 函数原型：

```
dialog_highlighter_t* dialog_highlighter_factory_create_highlighter (dialog_highlighter_factory_t* factory, const char* args, widget_t* dialog);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | dialog\_highlighter\_t* | 返回窗口动画对象。 |
| factory | dialog\_highlighter\_factory\_t* | 对话框高亮策略工厂对象。 |
| args | const char* | 参数。如default(alpha=90) |
| dialog | widget\_t* | 对话框对象。 |
#### dialog\_highlighter\_factory\_destroy 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_factory_t_dialog_highlighter_factory_destroy">析构并释放对话框高亮策略工厂对象。

* 函数原型：

```
ret_t dialog_highlighter_factory_destroy (dialog_highlighter_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | dialog\_highlighter\_factory\_t* | 对话框高亮策略工厂对象。 |
#### dialog\_highlighter\_factory\_register 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_factory_t_dialog_highlighter_factory_register">注册对话框高亮策略创建函数。

* 函数原型：

```
ret_t dialog_highlighter_factory_register (dialog_highlighter_factory_t* factory, const char* type, dialog_highlighter_create_t create);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | dialog\_highlighter\_factory\_t* | highlighter工厂对象。 |
| type | const char* | 对话框高亮策略类型。 |
| create | dialog\_highlighter\_create\_t | 对话框高亮策略创建函数。 |
#### dialog\_highlighter\_factory\_set 函数
-----------------------

* 函数功能：

> <p id="dialog_highlighter_factory_t_dialog_highlighter_factory_set">设置缺省的对话框高亮策略工厂对象。

* 函数原型：

```
ret_t dialog_highlighter_factory_set (dialog_highlighter_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | dialog\_highlighter\_factory\_t* | 对话框高亮策略工厂对象。 |
