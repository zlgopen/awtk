var code = '
function foo(a, b) {
  return a + b
}
function bar(a, b) {
  return a - b
}
'
var m = require_str(code)
print(m.foo(100, 99))
print(m.bar(100, 99))

