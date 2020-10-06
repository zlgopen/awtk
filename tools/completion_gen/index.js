const fs = require('fs')
const path = require('path')

let inputIDL = path.normalize(path.join(__dirname, '../idl_gen/idl.json'));

if(process.argv.length >= 3) {
  inputIDL = process.argv[2] || inputIDL;
}

if(inputIDL === '-h' || inputIDL === '--help') {
  console.log('Usage: node index.js inputIDL');
  process.exit(0);
}


function genCompletion(json) {
  widgets = json.filter((iter) => {
    return iter.annotation && iter.annotation.widget;
  });

  let result = {}
  result.widgets = widgets.map((iter) => {
    let w = {}
    w.name = iter.name.replace(/_t$/, '');
    w.desc = iter.desc.split('\n')[0].trim('\r\n ');

    let props = iter.properties.filter((iter) => {
      return iter.annotation && iter.annotation.design;
    });

    w.props = props.map(iter => {
      delete iter.annotation;
      return iter;
    });

    return w;
  });

  let str = JSON.stringify(result, null, '\t');
  fs.writeFileSync("awtk_ui.json", str);

  console.log(str);
}

genCompletion(JSON.parse(fs.readFileSync(inputIDL).toString()));

