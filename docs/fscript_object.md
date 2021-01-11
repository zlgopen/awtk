## object 扩展函数

object 适用于app\_conf、typed\_array和array等各种对象。

### 1. object_create

> 创建缺省的 object 对象，可以当一个 map 使用。
----------------------------

#### 原型

```
object_create()
```

#### 示例

```
a = object_create()
```

### 2. object_get

> 获取对象的属性。
----------------------------

#### 原型

```
object_get(o, key)
```

#### 示例

```
print(object_get(a, "age"))
```

### 3. object_set

> 设置对象的属性。
----------------------------

#### 原型

```
object_set(a, key, value)
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
object_remove(a, key)
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
assert(object_get(a, "age") == 123)
print(object_get(a, "age"))
assert(object_remove(a, "age"))
assert(!value_is_valid(object_get(a, "age")))

unset(a)
```
