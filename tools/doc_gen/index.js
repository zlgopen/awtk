const fs = require('fs')

const graphviz_default_style=`
    rankdir  = BT
    fontname = "Courier New"
    fontsize = 12

    node [
        fontname = "Courier New"
        fontsize = 12
        shape    = "record"
        width = 0.4
    ]
`

function toBool(v) {
  return v ? "是" : "否";
}

function isScriptable(obj) {
  return obj.annotation && obj.annotation.scriptable;
}

function isFake(obj) {
  return obj.annotation && obj.annotation.fake;
}

function isStatic(obj) {
  return obj.annotation && obj.annotation.static;
}

function isPersitent(obj) {
  return obj.annotation && obj.annotation.persitent;
}

function isDesign(obj) {
  return obj.annotation && obj.annotation.design;
}

function isReadable(obj) {
  return obj.annotation && obj.annotation.readable;
}

function isWritable(obj) {
  return obj.annotation && obj.annotation.writable;
}

function isGetProp(obj) {
  return obj.annotation && obj.annotation.get_prop;
}

function isSetProp(obj) {
  return obj.annotation && obj.annotation.set_prop;
}

function isPrivate(obj) {
  return obj.annotation && obj.annotation.private;
}

function isCustom(obj) {
  return obj.annotation && obj.annotation.scriptable == 'custom';
}

function isConstructor(obj) {
  return obj.annotation && obj.annotation.constructor === true;
}

function isCast(obj) {
  return obj.annotation && obj.annotation.cast
}

function isEnumString(obj) {
  return obj.annotation && obj.annotation.string === true;
}

class ApiGenerator {
  genOneFunc(cls, m) {
    let result = '';
    this.graphviz_index = 0;

    return result;
  }
  
  genOneConst(cls, c) {
    let result = '';

    return result;
  }
  
  genOneEvent(cls, e) {
    let result = '';

    return result;
  }

  encodeStr(str) { 
    return str.replace(/_/g, "\\_");
  }

  preprocess(str, clsName) {
    let result = '';
    let start = 0;
    let end = 0;

    while(true) {
      let last_start = start;
      start = str.indexOf('digraph G', last_start);
      if(start < 0) {
        break;
      }
      end = str.indexOf('}\n', start);
      if(end < 0) {
        break;
      }
      end += 3;

      let name = `${clsName}_${this.graphviz_index++}`;
      let filename = `dots/${name}.dot`;
      let dot_str = str.substr(start, end - start);

      dot_str = dot_str.replace(/\[default_style\]/g, graphviz_default_style);

      fs.writeFileSync(filename, dot_str);
      
      result += this.encodeStr(str.substr(last_start, start-last_start));
      result += `![image](images/${name}.png)\n`

      start = end;
    }

    result += this.encodeStr(str.substr(end));

    return result;
  }

  writeResult(filename, clsName, result) {
    let str = this.preprocess(result, clsName);

    fs.writeFileSync(filename, str);
  }

  genOneClass(cls) {
    let result = `## ${cls.name}\n`;
    
    result += `### 概述\n`;

    result += cls.desc;

    this.clsName = cls.name;
    this.graphviz_index = 0;

    if (cls.methods) {
      result += `### 函数\n`
      result += '| 名称 | 说明 | \n';
      result += '| -------- | ------------ | \n';

      cls.methods.sort((a, b) => {
        return a.name.localeCompare(b.name);
      });

      cls.methods.forEach((iter) => {
        result += `| <a href="#${iter.name}">${iter.name}</a> | ${iter.desc.split('\n')[0]} |\n`;
      });
      cls.methods.forEach(iter => {
        result += this.genOneFunc(cls, iter);
      });
    }

    if (cls.properties) {
      result += `### 属性\n`
      result += '| 名称 | 类型 | 说明 | \n';
      result += '| -------- | ----- | ------------ | \n';

      cls.properties.sort((a, b) => {
        return a.name.localeCompare(b.name);
      });

      cls.properties.forEach((p) => {
        result += `| <a href="#${p.name}">${p.name}</a> | ${p.type} | ${p.desc.split('\n')[0]} |\n`;
      });

      cls.properties.forEach((p) => {
        result += this.genOneProperty(cls, p);
      });
    }

    if (cls.consts) {
      result += `### 常量\n`
      cls.consts.forEach(iter => {
        result += this.genOneConst(cls, iter);
      });
    }
    
    if (cls.events) {
      result += `### 事件\n`
      cls.events.forEach(iter => {
        result += this.genOneEvent(cls, iter);
      });
    }
    
    this.writeResult(`docs/${cls.name}.md`, cls.name, result);

    return ;
  }


  genOneProperty(cls, p) {
    let result = `#### ${p.name} 属性\n`;
    result += `-----------------------\n`;
    result += `<p id="${p.name}">${p.desc}\n\n`;
    result += `* 类型：${p.type}\n\n`;

    result += `| 特性 | 是否支持 |\n`;
    result += `| -------- | ----- |\n`;
    result += `| 可直接读取 | ${toBool(isReadable(p))} |\n`;
    result += `| 可直接修改 | ${toBool(isWritable(p))} |\n`;
    result += `| 可持久化   | ${toBool(isPersitent(p))} |\n`;
    result += `| 可脚本化   | ${toBool(isScriptable(p))} |\n`;
    result += `| 可在IDE中设置 | ${toBool(isDesign(p))} |\n`;
    result += `| 可在XML中设置 | ${toBool(isGetProp(p))} |\n`;
    result += `| 支通过widget_get_prop读取 | ${toBool(isGetProp(p))} |\n`;
    result += `| 支通过widget_set_prop修改 | ${toBool(isSetProp(p))} |\n`;

    return result;
  }

  genOneEnum(cls) {
    let result = '';

    if (cls.consts) {
    }
    
    return result;
  }

  genOne(cls) {
    if (cls.type === 'class') {
      return this.genOneClass(cls);
    } else if(cls.type === 'enum') {
      return this.genOneEnum(cls);
    } else {
    }
  }

  genJsonAll(json) {
    json.forEach(iter => {
      this.genOne(iter);
    });
  }

  genAll(filename) {
    this.genJsonAll(JSON.parse(fs.readFileSync(filename).toString()));
  }

  static gen() {
    const gen = new ApiGenerator();
    const input = '../idl_gen/idl.json';

    gen.genAll(input);
  }
}

ApiGenerator.gen();
