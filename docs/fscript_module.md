## module 扩展函数

加载外部模块。

> 注意：
> * 不要在模块中定义全局变量，目前无法访问模块内的全局变量。
> * 不要在模块函数中使用成员变量，模块函数和当前运行共享同一个对象，避免产生访问冲突。

### 1.require\_str

> 从字符串加载模块。
----------------------------

#### 原型

```js
require_str(str) => object
```

#### 示例

```js
var code = '
function foo(a, b) {
  return a + b
}
function bar(a, b) {
  return a - b
}
'
var m = require_str(code)
print(m.foo(100, 99))
print(m.bar(100, 99))
```

### 2.require

> 从指定URL/文件加载模块。
----------------------------

#### 原型

```js
require(url) => object
```

#### 示例

* 模块文件：foobar.fs

```js
function foo(a, b) {
  return a + b
}

function bar(a, b) {
  return a - b
}
```

* 主程序：main.fs

```js
var m = require('foobar.fs')
print(m.foo(100, 99))
print(m.bar(100, 99))
```
