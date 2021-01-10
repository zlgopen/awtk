
assert(bit_clear(0b0111, 1) == 0b0101)
assert(bit_clear(0b0111, 0) == 0b0110)

assert(bit_set(0b0, 0) == 0b1)
assert(bit_set(0b0, 1) == 0b10)
assert(bit_set(0b0, 2) == 0b100)

assert(bit_toggle(0b000, 0) == 0b1)
assert(bit_toggle(0b111, 1) == 0b101)
assert(bit_toggle(0b101, 2) == 0b1)

assert(bit_get(0b0111, 0))
assert(bit_get(0b0111, 1))
assert(!bit_get(0b0011, 2))

assert((1<<1) == 2)
assert((1<<2) == 4)
assert((1<<8) == 256)
assert(((0xf0112233 >> 24) & 0xff)  == 0xf0)
