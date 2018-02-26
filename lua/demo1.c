#define LUA_COMPAT_MODULE
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

typedef struct _NumArray {
  int size;
  int values[1];
} NumArray;

static NumArray *checkarray (lua_State *L) {
  void *ud = luaL_checkudata(L, 1, "lftk.array");
  luaL_argcheck(L, ud != NULL, 1, "`array' expected");
  return (NumArray *)ud;
}

static int newarray(lua_State* L) {
  int n = luaL_checkint(L, 1);
  size_t nbytes = sizeof(NumArray) + (n - 1) * sizeof(double);
  NumArray* a = (NumArray*)calloc(1, nbytes);

  lua_pushlightuserdata(L, (void*)a);
  luaL_getmetatable(L, "lftk.array");
  lua_setmetatable(L, -2);

  a->size = n;

  return 1; /* new userdatum is already on the stack */
}

static int setarray(lua_State* L) {
  NumArray* a = checkarray(L);

  int index = luaL_checkint(L, 2);
  double value = luaL_checknumber(L, 3);
  luaL_argcheck(L, a != NULL, 1, "`array' expected");
  luaL_argcheck(L, 1 <= index && index <= a->size, 2, "index out of range");

  a->values[index - 1] = value;

  return 0;
}

static int getarray(lua_State* L) {
  NumArray* a = checkarray(L);

  int index = luaL_checkint(L, 2);
  luaL_argcheck(L, a != NULL, 1, "'array' expected");
  luaL_argcheck(L, 1 <= index && index <= a->size, 2, "index out of range");

  lua_pushnumber(L, a->values[index - 1]);

  return 1;
}

static int getsize(lua_State* L) {
  NumArray* a = checkarray(L);
  luaL_argcheck(L, a != NULL, 1, "`array' expected");

  lua_pushnumber(L, a->size);

  return 1;
}

static const struct luaL_Reg arraylib_f[] = {{"new", newarray}, {NULL, NULL}};

static const struct luaL_Reg arraylib_m[] = {
    {"set", setarray}, {"get", getarray}, {"size", getsize}, {NULL, NULL}};

int luaopen_array(lua_State* L) {
  luaL_newmetatable(L, "lftk.array");
  lua_pushstring(L, "__index");
  lua_pushvalue(L, -2); /* pushes the metatable */
  lua_settable(L, -3);  /* metatable.__index = metatable */

  luaL_openlib(L, NULL, arraylib_m, 0);
  luaL_openlib(L, "array", arraylib_f, 0);

  return 1;
}

int main(void) {
  lua_State* L = luaL_newstate();  // 创建Lua状态机。
  luaL_openlibs(L);                // 打开Lua状态机"L"中的所有Lua标准库。
  luaopen_array(L);

  if(luaL_dofile(L, "./demo1.lua")) {
     fprintf(stderr, "%s\n", lua_tostring(L, -1));
     lua_pop(L, 1);  
  }

  lua_close(L);  // 关闭Lua状态机。

  return 0;
}
