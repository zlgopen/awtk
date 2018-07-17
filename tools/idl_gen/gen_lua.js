const fs = require('fs');

const builtin = `#include "custom.c"\n\n`;

function genAll(json) {
  function toLuaClassName(name) {
    name = name.replace(/_t$/, '');
    name = name.replace(/(^|_)[a-z]/g, r => {
      if (r.length > 1) {
        r = r.substr(1);
      }

      return r.toUpperCase();
    });

    return name;
  }

  function getClassInfo(name) {
    for (let i = 0; i < json.length; i++) {
      let iter = json[i];
      if (iter.type === 'class' && iter.name === name) {
        return iter;
      }
    }

    return null;
  }

  function genClassChain(name) {
    let str = '';
    let info = getClassInfo(name);

    while (info) {
      str += '/' + info.name;
      if (!info.parent) {
        break;
      }
      info = getClassInfo(info.parent);
    }

    return str;
  }

  function genDecl(index, type, name) {
    let str = '';
    str += `  ${type} ${name} = `;
    if (index < 0) {
      if (type.indexOf('*') >= 0) {
        str += 'NULL;\n';
      } else {
        str += '0;\n';
      }
    } else {
      if (type.indexOf('char*') >= 0) {
        str += `(${type})luaL_checkstring(L, ${index+1});\n`;
      } else if (type.indexOf('wchar_t*') >= 0) {
        str += `(${type})lua_touserdata(L, ${index+1});\n`;
      } else if (type.indexOf('void*') >= 0) {
        if (name !== 'ctx') {
          str += `(${type})lua_touserdata(L, ${index+1});\n`;
        } else {
          str += ' NULL;\n';
        }
      } else if (type.indexOf('*') >= 0) {
        const type_name = type.replace(/\*/g, '');
        str += `(${type})tk_checkudata(L, ${index+1}, "${type_name}");\n`;
      } else if (type.indexOf('float') >= 0 || type.indexOf('double') >= 0) {
        str += `(${type})luaL_checknumber(L, ${index+1});\n`;
      } else if (type.indexOf('bool_t') >= 0) {
        str += `(${type})lua_toboolean(L, ${index+1});\n`;
      } else if (type.indexOf('func_t') >= 0) {
        str += `(${type})lua_tocfunction(L, ${index+1});\n`;
      } else {
        str += `(${type})luaL_checkinteger(L, ${index+1});\n`;
      }
    }

    return str;
  }

  function genParamsDecl(m) {
    let str = '';
    if (m.return != 'void') {
      str = genDecl(-1, m.return, 'ret');
    }

    m.params.forEach((iter, index) => {
      str += genDecl(index, iter.type, iter.name);
    })

    return str;
  }

  function genReturnData(type, name) {
    let str = '';
    if (type.indexOf('char*') >= 0) {
      str = `  lua_pushstring(L,(char*)(${name}));\n\n`;
      str += '  return 1;\n';
    } else if (type.indexOf('wchar_t*') >= 0) {
      str = `  lua_pushlightuserdata(L,(void*)(${name}));\n\n`;
      str += '  return 1;\n';
    } else if (type.indexOf('*') >= 0) {
      const typeName = type.replace(/\*/g, "");
      str += `  return tk_newuserdata(L, (void*)${name}, "${genClassChain(typeName)}", "awtk.${typeName}");\n`;
    } else if (type.indexOf('int') >= 0) {
      str = `  lua_pushinteger(L,(lua_Integer)(${name}));\n\n`;
      str += '  return 1;\n';
    } else if (type.indexOf('bool_t') >= 0) {
      str = `  lua_pushboolean(L,(lua_Integer)(${name}));\n\n`;
      str += '  return 1;\n';
    } else {
      str = `  lua_pushnumber(L,(lua_Number)(${name}));\n\n`;
      str += '  return 1;\n';
    }

    return str;
  }

  function genCallMethod(cls, m) {
    const ret_type = m.return;
    let str = ret_type == 'void' ? '  ' : '  ret = '
    str += `(${ret_type})${m.name}(`;
    m.params.forEach((iter, index) => {
      if (index > 0) {
        str += ', ' + iter.name;
      } else {
        str += iter.name;
      }
    })

    str += ');\n';

    str += '\n';
    if (ret_type == 'void') {
      str += '  return 0;\n';
    } else {
      if (m.isConstructor) {
        str += genReturnData(`${cls.name}*`, 'ret');
      } else {
        str += genReturnData(ret_type, 'ret');
      }
    }

    return str;
  }

  function genMethod(cls, m) {
    const args_nr = m.params.length;
    let str = `static int wrap_${m.name}(lua_State* L) {\n`;
    str += genParamsDecl(m);
    str += genCallMethod(cls, m);
    str += '}\n\n';

    return str;
  }

  function genSetProperty(index, cls, p) {
    let str = '';
    if (index === 0) {
      str += `  if(strcmp(name, "${p.name}") == 0) {\n`;
    } else {
      str += `  else if(strcmp(name, "${p.name}") == 0) {\n`;
    }
    if (p.readonly) {
      str += `    printf("${p.name} is readonly\\n");\n`;
      str += `    return 0;\n`;
    } else {
      str += genDecl(2, p.type, p.name);
      str += `    obj->${p.name} = ${p.name};\n`;
    }
    str += '  }\n';

    return str;
  }

  function genGetProperty(index, cls, p) {
    let str = '';
    if (index === 0) {
      str += `  if(strcmp(name, "${p.name}") == 0) {\n`;
    } else {
      str += `  else if(strcmp(name, "${p.name}") == 0) {\n`;
    }
    str += '  ';
    str += genReturnData(p.type, `obj->${p.name}`);
    str += '  }\n';

    return str;
  }

  function methodToShortName(clsName, methodName) {
    return methodName.replace(clsName.replace(/_t$/, '') + "_", '')
  }

  function genSetProp(cls) {
    let str = '';
    const clsName = cls.name;

    str += `static int wrap_${clsName}_set_prop(lua_State* L) {\n`;
    str += genDecl(0, cls.name + '*', "obj");
    str += genDecl(1, "const char*", "name");
    str += '  (void)obj;\n';
    str += '  (void)name;\n';

    let hasSetProps = false;
    cls.properties.forEach((p, index) => {
      if(!p.readonly && !p.isFake) {
        str += genSetProperty(index, cls, p);
        hasSetProps = true;
      }
    });

    if (hasSetProps) {
      str += `  else {\n`;
    }
    if (cls.parent) {
      str += `  return wrap_${cls.parent}_set_prop(L);\n`;
    } else if (hasSetProps) {
      str += `    printf("%s: not supported %s\\n", __FUNCTION__, name);\n`;
      str += `    return 0;\n`;
    }
    if (hasSetProps) {
      str += `  }\n`;
    } else {
      str += `    printf("%s: not supported %s\\n", __FUNCTION__, name);\n`;
      str += `    return 0;\n`;
    }

    str += `}\n\n`;

    return str;
  }

  function genGetProp(cls) {
    let str = '';
    const clsName = cls.name;

    str += `static int wrap_${clsName}_get_prop(lua_State* L) {\n`;
    str += genDecl(0, cls.name + '*', "obj");
    str += genDecl(1, "const char*", "name");
    str += `  const luaL_Reg* ret = find_member(${cls.name}_member_funcs, name);\n\n`;

    str += '  (void)obj;\n';
    str += '  (void)name;\n';
    str += '  if(ret) {\n';
    str += '    lua_pushcfunction(L, ret->func);\n';
    str += '    return 1;\n';
    str += '  }\n';

    cls.properties.forEach((p, index) => {
      if(!p.isFake) {
        str += genGetProperty(index, cls, p);
      }
    });

    str += `  else {\n`;
    if (cls.parent) {
      str += `    return wrap_${cls.parent}_get_prop(L);\n`;
    } else {
      if(cls.name === 'widget_t') {
        str += `    widget_t* child = widget_lookup(obj, name, FALSE);\n`;
        str += `    if(child != NULL) {\n`;
        str += `      return tk_newuserdata(L, (void*)child, "/widget_t", "awtk.widget_t");\n`;
        str += `    }\n`;
      }
      str += `    printf("%s: not supported %s\\n", __FUNCTION__, name);\n`;
      str += `    return 0;\n`;
    }
    str += `  }\n`;

    str += `}\n\n`;

    return str;
  }

  function genClassInit(cls) {
    let str = '';
    const clsName = cls.name;

    str += `static void ${cls.name}_init(lua_State* L) {\n`;
    str += '  static const struct luaL_Reg static_funcs[] = {\n'
    cls.methods.forEach(m => {
      const name = methodToShortName(cls.name, m.name);
      if (m.isConstructor || m.isStatic) {
        str += `    {"${name}", wrap_${m.name}},\n`;
      }
    });

    str += `    {NULL, NULL}\n`;
    str += '  };\n\n'


    if(!cls.isFake) {
      str += '  static const struct luaL_Reg index_funcs[] = {\n'
      str += `    {"__index", wrap_${clsName}_get_prop},\n`;
      str += `    {"__newindex", wrap_${clsName}_set_prop},\n`;
      str += `    {NULL, NULL}\n`;
      str += '  };\n\n'

      str += `  luaL_newmetatable(L, "awtk.${cls.name}");\n`;
      str += `  lua_pushstring(L, "__index");\n`;
      str += '  lua_pushvalue(L, -2);\n';
      str += '  lua_settable(L, -3);\n';
      str += `  luaL_openlib(L, NULL, index_funcs, 0);\n`;
    }

    str += `  luaL_openlib(L, "${toLuaClassName(cls.name)}", static_funcs, 0);\n`;

    str += '  lua_settop(L, 0);\n';
    str += '}\n';

    return str;
  }

  function genMethods(cls) {
    let str = '';
    const clsName = cls.name;

    cls.methods.forEach(m => {
      if (!m.isPrivate && !m.isCustom) {
        str += genMethod(cls, m);
      }
    });

    if(!cls.isFake) {
      str += `\nstatic const struct luaL_Reg ${cls.name}_member_funcs[] = {\n`
      cls.methods.forEach(m => {
        const name = methodToShortName(cls.name, m.name);
        if (!m.isConstructor && !m.isStatic) {
          str += `  {"${name}", wrap_${m.name}},\n`;
        }
      });
      str += `  {NULL, NULL}\n`;
      str += '};\n\n'
    }

    return str;
  }

  function genClass(cls) {
    let str = '';

    str += genMethods(cls);
    if(!cls.isFake) {
      str += genSetProp(cls);
      str += genGetProp(cls);
    }
    str += genClassInit(cls);

    return str;
  }

  function genEnum(cls) {
    let str = `static void ${cls.name}_init(lua_State* L) {\n`;

    str += '  lua_newtable(L);\n';
    str += `  lua_setglobal(L, "${toLuaClassName(cls.name)}");\n`;
    str += `  lua_getglobal(L, "${toLuaClassName(cls.name)}");\n\n`;

    const clsNamePrefix = cls.prefix;
    cls.consts.forEach(iter => {
      const name = iter.name.replace(clsNamePrefix, "");

      str += `  lua_pushstring(L, "${name}");\n`
      str += `  lua_pushinteger(L, ${iter.name});\n`;
      str += `  lua_settable(L, -3); \n\n`;
    });

    str += '}\n\n';

    return str;
  }

  function genOne(cls) {
    if(!cls.scriptable) {
      return '';
    }

    if (cls.type == 'class') {
      return genClass(cls);
    } else if (cls.type == 'enum') {
      return genEnum(cls);
    } else {
      return '';
    }
  }

  function genIncludes(json) {
    let result = '/*XXX: generated by lua_gen. dont edit it.*/\n';

    result += '#include "lua/lua.h"\n';
    result += '#include "lua/lualib.h"\n';
    result += '#include "lua/lauxlib.h"\n';
    result += '#include "base/utf8.h"\n';
    json.forEach(iter => {
      if (result.indexOf(iter.header) <= 0) {
        result += `#include "${iter.header}"\n`;
      }
    });
    result += "\n";

    return result;
  }

  function genFuncDecls(json) {
    let result = '';

    json.forEach(iter => {
      if (iter.type == 'class') {
        const clsName = iter.name;
        if(!iter.isFake && iter.scriptable) {
          result += `static int wrap_${clsName}_get_prop(lua_State* L);\n`;
          result += `static int wrap_${clsName}_set_prop(lua_State* L);\n`;
        }
      }
    });
    result += '\n';

    return result;
  }

  function genInit(json) {
    let result = '';

    result += `\nvoid luaL_openawtk(lua_State* L) {\n`;
    result += `  globals_init(L);\n`;
    json.forEach(iter => {
      if (iter.type === 'class' || iter.type === 'enum') {
        if(iter.scriptable) {
          result += `  ${iter.name}_init(L);\n`;
        }
      }
    });
    result += '  s_current_L = L;\n';
    result += '}\n';

    return result;
  }

  function genGlobals(json) {
    let str = '';

    json.forEach(iter => {
      if (iter.type == 'method') {
        str += genMethod({}, iter);
      }
    });

    str += 'static void globals_init(lua_State* L) {\n';

    json.forEach(iter => {
      if (iter.type == 'method') {
        str += `  lua_pushcfunction(L, wrap_${iter.name});\n`;
        str += `  lua_setglobal(L, "${iter.name}");\n`;
      } else if (iter.type == 'const') {
        str += `  lua_pushinteger(L, ${iter.name});\n`;
        str += `  lua_setglobal(L, "${iter.name}");\n`;
      }
    });

    str += '  lua_pushcfunction(L, to_str);\n';
    str += '  lua_setglobal(L, "to_str");\n';
    str += '  lua_pushcfunction(L, to_wstr);\n';
    str += '  lua_setglobal(L, "to_wstr");\n';

    str += '}\n\n';

    return str;
  }

  let result = genIncludes(json);

  result += builtin;
  result += genFuncDecls(json);
  result += genGlobals(json);
  json.forEach(iter => {
    result += genOne(iter);
  });

  result += genInit(json);

  return result;
}

function run() {
  fs.writeFileSync('../../lua/tk_lua.c', genAll(JSON.parse(fs.readFileSync('idl.json').toString())));
}

run();
