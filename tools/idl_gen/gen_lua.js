const fs = require('fs');

/*https://www.codingnow.com/2000/download/lua_manual.html*/
/*http://book.luaer.cn/_3.htm*/
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
      str += `(${type})luaL_checkstring(L,${index+1});\n`;
    } else if (type.indexOf('*') >= 0) {
      const type_name = type.replace(/\*/g, '');
      str += `(${type})luaL_checkudata(L, ${index+1}, "lftk.${type_name}");\n`;
    } else if (type.indexOf('int') >= 0) {
      str += `(${type})luaL_checkinteger(L,${index+1});\n`;
    } else if (type.indexOf('bool_t') >= 0) {
      str += `(${type})lua_toboolean(L, ${index+1});\n`;
    } else {
      str += `(${type})luaL_checknumber(L,${index+1});\n`;
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
    str = `  lua_pushstring(L,(char*)(${name}));\n`;
  } else if (type.indexOf('*') >= 0) {
    str = `  lua_pushlightuserdata(L,(void*)(${name}));\n`;
  } else if (type.indexOf('int') >= 0) {
    str = `  lua_pushinteger(L,(lua_Integer)(${name}));\n`;
  } else if (type.indexOf('bool_t') >= 0) {
    str = `  lua_pushboolean(L,(lua_Integer)(${name}));\n`;
  } else {
    str = `  lua_pushnumber(L,(lua_Number)(${name}));\n`;
  }

  return str;
}

function genMethodCall(cls, m) {
  const ret_type = m.return;
  let str = ret_type == 'void' ? '  ' : '  ret = '
  str += `${m.name}(`;
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
    str += genReturnData(ret_type, 'ret');
    if (m.constructor) {
      str += `  luaL_getmetatable(L, "lftk.${cls.name}");\n`;
      str += `  lua_setmetatable(L, -2);\n`;
    }
    str += '\n';
    str += '  return 1;\n';
  }
  return str;
}

function genMethod(cls, m) {
  const args_nr = m.params.length;
  let str = `static int wrap_${m.name}(lua_State* L) {\n`;
  str += genParamsDecl(m);
  str += genMethodCall(cls, m);
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
  str += '  ';
  if (p.readonly) {
    str += `    printf("${p.name} is readonly\\n");\n`;
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

function genClass(cls) {
  let str = '';
  const clsName = cls.name;

  cls.methods.forEach(m => {
    str += genMethod(cls, m);
  });

  if (cls.properties.length > 0) {
    str += `static int wrap_${clsName}_set_prop(lua_State* L) {\n`;
    str += genDecl(0, cls.name + '*', "obj");
    str += genDecl(1, "const char*", "name");

    cls.properties.forEach((m, index) => {
      str += genSetProperty(index, cls, m);
    });
    str += `  else {\n`;
    str += `    printf("not supported %s\\n", name);\n`;
    str += `  }\n`;

    str += `  return 0;\n`;
    str += `}\n`;

    str += `static int wrap_${clsName}_get_prop(lua_State* L) {\n`;
    str += genDecl(0, cls.name + '*', "obj");
    str += genDecl(1, "const char*", "name");

    cls.properties.forEach((m, index) => {
      str += genGetProperty(index, cls, m);
    });

    str += `  else {\n`;
    str += `    printf("not supported %s\\n", name);\n`;
    str += `  }\n`;
    str += `  return 1;\n`;
    str += `}\n`;
  }

  str += `static void ${cls.name}_init(lua_State* L) {\n`;
  str += '  static const struct luaL_Reg static_funcs[] = {\n'
  cls.methods.forEach(m => {
    const name = methodToShortName(cls.name, m.name);
    if (m.isConstructor) {
      str += `    {"${name}", wrap_${m.name}},\n`;
    }
  });

  str += `    {NULL, NULL}\n`;
  str += '  };\n'

  str += '  static const struct luaL_Reg member_funcs[] = {\n'
  cls.methods.forEach(m => {
    const name = methodToShortName(cls.name, m.name);
    if (!m.isConstructor) {
      str += `    {"${name}", wrap_${m.name}},\n`;
    }
  });
  if (cls.properties.length > 0) {
    str += `    {"_set", wrap_${clsName}_set_prop},\n`;
    str += `    {"_get", wrap_${clsName}_get_prop},\n`;
  }

  str += `    {NULL, NULL}\n`;
  str += '  };\n'

  str += `  luaL_newmetatable(L, "lftk.${cls.name}");\n`;
  str += `  lua_pushstring(L, "__index");\n`;
  str += '  lua_pushvalue(L, -2);\n';
  str += '  lua_settable(L, -3);\n';
  str += `  luaL_openlib(L, NULL, member_funcs, 0);\n`;
  str += `  luaL_openlib(L, "${cls.name}", static_funcs, 0);\n`;

  str += '}\n\n';

  return str;
}

function genEnum(cls) {
  let str = `static void ${cls.name}_init(lua_State* L) {\n`;
  cls.consts.forEach(iter => {
    const name = iter.name;
    str += `  lua_pushinteger(L, ${name});\n`;
    str += `  lua_setglobal(L, "${name}");\n`;
  });
  str += '}\n\n';

  return str;
}

function genOne(cls) {
  if (cls.type == 'class') {
    return genClass(cls);
  } else if (cls.type == 'enum') {
    return genEnum(cls);
  } else {
    return '';
  }
}

function genAll() {
  let result = '#define LUA_COMPAT_MODULE\n';
  result += '#include "lua.h"\n';
  result += '#include "lauxlib.h"\n';

  const content = fs.readFileSync('idl.json').toString();
  const json = JSON.parse(content);

  json.forEach(iter => {
    if (result.indexOf(iter.header) <= 0) {
      result += `#include "${iter.header}"\n`;
    }
  });
  result += "\n";

  json.forEach(iter => {
    result += genOne(iter);
  });

  result += `\nvoid luaL_openlftk(lua_State* L) {\n`;
  json.forEach(iter => {
    result += `  ${iter.name}_init(L);\n`;
  });
  result += '}\n';

  fs.writeFileSync('../../lua/lftk_lua.c', result);
}

genAll();