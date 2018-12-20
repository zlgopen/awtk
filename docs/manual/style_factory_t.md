## style\_factory\_t
### 概述
 style_factory接口。

### 函数
<p id="style_factory_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#style_factory_t_style_factory">style\_factory</a> |  获取缺省的style factory。 |
| <a href="#style_factory_t_style_factory_create_style">style\_factory\_create\_style</a> |  创建style。 |
| <a href="#style_factory_t_style_factory_set">style\_factory\_set</a> |  设置缺省的style factory(需要自定义的style factory才设置)。 |
### 属性
<p id="style_factory_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="style_factory_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### style\_factory 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | style\_factory\_t* | 返回style factory对象。 |
<p id="style_factory_t_style_factory"> 获取缺省的style factory。



#### style\_factory\_create\_style 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | style\_t* | 返回style对象。 |
| factory | style\_factory\_t* | factory对象。 |
| widget | widget\_t* | 控件对象。 |
<p id="style_factory_t_style_factory_create_style"> 创建style。




#### style\_factory\_set 函数
-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | style\_factory\_t* | factory对象。 |
<p id="style_factory_t_style_factory_set"> 设置缺省的style factory(需要自定义的style factory才设置)。




