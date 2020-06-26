const fs = require('fs')
const path = require('path')

const graphviz_default_style = `
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

function assertEq(a, b) {
  if (a != b) {
    console.log(`${a} != ${b}`);
    process.exit(0);
  }
}

function genA(clsName, iter) {
  return `<a href="#${clsName}_${iter.name}">${encodeStr(iter.name)}</a>`
}

function genAnchor(clsName, iter) {
  return `<p id="${clsName}_${iter.name}">`;
}

function toBool(v) {
  return v ? "是" : "否";
}

function isScriptable(obj) {
  return obj.annotation && obj.annotation.scriptable;
}

function isMacro(obj) {
  return obj.annotation && obj.annotation.macro;
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

function encodeStr(str) {
  return str.replace(/_/g, "\\_");
}


class ApiGenerator {
  genFuncPrototype(p) {
    let result = '* 函数原型：\n\n';
    result += '```\n';
    console.log(p.name)
    result += `${p.return.type} ${p.name} (`;
    p.params.forEach((iter, index) => {
      if(index) {
        result += ', ';
      }
      result += `${iter.type} ${iter.name}`;
    })
    result += ');\n';
    result += '```\n\n';

    return result;
  }

  genOneFunc(cls, p) {
    let result = `#### ${encodeStr(p.name)} ${isMacro(p) ? "宏" : "函数"}\n`;
    result += `-----------------------\n\n`;
    result += '* 函数功能：\n\n';
    result += `> ${genAnchor(cls.name, p)}${p.desc}\n\n`;

    result += this.genFuncPrototype(p);
    result += '* 参数说明：\n\n';
    result += `| 参数 | 类型 | 说明 |\n`;
    result += `| -------- | ----- | --------- |\n`;
    result += `| 返回值 | ${encodeStr(p.return.type)} | ${encodeStr(p.return.desc)} |\n`;

    p.params.forEach(iter => {
      result += `| ${encodeStr(iter.name)} | ${encodeStr(iter.type)} | ${encodeStr(iter.desc)} |\n`;
    })

    return result;
  }

  split(str, type, stag, etag) {
    let result = [];
    let start = 0;
    let end = 0;

    while (true) {
      let last_start = start;

      start = str.indexOf(stag, last_start);
      if (start < 0) {
        break;
      }
      end = str.indexOf(etag, start + stag.length);
      if (end < 0) {
        break;
      }
      end += etag.length;

      if (start > last_start) {
        result.push({
          type: "text",
          data: str.substr(last_start, start - last_start)
        });
      }

      result.push({
        type: type,
        data: str.substr(start, end - start)
      });

      start = end;
    }

    if ((end + 1) < str.length) {
      result.push({
        type: "text",
        data: str.substr(end)
      });
    }

    return result;
  }

  splitStrs(arr, type, stag, etag) {
    let result = [];

    arr.forEach(iter => {
      if (iter.type == 'text') {
        result = result.concat(this.split(iter.data, type, stag, etag));
      } else {
        result.push(iter);
      }
    });

    return result;
  }

  splitAll(str) {
    let strs = [{
      type: "text",
      data: str
    }];

    strs = this.splitStrs(strs, 'code', '```', '```');

    return strs;
  }

  preprocessGraphviz(str, clsName) {
    let name = `${clsName}_${this.imageIndex++}`;
    let filename = `dots/${name}`;
    let dotStr = str.replace(/\[default_style\]/g, graphviz_default_style);

    fs.writeFileSync(filename, dotStr);

    return `![image](images/${name}.png)\n`
  }

  preprocessUML(str, clsName) {
    let name = `${clsName}_${this.imageIndex++}`;
    let filename = `umls/${name}.uml`;

    fs.writeFileSync(filename, str);

    return `![image](images/${name}.png)\n`
  }

  preprocess(str, clsName) {
    let strs = this.splitAll(str);

    strs.forEach(iter => {
      if (iter.type == 'code') {
        let data = iter.data;
        if (data.indexOf('```graphviz') >= 0) {
          data = data.replace('```graphviz', 'digraph G {');
          data = data.replace('```', '}');
          iter.data = this.preprocessGraphviz(data, clsName);
        } else if (data.indexOf('```uml') >= 0) {
          data = data.replace('```uml', '@startuml');
          data = data.replace('```', '@enduml');
          iter.data = this.preprocessUML(data, clsName);
        }
      }
    });

    let result = strs.reduce((result, iter) => {
      return result + iter.data;
    }, '');

    return result;
  }

  writeResult(clsName, result) {
    let filename = `docs/${clsName}.md`;
    let str = this.preprocess(result, clsName);

    fs.writeFileSync(filename, str);
  }

  genFunctionsIndex(cls) {
    let result = '';

    if (cls.methods && cls.methods.length) {
      result += `### 函数\n`
      result += genAnchor(cls.name, {
        name: 'methods'
      }) + '\n\n';
      result += '| 函数名称 | 说明 | \n';
      result += '| -------- | ------------ | \n';

      cls.methods.sort((a, b) => {
        return a.name.localeCompare(b.name);
      });

      cls.methods.forEach((iter) => {
        if (!isPrivate(iter)) {
          result += `| ${genA(cls.name, iter)} | ${iter.desc.split('\n')[0].trim()} |\n`;
        }
      });
    }

    return result;
  }

  genFunctionsDetail(cls) {
    let result = '';

    if (cls.methods) {
      cls.methods.forEach(iter => {
        if (!isPrivate(iter)) {
          result += this.genOneFunc(cls, iter);
        }
      });
    }

    return result;
  }

  genPropertiesIndex(cls) {
    let result = '';
    if (cls.properties && cls.properties.length) {
      result += `### 属性\n`
      result += genAnchor(cls.name, {
        name: 'properties'
      }) + '\n\n';
      result += '| 属性名称 | 类型 | 说明 | \n';
      result += '| -------- | ----- | ------------ | \n';

      cls.properties.sort((a, b) => {
        return a.name.localeCompare(b.name);
      });

      cls.properties.forEach((p) => {
        if (!isPrivate(p)) {
          result += `| ${genA(cls.name, p)} | ${encodeStr(p.type)} | ${p.desc.split('\n')[0].trim()} |\n`;
        }
      });
    }

    return result;
  }

  genPropertiesDetail(cls) {
    let result = '';
    if (cls.properties) {
      cls.properties.forEach((p) => {
        if (!isPrivate(p)) {
          result += this.genOneProperty(cls, p);
        }
      });
    }

    return result;
  }

  genConsts(cls) {
    let result = '';

    if (cls.consts && cls.consts.length) {
      result += `\n### 常量\n`
      result += genAnchor(cls.name, {
        name: 'consts'
      }) + '\n\n';
      result += '| 名称 | 说明 | \n';
      result += '| -------- | ------- | \n';

      cls.consts.forEach(iter => {
        let desc = iter.desc.trim();
        desc = desc.replace('\r\n', '');
        desc = desc.replace('\n', '');
        desc = desc.replace('\r', '');
        result += `| ${encodeStr(iter.name)} | ${encodeStr(desc)} |\n`;
      });
    }

    return result;
  }

  genEvents(cls) {
    let result = '';

    if (cls.events && cls.events.length) {
      result += `### 事件\n`
      result += genAnchor(cls.name, {
        name: 'events'
      }) + '\n\n';
      result += '| 事件名称 | 类型  | 说明 | \n';
      result += '| -------- | ----- | ------- | \n';

      cls.events.forEach(iter => {
        result += `| ${encodeStr(iter.name)} | ${encodeStr(iter.type)} | ${encodeStr(iter.desc.trim())} |\n`;
      });
    }

    return result;
  }

  genOneClass(cls) {
    let result = `## ${encodeStr(cls.name)}\n`;

    result += `### 概述\n`;

    if(cls.parent) {
      result += '```graphviz\n';
      result += '[default_style]\n';
      result += `${cls.name} -> ${cls.parent}[arrowhead = "empty"]`;
      result += '```\n';
    }
    
    result += cls.desc;
    result += '\n----------------------------------\n';

    result += this.genFunctionsIndex(cls);
    result += this.genPropertiesIndex(cls);
    result += this.genConsts(cls);
    result += this.genEvents(cls);

    result += this.genFunctionsDetail(cls);
    result += this.genPropertiesDetail(cls);

    this.writeResult(cls.name, result);

    return;
  }

  genOneProperty(cls, p) {
    let result = `#### ${encodeStr(p.name)} 属性\n`;
    result += `-----------------------\n`;
    result += `> ${genAnchor(cls.name, p)}${p.desc}\n\n`;
    result += `* 类型：${encodeStr(p.type)}\n\n`;

    if(p.annotation) {
      result += `| 特性 | 是否支持 |\n`;
      result += `| -------- | ----- |\n`;
      result += `| 可直接读取 | ${toBool(isReadable(p))} |\n`;
      result += `| 可直接修改 | ${toBool(isWritable(p))} |\n`;

      if(isPersitent(p)) {
        result += `| 可持久化   | ${toBool(isPersitent(p))} |\n`;
      }
      if(isScriptable(p)) {
        result += `| 可脚本化   | ${toBool(isScriptable(p))} |\n`;
      }
      if(isDesign(p)) {
        result += `| 可在IDE中设置 | ${toBool(isDesign(p))} |\n`;
      }
      if(isSetProp(p)) {
        result += `| 可在XML中设置 | ${toBool(isSetProp(p))} |\n`;
      }
      if(isGetProp(p)) {
        result += `| 可通过widget\\_get\\_prop读取 | ${toBool(isGetProp(p))} |\n`;
      }
      if(isSetProp(p)) {
        result += `| 可通过widget\\_set\\_prop修改 | ${toBool(isSetProp(p))} |\n`;
      }
    }

    return result;
  }

  genOneEnum(cls) {
    let result = `## ${encodeStr(cls.name)}\n`;

    result += `### 概述\n`;

    result += cls.desc;

    if (cls.consts) {
      result += this.genConsts(cls);
    }

    this.writeResult(cls.name, result);

    return result;
  }

  genOne(cls) {
    this.imageIndex = 0;
    this.clsName = cls.name;

    if (cls.type === 'class') {
      return this.genOneClass(cls);
    } else if (cls.type === 'enum') {
      return this.genOneEnum(cls);
    } else {}
  }

  genJsonAll(json) {
    json.forEach(iter => {
      this.genOne(iter);
    });
  }

  genAll(filename) {
    this.genJsonAll(JSON.parse(fs.readFileSync(filename).toString()));
  }

  static gen(inputIDL) {
    const gen = new ApiGenerator();

    gen.genAll(inputIDL);

    console.log(`${inputIDL} => docs`);
  }
}

function rimraf(dir_path) {
    if (fs.existsSync(dir_path)) {
        fs.readdirSync(dir_path).forEach(function(entry) {
            var entry_path = path.join(dir_path, entry);
            if (fs.lstatSync(entry_path).isDirectory()) {
                rimraf(entry_path);
            } else {
                fs.unlinkSync(entry_path);
            }
        });
        fs.rmdirSync(dir_path);
    }
}

let inputIDL = path.normalize(path.join(__dirname, '../idl_gen/idl.json'));

if(process.argv.length >= 3) {
  inputIDL = process.argv[2] || inputIDL;
}

if(inputIDL === '-h' || inputIDL === '--help') {
  console.log('Usage: node index.js inputIDL');
  process.exit(0);
}

['docs', 'dots', 'umls', 'docs/images'].forEach(iter => {
  if(fs.existsSync(iter)) {
    rimraf(iter);
  }
  fs.mkdirSync(iter);
  console.log(`mkdir ${iter}`);
});

ApiGenerator.gen(inputIDL);

