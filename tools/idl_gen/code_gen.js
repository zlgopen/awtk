const fs = require('fs');

class CodeGen {
  constructor() {
    this.result = '';
    this.BOM = "\ufeff";
    this.classNamePrefix = 'T';
  }

  toClassName(name) {
    return this.classNamePrefix + this.upperCamelName(name);
  }

  typeToNativeName(type) {
    return type.replace("*", "").replace("const ", "").replace(" ", "");
  }

  typeToName(type) {
    let name = this.typeToNativeName(type);
    let cls = this.getClassOrEnumInfo(name);
    if (cls) {
      return this.toClassName(cls.name);
    } else {
      return null;
    }
  }

  isClassName(name) {
    return this.getClassInfo(this.typeToNativeName(name)) !== null;
  }

  isEnumName(name) {
    return this.getEnumInfo(name) !== null;
  }

  typeIsPointer(type) {
    return type.indexOf('*') > 0;
  }

  typeIsClassPointer(type) {
    return this.typeIsPointer(type) && this.getClassInfo(type) != null;
  }

  typeIsEnum(type) {
    return this.getClassInfo(type) != null;
  }

  typeIsFunction(type) {
    return type.indexOf('event_func_t') >= 0 ||
      type.indexOf('tk_visit_t') >= 0 ||
      type.indexOf('idle_func_t') >= 0 ||
      type.indexOf('timer_func_t') >= 0;
  }

  typeIsLongInteger(type) {
    return type.indexOf('int64') >= 0 || type.indexOf('long long') >= 0;
  }

  typeIsInteger(type) {
    return type.indexOf('int') >= 0 || type.indexOf('ret_t') >= 0 ||
      type.indexOf('wh_t') >= 0 || type.indexOf("xy_t") >= 0 ||
      type.indexOf('font_size_t') >= 0 || type.indexOf('long') >= 0;
  }

  typeIsFloat(type) {
    return type.indexOf('float') >= 0 || type.indexOf("double") >= 0;
  }

  typeIsNumber(type) {
    return this.typeIsInteger(type) || this.typeIsFloat(type);
  }

  typeIsBool(type) {
    return type.indexOf('bool_t') >= 0;
  }

  typeIsString(type) {
    return type.indexOf('char*') >= 0;
  }

  typeIsWString(type) {
    return type.indexOf('wchar_t*') >= 0;
  }

  camelCase(name) {
    if (name.indexOf('_') > 0) {
      name = name.replace(/(_)[a-z]/g, r => {
        if (r.length > 1) {
          r = r.substr(1);
        }

        return r.toUpperCase();
      });
    }

    return name;
  }

  upperCamelName(name) {
    name = name.replace(/_t$/, '');
    name = name.replace(/(^|_)[a-z]/g, r => {
      if (r.length > 1) {
        r = r.substr(1);
      }

      return r.toUpperCase();
    });

    return name;
  }

  isGcConstructor(obj) {
    return obj.annotation && (obj.annotation.constructor === true && obj.annotation.gc);
  }

  isGcDeconstructor(obj) {
    return obj.annotation && (obj.annotation.deconstructor === true && obj.annotation.gc);
  }

  isScriptable(obj) {
    return obj.annotation && obj.annotation.scriptable;
  }

  isFake(obj) {
    return obj.annotation && obj.annotation.fake;
  }

  isStatic(obj) {
    return obj.annotation && obj.annotation.static;
  }

  isReadable(obj) {
    return obj.annotation && obj.annotation.readable;
  }

  isWritable(obj) {
    return obj.annotation && obj.annotation.writable;
  }

  isPrivate(obj) {
    return obj.annotation && obj.annotation.private;
  }

  isCustom(obj) {
    return obj.annotation && obj.annotation.scriptable == 'custom';
  }

  isConstructor(obj) {
    return obj.annotation && obj.annotation.constructor === true;
  }

  isCast(obj) {
    return obj.annotation && obj.annotation.cast
  }

  isEnumString(obj) {
    return obj.annotation && obj.annotation.string === true;
  }

  isNormalMethod(m) {
    return !this.isCast(m) && !this.isStatic(m) && !this.isConstructor(m);
  }

  hasSetterFor(cls, name) {
    let clsName = cls.name.replace(/_t$/, '');
    let settter = clsName + '_set_' + name;

    return cls.methods.find(iter => (iter.name === settter));
  }
  
  getBaseClassInfo(cls) {
    if(cls && cls.parent) {
      return this.getBaseClassInfo(this.getClassInfo(cls.parent));
    } else {
      return cls;
    }
  }

  getClassInfo(name) {
    const json = this.json;

    for (let i = 0; i < json.length; i++) {
      let iter = json[i];
      if ((iter.type === 'class') && iter.name === name) {
        return iter;
      }
    }

    return null;
  }

  getEnumInfo(name) {
    const json = this.json;

    for (let i = 0; i < json.length; i++) {
      let iter = json[i];
      if ((iter.type === 'enum') && iter.name === name) {
        return iter;
      }
    }

    return null;
  }

  getClassOrEnumInfo(name) {
    const json = this.json;

    for (let i = 0; i < json.length; i++) {
      let iter = json[i];
      if ((iter.type === 'class' || iter.type == 'enum') && iter.name === name) {
        return iter;
      }
    }

    return null;
  }

  getClassName(cls) {
    return cls.alias || cls.name;
  }

  getParentClassName(cls) {
    return this.getClassName(this.getClassInfo(cls.parent));
  }

  filterScriptableJson(ojson) {
    let json = ojson.filter(this.isScriptable);

    json.forEach(iter => {
      if (iter.methods && iter.methods.length) {
        iter.methods = iter.methods.filter(this.isScriptable);
      }

      if (iter.properties && iter.properties.length) {
        iter.properties = iter.properties.filter(this.isScriptable);
      }
    })

    fs.writeFileSync('filter.json', JSON.stringify(json, null, '  '));
    this.json = json;

    return json;
  }

  genJsonAll(ojson) {}

  genOne(cls) {
    if (cls.type === 'class') {
      return this.genClass(cls);
    } else if (cls.type === 'enum') {
      return this.genEnum(cls);
    } else {
      return '';
    }
  }

  genAll(filename) {
    this.json = JSON.parse(fs.readFileSync(filename).toString());
    this.genJsonAll(this.json);
  }

  saveResult(filename) {
    fs.writeFileSync(filename, this.BOM + this.result);
    console.log(`write result to ${filename}`);
  }

  getJsonIDL() {
    return '../../../../awtk/tools/idl_gen/idl.json';
  }
}

module.exports = CodeGen;
