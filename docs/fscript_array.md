##  array 扩展函数

array是一个通用的数组，里面可以放混合数据类型的数据。

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

### 2.array\_push

> 追加一个或多个数据。
----------------------------

#### 原型

```js
array_push(array, v, ...) => uint32_t
```

> 返回写入数据的个数

### 3.array\_pop

> 弹出最后一个元素
----------------------------

#### 原型

```js
array_pop(array) => value
```

### 4.array\_get

> 获取指定位置的元素
----------------------------

#### 原型

```js
array_get(array, index) => value
```

### 5.array\_set

> 修改指定位置的元素
----------------------------

#### 原型

```js
array_set(array, index, value) => bool
```

### 6.array\_insert

> 插入元素到指定位置
----------------------------

#### 原型

```js
array_insert(array, index, value) => bool
```

### 7.array\_remove

> 删除指定位置的元素
----------------------------

#### 原型

```js
array_remove(array, index) => bool
```

### 8.array\_clear

> 删除全部元素
----------------------------

#### 原型

```js
array_clear(array) => bool
```

### 9.array\_join

> 连接全部元素成一个字符串。
----------------------------

#### 原型

```js
array_join(array, sep) => str
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

