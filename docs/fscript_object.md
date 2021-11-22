## object 扩展函数

object 适用于 app\_conf、typed\_array 和 array 等各种对象。

### 1. object\_create

> 创建缺省的 object 对象，可以当一个 map 使用。
----------------------------

#### 原型

```
object_create() => object
```

#### 示例

```
a = object_create()
```

### 2. object\_get

> 获取对象的属性。
----------------------------

可以通过 value\_is\_valid 来判断返回值是否有效。

#### 原型

```
object_get(o, key) => value
object_get(o, key, defval) => value
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

### 3. object\_set

> 设置对象的属性。
----------------------------

#### 原型

```
object_set(obj, key, value) => bool
```

#### 示例

```
object_set(a, "age", 123);
```

### 4. object\_remove

> 删除对象的属性。 
----------------------------

#### 原型

```
object_remove(obj, key) => bool
```

#### 示例

```
object_remove(a, "age")
```

### 5. object\_ref

> 引用计数加1。 
----------------------------

#### 原型

```
object_ref(obj) => object
```

#### 示例

```
object_ref(a)
```

### 5. object\_unref

> 引用计数减1。引用计数为0时，销毁对象。 
----------------------------

#### 原型

```
object_unref(obj)
```

#### 示例

```
object_ref(a)
```

### 6. object\_forward\_events

> 将源对象的事件转发目标对象。 
----------------------------

目前支持以下事件的转发：

* EVT_PROP_CHANGED：对象中的属性改变事件。
* EVT_PROPS_CHANGED：对象中的属性改变事件。
* EVT_ITEMS_CHANGED：集合对象中的项目改变事件

#### 原型

```
object_forward_events(src_obj, dst_obj)
```

#### 示例

```
global = object_create()
global.arr = array_create()
object_forward_events(global.arr, global)
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

global = object_create()
global.arr = array_create()
object_forward_events(global.arr, global)

unset(person)
unset(global)
```
