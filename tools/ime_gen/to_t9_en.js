const fs = require('fs');
const helper = require('./helper');

function toT9(filename, lang) {
  let obj = {}
  let result = ''
  const content = fs.readFileSync(filename).toString();
  const lines = content.split('\n');

  lines.forEach((line, index) => {
    const word = line;
    const freq = lines.length - index;
    const key = helper.mapStr(word);
    const notExist = !obj[key];

    if (!key) {
      return;
    }

    console.log(`${word} => ${key}`);
    if (notExist) {
      obj[key] = {
        key: key,
        word: '',
        items: []
      }
    }

    let value = obj[key];
    if (value.word.indexOf(' ' + word) < 0) {
      value.word = value.word + ' ' + word;
    }

    value.items.push({
      word: word,
      freq: freq
    });
  });


  let arr = []
  for (let key in obj) {
    let value = obj[key];
    if (value.items.length == 0) {
      continue;
    }

    value.items.sort((a, b) => {
      return b.freq - a.freq;
    });

    result += `/*${value.word}*/\n`;
    result += `static const char* s_${lang}_${key}[] = {\n`
    value.items.forEach(iter => {
      result += `  \"${iter.word}\",\n`;
    });
    result += '  NULL\n';
    result += '};\n';

    arr.push(value);
  }

  arr.sort((a, b) => {
    return a.key.localeCompare(b.key);
  });

  result += `static const table_entry_t s_${lang}_items[] = {\n`;
  arr.forEach(iter => {
    let key = iter.key;
    let word = iter.items[0].word;
    result += `  {"${key}", "${word}", s_${lang}_${key}},\n`
  });

  result += '};\n';

  helper.saveResult(`t9_${lang}.inc`, result);
}

toT9('english_words.txt', 'en_us');
