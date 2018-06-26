var Crawler = require("crawler");
var Segment = require('segment');

var segment = new Segment()

segment.useDefault();

var c = new Crawler({
    rateLimit: 1000, // `maxConnections` will be forced to 1
    callback: function(err, res, done){
		const text = res.$("body").text();
        const words = segment.doSegment(text);
		console.log(words);
        done();
    }
});

c.queue("https://blog.csdn.net/")
