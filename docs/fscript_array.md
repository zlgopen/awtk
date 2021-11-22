##  array 扩展函数

array 是一个通用的数组，里面可以放混合数据类型的数据。

array 有以下属性：

* size 元素的个数
* capacity 元素的最大个数

### 1.array\_create

> 创建 array 对象。
----------------------------

#### 原型

```js
array_create() => object
```

### 2.array\_create\_with\_str

> 创建 array 对象。以指定的分隔符把字符串拆开，并转换为指定类型的数据，放入数组中。
----------------------------

#### 原型

```js
array_create_with_str(str, sep, type) => object
```

type 为可选参数，可选取值为：

* int 表示整数。
* double 表示浮点数。
* 其它或默认为字符串。

#### 示例

```
a = array_create_with_str("2,1,3", ",", "int");
a = array_create_with_str("a2,a1,a3", ",");
```

### 3.array\_create\_repeated

> 创建 array 对象。
----------------------------

#### 原型

```js
array_create_repeated(value, nr) => object
```

* value 为初始化的值。
* nr 为值的个数。

#### 示例

```
a = array_create_repeated(123, 3);
```

### 4.array\_dup

> 创建 array 对象。
----------------------------

#### 原型

```js
array_dup(arr, start, end) => object
```

start 为开始的位置，默认为 0。
end 为结束的位置（不包含）

#### 示例

```
a = array_create_with_str("2,1,3", ",", "int");
b = array_dup(a);
c = array_dup(a, 1);
d = array_dup(a, 1, 2);
```

### 5.array\_push

> 追加一个或多个数据。
----------------------------

#### 原型

```js
array_push(array, v, ...) => uint32_t
```

> 返回写入数据的个数

### 6.array\_pop

> 弹出最后一个元素
----------------------------

#### 原型

```js
array_pop(array) => value
```

### 7.array\_shift

> 弹出第一个元素
----------------------------

#### 原型

```js
array_shift(array) => value
```

### 8.array\_get

> 获取指定位置的元素
----------------------------

#### 原型

```js
array_get(array, index) => value
```

> index 为负数时，从后往前取。比如-1 表示最后一个元素。
>
> 此外，也可以通过 array.[index] 的方式访问数组元素。

### 9.array\_set

> 修改指定位置的元素
----------------------------

#### 原型

```js
array_set(array, index, value) => bool
```

> index 为负数时，从后往前取。比如-1 表示最后一个元素。

### 10.array\_insert

> 插入元素到指定位置
----------------------------

#### 原型

```js
array_insert(array, index, value) => bool
```

> index 为负数时，从后往前取。比如-1 表示最后一个元素。

### 11.array\_index\_of

> 获取元素首次出现的位置。
----------------------------

#### 原型

```js
array_index_of(array, value) => index
```

### 12.array\_last_index\_of

> 获取元素最后出现的位置。
----------------------------

#### 原型

```js
array_last_index_of(array, value) => index
```

### 13.array\_remove

> 删除指定位置的元素
----------------------------

#### 原型

```js
array_remove(array, index) => bool
```

> index 为负数时，从后往前取。比如-1 表示最后一个元素。

### 14.array\_get\_and\_remove

> 删除指定位置的元素，并返回改元素。
----------------------------

#### 原型

```js
array_get_and_remove(array, index) => value
```

> index 为负数时，从后往前取。比如-1 表示最后一个元素。

### 15.array\_clear

> 删除全部元素
----------------------------

#### 原型

```js
array_clear(array) => bool
```

### 16.array\_join

> 连接全部元素成一个字符串。
----------------------------

#### 原型

```js
array_join(array, sep) => str
```

### 17.array\_sort

> 根据当前的数据类型排序，以第一个元素类型为准。
----------------------------

#### 原型

```js
array_sort(array, ascending, ignore_case) => bool
```

* ascending 是否为升序，默认是。
* ignore_case 是否忽略大小写，仅用于字符串，默认为否。

#### 示例

```
a = array_create_with_str("2,1,3", ",", "int");
assert(array_sort(a));
d = array_join(a, ":");
assert(d == "1:2:3")

assert(array_sort(a, false));
d = array_join(a, ":");
assert(d == "3:2:1")
```

### 18.array\_min

> 获取最小值。
----------------------------

#### 原型

```js
array_min(array) => double
```

### 19.array\_max

> 获取最大值。
----------------------------

#### 原型

```js
array_max(array) => double
```

### 20.array\_avg

> 求平均值。
----------------------------

#### 原型

```js
array_avg(array) => double
```

### 21.array\_sum

> 求和。
----------------------------

#### 原型

```js
array_sum(array) => double
```

### 22.array\_clone\_and\_sort

> clone一份数据，根据当前的数据类型排序，以第一个元素类型为准。
----------------------------

#### 原型

```js
array_clone_and_sort(array, ascending, ignore_case) => array
```

* ascending 是否为升序，默认是。
* ignore_case 是否忽略大小写，仅用于字符串，默认为否。

#### 示例

```
a = array_create_with_str("2,1,3", ",", "int");
b = array_clone_and_sort(a);
d = array_join(b, ":");
assert(d == "1:2:3")
```

### 更多示例

```js
a = array_create()
assert(array_push(a,1,2,3) == 3)
assert(array_push(a,"a","b","c") == 3)
assert(array_pop(a), "c")
assert(array_pop(a), "b")
assert(array_pop(a), "a")
assert(array_pop(a), "3")
assert(array_pop(a), "2")
assert(array_pop(a), "1")
assert(value_is_valid(array_pop(a)))
assert(array_clear(a))
assert(a.size == 0)

assert(array_insert(a, 0, 'a'))
assert(array_insert(a, 1, 'b'))
assert(array_insert(a, 2, 'c'))
assert(array_insert(a, 3, 'd'))
assert(a.size == 4)
assert(array_remove(a, 0))

assert(array_get(a, 0) == 'b')
assert(array_set(a, 0, 'bbb'))
assert(array_get(a, 0) == 'bbb')
assert(array_set(a, 0, 'b'))

assert(array_get(a, 1) == 'c')
assert(array_set(a, 1, 'ccc'))
assert(array_get(a, 1) == 'ccc')
assert(array_set(a, 1, 'c'))
assert(array_join(a, ',') == 'b,c,d')
print(a.size, a.capacity)
```
