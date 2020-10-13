const fs = require('fs')
const path = require('path')

class DefGenerator {
  genMethod(iter) {
    let name = iter.name;
    if (iter.export) {
      if (iter.export === 'none') {
        console.log('skip:' + iter.name);
        return;
      } else {
        name = iter.export
      }
    }

    if (this.withIndex) {
      this.result += `    ${name} @${exportIndex++}\n`
    } else {
      this.result += `    ${name}\n`
    }
  }

  genOneClass(cls) {
    cls.methods.forEach(iter => this.genMethod(iter));
  }

  genOne(cls) {
    if (cls.type === 'class') {
      this.genOneClass(cls);
    } else if (cls.type === 'method') {
      this.genMethod(cls);
    }
  }

  writeResult(filename, result) {
    fs.writeFileSync(filename, result);
    console.log("write to " + filename)
  }

  genJsonAll(json, outputDef) {
    let ext =  path.extname(outputDef);
    let name = path.basename(outputDef, ext);

    this.result = ''
    this.result += `LIBRARY   ${name}\n`
    this.result += 'EXPORTS\n'

    console.log(outputDef, name)
    json.forEach(iter => {
      this.genOne(iter);
    });

    this.writeResult(outputDef, this.result);
  }

  genAll(filename, outputDef) {
    this.genJsonAll(JSON.parse(fs.readFileSync(filename).toString()), outputDef);
  }

  static gen(inputIDL, outputDef, withIndex) {
    const gen = new DefGenerator();

    gen.withIndex = withIndex;
    gen.genAll(inputIDL, outputDef);

    console.log(`${inputIDL} => ${outputDef}`);
  }
}

module.exports = DefGenerator;

