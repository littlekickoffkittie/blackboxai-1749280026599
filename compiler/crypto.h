#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <stddef.h>

// Hash functions
void sha256(const unsigned char *data, size_t len, unsigned char *hash);
void ripemd160(const unsigned char *data, size_t len, unsigned char *hash);
void hash160(const unsigned char *data, size_t len, unsigned char *hash);

// Key generation and signing
EC_KEY* generate_key_pair(void);
int sign_data(EC_KEY *key, const unsigned char *data, size_t len, unsigned char *sig, size_t *sig_len);
int verify_signature(EC_KEY *key, const unsigned char *data, size_t len, const unsigned char *sig, size_t sig_len);

// OpenSSL initialization and cleanup
void init_crypto(void);
void cleanup_crypto(void);

#endif /* CRYPTO_H */
