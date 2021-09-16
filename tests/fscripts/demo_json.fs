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
