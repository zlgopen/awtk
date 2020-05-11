## 抓取网页，生成输入法联想词库。

### 生成数据

在当前目录下运行：

* 准备

```
npm install
```

* 抓取网页，生成words.json

> 可以修改maxURLS改变最大网页数量。

```
node gen_words_json.js 
```


* 生成二进制的words.bin文件

> 可以根据自己的需要进行编辑words.json。

```
node to_words_bin.js
```

### 使用现有数据

chinese\_with\_freq.txt是从 https://github.com/ling0322/webdict 下载的。

如果不想自己生成，可以直接使用该文件：

```
node to_json.js
```

### 更新数据

在awtk根目录下运行：

```
cp tools/word_gen/words.bin demos/assets/default/raw/data/suggest_words_zh_cn.dat
```

如果不支持文件系统，还需要运行更新资源的脚本：

```
python scripts/update_res.py all
```

### 注意：
node\_modules/segment/lib/module/DictTokenizer.js#getChunks 可能导致OOM。

如果遇到问题，可以限制chunks.length的大小，如下面限制为5000。

```
let getChunksCallsNr = 0;
var getChunks = function (wordpos, pos, text) {
  var words = wordpos[pos] || [];
  // debug('getChunks: ');
  // debug(words);
  // throw new Error();
  var ret = [];
  if(getChunksCallsNr > 150) {
    throw "get Chunks error";
  }
  
  getChunksCallsNr++;
  for (var i = 0; i < words.length; i++) {
    var word = words[i];
    //debug(word);
    var nextcur = word.c + word.w.length;
    if (!wordpos[nextcur]) {
      ret.push([word]);
    } else  {
      var chunks = getChunks(wordpos, nextcur);
      for (var j = 0; j < chunks.length && j < 5000; j++) {
        ret.push([word].concat(chunks[j]));
      }
    }
  }
  getChunksCallsNr--;

  return ret;
};
```

