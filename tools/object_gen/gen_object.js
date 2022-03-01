const fs = require('fs')
const path = require('path')
const fse = require('fs-extra')

class CodeGen {
  genPropDecls(json) {
    let result = '';
    if (!json.props || !json.props.length) {
      return result;
    }

    result = json.props.map(iter => {
      let propDecl = `
  /**
   * @property {${iter.type}} ${iter.name}
   * @annotation ["readable"]
   * ${iter.desc}
   */
  ${iter.type} ${iter.name};
`
      return propDecl;
    }).join('');

    return result;
  }

  genPropSetterDecls(json) {
    let result = '';
    if (!json.props || !json.props.length) {
      return result;
    }

    const className = json.name;
    result = json.props.map(iter => {
      return `
/**
 * @method ${className}_set_${iter.name}
 * 设置 ${iter.desc}
 * @param {${className}_t*} ${className} ${className}对象。
 * @param {${this.mapType(iter.type)}} ${iter.name} ${iter.desc}
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ${className}_set_${iter.name}(${className}_t* ${className}, ${this.genParamDecl(iter.type, iter.name)});
`
    }).join('');

    return result;
  }

  genPropDefines(json) {
    let result = '';
    if (!json.props || !json.props.length) {
      return result;
    }

    const uclassName = json.name.toUpperCase();

    result = json.props.map(iter => {
      const uPropName = iter.name.toUpperCase();
      return `#define ${uclassName}_PROP_${uPropName} "${iter.name}"`
    }).join('\n');

    return result;
  }

  genAssignValue(type, name, value) {
    if (type.indexOf("char*") >= 0) {
      return `${name} = tk_str_copy(${name}, ${value});`;
    } else {
      return `${name} = ${value};`;
    }
  }

  mapType(type) {
    if (type.indexOf("char*") >= 0) {
      return `const char*`;
    } else {
      return type;
    }
  }

  genParamDecl(type, name) {
    return `${this.mapType(type)} ${name}`;
  }

  genPropSetterImpls(json) {
    let result = '';
    if (!json.props || !json.props.length) {
      return result;
    }

    const className = json.name;
    const uclassName = className.toUpperCase();
    result = json.props.map(iter => {
      let type = `${iter.type}`;
      const paramDecl = this.genParamDecl(iter.type, iter.name);
      const assignValue = this.genAssignValue(iter.type, `${className}->${iter.name}`, iter.name);

      let propSetter = `
ret_t ${className}_set_${iter.name}(${className}_t* ${className}, ${paramDecl}) {
  return_value_if_fail(${className} != NULL, RET_BAD_PARAMS);

  ${assignValue}

  return RET_OK;
}
`
      return propSetter;
    }).join('');

    return result;
  }

  genPropList(json) {
    let result = '';
    if (!json.props || !json.props.length) {
      return result;
    }

    const uclassName = json.name.toUpperCase();

    result = json.props.map(iter => {
      const uPropName = iter.name.toUpperCase();
      return `  ${uclassName}_PROP_${uPropName}`
    }).join(',\n');

    return result;
  }

  genToValue(clsName, type, name) {
    switch (type) {
      case 'int8_t':
      case 'int16_t':
      case 'int32_t':
      case 'int64_t':
      case 'uint8_t':
      case 'uint16_t':
      case 'uint32_t':
      case 'uint64_t':
      case 'bool_t':
      case 'float_t':
      case 'float':
      case 'double': {
        let typeName = type.replace(/_t$/, '');
        return `value_set_${typeName}(v, ${clsName}->${name});`;
      }
      case 'char*': {
        return `value_set_str(v, ${clsName}->${name});`;
      }
      case 'void*': {
        return `value_set_pointer(v, ${clsName}->${name});`;
      }
      case 'color_t': {
        return `value_set_uint32(v, ${clsName}->${name}.color);`;
      }
      default: {
        console.log(`not supported ${type} for ${name}`);
        process.exit(0);
      }
    }
  }

  genFromValue(clsName, type, name) {
    let typeName = '';
    switch (type) {
      case 'int8_t':
      case 'int16_t':
      case 'int32_t':
      case 'int64_t':
      case 'uint8_t':
      case 'uint16_t':
      case 'uint32_t':
      case 'uint64_t':
      case 'bool_t':
      case 'float_t':
      case 'float':
      case 'double': {
        typeName = type.replace(/_t$/, '');
        break;
      }
      case 'color_t': {
        return `${clsName}_set_${name}(${clsName}, color_parse(value_str(v)));`;
        break;
      }
      case 'char*': {
        typeName = 'str';
        break;
      }
      case 'void*': {
        typeName = 'pointer';
        break;
      }
      default: {
        console.log(`not supported ${type} for ${name}`);
        process.exit(0);
      }
    }
    return `${clsName}_set_${name}(${clsName}, value_${typeName}(v));`;
  }

