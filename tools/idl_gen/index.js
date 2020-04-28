const fs = require('fs');
const path = require('path');
const glob = require('glob')
const IDLGen = require('./idl_gen.js')

let outputIDL = 'idl.json';
let sourcesPath = path.normalize(path.join(__dirname, '../../src'));

if(process.argv.length == 3) {
  outputIDL = process.argv[2];
} else if(process.argv.length > 3) {
  outputIDL = process.argv[2];
  sourcesPath = process.argv[3];
}

if(sourcesPath === '-h' || sourcesPath === '--help') {
  console.log('Usage: node index.js outputIDL sourcesPath');
  process.exit(0);
}

IDLGen.gen(sourcesPath, outputIDL)

