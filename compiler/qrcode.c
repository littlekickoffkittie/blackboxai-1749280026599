#include "qrcode.h"
#include "crypto.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// QR Code constants
#define MAX_VERSION 40
#define MIN_VERSION 1
#define MIN_DENSITY 0.3
#define MAX_DENSITY 0.8
#define MAX_NOISE 0.2

// Internal helper functions
static void initialize_modules(QRCode *qr);
static void add_finder_patterns(QRCode *qr);
static void add_alignment_patterns(QRCode *qr);
static void add_timing_patterns(QRCode *qr);
static void add_format_info(QRCode *qr);
static void add_data(QRCode *qr, const uint8_t *data, size_t length);

QRCode* qrcode_create(const uint8_t *data, size_t length, QRCodeECC ecc) {
    // Calculate required version for data length
    int version = 1;
    while (version <= MAX_VERSION) {
        int capacity = qrcode_get_size_for_version(version) * qrcode_get_size_for_version(version) / 8;
        if (capacity >= length) break;
        version++;
    }
    
    if (version > MAX_VERSION) return NULL;
    
    // Allocate QR Code structure
    QRCode *qr = malloc(sizeof(QRCode));
    if (!qr) return NULL;
    
    qr->version = version;
    qr->size = qrcode_get_size_for_version(version);
    qr->ecc = ecc;
    
    // Allocate module matrix
    size_t matrix_size = qr->size * qr->size;
    qr->modules = calloc(matrix_size, sizeof(uint8_t));
    if (!qr->modules) {
        free(qr);
        return NULL;
    }
    
    // Initialize QR Code
    initialize_modules(qr);
    add_finder_patterns(qr);
    add_alignment_patterns(qr);
    add_timing_patterns(qr);
    add_format_info(qr);
    add_data(qr, data, length);
    
    // Calculate metrics
    qr->density = qrcode_calculate_density(qr);
    qr->noise = qrcode_calculate_noise(qr);
    
    return qr;
}

void qrcode_destroy(QRCode *qr) {
    if (qr) {
        free(qr->modules);
        free(qr);
    }
}

bool qrcode_validate_pow(const QRCode *qr, const uint8_t *target) {
    if (!qr || !target) return false;
    
    // Check density requirements
    if (qr->density < MIN_DENSITY || qr->density > MAX_DENSITY) return false;
    
    // Check noise ratio
    if (qr->noise > MAX_NOISE) return false;
    
    // Calculate QR code hash
    uint8_t hash[SHA256_DIGEST_LENGTH];
    sha256((uint8_t*)qr->modules, qr->size * qr->size, hash);
    
    // Compare with target
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        if (hash[i] > target[i]) return false;
        if (hash[i] < target[i]) return true;
    }
    
    return true;
}

float qrcode_calculate_density(const QRCode *qr) {
    if (!qr) return 0.0f;
    
    int set_modules = 0;
    size_t total_modules = qr->size * qr->size;
    
    for (size_t i = 0; i < total_modules; i++) {
        if (qr->modules[i]) set_modules++;
    }
    
    return (float)set_modules / total_modules;
}

float qrcode_calculate_noise(const QRCode *qr) {
    if (!qr) return 1.0f;
    
    int noise_count = 0;
    
    // Check each module against its neighbors
    for (int y = 1; y < qr->size - 1; y++) {
        for (int x = 1; x < qr->size - 1; x++) {
            bool current = qr->modules[y * qr->size + x];
            int different_neighbors = 0;
            
            // Check 8 neighboring modules
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx == 0 && dy == 0) continue;
                    
                    bool neighbor = qr->modules[(y + dy) * qr->size + (x + dx)];
                    if (neighbor != current) different_neighbors++;
                }
            }
            
            // If more than 5 different neighbors, consider it noise
            if (different_neighbors > 5) noise_count++;
        }
    }
    
    return (float)noise_count / (qr->size * qr->size);
}

void qrcode_print(const QRCode *qr) {
    if (!qr) return;
    
    // Print top border
    printf("╔");
    for (int i = 0; i < qr->size * 2; i++) printf("═");
    printf("╗\n");
    
    // Print QR code
    for (int y = 0; y < qr->size; y++) {
        printf("║");
        for (int x = 0; x < qr->size; x++) {
            printf(qr->modules[y * qr->size + x] ? "██" : "  ");
        }
        printf("║\n");
    }
    
    // Print bottom border
    printf("╚");
    for (int i = 0; i < qr->size * 2; i++) printf("═");
    printf("╝\n");
}

char* qrcode_get_ascii(const QRCode *qr) {
    if (!qr) return NULL;
    
    size_t buf_size = (qr->size + 1) * (qr->size + 1);
    char *ascii = malloc(buf_size);
    if (!ascii) return NULL;
    
    size_t pos = 0;
    for (int y = 0; y < qr->size; y++) {
        for (int x = 0; x < qr->size; x++) {
            ascii[pos++] = qr->modules[y * qr->size + x] ? '█' : ' ';
        }
        ascii[pos++] = '\n';
    }
    ascii[pos] = '\0';
    
    return ascii;
}

