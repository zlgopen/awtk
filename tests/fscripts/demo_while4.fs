a=0
b=0

while(a < 100) {
  a=a+1
  b=b+a
  if(a > 20) {
    break;
  }
  print(a, b)
}
print(b)