  genSetPropDispatch(json) {
    let result = '';
    if (!json.props || !json.props.length) {
      return result;
    }

    const className = json.name;
    const uclassName = className.toUpperCase();

    result = json.props.map((iter, index) => {
      const uPropName = iter.name.toUpperCase();
      const propMacro = `${uclassName}_PROP_${uPropName}`
      let setProp = '';
      if (index) {
        setProp = `\n  } else if (tk_str_eq(${propMacro}, name)) {\n`
      } else {
        setProp = `  if (tk_str_eq(${propMacro}, name)) {\n`
      }
      setProp += `     ${this.genFromValue(className, iter.type, iter.name)}\n`;
      setProp += '     return RET_OK;';
      return setProp;
    }).join('');

    result += '\n  }';

    return result;
  }

  genPropInits(json) {
    let result = '';
    if (!json.props || !json.props.length) {
      return result;
    }

    const className = json.name;
    result = json.props.map((iter) => {
      if (iter.init_value) {
        if (iter.type === 'char*' && iter.init_value != 'NULL') {
          return `  ${className}->${iter.name} = tk_strdup("${iter.init_value}");\n`;
        } else {
          return `  ${className}->${iter.name} = ${iter.init_value};\n`;
        }
      }
    }).join('');

    return result;
  }

  genFreeProps(json) {
    let result = '';
    const className = json.name;
    if (!json.props || !json.props.length) {
      return result;
    }

    result = json.props.map((iter, index) => {
      if (iter.type.indexOf('char*') >= 0) {
        return `  TKMEM_FREE(${className}->${iter.name});\n`;
      } else {
        return '';
      }
    }).join('');

    return result;
  }

  genGetPropDispatch(json) {
    let result = '';
    if (!json.props || !json.props.length) {
      return result;
    }

    const className = json.name;
    const uclassName = className.toUpperCase();

    result = json.props.map((iter, index) => {
      const uPropName = iter.name.toUpperCase();
      const propMacro = `${uclassName}_PROP_${uPropName}`
      let setProp = '';
      if (index) {
        setProp = `\n  } else if (tk_str_eq(${propMacro}, name)) {\n`
      } else {
        setProp = `  if (tk_str_eq(${propMacro}, name)) {\n`
      }
      setProp += `     ${this.genToValue(className, iter.type, iter.name)}\n`;
      setProp += '     return RET_OK;';
      return setProp;
    }).join('');

    result += '\n  }';

    return result;
  }

  genH(json) {
    let parent = "";
    let includes = '#include "tkc/object.h"\n';
    let parentAnnotation = "";
    const className = json.name;
    const uclassName = json.name.toUpperCase();

    if (json.parent) {
      parent = `${json.parent}_t ${json.parent};`
      parentAnnotation = `@parent ${json.parent}`
      if (json.parent != 'object') {
        includes = `#include "${json.parent}.h"\n`
      }
    }

    if (json.includes) {
      json.includes.forEach(iter => {
        includes += `#include "${iter}"\n`
      });
    }
    return `
/**
 * File:   ${className}.h
 * Author: AWTK Develop Team
 * Brief:  ${json.desc}
 *
 * Copyright (c) ${json.copyright}
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * ${json.date} ${json.author} created
 *
 */
#ifndef TK_${uclassName}_H
#define TK_${uclassName}_H

${includes}

BEGIN_C_DECLS

/**
 * @class ${className}_t 
 * ${parentAnnotation}
 * ${json.desc}
 */
typedef struct _${className}_t {
  ${parent}
  ${this.genPropDecls(json)}
} ${className}_t;

${this.genPropSetterDecls(json)}

${this.genPropDefines(json)}

/*public for subclass*/
/**
 * @method ${className}_deinit
 * ~初始化对象。
 * @param {${className}_t*} ${className} ${className}对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ${className}_deinit(${className}_t* ${className});

/**
 * @method ${className}_init
 * 初始化对象。
 * @param {${className}_t*} ${className} ${className}对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
${className}_t* ${className}_init(${className}_t* ${className});

/**
 * @method ${className}_set_prop
 * 设置属性。
 * @param {${className}_t*} ${className} ${className}对象。
 * @param {const char*} name 属性名。
 * @param {const value_t*} v 属性值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ${className}_set_prop(${className}_t* ${className}, const char* name, const value_t* v);

/**
 * @method ${className}_get_prop
 * 获取属性。
 * @param {${className}_t*} ${className} ${className}对象。
 * @param {const char*} name 属性名。
 * @param {value_t*} v 属性值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ${className}_get_prop(${className}_t* ${className}, const char* name, value_t* v);

/**
 * @method ${className}_cast
 * 类型转换。
 * @param {${className}_t*} ${className} ${className}对象。
 *
 * @return {${className}_t*} 返回非NULL表示成功，否则表示失败。
 */
${className}_t* ${className}_cast(object_t* obj);

/**
 * @method ${className}_create
 * 创建对象。
 * @return {${className}_t*} 返回非NULL表示成功，否则表示失败。
 */
${className}_t* ${className}_create(void);

#define ${uclassName}(obj) ${className}_cast(OBJECT(obj))

END_C_DECLS

#endif/*TK_${uclassName}_H*/
`
  }

