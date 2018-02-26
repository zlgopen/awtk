#define LUA_COMPAT_MODULE
#include "lua.h"
#include "lauxlib.h"
#include "base/value.h"

static void value_type_t_init(lua_State* L) {
  lua_pushinteger(L, VALUE_TYPE_INVALID);
  lua_setglobal(L, "VALUE_TYPE_INVALID");
  lua_pushinteger(L, VALUE_TYPE_BOOL);
  lua_setglobal(L, "VALUE_TYPE_BOOL");
  lua_pushinteger(L, VALUE_TYPE_INT8);
  lua_setglobal(L, "VALUE_TYPE_INT8");
  lua_pushinteger(L, VALUE_TYPE_UINT8);
  lua_setglobal(L, "VALUE_TYPE_UINT8");
  lua_pushinteger(L, VALUE_TYPE_INT16);
  lua_setglobal(L, "VALUE_TYPE_INT16");
  lua_pushinteger(L, VALUE_TYPE_UINT16);
  lua_setglobal(L, "VALUE_TYPE_UINT16");
  lua_pushinteger(L, VALUE_TYPE_INT32);
  lua_setglobal(L, "VALUE_TYPE_INT32");
  lua_pushinteger(L, VALUE_TYPE_UINT32);
  lua_setglobal(L, "VALUE_TYPE_UINT32");
  lua_pushinteger(L, VALUE_TYPE_INT64);
  lua_setglobal(L, "VALUE_TYPE_INT64");
  lua_pushinteger(L, VALUE_TYPE_UINT64);
  lua_setglobal(L, "VALUE_TYPE_UINT64");
  lua_pushinteger(L, VALUE_TYPE_POINTER);
  lua_setglobal(L, "VALUE_TYPE_POINTER");
  lua_pushinteger(L, VALUE_TYPE_FLOAT32);
  lua_setglobal(L, "VALUE_TYPE_FLOAT32");
  lua_pushinteger(L, VALUE_TYPE_FLOAT64);
  lua_setglobal(L, "VALUE_TYPE_FLOAT64");
  lua_pushinteger(L, VALUE_TYPE_STRING);
  lua_setglobal(L, "VALUE_TYPE_STRING");
  lua_pushinteger(L, VALUE_TYPE_WSTRING);
  lua_setglobal(L, "VALUE_TYPE_WSTRING");
}

