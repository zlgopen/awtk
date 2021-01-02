a=0
b=0

while(a < 100) {
  a=a+1
  if((a < 20) || (a > 50)) {
    continue
  }
  b=b+a
  print(a, b)
}
print(b)

