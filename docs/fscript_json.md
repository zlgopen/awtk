## JSON 扩展函数

### 1.json\_load

> 加载 json 数据。
----------------------------

#### 原型

```js
json_load(str) => object
json_load(binary) => object
json_load(data, size) => object
```

### 2.json\_save

> 保存 json 数据。
----------------------------

#### 原型

```js
json_save(obj, filename) => bool
```

### 3.ubjson\_load

> 加载 ubjson 数据。
----------------------------

#### 原型

```js
ubjson_load(binary) => object
ubjson_load(data, size) => object
```

### 4.ubjson\_save

> 保存 ubjson 数据。
----------------------------

#### 原型

```js
ubjson_save(obj, filename) => bool
```

### 完整示例

```js
//load from string
a=json_load('
  {
    "name":"fscript", 
    "age":100
  }
')

assert(object_get(a, "name")=="fscript");
assert(object_get(a, "age")==100);

//save to file
b=json_save(a)
assert(file_write("test.json", b))

//load from file as text
a = json_load(file_read_text("test.json"))

assert(object_get(a, "name")=="fscript");
assert(object_get(a, "age")==100);

//load from file as binary
a = json_load(file_read_binary("test.json"))
assert(file_remove("test.json"))

assert(object_get(a, "name")=="fscript");
assert(object_get(a, "age")==100);

assert(file_remove("test.json"))
unset(a)
unset(b)
```
