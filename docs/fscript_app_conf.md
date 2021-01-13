## app_conf 扩展函数

获取 app\_conf 对象后，可以直接用 [object](fscript_object.md) 的接口访问 app\_conf。

### 1. app_conf

> 获取 app_conf 对象
----------------------------

#### 原型

```
app_conf() => object
```

#### 示例

```
assert(object_set(app_conf(), "timeout", 100))
```

### 2. app_conf_save

> 保存修改。
----------------------------

#### 原型

```
app_conf_save() => bool
```

#### 示例

```
assert(app_conf_save())
```

### 完整示例

```js
assert(object_set(app_conf(), "timeout", 100))
assert(object_set(app_conf(), "serial.device", "/dev/tty01"))
assert(object_set(app_conf(), "serial.baudrate", 115200))
assert(app_conf_save())

assert(object_get(app_conf(), "timeout") == 100)
assert(object_get(app_conf(), "serial.device") == "/dev/tty01")
assert(object_get(app_conf(), "serial.baudrate") == 115200)

assert(object_remove(app_conf(), "timeout"))
assert(object_remove(app_conf(), "serial"))

assert(object_get(app_conf(), "timeout", 10) == 10)
assert(object_get(app_conf(), "serial.device", "a") == "a")
assert(object_get(app_conf(), "serial.baudrate", 1000) == 1000) 
assert(object_set(app_conf(), "timeout", 100))

assert(app_conf_save())
```
