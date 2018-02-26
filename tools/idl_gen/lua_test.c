// aconf.cpp
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
  int main() {
      int var1 = 0, var2 = 0;
   
    lua_State* L = lua_open();
      luaL_openlibs(L);
      if (luaL_loadfile(L, "config.lua") || lua_pcall(L, 0, 0, 0))
    {
            printf("error: %s", lua_tostring(L, -1));
            return -1;
        
  }
   
    lua_getglobal(L, "var1");
      lua_getglobal(L, "var2");
      if (!lua_isnumber(L, -2)) {
            printf("`var1' should be a number\n");
            return -1;
        
  }
      if (!lua_isnumber(L, -1))
    {
            printf("`var2 should be a number\n");
            return -1;
        
  }
      var1 = (int)lua_tonumber(L, -2);
      var2 = (int)lua_tonumber(L, -1);
      printf("var1: %d\nvar2: %d\n", var1, var2);
      lua_close(L);
   
    return 0;
}
