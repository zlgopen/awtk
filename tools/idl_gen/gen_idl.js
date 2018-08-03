const fs = require('fs');
var glob = require('glob')

function parseTokens(str) {
  const ret = str.match(/[a-z|_|A-Z|\d|*]+/g);
  return ret.filter(item => {
    return !!item;
  });
}

function parseClass(cls, comment) {
  comment.split('\n').forEach(iter => {
    if (iter.indexOf('@class') >= 0) {
      cls.name = parseTokens(iter)[2];
    } else if (iter.indexOf('@scriptable') >= 0) {
      cls.scriptable = true;
    } else if (iter.indexOf('@parent') >= 0) {
      cls.parent = parseTokens(iter)[2];
    } else if (iter.indexOf('@fake') >= 0) {
      cls.isFake = true;
    }
  });
}

function parseEnum(cls, comment) {
  comment.split('\n').forEach(iter => {
    if (iter.indexOf('@enum') >= 0) {
      cls.name = parseTokens(iter)[2];
      cls.prefix = cls.name.replace(/t$/, "").toUpperCase();
    } else if (iter.indexOf('@scriptable') >= 0) {
      cls.scriptable = true;
    } else if (iter.indexOf('@prefix') >= 0) {
      cls.prefix = parseTokens(iter)[2].toUpperCase();
    }
  });
}

function parseConst(comment) {
  let c = {};
  comment.split('\n').forEach(iter => {
    if (iter.indexOf('@const') >= 0) {
      c.name = parseTokens(iter)[2];
    }
  });

  return c;
}

function parseProperty(comment) {
  const property = {};
  comment.split('\n').forEach(iter => {
    if (iter.indexOf('@property') >= 0) {
      const arr = parseTokens(iter);
      property.type = arr[2];
      property.name = arr[3];
    } else if (iter.indexOf('@readonly') >= 0) {
      property.readonly = true;
    } else if (iter.indexOf(' @private') >= 0) {
      property.isPrivate = true;
    } else if (iter.indexOf(' @fake') >= 0) {
      property.isFake= true;
    } else if (iter.indexOf(' @scriptable') >= 0 && iter.indexOf('no') >= 0) {
      property.isPrivate = true;
    }
  });

  return property;
}

function parseMethod(comment) {
  const method = {
    params: []
  };

  comment.split('\n').forEach(iter => {
    if (iter.indexOf('@method') >= 0) {
      method.name = parseTokens(iter)[2];
    } else if (iter.indexOf(' @annotation') >= 0) {
      method.isDeconstructor = true;
    } else if (iter.indexOf(' @constructor') >= 0) {
      method.isConstructor = true;
    } else if (iter.indexOf(' @static') >= 0) {
      method.isStatic = true;
    } else if (iter.indexOf(' @global') >= 0) {
      method.isGlobal = true;
    } else if (iter.indexOf(' @private') >= 0) {
      method.isPrivate = true;
    } else if (iter.indexOf(' @scriptable') >= 0 && iter.indexOf('no') >= 0) {
      method.isPrivate = true;
    } else if (iter.indexOf(' @scriptable') >= 0 && iter.indexOf('custom') >= 0) {
      method.isCustom = true;
    } else if (iter.indexOf(' @param') >= 0) {
      const param = {};
      const arr = parseTokens(iter);
      param.type = arr[2];
      param.name = arr[3];
      method.params.push(param);
    } else if (iter.indexOf(' @return') >= 0) {
      const type = iter.match(/{[a-z|A-Z| |_\d\*]*}/)[0];
      method.return = type.substr(1, type.length - 2);
    }
  });

  return method;
}

function extractIDL(result, filename, content) {
  let str = content;
  let start = 0;
  let end = 0;
  let cls = null;
  do {
    start = str.indexOf('/**');
    if (start >= 0) {
      str = str.substr(start);
      end = str.indexOf('*/');
      if (end >= 0) {
        const comment = str.substr(0, end + 3);
        if (comment.indexOf('* @') >= 0) {
          if (comment.indexOf(' @class') >= 0) {
            cls = {};
            cls.type = 'class';
            parseClass(cls, comment);
            result.push(cls);
            cls.methods = [];
            cls.properties = [];
            cls.header = filename;
          } else if (comment.indexOf(' @method') >= 0) {
            const m = parseMethod(comment);
            if (m && !m.isPrivate) {
              if (m.isGlobal) {
                m.type = 'method';
                m.header = filename;
                result.push(m);
              } else if(cls) {
                cls.methods.push(m);
              }
            }
          } else if (comment.indexOf(' @property') >= 0) {
            const p = parseProperty(comment);
            if (p && !p.isPrivate) {
              cls.properties.push(p);
            }
          } else if (comment.indexOf(' @const') >= 0) {
            cls.consts.push(parseConst(comment));
          } else if (comment.indexOf(' @enum') >= 0) {
            cls = {};
            cls.type = 'enum';
            cls.header = filename;
            parseEnum(cls, comment);
            result.push(cls);
            cls.consts = [];
          }
        }

        str = str.substr(end + 3);
      }
    }

  } while (start >= 0 && end >= 0)
}

function genOneFile(result, filename) {
  const content = fs.readFileSync(filename).toString();
  const name = filename.match(/[a-z|_\d|\.|A-Z]*\/[a-z|_\d|\.|A-Z]*$/)[0];
  console.log(`process ${filename}`);
  extractIDL(result, name, content);
}

function genAll() {
  const result = [];
  glob('../../src/**/*.h', null, function (er, files) {
    files.forEach(iter => {
      genOneFile(result, iter);
    });

    fs.writeFileSync('idl.json', JSON.stringify(result, null, ' '));
  })
}

genAll();
