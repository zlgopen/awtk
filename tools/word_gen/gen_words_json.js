const fs = require('fs');
const URL = require('url')
const Crawler = require("crawler");
const Segment = require('novel-segment');

let allWords = {};
let doneURLS = {};
let maxURLS = 100000;
let errorPages = 0;
let reservedPages = maxURLS;
const maxWordsPerChar = 15;
let rootURL = ['https://www.qisuu.la/du/', 'http://blog.sina.com.cn/', 'https://blog.csdn.net/'];

function isValidURL(url) {
  if (url.indexOf('javascript:') >= 0 || url.indexOf('css') >= 0 || url.indexOf(':') > 8) {
    return false;
  }

  if (doneURLS[url] || url.indexOf('#') >= 0 || url.indexOf('ico') >= 0) {
    return false;
  }

  if (url.indexOf('api.') >= 0 || url.indexOf('download') >= 0) {
    return false;
  }

  for (let i = 0; i < rootURL.length; i++) {
    let iter = rootURL[i];
    if (url.indexOf(iter) >= 0) {
      return true;
    }
  }

  return false;
}

function tidyResult() {
  const arr = [];

  for (let c in allWords) {
    const words = allWords[c];
    let item = {
      c: c,
      words: []
    };

    for (let w in words) {
      const f = words[w];
      item.words.push({
        w: w,
        f: f
      })
    }

    item.words.sort((a, b) => {
      return b.f - a.f;
    })

    if (item.words.length > maxWordsPerChar) {
      item.words.length = maxWordsPerChar;
    }

    arr.push(item);
  }

  arr.sort((a, b) => {
    return a.c.charCodeAt(0) - b.c.charCodeAt(0);
  })

  console.log(JSON.stringify(arr, null, ' '));

  return arr;
}

function outputAndQuit() {
  const result = tidyResult();
  const json = JSON.stringify(result, null, '\t');
  fs.writeFileSync("words.json", json);

  console.log(`errorPages=${errorPages}`);
  console.log(`Output: words.json`);
  process.exit(0);
}

function isValidWord(w) {
  if (w.length > 8 || w.length < 2) {
    return false;
  }

  for (let i = 0; i < w.length; i++) {
    const c = w.charCodeAt(i);
    if (c < 0x80) {
      return false;
    }
  }

  return true;
}

function addWord(w) {
  const c = w.substring(0, 1);
  const others = w.substring(1);

  if (!isValidWord(w)) {
    return;
  }

  if (!allWords[c]) {
    allWords[c] = {};
  }

  if (!(allWords[c][others])) {
    allWords[c][others] = 1;
  } else {
    allWords[c][others] = allWords[c][others] + 1;
  }
}

function addUrls(requestUrl, urls, c) {
  for (let i = 0; i < urls.length; i++) {
    const iter = urls[i];
    const href = iter.attribs.href;
    const url = URL.resolve(requestUrl, href);


    if (isValidURL(url)) {
      maxURLS--;
      if (maxURLS >= 0) {
        console.log(`fetching: ${maxURLS} ${url}`);
        doneURLS[url] = true;
        c.queue(url);
      }
    } else {
      console.log(`skip: ${url}`);
    }
  }
}

function addWords(text) {
  const segment = new Segment()
  segment.useDefault();

  try {
    const words = segment.doSegment(text);
    words.forEach(element => {
      addWord(element.w);
    });
  } catch (e) {
    console.log(e);
    errorPages++;
  }
}

function onTaskDone(err, res, done) {
  if (reservedPages <= 0 || err) {
    outputAndQuit();

    done();
    return;
  }

  if (res.body.indexOf("UTF-8") < 0 && res.body.indexOf("utf-8") < 0) {
    done();
    return;
  }

  const contentType = res.headers['content-type'];
  if (!contentType || contentType.indexOf('html') < 0) {
    done();
    return;
  }

  addWords(res.$("body").text());

  reservedPages--;
  addUrls(res.request.uri.href, res.$("[href]"), c);

  console.log(`${reservedPages} ${res.request.uri.href}`);

  res = null;

  done();
}

var c = new Crawler({
  retries: 1,
  forceUTF8: false,
  timeout: 5000,
  skipDuplicates: true,
  callback: onTaskDone
});

c.queue(rootURL);
