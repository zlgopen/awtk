name = 'tom'
assert(name == 'tom')

var name='jim'
assert(name == 'jim')

unset(name);
assert(name == 'tom')

if(name == 'tom') {
  var age = 100;
}
assert(age == 100)

