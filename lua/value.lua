
local v = value_t.create()
local a = 100;

v:set_int8(100);
print(v:int())

v:set_str("hello");
print(v:str());

v:set_bool(true)
v:set_bool()
print(v:bool());

v:_set("test", 123);
print(v:_get("test"));

print("VALUE_TYPE_INVALID=" .. VALUE_TYPE_INVALID);
print("VALUE_TYPE_STRING=" .. VALUE_TYPE_STRING);

v:destroy();
