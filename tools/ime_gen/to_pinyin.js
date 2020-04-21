const fs = require('fs');
const helper = require('./helper');

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

    result += `static const char* s_py_${key}[] = {\n`
    value.items.forEach(iter => {
      result += `  \"${iter.char}\",\n`;
    });
    result += '  NULL\n';
    result += '};\n';

    arr.push(value);
  }

  arr.sort((a, b) => {
    return a.key.localeCompare(b.key);
  });

  result += `static const table_entry_t s_pinyin_chinese_items[] = {\n`;
  arr.forEach(iter => {
    let key = iter.key;
    result += `  {"${key}", NULL, s_py_${key}},\n`
  });
  result += '};\n';

  helper.saveResult(`pinyin_table.inc`, result);
}

function toPinyin(filename, onlySimpleChinese) {
  let obj = {}
  const content = fs.readFileSync(filename, "utf16le");
  const lines = content.split('\n');

  lines.forEach((line, index) => {
    const words = line.split(' ');
    const char = words[0];
    const freq = parseFloat(words[1]);
    const tw = words[2] === '1';
    const pinyin = helper.mergePinyin(words);
    const key = pinyin;
    const notExist = !obj[key];

    if (!key) {
      return;
    }

    if (onlySimpleChinese && tw) {
      return;
    }

    if (notExist) {
      obj[key] = {
        key: key,
        items: [],
      }
    }

    let value = obj[key];
    value.items.push({
      char: char,
      pinyin: pinyin,
      freq: freq
    });
  });

  genPinYinTable(obj);
}

toPinyin('chinese_words.txt', true);
