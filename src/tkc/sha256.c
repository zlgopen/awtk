/*
*   SHA-256 implementation.
*
*   Copyright (c) 2010 Ilya O. Levin, http://www.literatecode.com
*
*   Permission to use, copy, modify, and distribute this software for any
*   purpose with or without fee is hereby granted, provided that the above
*   copyright notice and this permission notice appear in all copies.
*
*   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
*   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
*   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
*   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
*   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
*   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
*   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/*
 * adapted from https://raw.githubusercontent.com/ilvn/SHA256/main/sha256.c
 */

#define SWAP_BYTES
// #define USE_STD_MEMCPY
// #define SELF_TEST

#include "sha256.h"
#include "tkc/utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RL(x, n) (((x) << n) | ((x) >> (32 - n)))
#define RR(x, n) (((x) >> n) | ((x) << (32 - n)))

#define S0(x) (RR((x), 2) ^ RR((x), 13) ^ RR((x), 22))
#define S1(x) (RR((x), 6) ^ RR((x), 11) ^ RR((x), 25))
#define G0(x) (RR((x), 7) ^ RR((x), 18) ^ ((x) >> 3))
#define G1(x) (RR((x), 17) ^ RR((x), 19) ^ ((x) >> 10))

#ifdef SWAP_BYTES
#define BSWP(x, y) _bswapw((uint32_t*)(x), (uint32_t)(y))
#else
#define BSWP(p, n)
#endif
#ifdef USE_STD_MEMCPY
#define MEMCP(x, y, z) memcpy((x), (y), (z))
#else
#define MEMCP(x, y, z) _memcp((x), (y), (z))
#endif

#ifndef __cdecl
#define __cdecl
#endif

static const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

/* -------------------------------------------------------------------------- */
static void _bswapw(uint32_t* p, uint32_t i) {
  while (i--) p[i] = (RR(p[i], 24) & 0x00ff00ff) | (RR(p[i], 8) & 0xff00ff00);

} /* _bswapw */

/* -------------------------------------------------------------------------- */
#ifndef USE_STD_MEMCPY
void* __cdecl _memcp(void* d, const void* s, uint32_t sz) {
  void* rv = d;

  while (sz--) *(char*)d = *(char*)s, d = (char*)d + 1, s = (char*)s + 1;

  return (rv);
} /* _memcp */
#endif

/* -------------------------------------------------------------------------- */
static void _rtrf(uint32_t* b, uint32_t* p, uint32_t i, uint32_t j) {
#define B(x, y) b[(x - y) & 7]
#define P(x, y) p[(x + y) & 15]

  B(7, i) += (j ? (p[i & 15] += G1(P(i, 14)) + P(i, 9) + G0(P(i, 1))) : p[i & 15]) + K[i + j] +
             S1(B(4, i)) + (B(6, i) ^ (B(4, i) & (B(5, i) ^ B(6, i))));
  B(3, i) += B(7, i);
  B(7, i) += S0(B(0, i)) + ((B(0, i) & B(1, i)) | (B(2, i) & (B(0, i) ^ B(1, i))));

#undef P
#undef B
} /* _rtrf */

/* -------------------------------------------------------------------------- */
static void _hash(tk_sha256_t* ctx) {
  uint32_t b[8], *p, j;

  b[0] = ctx->hash[0];
  b[1] = ctx->hash[1];
  b[2] = ctx->hash[2];
  b[3] = ctx->hash[3];
  b[4] = ctx->hash[4];
  b[5] = ctx->hash[5];
  b[6] = ctx->hash[6];
  b[7] = ctx->hash[7];

  for (p = ctx->buf, j = 0; j < 64; j += 16)
    _rtrf(b, p, 0, j), _rtrf(b, p, 1, j), _rtrf(b, p, 2, j), _rtrf(b, p, 3, j), _rtrf(b, p, 4, j),
        _rtrf(b, p, 5, j), _rtrf(b, p, 6, j), _rtrf(b, p, 7, j), _rtrf(b, p, 8, j),
        _rtrf(b, p, 9, j), _rtrf(b, p, 10, j), _rtrf(b, p, 11, j), _rtrf(b, p, 12, j),
        _rtrf(b, p, 13, j), _rtrf(b, p, 14, j), _rtrf(b, p, 15, j);

  ctx->hash[0] += b[0];
  ctx->hash[1] += b[1];
  ctx->hash[2] += b[2];
  ctx->hash[3] += b[3];
  ctx->hash[4] += b[4];
  ctx->hash[5] += b[5];
  ctx->hash[6] += b[6];
  ctx->hash[7] += b[7];

} /* _hash */

