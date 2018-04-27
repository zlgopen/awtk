
local v = Value.create()
local a = 100;

v:set_int8(100);
print(v:int())

v:set_str("hello");
print(v:str());

v:set_bool(true)
v:set_bool()
print(v:bool());

print(v.type);
print(ValueType)
print(ValueType.INVALID);
print(ValueType.BOOL);
print(ValueType.FLOAT);

v:destroy();

tk_quit();

