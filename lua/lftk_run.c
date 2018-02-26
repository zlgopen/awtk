#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "base/mem.h"

extern void luaL_openlftk(lua_State* L);

int main(int argc, char* argv[]) {
  lua_State* L = luaL_newstate();
  static uint32_t s_mem_heap[10 * 1024];
  const char* lua_file = argc == 2 ? argv[1] : "./demo.lua";

  luaL_openlibs(L); 
  luaL_openlftk(L); 
  
  mem_init(s_mem_heap, sizeof(s_mem_heap));

  if(luaL_dofile(L, lua_file)) {
     fprintf(stderr, "%s\n", lua_tostring(L, -1));
     lua_pop(L, 1);  
  }

  lua_close(L);

  return 0;
}
