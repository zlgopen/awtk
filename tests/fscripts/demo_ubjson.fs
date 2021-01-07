//create a json
a=json_load('
  {
    "name":"fscript", 
    "age":100
  }
')

assert(object_get(a, "name")=="fscript")
assert(object_get(a, "age")==100)

//save as ubjson
b=ubjson_save(a)
print(b)

//load from buffer
a = ubjson_load(b)
assert(object_get(a, "name")=="fscript")
assert(object_get(a, "age")==100)
//load from file as binary
assert(file_write("test.ubjson", b))
a = ubjson_load(file_read_binary("test.ubjson"))
assert(file_remove("test.ubjson"))

assert(object_get(a, "name")=="fscript")
assert(object_get(a, "age")==100)

assert(file_remove("test.ubjson"))
unset(a)
unset(b)
