const fs = require('fs');
const path = require('path');
const glob = require('glob')
const IDLGen = require('./idl_gen.js')

let outputIDL = 'tkc.json';
let sourcesPath = [
    path.normalize(path.join(__dirname, '../../src/tkc')), 
    path.normalize(path.join(__dirname, '../../src/ubjson')), 
    path.normalize(path.join(__dirname, '../../src/streams')), 
    path.normalize(path.join(__dirname, '../../src/conf_io'))].join(';');

IDLGen.gen(sourcesPath, outputIDL)

