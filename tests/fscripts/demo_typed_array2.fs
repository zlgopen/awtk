b=0
a=typed_array_create("i8", 100000)

while (b < 100000) {
  assert(typed_array_push(a, b) == 1)
  assert(typed_array_get_size(a), b)
  b = b + 1
}


b=0
while (true) {
  assert(typed_array_push(a, b) == 1)
  assert(typed_array_get_size(a), b)

  if (b % 2) {
    print(b, typed_array_get(a, b))
  }

  if(b > 10) {
    break;
  }
  b = b + 1
}

assert(typed_array_clear(a))
assert(typed_array_get_size(a) == 0)
unset(a)
