const fs = require('fs');
const path = require('path');
const glob = require('glob')
const IDLGen = require('./idl_gen.js')

let outputIDL = 'tkc.json';
if(process.argv.length == 3) {
  outputIDL = process.argv[2];
}

let sourcesPath = [
    path.normalize(path.join(__dirname, '../../src/tkc')), 
    path.normalize(path.join(__dirname, '../../src/xml')), 
    path.normalize(path.join(__dirname, '../../src/charset')), 
    path.normalize(path.join(__dirname, '../../src/fscript_ext')), 
    path.normalize(path.join(__dirname, '../../src/ubjson')), 
    path.normalize(path.join(__dirname, '../../src/csv')), 
    path.normalize(path.join(__dirname, '../../src/streams')), 
    path.normalize(path.join(__dirname, '../../src/hal')), 
    path.normalize(path.join(__dirname, '../../src/conf_io'))].join(';');

IDLGen.gen(sourcesPath, outputIDL)

