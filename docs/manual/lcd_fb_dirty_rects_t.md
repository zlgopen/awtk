## lcd\_fb\_dirty\_rects\_t
### 概述
lcd fb脏矩形。
----------------------------------
### 函数
<p id="lcd_fb_dirty_rects_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#lcd_fb_dirty_rects_t_lcd_fb_dirty_rects_add_fb_info">lcd\_fb\_dirty\_rects\_add\_fb\_info</a> | 添加新的 fb 信息 |
| <a href="#lcd_fb_dirty_rects_t_lcd_fb_dirty_rects_deinit">lcd\_fb\_dirty\_rects\_deinit</a> | 清除 lcd_fb_dirty_rects_t 对象数据 |
| <a href="#lcd_fb_dirty_rects_t_lcd_fb_dirty_rects_get_dirty_rects_by_fb">lcd\_fb\_dirty\_rects\_get\_dirty\_rects\_by\_fb</a> | 根据 fb 来获取脏矩形对象 |
| <a href="#lcd_fb_dirty_rects_t_lcd_fb_dirty_rects_init">lcd\_fb\_dirty\_rects\_init</a> | 初始化 lcd_fb_dirty_rects_t 对象 |
| <a href="#lcd_fb_dirty_rects_t_lcd_fb_dirty_rects_reinit">lcd\_fb\_dirty\_rects\_reinit</a> | 修改 lcd_fb_dirty_rects_t 对象的宽高 |
| <a href="#lcd_fb_dirty_rects_t_lcd_fb_dirty_rects_reset_dirty_rects_by_fb">lcd\_fb\_dirty\_rects\_reset\_dirty\_rects\_by\_fb</a> | 清除对应 fb 的脏矩形列表。 |
| <a href="#lcd_fb_dirty_rects_t_lcd_fb_dirty_rects_update_all_fb_dirty_rects">lcd\_fb\_dirty\_rects\_update\_all\_fb\_dirty\_rects</a> | 把当前脏矩形列表合并到所有 fb 的脏矩形中。 |
#### lcd\_fb\_dirty\_rects\_add\_fb\_info 函数
-----------------------

* 函数功能：

> <p id="lcd_fb_dirty_rects_t_lcd_fb_dirty_rects_add_fb_info">添加新的 fb 信息
如果已经存在的话，就不会重新添加

* 函数原型：

```
ret_t lcd_fb_dirty_rects_add_fb_info (lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, uint8_t* fb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd\_fb\_dirty\_rects | lcd\_fb\_dirty\_rects\_t* | lcd\_fb\_dirty\_rects\_t对象。 |
| fb | uint8\_t* | fb 地址。 |
#### lcd\_fb\_dirty\_rects\_deinit 函数
-----------------------

* 函数功能：

> <p id="lcd_fb_dirty_rects_t_lcd_fb_dirty_rects_deinit">清除 lcd_fb_dirty_rects_t 对象数据

* 函数原型：

```
ret_t lcd_fb_dirty_rects_deinit (lcd_fb_dirty_rects_t* lcd_fb_dirty_rects);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd\_fb\_dirty\_rects | lcd\_fb\_dirty\_rects\_t* | lcd\_fb\_dirty\_rects\_t对象。 |
#### lcd\_fb\_dirty\_rects\_get\_dirty\_rects\_by\_fb 函数
-----------------------

* 函数功能：

> <p id="lcd_fb_dirty_rects_t_lcd_fb_dirty_rects_get_dirty_rects_by_fb">根据 fb 来获取脏矩形对象

* 函数原型：

```
const dirty_rects_t* lcd_fb_dirty_rects_get_dirty_rects_by_fb (lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, uint8_t* fb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const dirty\_rects\_t* | 成功返回脏矩形对象，失败返回 NULL。 |
| lcd\_fb\_dirty\_rects | lcd\_fb\_dirty\_rects\_t* | lcd\_fb\_dirty\_rects\_t对象。 |
| fb | uint8\_t* | fb 地址。 |
#### lcd\_fb\_dirty\_rects\_init 函数
-----------------------

* 函数功能：

> <p id="lcd_fb_dirty_rects_t_lcd_fb_dirty_rects_init">初始化 lcd_fb_dirty_rects_t 对象

* 函数原型：

```
ret_t lcd_fb_dirty_rects_init (lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, wh_t lcd_w, wh_t lcd_h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd\_fb\_dirty\_rects | lcd\_fb\_dirty\_rects\_t* | lcd\_fb\_dirty\_rects\_t对象。 |
| lcd\_w | wh\_t | lcd 的宽。 |
| lcd\_h | wh\_t | lcd 的高。 |
#### lcd\_fb\_dirty\_rects\_reinit 函数
-----------------------

* 函数功能：

> <p id="lcd_fb_dirty_rects_t_lcd_fb_dirty_rects_reinit">修改 lcd_fb_dirty_rects_t 对象的宽高

* 函数原型：

```
ret_t lcd_fb_dirty_rects_reinit (lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, wh_t lcd_w, wh_t lcd_h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd\_fb\_dirty\_rects | lcd\_fb\_dirty\_rects\_t* | lcd\_fb\_dirty\_rects\_t对象。 |
| lcd\_w | wh\_t | lcd 的宽。 |
| lcd\_h | wh\_t | lcd 的高。 |
#### lcd\_fb\_dirty\_rects\_reset\_dirty\_rects\_by\_fb 函数
-----------------------

* 函数功能：

> <p id="lcd_fb_dirty_rects_t_lcd_fb_dirty_rects_reset_dirty_rects_by_fb">清除对应 fb 的脏矩形列表。

* 函数原型：

```
ret_t lcd_fb_dirty_rects_reset_dirty_rects_by_fb (lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, uint8_t* fb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd\_fb\_dirty\_rects | lcd\_fb\_dirty\_rects\_t* | lcd\_fb\_dirty\_rects\_t对象。 |
| fb | uint8\_t* | fb 地址。 |
#### lcd\_fb\_dirty\_rects\_update\_all\_fb\_dirty\_rects 函数
-----------------------

* 函数功能：

> <p id="lcd_fb_dirty_rects_t_lcd_fb_dirty_rects_update_all_fb_dirty_rects">把当前脏矩形列表合并到所有 fb 的脏矩形中。

* 函数原型：

```
ret_t lcd_fb_dirty_rects_update_all_fb_dirty_rects (lcd_fb_dirty_rects_t* lcd_fb_dirty_rects, const dirty_rects_t* dirty_rects);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd\_fb\_dirty\_rects | lcd\_fb\_dirty\_rects\_t* | lcd\_fb\_dirty\_rects\_t对象。 |
| dirty\_rects | const dirty\_rects\_t* | 当前脏矩形列表。 |
