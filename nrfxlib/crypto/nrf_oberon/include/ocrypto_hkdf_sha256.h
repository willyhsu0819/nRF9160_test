/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_hkdf HKDF - HMAC based Key Derivation Function
 * @ingroup nrf_oberon
 * @{
 * @brief HKDF is a key derivation function based on HMAC Extract-and-Expand
 * @}
 *
 * @defgroup nrf_oberon_hkdf_256 HKDF APIs using SHA-256
 * @ingroup nrf_oberon_hkdf
 * @{
 * @brief Type declarations and APIs for the HKDF-SHA256 algorithm.
 *
 * HKDF-SHA256 is a key derivation function based on HMAC-SHA256.
 *
 * @see [RFC 5869 - HMAC-based Extract-and-Expand Key Derivation Function (HKDF)](http://tools.ietf.org/html/rfc5869)
 */

#ifndef OCRYPTO_HKDF_SHA256_H
#define OCRYPTO_HKDF_SHA256_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>


/**
 * Maximum length of a derived key.
 */
#define ocrypto_hkdf_sha256_LENGTH_MAX (32)

/**
 * Maximum salt length.
 */
#define ocrypto_hkdf_sha256_SALT_LENGTH_MAX (64)


/**
 * HKDF-SHA256 algorithm.
 *
 * A new pseudo-random key of length @p r_len is derived from an input key
 * @p key, a salt @p salt and additional information @p info. The new key is put
 * into @p r.
 *
 * @param[out] r        Output key.
 * @param      r_len    Length of @p r, 0 < @p r_len <= @c ocrypto_hkdf_sha256_LENGTH_MAX.
 * @param      key      Input key.
 * @param      key_len  Length of @p key.
 * @param      salt     Salt.
 * @param      salt_len Length of salt @p salt. 0 <= @p salt_len <= @c ocrypto_hkdf_sha256_SALT_LENGTH_MAX.
 * @param      info     Additional information.
 * @param      info_len Length of @p info.
 */
void ocrypto_hkdf_sha256(
    uint8_t* r, size_t r_len,
    const uint8_t* key, size_t key_len,
    const uint8_t* salt, size_t salt_len,
    const uint8_t* info, size_t info_len);

#ifdef __cplusplus
}
#endif

#endif

/** @} */
