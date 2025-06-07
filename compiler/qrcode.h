#ifndef QRCODE_H
#define QRCODE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// QR Code error correction levels
typedef enum {
    QR_ECLEVEL_L = 0,  // 7% recovery
    QR_ECLEVEL_M = 1,  // 15% recovery
    QR_ECLEVEL_Q = 2,  // 25% recovery
    QR_ECLEVEL_H = 3   // 30% recovery
} QRCodeECC;

// QR Code structure
typedef struct {
    int version;        // QR Code version (1-40)
    int size;          // Size of QR Code (modules)
    QRCodeECC ecc;     // Error correction level
    uint8_t *modules;  // Module matrix
    float density;     // Module density
    float noise;       // Visual noise ratio
} QRCode;

// QR Code generation functions
QRCode* qrcode_create(const uint8_t *data, size_t length, QRCodeECC ecc);
void qrcode_destroy(QRCode *qr);

// QR Code validation functions
bool qrcode_validate_pow(const QRCode *qr, const uint8_t *target);
float qrcode_calculate_density(const QRCode *qr);
float qrcode_calculate_noise(const QRCode *qr);

// QR Code visualization
void qrcode_print(const QRCode *qr);
char* qrcode_get_ascii(const QRCode *qr);

// Mining-specific functions
bool qrcode_check_mining_criteria(const QRCode *qr, float min_density, float max_noise);
uint8_t* qrcode_generate_pow_nonce(const uint8_t *block_header, size_t length, const uint8_t *target);

// Utility functions
void qrcode_get_module(const QRCode *qr, int x, int y, bool *module);
void qrcode_set_module(QRCode *qr, int x, int y, bool value);
bool qrcode_is_valid_version(int version);
int qrcode_get_size_for_version(int version);

#endif /* QRCODE_H */
