## eval\_t
### 概述
表达式。
----------------------------------
### 函数
<p id="eval_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#eval_t_eval_default_hooks">eval\_default\_hooks</a> | 获取内置函数。 |
| <a href="#eval_t_eval_execute">eval\_execute</a> | 计算表达式的值。 |
| <a href="#eval_t_eval_result_to_string">eval\_result\_to\_string</a> | 将错误码转换成字符串。 |
| <a href="#eval_t_expr_value_clear">expr\_value\_clear</a> | 清除值。 |
| <a href="#eval_t_expr_value_get_number">expr\_value\_get\_number</a> | 获取浮点数值。 |
| <a href="#eval_t_expr_value_get_string">expr\_value\_get\_string</a> | 获取字符串。 |
| <a href="#eval_t_expr_value_init">expr\_value\_init</a> | 初始化值。 |
| <a href="#eval_t_expr_value_set_number">expr\_value\_set\_number</a> | 设置浮点数值。 |
| <a href="#eval_t_expr_value_set_string">expr\_value\_set\_string</a> | 设置字符串。 |
| <a href="#eval_t_tk_expr_eval">tk\_expr\_eval</a> | 计算表达式返回浮点数。 |
| <a href="#eval_t_tk_expr_eval_str">tk\_expr\_eval\_str</a> | 计算表达式返回字符串。 |
#### eval\_default\_hooks 函数
-----------------------

* 函数功能：

> <p id="eval_t_eval_default_hooks">获取内置函数。

* 函数原型：

```
const EvalHooks* eval_default_hooks ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const EvalHooks* | 回调函数列表。 |
#### eval\_execute 函数
-----------------------

* 函数功能：

> <p id="eval_t_eval_execute">计算表达式的值。

* 函数原型：

```
EvalResult eval_execute (const char* expr, const EvalHooks* hooks, void* ctx, ExprValue* output);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | EvalResult | 返回EVAL\_RESULT\_OK表示成功，否则表示失败。 |
| expr | const char* | 表达式 |
| hooks | const EvalHooks* | 回调函数。 |
| ctx | void* | 回调函数的上下文。 |
| output | ExprValue* | 计算结果。 |
#### eval\_result\_to\_string 函数
-----------------------

* 函数功能：

> <p id="eval_t_eval_result_to_string">将错误码转换成字符串。

* 函数原型：

```
const char* eval_result_to_string (EvalResult result);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回对应的字符串。 |
| result | EvalResult | 值。 |
#### expr\_value\_clear 函数
-----------------------

* 函数功能：

> <p id="eval_t_expr_value_clear">清除值。

* 函数原型：

```
void expr_value_clear (ExprValue* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回void。 |
| v | ExprValue* | 值对象。 |
#### expr\_value\_get\_number 函数
-----------------------

* 函数功能：

> <p id="eval_t_expr_value_get_number">获取浮点数值。

* 函数原型：

```
double expr_value_get_number (const ExprValue* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | double | 返回浮点数值。 |
| v | const ExprValue* | 值对象。 |
#### expr\_value\_get\_string 函数
-----------------------

* 函数功能：

> <p id="eval_t_expr_value_get_string">获取字符串。

* 函数原型：

```
const char* expr_value_get_string (const ExprValue* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回字符串。 |
| v | const ExprValue* | 值对象。 |
#### expr\_value\_init 函数
-----------------------

* 函数功能：

> <p id="eval_t_expr_value_init">初始化值。

* 函数原型：

```
void expr_value_init (ExprValue* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回void。 |
| v | ExprValue* | 值对象。 |
#### expr\_value\_set\_number 函数
-----------------------

* 函数功能：

> <p id="eval_t_expr_value_set_number">设置浮点数值。

* 函数原型：

```
EvalResult expr_value_set_number (ExprValue* v, double val);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | EvalResult | 返回EVAL\_RESULT\_OK表示成功，否则表示失败。 |
| v | ExprValue* | 值对象。 |
| val | double | 浮点数的值。 |
#### expr\_value\_set\_string 函数
-----------------------

* 函数功能：

> <p id="eval_t_expr_value_set_string">设置字符串。

* 函数原型：

```
EvalResult expr_value_set_string (ExprValue* v, const char* str, size_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | EvalResult | 返回EVAL\_RESULT\_OK表示成功，否则表示失败。 |
| v | ExprValue* | 值对象。 |
| str | const char* | 字符串。 |
| len | size\_t | 字符串长度。 |
#### tk\_expr\_eval 函数
-----------------------

* 函数功能：

> <p id="eval_t_tk_expr_eval">计算表达式返回浮点数。

* 函数原型：

```
double tk_expr_eval (const char* expor);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | double | 返回结果。 |
| expor | const char* | 表达式 |
#### tk\_expr\_eval\_str 函数
-----------------------

* 函数功能：

> <p id="eval_t_tk_expr_eval_str">计算表达式返回字符串。

* 函数原型：

```
const char* tk_expr_eval_str (const char* expor, char* result, uint32_t );
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回结果。 |
| expor | const char* | 表达式 |
| result | char* | 结果字符串 |
|  | uint32\_t | 。 |
