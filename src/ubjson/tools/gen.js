const fs = require('fs');

class CodeGen {
  CodeGen() {
    this.result = '';
  }

  getType(iter) {
    if(iter.type === 'char') {
      return iter.type;
    } else if(iter.alias) {
      return iter.alias;
    } else {
      return iter.type + '_t';
    }
  }

  genRead(ojson) {
    let result = '';
    ojson.forEach(iter => {
      result += `    case UBJSON_MARKER_${iter.type.toUpperCase()}: {\n`
      if(iter.payload) {
        result += `      ${this.getType(iter)} value = 0;\n`;
        result += `      return_value_if_fail(ubjson_reader_read(reader, &value, sizeof(value)) == RET_OK, RET_FAIL);\n`;
      } else {
      }
      result += `      break;\n`; 
      result += `    };\n`; 

    });

    console.log(result);
  }
  
  genWriteDecl(ojson) {
    let result = '';
    ojson.forEach(iter => {
      result += `ret_t ubjson_writer_write_${iter.type}(ubjson_writer_t* writer`;

      if(iter.payload) {
        result += `, ${this.getType(iter)} value);\n`;
      } else {
        result += `);\n`;
      }
    });

    console.log(result);
  }

  genWriteImpl(ojson) {
    let result = '';
    
    ojson.forEach(iter => {
      if(iter.payload && iter.type.indexOf('8') < 0 && iter.type.indexOf('char') < 0 && iter.type.indexOf('string') < 0) {
        let type = this.getType(iter);
        result += `static ${type}  ${iter.type}_to_big_endian(${type} value) {\n`;
        result += `  return value;\n`;
        result += `}\n\n`;
      }
    });

    ojson.forEach(iter => {
      result += `ret_t ubjson_writer_write_${iter.type}(ubjson_writer_t* writer`;

      if(iter.payload) {
        result += `, ${this.getType(iter)} value) {\n`;
      } else {
        result += `) {\n`;
      }
      result += `  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);\n`;
      result += `  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_${iter.type.toUpperCase()}) == RET_OK, RET_OOM);\n`;
      
      if(iter.payload) {
        if(iter.type === 'string') {
          result += `  return_value_if_fail(ubjson_writer_write_data(writer, value, strlen(value)) == RET_OK, RET_OOM);\n`;
        } else {
          if(iter.type.indexOf('8') < 0 && iter.type.indexOf('char') < 0) {
            result += `  value = ${iter.type}_to_big_endian(value);\n`;
          }
          result += `  return_value_if_fail(ubjson_writer_write_data(writer, &value, sizeof(value)) == RET_OK, RET_OOM);\n`;
        }
      } else {
      }
      result += `\n  return RET_OK;\n`;
      result += `}\n\n`;
    });

    console.log(result);
  }

  genJsonAll(ojson) {
    //console.log(ojson);
    //this.genWriteDecl(ojson);
    //this.genWriteImpl(ojson);
    this.genRead(ojson);
  }

  genAll(filename) {
    this.json = JSON.parse(fs.readFileSync(filename).toString());
    this.genJsonAll(this.json);
  }

  getJsonIDL() {
    return './def.json';
  }

  static gen() {
    const gen = new CodeGen();

    gen.genAll(gen.getJsonIDL());
  }
}

CodeGen.gen();