bool qrcode_check_mining_criteria(const QRCode *qr, float min_density, float max_noise) {
    if (!qr) return false;
    
    return (qr->density >= min_density &&
            qr->density <= MAX_DENSITY &&
            qr->noise <= max_noise);
}

uint8_t* qrcode_generate_pow_nonce(const uint8_t *block_header, size_t length, const uint8_t *target) {
    printf("Starting mining function\n");
    uint8_t *nonce = malloc(32);
    if (!nonce) {
        printf("Failed to allocate nonce\n");
        return NULL;
    }
    
    uint64_t attempt = 0;
    bool found = false;
    // Remove max_attempts limit for real mining
    while (!found) {
        if (attempt % 1000 == 0) {
            printf("Mining attempt: %lu\n", (unsigned long)attempt);
        }
        
        // Create test data with nonce
        size_t test_len = length + 8;
        uint8_t *test_data = malloc(test_len);
        if (!test_data) {
            printf("Failed to allocate test_data\n");
            free(nonce);
            return NULL;
        }
        
        memcpy(test_data, block_header, length);
        memcpy(test_data + length, &attempt, 8);
        
        // Generate QR code
        QRCode *qr = qrcode_create(test_data, test_len, QR_ECLEVEL_H);
        if (qr) {
            if (qrcode_validate_pow(qr, target)) {
                printf("Found valid nonce: %lu\n", (unsigned long)attempt);
                memcpy(nonce, &attempt, 8);
                found = true;
            }
            qrcode_destroy(qr);
        }
        
        free(test_data);
        attempt++;
    }
    
    return nonce;
}

bool qrcode_is_valid_version(int version) {
    return version >= MIN_VERSION && version <= MAX_VERSION;
}

int qrcode_get_size_for_version(int version) {
    if (!qrcode_is_valid_version(version)) return 0;
    return 17 + 4 * version;
}

// Internal implementation of helper functions
static void initialize_modules(QRCode *qr) {
    memset(qr->modules, 0, qr->size * qr->size);
}

static void add_finder_patterns(QRCode *qr) {
    // Add the three finder patterns
    const int positions[][2] = {
        {0, 0},                          // Top-left
        {qr->size - 7, 0},              // Top-right
        {0, qr->size - 7}               // Bottom-left
    };
    
    for (int p = 0; p < 3; p++) {
        int row = positions[p][0];
        int col = positions[p][1];
        
        // Draw 7x7 finder pattern
        for (int r = 0; r < 7; r++) {
            for (int c = 0; c < 7; c++) {
                bool is_border = (r == 0 || r == 6 || c == 0 || c == 6);
                bool is_inner = (r >= 2 && r <= 4 && c >= 2 && c <= 4);
                qr->modules[(row + r) * qr->size + (col + c)] = is_border || is_inner;
            }
        }
    }
}

static void add_alignment_patterns(QRCode *qr) {
    // Simplified alignment pattern placement
    if (qr->version > 1) {
        int pos = qr->size - 9;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                qr->modules[(pos + i) * qr->size + (pos + j)] = (i == 0 || i == 4 || j == 0 || j == 4);
            }
        }
    }
}

static void add_timing_patterns(QRCode *qr) {
    // Add horizontal timing pattern
    for (int i = 8; i < qr->size - 8; i++) {
        qr->modules[6 * qr->size + i] = (i % 2 == 0);
    }
    
    // Add vertical timing pattern
    for (int i = 8; i < qr->size - 8; i++) {
        qr->modules[i * qr->size + 6] = (i % 2 == 0);
    }
}

static void add_format_info(QRCode *qr) {
    // Simplified format information
    uint16_t format = (uint16_t)qr->ecc << 13;
    
    // Add format info around top-left finder pattern
    for (int i = 0; i < 15; i++) {
        bool bit = (format >> i) & 1;
        if (i < 6) {
            qr->modules[i * qr->size + 8] = bit;
        } else if (i < 8) {
            qr->modules[(i + 1) * qr->size + 8] = bit;
        } else {
            qr->modules[8 * qr->size + (qr->size - 15 + i)] = bit;
        }
    }
}

static void add_data(QRCode *qr, const uint8_t *data, size_t length) {
    // Simplified data encoding
    int pos = qr->size - 1;
    int up = true;
    size_t data_idx = 0;
    
    while (pos >= 0 && data_idx < length) {
        int row = up ? qr->size - 1 : 0;
        int row_end = up ? -1 : qr->size;
        int row_step = up ? -1 : 1;
        
        for (; row != row_end; row += row_step) {
            for (int col = pos; col > pos - 2 && col >= 0; col--) {
                if (!qr->modules[row * qr->size + col]) {
                    qr->modules[row * qr->size + col] = (data[data_idx / 8] >> (7 - (data_idx % 8))) & 1;
                    data_idx++;
                    if (data_idx >= length * 8) return;
                }
            }
        }
        
        pos -= 2;
        up = !up;
    }
}
