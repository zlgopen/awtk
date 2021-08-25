function add(v1, v2, v3){
   return (v1+v2+v3);
}
assert(add(100, 200, 300) == 600)

function f(n) {
  if(n == 0) {
    return (0);
  } else if(n == 1) {
    return (1);
  } else {
    return (f(n-1) + f(n-2));
  }
}

assert(f(0) == 0);
assert(f(1) == 1);
assert(f(2) == 1);
assert(f(3) == 2);
assert(f(4) == 3);
assert(f(5) == 5);
assert(f(6) == 8);
assert(f(7) == 13);
assert(f(8) == 21);

var cc = 100;
function bar(aa, bb) {
  var cc = aa + bb;
  return (cc)
}
assert(bar(100, 200) == 300);
assert(cc == 100);


