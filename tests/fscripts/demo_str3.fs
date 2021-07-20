a="致远电子"
assert(ulen(a) == 4);
assert(ulen(a+"abc") == 7);
assert(usubstr(a) == '致远电子');
assert(usubstr(a,0) == '致远电子');
assert(usubstr(a,0,1) == '致');
assert(usubstr(a,0,-1) == '致远电');
assert(usubstr(a,-2,-1) == '电');
assert(usubstr(a, -3) == '远电子');
assert(usubstr(a, -3, ulen(a)) == '远电子');


