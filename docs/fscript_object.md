## object 扩展函数

object 适用于 app\_conf、typed\_array 和 array 等各种对象。

### 1. object_create

> 创建缺省的 object 对象，可以当一个 map 使用。
----------------------------

#### 原型

```
object_create() => value
```

#### 示例

```
a = object_create()
```

### 2. object_get

> 获取对象的属性。

可以通过 value\_is\_valid 来判断返回值是否有效。
----------------------------

#### 原型

```
object_get(o, key) => value
```

也可以直接访问对象的属性：

```
print(o.key)
```

#### 示例

```
print(object_get(a, "age"))
print(a.age)
```

### 3. object_set

> 设置对象的属性。
----------------------------

#### 原型

```
object_set(a, key, value) => bool
```

#### 示例

```
object_set(a, "age", 123);
```

### 4. object_remove

> 删除对象的属性。 
----------------------------

#### 原型

```
object_remove(a, key) => bool
```

#### 示例

```
object_remove(a, "age")
```

### 完整示例

```js
a = object_create()
assert(!value_is_null(a))
assert(object_set(a, "age", 123))
assert(a.age ==  123)

assert(object_set(a, "name", "awtk"))
assert(a.name == "awtk")

assert(object_get(a, "age") == 123)
print(object_get(a, "age"))
assert(object_remove(a, "age"))
assert(!value_is_valid(object_get(a, "age")))
unset(a)

person = object_create()
assert(!value_is_null(person))

assert(object_set(person, "age", 123))
assert(person.age ==  123)

assert(object_set(person, "name", "awtk"))
assert(person.name == "awtk")

unset(person)


```
