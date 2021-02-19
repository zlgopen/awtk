var a = 1;
var b = "";

if(a == 1) {
  b = "a"
  print(b);
} else if(a == 2) {
  b = "b"
  print(b);
} else if(a == 3) {
  b = "c"
  print(b);
} else if(a == 4) {
  b = "d"
  print(b);
} else {
  b = "other"
  print(b);
}

assert(b == "a")

a = 2;
if(a == 1) {
  b = "a"
  print(b);
} else if(a == 2) {
  b = "b"
  print(b);
} else if(a == 3) {
  b = "c"
  print(b);
} else if(a == 4) {
  b = "d"
  print(b);
} else {
  b = "other"
}

assert(b == "b")

a = 3;
if(a == 1) {
  b = "a"
} else if(a == 2) {
  b = "b"
} else if(a == 3) {
  b = "c"
} else if(a == 4) {
  b = "d"
} else {
  b = "other"
}

assert(b == "c")

a = 4;
if(a == 1) {
  b = "a"
} else if(a == 2) {
  b = "b"
} else if(a == 3) {
  b = "c"
} else if(a == 4) {
  b = "d"
} else {
  b = "other"
}

assert(b == "d")

a = 50;
if(a == 1) {
  b = "a"
} else if(a == 2) {
  b = "b"
} else if(a == 3) {
  b = "c"
} else if(a == 4) {
  b = "d"
} else {
  b = "other"
}

assert(b == "other")
