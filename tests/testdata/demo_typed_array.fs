a=typed_array_create("i32", 10)

typed_array_push(a, 123);
typed_array_push(a, 234); 
print(typed_array_get(a, 0))
print(typed_array_get(a, 1))
typed_array_insert(a, 0, 0); 
print(typed_array_size(a))
print(typed_array_get(a, 0))
typed_array_remove(a, 0);
print(typed_array_size(a))

object_unref(a)
