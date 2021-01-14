## typed array 扩展函数

typed\_array是一个单一类型的动态数组，里面只能放相同类型的元素。

typed\_array 有以下属性：

* data 缓冲区的指针
* size 元素的个数
* capacity 元素的最大个数
* size\_in\_bytes 元素的个数 * 元素的大小
* capacity\_in\_bytes 元素的最大个数 * 元素的大小

### 1.typed\_array\_create

> 创建 typed\_array 对象。
----------------------------

#### 原型

```js
typed_array_create() => object
```

### 2.typed\_array\_push

> 追加一个或多个数据。
----------------------------

#### 原型

```js
typed_array_push(array, v, ...) => uint32_t
```

> 返回写入数据的个数

### 3.typed\_array\_pop

> 弹出最后一个元素
----------------------------

#### 原型

```js
typed_array_pop(array) => value
```

### 4.typed\_array\_get

> 获取指定位置的元素
----------------------------

#### 原型

```js
typed_array_get(array, index) => value
```

### 5.typed\_array\_set

> 修改指定位置的元素
----------------------------

#### 原型

```js
typed_array_set(array, index, value) => bool
```

### 6.typed\_array\_insert

> 插入元素到指定位置
----------------------------

#### 原型

```js
typed_array_insert(array, index, value) => bool
```

### 7.typed\_array\_remove

> 删除指定位置的元素
----------------------------

#### 原型

```js
typed_array_remove(array, index) => bool
```

### 8.typed\_array\_clear

> 删除全部元素
----------------------------

#### 原型

```js
typed_array_clear(array) => bool
```

### 9.typed\_array\_join

> 连接全部元素成一个字符串。
----------------------------

#### 原型

```js
typed_array_join(array, sep) => str
```

### 更多示例

```js
a=typed_array_create("i32", 10)

assert(typed_array_push(a, 1) == 1)
assert(a.size, 1);
assert(a.bytes, 4);

assert(typed_array_push(a, 2, 3) == 2)
assert(a.size, 3);
assert(a.bytes, 12);
assert(typed_array_get(a, 0)==1)
assert(typed_array_get(a, 1)==2)
assert(typed_array_get(a, 2)==3)

assert(typed_array_set(a, 0, 11))
assert(typed_array_set(a, 1, 22))
assert(typed_array_set(a, 2, 33))

assert(typed_array_get(a, 0)==11)
assert(typed_array_get(a, 1)==22)
assert(typed_array_get(a, 2)==33)

assert(typed_array_pop(a), 33);
assert(typed_array_pop(a), 22);
assert(typed_array_pop(a), 11);
assert(a.size == 0);

assert(typed_array_push(a,1, 2, 3) == 3)
assert(typed_array_remove(a, 0))
assert(a.size == 2);
assert(typed_array_remove(a, 1))
assert(typed_array_remove(a, 0))
assert(a.size == 0);

assert(typed_array_push(a,1, 2, 3) == 3)
print(typed_array_join(a, ',') == '1,2,3')
unset(a);
```
