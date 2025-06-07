#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "crypto.h"
#include "qrcode.h"

// Extended instruction set
enum {
    OP_PUSH = 0x01,
    OP_POP = 0x02,
    OP_ADD = 0x03,
    OP_SUB = 0x04,
    OP_MUL = 0x05,
    OP_DIV = 0x06,
    OP_STORE = 0x07,
    OP_LOAD = 0x08,
    OP_CALL = 0x09,
    OP_RET = 0x0A,
    OP_JMP = 0x0B,
    OP_JZ = 0x0C,
    OP_PRINT = 0x0D,
    OP_HASH = 0x0E,
    OP_VERIFY = 0x0F,
    OP_QR_MINE = 0x10,
    OP_QR_GENERATE = 0x11,
    OP_QR_PRINT = 0x12,
    OP_QR_VERIFY = 0x13,
    OP_CONCAT = 0x14,
    OP_DUP = 0x15,
    OP_SWAP = 0x16
};

// Stack implementation
#define STACK_SIZE 1024
#define VM_MEMORY_SIZE (1024 * 1024)  // 1MB total memory
#define STRING_POOL_SIZE 4096         // 4KB for string pool
#define STRING_POOL_START 65536       // Start strings at 64KB
#define STRING_MARKER 0xFF            // Marker for string values
typedef struct {
    int data[STACK_SIZE];
    int top;
} Stack;

// Initialize stack
void stack_init(Stack *s) {
    s->top = -1;
}

// Push value to stack
bool stack_push(Stack *s, int value) {
    if (s->top < STACK_SIZE - 1) {
        s->data[++s->top] = value;
        return true;
    }
    return false;
}

// Pop value from stack
bool stack_pop(Stack *s, int *value) {
    if (s->top >= 0) {
        *value = s->data[s->top--];
        return true;
    }
    return false;
}

// Chrysalis VM
typedef struct {
    Stack stack;
    unsigned char *memory;
    size_t mem_size;
    int *call_stack;
    int call_stack_ptr;
    bool running;
} VM;

// Initialize VM
VM* vm_init(size_t mem_size) {
    VM *vm = malloc(sizeof(VM));
    if (!vm) return NULL;
    
    stack_init(&vm->stack);
    vm->memory = calloc(mem_size, 1);
    if (!vm->memory) {
        free(vm);
        return NULL;
    }
    
    vm->mem_size = mem_size;
    vm->call_stack = malloc(sizeof(int) * 1024);
    if (!vm->call_stack) {
        free(vm->memory);
        free(vm);
        return NULL;
    }
    
    vm->call_stack_ptr = 0;
    vm->running = true;
    
    return vm;
}

