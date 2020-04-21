const fs = require('fs');
const helper = require('./helper');

function mergePinyin(words) {
  let pinyin = '';
  for (let i = 3; i < words.length; i++) {
    pinyin += words[i];
  }

  return pinyin;
}

function genPinYinTable(obj) {
  let arr = []
  let result = ''

  for (let key in obj) {
    let value = obj[key];
    if (value.items.length == 0) {
      continue;
    }

    value.items.sort((a, b) => {
      return b.freq - a.freq;
    });

    result += `/*${value.pinyin}*/\n`;
    result += `static const char* s_${key}[] = {\n`
    for(let pinyin in value.pinyin) {
      result += `  \"${pinyin}\",\n`;
    }
    result += '  NULL\n';
    result += '};\n';

    arr.push(value);
  }

  arr.sort((a, b) => {
    return a.key.localeCompare(b.key);
  });

  result += `static const table_entry_t s_t9ext_numbers_pinyin[] = {\n`;
  arr.forEach(iter => {
    let key = iter.key;
    let pinyin = iter.items[0].pinyin;
    result += `  {"${key}", NULL, s_${key}},\n`
  });

  result += '};\n';

  helper.saveResult(`t9ext_zh_cn.inc`, result);
}

function toT9(filename, onlySimpleChinese) {
  let obj = {}
  const content = fs.readFileSync(filename, "utf16le");
  const lines = content.split('\n');

  lines.forEach((line, index) => {
    const words = line.split(' ');
    const char = words[0];
    const freq = parseFloat(words[1]);
    const tw = words[2] === '1';
    const pinyin = mergePinyin(words);
    const key = helper.mapStr(pinyin);
    const notExist = !obj[key];

    if (!key) {
      return;
    }

    if (onlySimpleChinese && tw) {
      return;
    }

    console.log(`${pinyin} => ${key}`);

    if (notExist) {
      obj[key] = {
        key: key,
        items: [],
        pinyin: {}
      }
    }

    let value = obj[key];
    value.pinyin[pinyin] = true

    value.items.push({
      pinyin: pinyin,
      char: char,
      freq: freq
    });
  });

  genPinYinTable(obj)
}

toT9('chinese_words.txt', true);
