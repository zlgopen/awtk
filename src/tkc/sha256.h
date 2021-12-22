
#ifndef TK_SHA256_H
#define TK_SHA256_H

#include "tkc/str.h"

BEGIN_C_DECLS

/**
 * @class tk_sha256_t
 * sha256
 */
typedef struct {
  uint32_t buf[16];
  uint32_t hash[8];
  uint32_t len[2];
} tk_sha256_t;

#define TK_SHA256_HASH_LEN 32

/**
 * @method tk_sha256_init
 * 初始化。
 * @param {tk_sha256_t*} sha256 sha256对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_sha256_init(tk_sha256_t* sha256);

/**
 * @method tk_sha256_hash
 * 更新数据。
 * @param {tk_sha256_t*} sha256 sha256对象。
 * @param {const uint8_t*} data 数据。
 * @param {uint32_t} len 数据长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_sha256_hash(tk_sha256_t* sha256, const uint8_t* data, uint32_t len);

/**
 * @method tk_sha256_done
 * 完成计算。
 * @param {tk_sha256_t*} sha256 sha256对象。
 * @param {uint8_t*} hash 用于返回计算结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_sha256_done(tk_sha256_t* sha256, uint8_t hash[TK_SHA256_HASH_LEN + 1]);

/**
 * @method tk_sha256
 * 计算数据的sha256哈希值。
 * @param {const void*} data 数据。
 * @param {uint32_t} len 数据长度。
 * @param {str_t*} hash 用于返回计算结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_sha256(const void* data, uint32_t len, str_t* hash);

END_C_DECLS

#endif /*TK_SHA256_H*/
