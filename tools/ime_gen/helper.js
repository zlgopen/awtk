const fs = require('fs');

const mapTable = {
  'a': '2',
  'b': '2',
  'c': '2',
  'd': '3',
  'e': '3',
  'f': '3',
  'g': '4',
  'h': '4',
  'i': '4',
  'j': '5',
  'k': '5',
  'l': '5',
  'm': '6',
  'n': '6',
  'o': '6',
  'p': '7',
  'q': '7',
  'r': '7',
  's': '7',
  't': '8',
  'u': '8',
  'v': '8',
  'w': '9',
  'x': '9',
  'y': '9',
  'z': '9'
};

function mapChar(c) {
  return mapTable[c]
}

function mapStr(str) {
  s = ''

  if (!str) {
    return s;
  }

  for (let i = 0; i < str.length; i++) {
    s += mapChar(str[i])
  }

  return s;
}

function saveResult(filename, result) {
  const BOM = "\ufeff";

  fs.writeFileSync(filename, BOM + result);
  console.log(`write result to ${filename}`);
}

function mergePinyin(words) {
  let pinyin = '';
  for (let i = 3; i < words.length; i++) {
    pinyin += words[i];
  }

  return pinyin;
}


module.exports = {
 saveResult:saveResult,
  mergePinyin : mergePinyin,
  mapStr : mapStr
};
