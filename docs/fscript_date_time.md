## 日前扩展函数

### 1.date\_time\_create

### 2.date\_time\_to\_time

### 3.date\_time\_from\_time

### 4.date\_time\_set

### 5.time\_now\_us

### 6.time\_now\_ms

### 7.time\_now\_s

### 8.is\_leap\_year

### 9.get\_days\_of\_month

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

