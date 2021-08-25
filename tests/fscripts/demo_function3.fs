function foo4 (v1, v2) {
  var v3 = v1 + v2;
  if(v3 < 100) {
    var name = "awtk";
  } else {
    var name = "react-awtk";
  }

  return name;
}

assert(foo4(10, 20) == 'awtk')
assert(foo4(100, 200) == 'react-awtk')
