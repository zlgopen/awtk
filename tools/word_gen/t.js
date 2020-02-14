let Segment = require('novel-segment')
let fetch = require('cross-fetch')
let Bluebird = require('bluebird')
let fs = require('fs')
let rootURL = ['https://www.qisuu.la/du/', 'http://blog.sina.com.cn/', 'https://blog.csdn.net/'];

function outputJSON(filename, json) {
  let str = JSON.stringify(json, null, '  ');
  fs.writeFileSync(filename. str);

  return;
}

const segment = new Segment({
	autoCjk: true,
	all_mod: true,
});

Bluebird
	.resolve(rootURL)
	.map(async (url) =>
	{
		let text = await fetch(url).then(r => r.text()).catch(e => null)
    console.log(url);

		if (!text)
		{
			return;
		}

		const words = segment.doSegment(text, {
			simple: true,
		});

		return outputJSON('_' + new URL(url).hostname + '.json', words)
	})
;

