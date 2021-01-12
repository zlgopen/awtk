assert(is_little())
assert(htonl(0x11223344) == 0x44332211)
assert(ntohl(0x11223344) == 0x44332211)
assert(htons(0x1122) == 0x2211)
assert(ntohs(0x1122) == 0x2211)

