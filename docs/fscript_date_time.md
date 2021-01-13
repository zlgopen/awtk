## 日期时间扩展函数

### 1.date\_time\_create

> 创建日期对象。
----------------------------

#### 原型

```
date_time_create() => object
```

日期对象有下列属性：

* year 年 （读写）
* month 月 （读写）
* day 日 （读写）
* hour 时 （读写）
* minute 分 （读写）
* second 秒 （读写）
* wday 周几 （只写）

### 2.date\_time\_to\_time

> 转成相对于 00:00:00 UTC on 1 January 1970 的秒数。
----------------------------

#### 原型

```
date_time_to_time(dt) => uint64_t
```

### 3.date\_time\_from\_time
> 将相对于 00:00:00 UTC on 1 January 1970 的秒数转成成 date time。
----------------------------

#### 原型

```
date_time_from_time(dt, time) => bool
```

### 4.date\_time\_set

> 把 date time 中的时间设置为系统时间。
----------------------------

#### 原型

```
date_time_set(dt) => bool
```

> 需要有设置系统时间的权限。

### 5.time_now 

> 获取当前时间戳函数 (s)。
----------------------------

#### 原型

```
time\_now()  => uint64_t
```

### 6.time\_now\_ms 

> 获取当前时间戳函数 (ms)。
----------------------------

#### 原型

```
time_now_ms()  => uint64_t
```

### 7.time_now_us 

> 获取当前时间戳函数 (us)。
----------------------------

#### 原型

```
time_now_us() => uint64_t
```

### 8.is\_leap\_year

> 判断指定年份是否是闰年。
----------------------------

#### 原型

```
is_leap_year(year) => bool
```


### 9.get\_days\_of\_month

> 获取指定年月的天数。
----------------------------

#### 原型

```
get_days_of_month(year, month) => uint32_t
```

### 更多示例

```js
dt = date_time_create()
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
