let fs = require("fs");

let content = fs.readFileSync("chinese_with_freq.txt").toString();

let lines = content.split('\n');

let obj = {};
lines.forEach(line => {
  const words = line.split(' ');
  const chinese = words[0];
  const freq = words[1];
  const c = chinese.substr(0, 1);

  if(chinese.length == 1) {
    return;
  }

  if(!obj[c]) {
    obj[c] = {
      c : c,
      words : []
    }
  }

  let value =  obj[c];
  value.words.push({
    w:chinese.substr(1),
    f:freq
  });
});

let arr = []
for(let key in obj) {
  let value = obj[key];
  value.words.sort((a, b) => {
    return b.f - a.f;
  });
  
  if(value.words.length > 10) {
    value.words.length = 10;
  }
  arr.push(value);
}

arr.sort((a, b) => {
  console.log(a.c, b.c)
  return a.c.charCodeAt(0) - b.c.charCodeAt(0);
});

fs.writeFileSync("words.json", JSON.stringify(arr, null, '\t')); 
console.log('write to words.json');