static int wrap_value_set_bool(lua_State* L) {
  value_t* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  bool_t value = (bool_t)lua_toboolean(L, 2);
  ret = value_set_bool(v, value);

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_bool(lua_State* L) {
  bool_t ret = 0;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_bool(v);

  lua_pushboolean(L,(lua_Integer)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_set_int8(lua_State* L) {
  value_t* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  int8_t value = (int8_t)luaL_checkinteger(L,2);
  ret = value_set_int8(v, value);

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_int8(lua_State* L) {
  int8_t ret = 0;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_int8(v);

  lua_pushinteger(L,(lua_Integer)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_set_uint8(lua_State* L) {
  value_t* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  uint8_t value = (uint8_t)luaL_checkinteger(L,2);
  ret = value_set_uint8(v, value);

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_uint8(lua_State* L) {
  int8_t ret = 0;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_uint8(v);

  lua_pushinteger(L,(lua_Integer)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_set_int16(lua_State* L) {
  value_t* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  int16_t value = (int16_t)luaL_checkinteger(L,2);
  ret = value_set_int16(v, value);

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_int16(lua_State* L) {
  int16_t ret = 0;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_int16(v);

  lua_pushinteger(L,(lua_Integer)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_set_uint16(lua_State* L) {
  value_t* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  uint16_t value = (uint16_t)luaL_checkinteger(L,2);
  ret = value_set_uint16(v, value);

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_uint16(lua_State* L) {
  uint16_t ret = 0;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_uint16(v);

  lua_pushinteger(L,(lua_Integer)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_set_int32(lua_State* L) {
  value_t* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  int32_t value = (int32_t)luaL_checkinteger(L,2);
  ret = value_set_int32(v, value);

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_int32(lua_State* L) {
  int32_t ret = 0;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_int32(v);

  lua_pushinteger(L,(lua_Integer)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_set_uint32(lua_State* L) {
  value_t* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  uint32_t value = (uint32_t)luaL_checkinteger(L,2);
  ret = value_set_uint32(v, value);

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_uint32(lua_State* L) {
  uint32_t ret = 0;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_uint32(v);

  lua_pushinteger(L,(lua_Integer)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_set_int64(lua_State* L) {
  value_t* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  int64_t value = (int64_t)luaL_checkinteger(L,2);
  ret = value_set_int64(v, value);

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_int64(lua_State* L) {
  int64_t ret = 0;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_int64(v);

  lua_pushinteger(L,(lua_Integer)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_set_uint64(lua_State* L) {
  value_t* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  uint64_t value = (uint64_t)luaL_checkinteger(L,2);
  ret = value_set_uint64(v, value);

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_uint64(lua_State* L) {
  uint64_t ret = 0;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_uint64(v);

  lua_pushinteger(L,(lua_Integer)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_set_pointer(lua_State* L) {
  value_t* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  pointer_t value = (pointer_t)luaL_checkinteger(L,2);
  ret = value_set_pointer(v, value);

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_pointer(lua_State* L) {
  void* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_pointer(v);

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_set_float(lua_State* L) {
  value_t* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  float value = (float)luaL_checknumber(L,2);
  ret = value_set_float(v, value);

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_float(lua_State* L) {
  float ret = 0;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_float(v);

  lua_pushnumber(L,(lua_Number)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_set_double(lua_State* L) {
  value_t* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  double value = (double)luaL_checknumber(L,2);
  ret = value_set_double(v, value);

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_double(lua_State* L) {
  double ret = 0;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_double(v);

  lua_pushnumber(L,(lua_Number)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_set_str(lua_State* L) {
  value_t* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  char* value = (char*)luaL_checkstring(L,2);
  ret = value_set_str(v, value);

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_str(lua_State* L) {
  const char* ret = NULL;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_str(v);

  lua_pushstring(L,(char*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_is_null(lua_State* L) {
  bool_t ret = 0;
  value_t* value = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_is_null(value);

  lua_pushboolean(L,(lua_Integer)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_int(lua_State* L) {
  int ret = 0;
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  ret = value_int(v);

  lua_pushinteger(L,(lua_Integer)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_copy(lua_State* L) {
  value_t* dst = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  value_t* src = (value_t*)luaL_checkudata(L, 2, "lftk.value_t");
  value_copy(dst, src);

  return 0;
}

static int wrap_value_create(lua_State* L) {
  value_t* ret = NULL;
  ret = value_create();

  lua_pushlightuserdata(L,(void*)(ret));
  luaL_getmetatable(L, "lftk.value_t");
  lua_setmetatable(L, -2);

  return 1;
}

static int wrap_value_destroy(lua_State* L) {
  value_t* v = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  value_destroy(v);

  return 0;
}

static int wrap_value_t_set_prop(lua_State* L) {
  value_t* obj = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  const char* name = (const char*)luaL_checkstring(L,2);
  if(strcmp(name, "type") == 0) {
      printf("type is readonly\n");
  }
  else {
    printf("not supported %s\n", name);
  }
  return 0;
}
static int wrap_value_t_get_prop(lua_State* L) {
  value_t* obj = (value_t*)luaL_checkudata(L, 1, "lftk.value_t");
  const char* name = (const char*)luaL_checkstring(L,2);
  if(strcmp(name, "type") == 0) {
    lua_pushinteger(L,(lua_Integer)(obj->type));
  }
  else {
    printf("not supported %s\n", name);
  }
  return 1;
}
static void value_t_init(lua_State* L) {
  static const struct luaL_Reg static_funcs[] = {
    {"create", wrap_value_create},
    {NULL, NULL}
  };
  static const struct luaL_Reg member_funcs[] = {
    {"set_bool", wrap_value_set_bool},
    {"bool", wrap_value_bool},
    {"set_int8", wrap_value_set_int8},
    {"int8", wrap_value_int8},
    {"set_uint8", wrap_value_set_uint8},
    {"uint8", wrap_value_uint8},
    {"set_int16", wrap_value_set_int16},
    {"int16", wrap_value_int16},
    {"set_uint16", wrap_value_set_uint16},
    {"uint16", wrap_value_uint16},
    {"set_int32", wrap_value_set_int32},
    {"int32", wrap_value_int32},
    {"set_uint32", wrap_value_set_uint32},
    {"uint32", wrap_value_uint32},
    {"set_int64", wrap_value_set_int64},
    {"int64", wrap_value_int64},
    {"set_uint64", wrap_value_set_uint64},
    {"uint64", wrap_value_uint64},
    {"set_pointer", wrap_value_set_pointer},
    {"pointer", wrap_value_pointer},
    {"set_float", wrap_value_set_float},
    {"float", wrap_value_float},
    {"set_double", wrap_value_set_double},
    {"double", wrap_value_double},
    {"set_str", wrap_value_set_str},
    {"str", wrap_value_str},
    {"is_null", wrap_value_is_null},
    {"int", wrap_value_int},
    {"copy", wrap_value_copy},
    {"destroy", wrap_value_destroy},
    {"_set", wrap_value_t_set_prop},
    {"_get", wrap_value_t_get_prop},
    {NULL, NULL}
  };
  luaL_newmetatable(L, "lftk.value_t");
  lua_pushstring(L, "__index");
  lua_pushvalue(L, -2);
  lua_settable(L, -3);
  luaL_openlib(L, NULL, member_funcs, 0);
  luaL_openlib(L, "value_t", static_funcs, 0);
}


void luaL_openlftk(lua_State* L) {
  value_type_t_init(L);
  value_t_init(L);
}
