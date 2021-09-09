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

> 将对象保存为 json 数据。
----------------------------

#### 原型

```js
json_save(obj) => binary 
```

### 3.json\_save\_to\_string

> 将对象保存为字符串数据。
----------------------------

#### 原型

```js
json_save_to_string(obj) => string 
```

### 4.ubjson\_load

> 加载 ubjson 数据。
----------------------------

#### 原型

```js
ubjson_load(binary) => object
ubjson_load(data, size) => object
```

### 5.ubjson\_save

> 将对象保存为 ubjson 数据。
----------------------------

#### 原型

```js
ubjson_save(obj) => binary
```

### 完整示例

```js
//load from string
str='{
    "name" : "fscript",
    "age" : 100
}'
a=json_load(str)

assert(object_get(a, "name")=="fscript");
assert(object_get(a, "age")==100);

//save to file
b=json_save(a)
assert(file_write("test.json", b))

//json obj to string
c=json_save_to_string(a)
assert(c==str);

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
unset(c)
unset(str)
```
