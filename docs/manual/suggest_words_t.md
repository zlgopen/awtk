## suggest\_words\_t
### 概述
输入法联想字词。
----------------------------------
### 函数
<p id="suggest_words_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#suggest_words_t_suggest_words_create">suggest\_words\_create</a> | 创建联想字对象。 |
| <a href="#suggest_words_t_suggest_words_destroy">suggest\_words\_destroy</a> | @deconstructor |
| <a href="#suggest_words_t_suggest_words_find">suggest\_words\_find</a> | 查找prefix对应的联想字列表，成功后会更新words/words_nr。 |
### 属性
<p id="suggest_words_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
#### suggest\_words\_create 函数
-----------------------

* 函数功能：

> <p id="suggest_words_t_suggest_words_create">创建联想字对象。

* 函数原型：

```
suggest_words_t* suggest_words_create (asset_info_t* res);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | suggest\_words\_t* | 返回联想字对象。 |
| res | asset\_info\_t* | 数据资源。 |
#### suggest\_words\_destroy 函数
-----------------------

* 函数功能：

> <p id="suggest_words_t_suggest_words_destroy">@deconstructor
销毁联想字对象。

* 函数原型：

```
ret_t suggest_words_destroy (suggest_words_t* suggest_words);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| suggest\_words | suggest\_words\_t* | 联想字对象。 |
#### suggest\_words\_find 函数
-----------------------

* 函数功能：

> <p id="suggest_words_t_suggest_words_find">查找prefix对应的联想字列表，成功后会更新words/words_nr。

* 函数原型：

```
ret_t suggest_words_find (suggest_words_t* suggest_words, wchar_t c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| suggest\_words | suggest\_words\_t* | 联想字对象。 |
| c | wchar\_t | 字符。 |
