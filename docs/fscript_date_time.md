## 日期时间扩展函数

### 1.date\_time\_create

> 创建日期对象。
----------------------------

#### 原型

```js
date_time_create() => object
```

日期对象有下列属性：

* year 年 （可读写）
* month 月 （可读写）
* day 日 （可读写）
* hour 时 （可读写）
* minute 分 （可读写）
* second 秒 （可读写）
* wday 周几 （可只读）

#### 示例

```js
var dt = date_time_create();
print(dt.year, "-", dt.month, "-", dt.day, " ", dt.hour, ":", dt.minute, ":", dt.second, "(", dt.wday, ")");
```

### 2.date\_time\_to\_time

> 转成相对于 00:00:00 UTC on 1 January 1970 的秒数。
----------------------------

#### 原型

```js
date_time_to_time(dt) => uint64_t
```

#### 示例

```js
var dt = date_time_create();
var time = date_time_to_time(dt);
```

### 3.date\_time\_from\_time

> 将相对于 00:00:00 UTC on 1 January 1970 的秒数转成成 date time。
----------------------------

#### 原型

```js
date_time_from_time(dt, time) => bool
```

#### 示例

```js
var dt = date_time_create();
var time = date_time_to_time(dt);
assert(date_time_from_time(dt, time));
```

### 4.date\_time\_set

> 把 date time 中的时间设置为系统时间。
----------------------------

#### 原型

```js
date_time_set(dt) => bool
```

> 需要有设置系统时间的权限。

#### 示例

```js
var dt = date_time_create();
dt.year = 2022;
dt.month = 10;
dt.day = 1;
dt.hour = 2;
dt.minute = 3;
dt.second = 4;
assert(date_time_set(dt));
```

### 5.time\_now/time\_now\_s

> 获取当前时间戳函数 (s)。
----------------------------

#### 原型

```js
time_now()  => uint64_t
time_now_s()  => uint64_t
```

#### 示例

```js
var a = time_now();
var b = time_now_s();
```

### 6.time\_now\_ms 

> 获取当前时间戳函数 (ms)。
----------------------------

#### 原型

```js
time_now_ms()  => uint64_t
```

#### 示例

```js
var a = time_now_ms();
```

### 7.time\_now\_us 

> 获取当前时间戳函数 (us)。
----------------------------

#### 原型

```js
time_now_us() => uint64_t
```

#### 示例

```js
var a = time_now_us();
```

### 8.is\_leap\_year

> 判断指定年份是否是闰年。
----------------------------

#### 原型

```js
is_leap_year(year) => bool
```

#### 示例

```js
var dt = date_time_create();
print(is_leap_year(dt.year));
```

### 9.get\_days\_of\_month

> 获取指定年月的天数。
----------------------------

#### 原型

```js
get_days_of_month(year, month) => uint32_t
```

#### 示例

```js
var dt = date_time_create();
print(get_days_of_month(dt.year, dt.month));
```

### 10.date\_time\_set\_prop

> 设置日期对象中的属性。
----------------------------

#### 原型

```js
date_time_set_prop(dt, name, value) => bool
```

#### 示例

```js
var dt = date_time_create();
date_time_set_prop(dt, "year", 2012);
```

### 11.date\_time\_get\_prop

> 获取日期对象中的属性。
----------------------------

#### 原型

```js
date_time_get_prop(dt, name) => value
```

#### 示例

```js
var dt = date_time_create();
var year = date_time_get_prop(dt, "year");
```

### 更多示例

```js
var dt = date_time_create()
print(dt.year, "-", dt.month, "-", dt.day, " ", dt.hour, ":", dt.minute, ":", dt.second, "(", dt.wday, ")")

dt.year = 2022
dt.month = 10
dt.day = 1
dt.hour = 2
dt.minute = 3
dt.second = 4
assert(dt.year == 2022)
assert(dt.month == 10)
assert(dt.day == 1)
assert(dt.hour == 2)
assert(dt.minute == 3)
assert(dt.second == 4)
print(dt.year, "-", dt.month, "-", dt.day, " ", dt.hour, ":", dt.minute, ":", dt.second, "(", dt.wday, ")")

assert(date_time_to_time(dt) == 1664560984)
assert(date_time_from_time(dt, 1664560985))

assert(dt.year == 2022)
assert(dt.month == 10)
assert(dt.day == 1)
assert(dt.hour == 2)
assert(dt.minute == 3)
assert(dt.second == 5)

print(time_now_us())
print(time_now_ms())
print(time_now_s())
```
