## typed array扩展函数

### 1.typed\_array\_create
### 2.typed\_array\_push
### 3.typed\_array\_pop
### 4.typed\_array\_get
### 5.typed\_array\_set
### 6.typed\_array\_insert
### 7.typed\_array\_remove
### 8.typed\_array\_clear
### 9.typed\_array\_join

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
