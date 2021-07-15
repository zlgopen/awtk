assert(len("abc") == 3)
assert(substr("abcd", 0, 2) == "ab")
assert(index_of('aaa bbb aaa bbb', 'bbb') == 4);
assert(last_index_of('aaa bbb aaa bbb', 'bbb') == 12);
assert(char_at("致远电子", 0) == '致')
assert(char_at("致远电子", 1) == '远')
assert(char_at("致远电子", 2) == '电')
assert(char_at("致远电子", 3) == '子')
assert(char_at("致远电子", -1) == '子')
assert(char_at("致远电子", -2) == '电')
assert(char_at("abc", -2) == 'b')