  genC(json) {
    let parent = "";
    let parentUpper = "";
    let parentSetProp = "";
    let parentGetProp = "";
    let parentInit = "";
    let parentDeinit = "";
    const className = json.name;
    const uclassName = json.name.toUpperCase();

    if (json.parent) {
      parent = json.parent;
      parentUpper = parent.toUpperCase();

      if(parent !== 'object') {
        parentSetProp = `
    if(${parent}_set_prop(${parentUpper}(${className}), name, v) == RET_OK) {
      return RET_OK;
    }`
        parentGetProp = `
    if(${parent}_get_prop(${parentUpper}(${className}), name, v) == RET_OK) {
      return RET_OK;
    }`
        parentInit = `  ${parent}_init(${parentUpper}(${className}));`;
        parentDeinit = `  ${parent}_deinit(${parentUpper}(${className}));`;
      }
    }
    return `
/**
 * File:   ${className}.c
 * Author: AWTK Develop Team
 * Brief:  ${json.desc}
 *
 * Copyright (c) ${json.copyright}
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * ${json.date} ${json.author} created
 *
 */

#include "tkc/mem.h"
#include "tkc/str.h"
#include "tkc/utils.h"
#include "${className}.h"

${this.genPropSetterImpls(json)}

ret_t ${className}_set_prop(${className}_t* ${className}, const char* name, const value_t* v) {
  return_value_if_fail(${className} != NULL && name != NULL && v != NULL , RET_BAD_PARAMS);
${parentSetProp}
${this.genSetPropDispatch(json)}

  return RET_NOT_FOUND;
}

static ret_t ${className}_set_prop_obj(object_t* obj, const char* name, const value_t* v) {
  ${className}_t* ${className} = ${uclassName}(obj);
  return_value_if_fail(${className} != NULL && name != NULL && v != NULL , RET_BAD_PARAMS);

  return ${className}_set_prop(${className}, name, v);
}

ret_t ${className}_get_prop(${className}_t* ${className}, const char* name, value_t* v) {
  return_value_if_fail(${className} != NULL && name != NULL && v != NULL , RET_BAD_PARAMS);

${parentGetProp}
${this.genGetPropDispatch(json)}

  return RET_NOT_FOUND;
}

static ret_t ${className}_get_prop_obj(object_t* obj, const char* name, value_t* v) {
  ${className}_t* ${className} = ${uclassName}(obj);
  return_value_if_fail(${className} != NULL && name != NULL && v != NULL , RET_BAD_PARAMS);

  return ${className}_get_prop(${className}, name, v);
}

ret_t ${className}_deinit(${className}_t* ${className}) {
  return_value_if_fail(${className} != NULL, RET_BAD_PARAMS);
${this.genFreeProps(json)}
${parentDeinit}

  return RET_OK;
}

static ret_t ${className}_on_destroy(object_t* obj) {
  return ${className}_deinit(${uclassName}(obj));
}

static const object_vtable_t s_${className}_vtable = {.type = "${className}",
                                                        .desc = "${className}",
                                                        .size = sizeof(${className}_t),
                                                        .is_collection = FALSE,
                                                        .on_destroy = ${className}_on_destroy,
                                                        .get_prop = ${className}_get_prop_obj,
                                                        .set_prop = ${className}_set_prop_obj};

${className}_t* ${className}_init(${className}_t* ${className}) {
  return_value_if_fail(${className} != NULL, NULL);

${parentInit}
${this.genPropInits(json)}

  return ${className};
}

${className}_t* ${className}_create(void) {
  object_t* o = object_create(&s_${className}_vtable);
  return_value_if_fail(o != NULL, NULL);

  return ${className}_init(${uclassName}(o));
}

${className}_t* ${className}_cast(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_${className}_vtable, NULL);

  return (${className}_t*)(obj);
}
`
  }

  genJson(json) {
    const header = this.genH(json);
    const content = this.genC(json);
    const headerFileName = `${json.name}.h`
    const contentFileName = `${json.name}.c`

    fs.writeFileSync(headerFileName, '\ufeff' + header);
    fs.writeFileSync(contentFileName, '\ufeff' + content);

    console.log(`output to ${headerFileName} and ${contentFileName}`);

  }

  genFile(filename) {
    this.genJson(JSON.parse(fs.readFileSync(filename).toString()));
  }

  static run(sourceIDL) {
    const gen = new CodeGen();
    gen.genFile(sourceIDL);
  }
}

if (process.argv.length < 3) {
  console.log(`Usage: node index.js foo.json`);
  process.exit(0);
}

CodeGen.run(process.argv[2], process.argv[3], process.argv[4]);

