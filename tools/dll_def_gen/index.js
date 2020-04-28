const fs = require('fs')
const path = require('path')

let exportIndex = 1;

class DefGenerator {
  genOneClass(cls) {
    cls.methods.forEach(iter => {
      let name = iter.name;
      if (iter.export) {
        if (iter.export === 'none') {
          console.log('skip:' + iter.name);
          return;
        } else {
          name = iter.export
        }
      }
      this.result += `    ${name} @${exportIndex++}\n`
    });
  }

  genOne(cls) {
    if (cls.type === 'class') {
      return this.genOneClass(cls);
    }
  }

  writeResult(name, result) {
    let filename = `../../dllexports/${name}.def`;
    fs.writeFileSync(filename, result);
    console.log("write to " + filename)
  }

  genJsonAll(json, name) {
    this.result = ''
    this.result += `LIBRARY   ${name}\n`
    this.result += 'EXPORTS\n'

    console.log(name)
    json.forEach(iter => {
      this.genOne(iter);
    });

    this.writeResult(name, this.result);
  }

  genAll(filename, name) {
    this.genJsonAll(JSON.parse(fs.readFileSync(filename).toString()), name);
  }

  static gen(inputIDL, name) {
    const gen = new DefGenerator();

    gen.genAll(inputIDL, name);

    console.log(`${inputIDL} => ../../dllexports/${name}.def`);
  }
}

let inputIDL = path.normalize(path.join(__dirname, '../idl_gen/idl.json'));
DefGenerator.gen(inputIDL, 'awtk');