// Execute Chrysalis bytecode
void vm_execute(VM *vm, unsigned char *bytecode, size_t length) {
    size_t pc = 0;
    int a, b;
    
    while (pc < length && vm->running) {
        switch (bytecode[pc]) {
            case OP_PUSH:
                pc++;
                if (pc < length) {
                    stack_push(&vm->stack, bytecode[pc]);
                }
                break;
                
            case OP_POP:
                stack_pop(&vm->stack, &a);
                break;
                
            case OP_ADD:
                if (stack_pop(&vm->stack, &b) && stack_pop(&vm->stack, &a)) {
                    stack_push(&vm->stack, a + b);
                }
                break;
                
            case OP_SUB:
                if (stack_pop(&vm->stack, &b) && stack_pop(&vm->stack, &a)) {
                    stack_push(&vm->stack, a - b);
                }
                break;
                
            case OP_MUL:
                if (stack_pop(&vm->stack, &b) && stack_pop(&vm->stack, &a)) {
                    stack_push(&vm->stack, a * b);
                }
                break;
                
            case OP_DIV:
                if (stack_pop(&vm->stack, &b) && stack_pop(&vm->stack, &a)) {
                    if (b != 0) {
                        stack_push(&vm->stack, a / b);
                    }
                }
                break;
                
            case OP_STORE:
                if (stack_pop(&vm->stack, &a) && stack_pop(&vm->stack, &b)) {
                    if ((size_t)b < vm->mem_size) {
                        vm->memory[b] = a;
                    }
                }
                break;
                
            case OP_LOAD:
                if (stack_pop(&vm->stack, &a)) {
                    if ((size_t)a < vm->mem_size) {
                        stack_push(&vm->stack, vm->memory[a]);
                    }
                }
                break;
                
            case OP_QR_MINE:
                {
                    // Get target from stack
                    uint8_t target[32];
                    if (stack_pop(&vm->stack, &a)) {
                        memcpy(target, &vm->memory[a], 32);
                    }
                    
                    // Get block header from stack
                    uint8_t header[64];
                    size_t header_len = 0;
                    if (stack_pop(&vm->stack, &a)) {
                        header_len = strlen((char*)&vm->memory[a]);
                        memcpy(header, &vm->memory[a], header_len);
                    }
                    
                    // Mine for valid QR code
                    uint8_t *nonce = qrcode_generate_pow_nonce(header, header_len, target);
                    if (nonce) {
                        // Push nonce to stack
                        stack_push(&vm->stack, *(int*)nonce);
                        free(nonce);
                    } else {
                        stack_push(&vm->stack, 0);
                    }
                }
                break;
                
            case OP_QR_GENERATE:
                {
                    // Get data from stack
                    uint8_t data[128];
                    size_t data_len = 0;
                    if (stack_pop(&vm->stack, &a)) {
                        data_len = strlen((char*)&vm->memory[a]);
                        memcpy(data, &vm->memory[a], data_len);
                    }
                    
                    // Generate QR code
                    QRCode *qr = qrcode_create(data, data_len, QR_ECLEVEL_H);
                    if (qr) {
                        // Store QR code in memory
                        size_t qr_size = sizeof(QRCode) + qr->size * qr->size;
                        memcpy(&vm->memory[vm->mem_size - qr_size], qr, qr_size);
                        stack_push(&vm->stack, vm->mem_size - qr_size);
                        qrcode_destroy(qr);
                    } else {
                        stack_push(&vm->stack, 0);
                    }
                }
                break;
                
            case OP_QR_PRINT:
                {
                    // Get QR code from stack
                    if (stack_pop(&vm->stack, &a)) {
                        QRCode *qr = (QRCode*)&vm->memory[a];
                        qrcode_print(qr);
                    }
                }
                break;
                
            case OP_QR_VERIFY:
                {
                    // Get QR code and target from stack
                    QRCode *qr = NULL;
                    uint8_t target[32];
                    
                    if (stack_pop(&vm->stack, &a)) {
                        qr = (QRCode*)&vm->memory[a];
                    }
                    if (stack_pop(&vm->stack, &b)) {
                        memcpy(target, &vm->memory[b], 32);
                    }
                    
                    if (qr) {
                        bool valid = qrcode_validate_pow(qr, target);
                        stack_push(&vm->stack, valid ? 1 : 0);
                    } else {
                        stack_push(&vm->stack, 0);
                    }
                }
                break;
                
            case OP_CONCAT:
                {
                    char combined[256];
                    if (stack_pop(&vm->stack, &b) && stack_pop(&vm->stack, &a)) {
                        strcpy(combined, (char*)&vm->memory[a]);
                        strcat(combined, (char*)&vm->memory[b]);
                        size_t new_loc = vm->mem_size - strlen(combined) - 1;
                        strcpy((char*)&vm->memory[new_loc], combined);
                        stack_push(&vm->stack, new_loc);
                    }
                }
                break;
                
            case OP_DUP:
                if (vm->stack.top >= 0) {
                    stack_push(&vm->stack, vm->stack.data[vm->stack.top]);
                }
                break;
                
            case OP_SWAP:
                if (vm->stack.top >= 1) {
                    int temp = vm->stack.data[vm->stack.top];
                    vm->stack.data[vm->stack.top] = vm->stack.data[vm->stack.top - 1];
                    vm->stack.data[vm->stack.top - 1] = temp;
                }
                break;
                
            case OP_PRINT:
                if (stack_pop(&vm->stack, &a)) {
                    if (a == STRING_MARKER) {
                        if (stack_pop(&vm->stack, &a)) {
                            printf("%s\n", (char*)&vm->memory[a]);
                        }
                    } else {
                        printf("%d\n", a);
                    }
                }
                break;
        }
        pc++;
    }
}

