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


function genUICompletion(json) {
  const widgets = json.filter((iter) => {
    return iter.annotation && iter.annotation.widget;
  });

  let result = {}
  result.widgets = widgets.map((iter) => {
    let w = {}
    w.name = iter.name.replace(/_t$/, '');
    if(iter.parent) {
      w.parent = iter.parent.replace(/_t$/, '');
    }
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

function genStyleCompletion(json) {
  let styleIDs = null;
  let widgetStates = null;

  const widgets = json.forEach((iter) => {
    if(iter.name === 'style_id_t') {
      styleIDs =  iter;
    } else if(iter.name === 'widget_state_t') { 
      widgetStates =  iter;
    }
  });

  let result = {}
  result.styleIDs = styleIDs.consts.map((iter) => {
    let item = {}
    item.name = iter.name.replace(styleIDs.prefix, '').toLowerCase();
    item.desc = iter.desc.split('\n')[0].trim('\r\n ');
    return item;
  });

  result.widgetStates = widgetStates.consts.map((iter) => {
    let item = {}
    item.name = iter.name.replace(widgetStates.prefix, '').toLowerCase();
    item.desc = iter.desc.split('\n')[0].trim('\r\n ');
    return item;
  });

  let str = JSON.stringify(result, null, '\t');
  fs.writeFileSync("awtk_style.json", str);

  console.log(str);
}

function genCompletion(json) {
  genUICompletion(json);
  genStyleCompletion(json);
}

genCompletion(JSON.parse(fs.readFileSync(inputIDL).toString()));

