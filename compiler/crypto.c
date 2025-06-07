#include "crypto.h"
#include <string.h>
#include <openssl/err.h>

void sha256(const unsigned char *data, size_t len, unsigned char *hash) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, len);
    SHA256_Final(hash, &sha256);
}

void ripemd160(const unsigned char *data, size_t len, unsigned char *hash) {
    RIPEMD160_CTX ripemd160;
    RIPEMD160_Init(&ripemd160);
    RIPEMD160_Update(&ripemd160, data, len);
    RIPEMD160_Final(hash, &ripemd160);
}

void hash160(const unsigned char *data, size_t len, unsigned char *hash) {
    unsigned char sha256_hash[SHA256_DIGEST_LENGTH];
    
    // First SHA256
    sha256(data, len, sha256_hash);
    
    // Then RIPEMD160
    ripemd160(sha256_hash, SHA256_DIGEST_LENGTH, hash);
}

EC_KEY* generate_key_pair(void) {
    EC_KEY *key = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (key == NULL) {
        return NULL;
    }
    
    if (!EC_KEY_generate_key(key)) {
        EC_KEY_free(key);
        return NULL;
    }
    
    return key;
}

int sign_data(EC_KEY *key, const unsigned char *data, size_t len, unsigned char *sig, size_t *sig_len) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    sha256(data, len, hash);
    
    ECDSA_SIG *signature = ECDSA_do_sign(hash, SHA256_DIGEST_LENGTH, key);
    if (signature == NULL) {
        return 0;
    }
    
    const BIGNUM *r, *s;
    ECDSA_SIG_get0(signature, &r, &s);
    
    // Convert signature to DER format
    unsigned char *der = NULL;
    int der_len = i2d_ECDSA_SIG(signature, &der);
    if (der_len < 0) {
        ECDSA_SIG_free(signature);
        return 0;
    }
    
    // Copy signature to output buffer
    if ((size_t)der_len <= *sig_len) {
        memcpy(sig, der, der_len);
        *sig_len = der_len;
    }
    
    OPENSSL_free(der);
    ECDSA_SIG_free(signature);
    
    return 1;
}

int verify_signature(EC_KEY *key, const unsigned char *data, size_t len, const unsigned char *sig, size_t sig_len) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    sha256(data, len, hash);
    
    // Convert DER signature back to ECDSA_SIG
    const unsigned char *sig_ptr = sig;
    ECDSA_SIG *signature = d2i_ECDSA_SIG(NULL, &sig_ptr, sig_len);
    if (signature == NULL) {
        return 0;
    }
    
    int result = ECDSA_do_verify(hash, SHA256_DIGEST_LENGTH, signature, key);
    
    ECDSA_SIG_free(signature);
    
    return result;
}

// Initialize OpenSSL
__attribute__((constructor))
void init_crypto(void) {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
}

// Cleanup OpenSSL
__attribute__((destructor))
void cleanup_crypto(void) {
    EVP_cleanup();
    ERR_free_strings();
}
