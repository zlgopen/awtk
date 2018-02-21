const fs = require('fs');
var glob = require('glob')

function extract(content) {
  let str = content;
  let start = 0;
  let end = 0;
  do {
    start = str.indexOf("/**");
    if(start >= 0) {
      str = str.substr(start);
      end = str.indexOf("*/");
      if(end >= 0) {
        const comment = str.substr(0, end-start+3);
        if(comment.indexOf("* @") >= 0) {
          console.log('{{{'+comment);
          console.log('}}}'+str.substr(end+3));
        }

        str = str.substr(end+3);
      }
    }

  }while(start >= 0 && end >= 0)
}

function genOneFile(filename) {
    const content = fs.readFileSync(filename).toString();
    console.log(filename);
    extract(content);
}

function genAll() {
  glob("../../src/**/*.h", null, function (er, files) {
    files.forEach(genOneFile);
  })
}

genAll();

