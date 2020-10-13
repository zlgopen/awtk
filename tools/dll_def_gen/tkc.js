const fs = require('fs')
const path = require('path')
const DefGenerator = require('./def_generator')


let inputIDL = path.normalize(path.join(__dirname, '../idl_gen/tkc.json'));
let outputDef = '../../dllexports/tkc.def';
let withIndex = false;

if(process.argv.length >= 4) {
  inputIDL = process.argv[2];
  outputDef = process.argv[3];
  if (process.argv.length >= 5) {
    withIndex = process.argv[4].toLowerCase().startsWith('t');
  }
} else if(process.argv.length != 2) {
  console.log('Usage: node index.js inputIDL outputDef withIndex[false|true]');
  process.exit(0);
}

DefGenerator.gen(inputIDL, outputDef, withIndex);