/* -------------------------------------------------------------------------- */
static void sha256_init(tk_sha256_t* ctx) {
  ctx->len[0] = ctx->len[1] = 0;
  ctx->hash[0] = 0x6a09e667;
  ctx->hash[1] = 0xbb67ae85;
  ctx->hash[2] = 0x3c6ef372;
  ctx->hash[3] = 0xa54ff53a;
  ctx->hash[4] = 0x510e527f;
  ctx->hash[5] = 0x9b05688c;
  ctx->hash[6] = 0x1f83d9ab;
  ctx->hash[7] = 0x5be0cd19;

} /* sha256_init */

/* -------------------------------------------------------------------------- */
static void sha256_hash(tk_sha256_t* ctx, uint8_t* dat, uint32_t sz) {
  register uint32_t i = ctx->len[0] & 63, l, j;

  if ((ctx->len[0] += sz) < sz) ++(ctx->len[1]);

  for (j = 0, l = 64 - i; sz >= l; j += l, sz -= l, l = 64, i = 0) {
    MEMCP((char*)ctx->buf + i, &dat[j], l);
    BSWP(ctx->buf, 16);
    _hash(ctx);
  }
  MEMCP((char*)ctx->buf + i, &dat[j], sz);

} /* _hash */

/* -------------------------------------------------------------------------- */
static void sha256_done(tk_sha256_t* ctx, uint8_t* buf) {
  uint32_t i = (uint32_t)(ctx->len[0] & 63), j = ((~i) & 3) << 3;

  BSWP(ctx->buf, (i + 3) >> 2);

  ctx->buf[i >> 2] &= 0xffffff80 << j; /* add padding */
  ctx->buf[i >> 2] |= 0x00000080 << j;

  if (i < 56)
    i = (i >> 2) + 1;
  else
    ctx->buf[15] ^= (i < 60) ? ctx->buf[15] : 0, _hash(ctx), i = 0;

  while (i < 14) ctx->buf[i++] = 0;

  ctx->buf[14] = (ctx->len[1] << 3) | (ctx->len[0] >> 29); /* add length */
  ctx->buf[15] = ctx->len[0] << 3;

  _hash(ctx);

  for (i = 0; i < 32; i++)
    ctx->buf[i % 16] = 0, /* may remove this line in case of a DIY cleanup */
        buf[i] = (uint8_t)(ctx->hash[i >> 2] >> ((~i & 3) << 3));

} /* sha256_done */

ret_t tk_sha256_init(tk_sha256_t* sha256) {
  return_value_if_fail(sha256 != NULL, RET_BAD_PARAMS);
  sha256_init(sha256);

  return RET_OK;
}

ret_t tk_sha256_hash(tk_sha256_t* sha256, const uint8_t* data, uint32_t len) {
  return_value_if_fail(sha256 != NULL && data != NULL, RET_BAD_PARAMS);

  sha256_hash(sha256, (uint8_t*)data, len);

  return RET_OK;
}

ret_t tk_sha256_done(tk_sha256_t* sha256, uint8_t hash[TK_SHA256_HASH_LEN + 1]) {
  return_value_if_fail(sha256 != NULL && hash != NULL, RET_BAD_PARAMS);
  sha256_done(sha256, (uint8_t*)hash);

  return RET_OK;
}

ret_t tk_sha256(const void* data, uint32_t len, str_t* hash) {
  char text[32];
  uint32_t i = 0;
  tk_sha256_t sha256;
  uint8_t result[TK_SHA256_HASH_LEN + 1];
  return_value_if_fail(data != NULL && hash != NULL, RET_BAD_PARAMS);

  tk_sha256_init(&sha256);
  tk_sha256_hash(&sha256, (const uint8_t*)data, len);
  tk_sha256_done(&sha256, result);

  for (i = 0; i < TK_SHA256_HASH_LEN; i++) {
    tk_snprintf(text, sizeof(text), "%02x", (int)(result[i]));
    str_append(hash, text);
  }

  return RET_OK;
}
