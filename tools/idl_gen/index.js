const fs = require('fs');
const path = require('path');
const glob = require('glob')
const IDLGen = require('./idl_gen.js')

let defDirlist = undefined;
let outputIDL = 'idl.json';
let sourcesPath = path.normalize(path.join(__dirname, '../../src'));

if(process.argv.length > 2) {
  outputIDL = process.argv[2];
}
if(process.argv.length > 3) {
  sourcesPath = process.argv[3];
}
if(process.argv.length > 4) {
  defDirlist = process.argv[4];
}

if(sourcesPath === '-h' || sourcesPath === '--help') {
  console.log('Usage: node index.js outputIDL sourcesPath');
  process.exit(0);
}

IDLGen.gen(sourcesPath, outputIDL, defDirlist)