// Compile Chrysalis source to bytecode
unsigned char* compile(const char *source, size_t *length) {
    unsigned char *bytecode = calloc(1, VM_MEMORY_SIZE);
    if (!bytecode) return NULL;
    
    *length = 0;
    static size_t str_offset = 0;
    str_offset = 0; // Reset string offset for each compilation
    char token[256];
    const char *p = source;
    
    while (*p) {
        // Skip whitespace and comments
        while (*p && ((*p == ' ' || *p == '\n' || *p == '\t') || (*p == '#'))) {
            if (*p == '#') {
                while (*p && *p != '\n') p++;
            }
            if (*p) p++;
        }
        if (!*p) break;
        
        // Read token
        int i = 0;
        while (*p && *p != ' ' && *p != '\n' && *p != '\t' && *p != '#' && i < 255) {
            token[i++] = *p++;
        }
        token[i] = '\0';
        
        // Parse token
        if (strcmp(token, "PUSH") == 0) {
            bytecode[(*length)++] = OP_PUSH;
            while (*p && (*p == ' ' || *p == '\t')) p++;
            if (*p == '"') {
                p++; // Skip opening quote
                static size_t str_offset = 0;
                size_t str_loc = STRING_POOL_START + str_offset;
                i = 0;
                
                // Store string in memory pool
                while (*p && *p != '"') {
                    bytecode[str_loc + i] = *p++;
                    i++;
                }
                bytecode[str_loc + i] = '\0';
                if (*p == '"') p++;
                
                // Push string marker and location
                bytecode[(*length)++] = OP_PUSH;
                bytecode[(*length)++] = STRING_MARKER;
                bytecode[(*length)++] = OP_PUSH;
                bytecode[(*length)++] = str_loc;
                
                str_offset += i + 1;
            } else {
                bytecode[(*length)++] = atoi(p);
                while (*p && *p >= '0' && *p <= '9') p++;
            }
        }
        else if (strcmp(token, "POP") == 0) bytecode[(*length)++] = OP_POP;
        else if (strcmp(token, "ADD") == 0) bytecode[(*length)++] = OP_ADD;
        else if (strcmp(token, "SUB") == 0) bytecode[(*length)++] = OP_SUB;
        else if (strcmp(token, "MUL") == 0) bytecode[(*length)++] = OP_MUL;
        else if (strcmp(token, "DIV") == 0) bytecode[(*length)++] = OP_DIV;
        else if (strcmp(token, "STORE") == 0) bytecode[(*length)++] = OP_STORE;
        else if (strcmp(token, "LOAD") == 0) bytecode[(*length)++] = OP_LOAD;
        else if (strcmp(token, "CALL") == 0) {
            bytecode[(*length)++] = OP_CALL;
            while (*p && (*p == ' ' || *p == '\t')) p++;
            if (strncmp(p, "qr_mine", 7) == 0) {
                bytecode[(*length)++] = OP_QR_MINE;
                p += 7;
            }
            else if (strncmp(p, "qr_generate", 11) == 0) {
                bytecode[(*length)++] = OP_QR_GENERATE;
                p += 11;
            }
            else if (strncmp(p, "qr_print", 8) == 0) {
                bytecode[(*length)++] = OP_QR_PRINT;
                p += 8;
            }
            else if (strncmp(p, "qr_verify", 9) == 0) {
                bytecode[(*length)++] = OP_QR_VERIFY;
                p += 9;
            }
        }
        else if (strcmp(token, "CONCAT") == 0) bytecode[(*length)++] = OP_CONCAT;
        else if (strcmp(token, "DUP") == 0) bytecode[(*length)++] = OP_DUP;
        else if (strcmp(token, "SWAP") == 0) bytecode[(*length)++] = OP_SWAP;
        else if (strcmp(token, "PRINT") == 0) bytecode[(*length)++] = OP_PRINT;
    }
    
    return bytecode;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *source = malloc(size + 1);
    if (!source) {
        fclose(f);
        return 1;
    }
    
    size_t bytes_read = fread(source, 1, size, f);
    if (bytes_read != (size_t)size) {
        free(source);
        fclose(f);
        return 1;
    }
    source[size] = '\0';
    fclose(f);

    size_t bytecode_length;
    unsigned char *bytecode = compile(source, &bytecode_length);
    if (!bytecode) {
        free(source);
        return 1;
    }

    VM *vm = vm_init(VM_MEMORY_SIZE);
    if (!vm) {
        free(source);
        free(bytecode);
        return 1;
    }
    
    // Copy string pool to VM memory
    memcpy(vm->memory + STRING_POOL_START, 
           bytecode + STRING_POOL_START, 
           STRING_POOL_SIZE);

    vm_execute(vm, bytecode, bytecode_length);

    free(source);
    free(bytecode);
    free(vm->memory);
    free(vm->call_stack);
    free(vm);

    return 0;
}
