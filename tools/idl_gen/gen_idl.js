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
    }
  });
}

function parseEnum(cls, comment) {
  comment.split('\n').forEach(iter => {
    if (iter.indexOf('@enum') >= 0) {
      cls.name = parseTokens(iter)[2];
    } else if (iter.indexOf('@scriptable') >= 0) {
      cls.scriptable = true;
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
    } else if (iter.indexOf(' @deconstructor') >= 0) {
      method.isDeconstructor = true;
    } else if (iter.indexOf(' @constructor') >= 0) {
      method.isConstructor = true;
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
            cls.methods.push(parseMethod(comment));
          } else if (comment.indexOf(' @property') >= 0) {
            cls.properties.push(parseProperty(comment));
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
  const name = filename.match(/[a-z|_\d|\.|A-Z]*\/[a-z|_\d|\.|A-Z]*$/);
  extractIDL(result, name, content);
}

function genAll() {
  const result = [];
  glob('../../src/**/value.h', null, function (er, files) {
    files.forEach(iter => {
      genOneFile(result, iter);
    });

    fs.writeFileSync('idl.json', JSON.stringify(result, null, ' '));
  })
}

genAll();