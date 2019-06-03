const fs = require('fs');
const path = require('path');
    
if(process.argv.length < 4)  {
  console.log(`Usage: node merge [idl files list]`);
  console.log(`Example: node merge idl.json myidl1.json myidl2.json`);
  process.exit(0);
}

let all = []
let files = process.argv.slice(2);

files.map(iter => {
  return JSON.parse(fs.readFileSync(iter).toString());
}).forEach(iter => {
  all = all.concat(iter);
});

fs.writeFileSync('all.json', JSON.stringify(all, null, '  '));

console.log(`Merged ${JSON.stringify(files)} ==> all.json`);
