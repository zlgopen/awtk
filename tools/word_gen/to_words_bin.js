let fs = require('fs');

function bufferWriteWord(buff, word, start) {
  let offset = start;
  let nr = word.length;

  for (let i = 0; i < nr; i++) {
    const code = word.charCodeAt(i);
    buff.writeUInt16LE(code, offset);
    offset += 2;
  }

  buff.writeUInt16LE(0, offset);
  offset += 2;

  return offset - start;
}

function outputWords(arr) {
  let offsetData = 0;
  let offsetIndex = 0;
  const nr = arr.length;
  let f = fs.openSync("words.bin", "w+");
  let headerBuffer = Buffer.alloc(8);
  let indexBuffer = Buffer.alloc(nr * 8);
  let contentBuffer = Buffer.alloc(10 * 1024 * 1024)

  headerBuffer.writeUInt32LE(0, 0);
  headerBuffer.writeUInt32LE(nr, 4);
  arr.forEach(iter => {
    const code = iter.c.charCodeAt(0);
    indexBuffer.writeUInt32LE(code, offsetIndex);
    indexBuffer.writeUInt32LE(offsetData, offsetIndex + 4);
    offsetIndex += 8;

    contentBuffer.writeUInt32LE(iter.words.length, offsetData);
    offsetData += 4;
    iter.words.forEach(w => {
      offsetData += bufferWriteWord(contentBuffer, w.w, offsetData);
    })
  });

  fs.writeSync(f, headerBuffer);
  fs.writeSync(f, indexBuffer);
  fs.writeSync(f, contentBuffer, 0, offsetData);
  fs.closeSync(f);
}

outputWords(JSON.parse(fs.readFileSync("./words.json").toString()))
console.log("write to words.bin");

