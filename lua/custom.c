typedef struct _userdata_info_t {
  const char* info;
  void* data;
}userdata_info_t;

static lua_State* s_current_L = NULL;
extern void luaL_openlib (lua_State *L, const char *libname, const luaL_Reg *l, int nup);

static int lftk_newuserdata(lua_State* L, void* data, const char* info, const char* metatable) {
  userdata_info_t* udata = NULL;
  return_value_if_fail(data != NULL, 0);

  udata = (userdata_info_t*)lua_newuserdata(L, sizeof(userdata_info_t));
  return_value_if_fail(data != NULL, 0);

  udata->data = data;
  udata->info = info;

  if(metatable != NULL) {
    luaL_getmetatable(L, metatable);
    lua_setmetatable(L, -2);
  }

  return 1;
}

static const luaL_Reg* find_member(const luaL_Reg* funcs, const char* name) {
  const luaL_Reg* iter = funcs;

  while(iter->name) {
    if(*iter->name == *name && strcmp(iter->name, name) == 0) {
      return iter;
    }   
    iter++;
  }

  return NULL;
}

static void* lftk_checkudata(lua_State* L, int idx, const char* name) {
  userdata_info_t* udata = (userdata_info_t*)lua_touserdata(L, idx);
  if(udata) {
    assert(strstr(udata->info, name) != NULL);
    return udata->data;
  } else {
    return NULL;
  }
}

static ret_t call_on_event(void* ctx, event_t* e) {
  lua_State* L = (lua_State*)s_current_L;
  int func_id = (char*)ctx - (char*)NULL;

  lua_settop(L, 0);
  lua_rawgeti(L, LUA_REGISTRYINDEX, func_id);
  lftk_newuserdata(L, e, "event_t", NULL);

  lua_pcall(L,1,1,0);

  return RET_OK;
}

static int wrap_widget_on(lua_State* L) {
  ret_t ret = 0;
  widget_t* widget = (widget_t*)lftk_checkudata(L, 1, "widget_t");
  event_type_t type = (event_type_t)luaL_checkinteger(L, 2);

  if(lua_isfunction(L, 3)) {
    int func_id = 0;
    lua_pushvalue(L, 3); 
    func_id = luaL_ref(L, LUA_REGISTRYINDEX);
    ret = (ret_t)widget_on(widget, type, call_on_event, (char*)NULL + func_id);
    lua_pushnumber(L,(lua_Number)ret);

    return 1;
  } else {
    return 0;
  }
}

static int wrap_widget_off(lua_State* L) {
  ret_t ret = 0;
  widget_t* widget = (widget_t*)lftk_checkudata(L, 1, "widget_t");
  uint32_t id = (uint32_t)luaL_checkinteger(L, 2);
  emitter_item_t* item = emitter_find(widget->emitter, id);

  if(item) {
    uint32_t func_id = (char*)(item->ctx) - (char*)NULL;
    luaL_unref(L, LUA_REGISTRYINDEX, func_id);
    ret = (ret_t)widget_off(widget, id);
  }

  lua_pushnumber(L,(lua_Number)(ret));

  return 1;
}

static int to_wstr(lua_State* L) {
  const char* str = (const char*)luaL_checkstring(L, 1); 
  uint32_t size = (strlen(str) + 1) * sizeof(wchar_t);
  wchar_t* p = (wchar_t*)lua_newuserdata(L, size);

  utf8_to_utf16(str, p, size);
  lua_pushlightuserdata(L, p); 

  return 1;
}

static int to_str(lua_State* L) {
  const wchar_t* str = (const wchar_t*)lua_touserdata(L, 1); 
  uint32_t size = (wcslen(str) + 1) * 3;
  char* p = (char*)lua_newuserdata(L, size);

  utf8_from_utf16(str, p, size);
  lua_pushstring(L, p); 

  return 1;
}

