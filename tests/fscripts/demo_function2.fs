
function f1() {
  var a = 10;
  var b = 20;
  var c = a + b;
  return c;
}

assert(f1() == 30);

function f2() {
  var a = 10;
  var b = 20;
  return a + b;
}
assert(f2() == 30);

function f3() {
  var a = 10;
  var b = 20;
  return (a + b);
}
assert(f3() == 30);

function f4() {
  var a = 10;
  var b = 20;
  return 30
}
assert(f4() == 30);
