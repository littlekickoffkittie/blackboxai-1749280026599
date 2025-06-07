# Chrysalis Language Specification

Chrysalis is a stack-based, postfix-notation programming language designed specifically for VentriQ's cryptocurrency implementation. This document provides the complete language specification.

## Table of Contents

1. [Language Overview](#language-overview)
2. [Core Concepts](#core-concepts)
3. [Data Types](#data-types)
4. [Stack Operations](#stack-operations)
5. [Control Flow](#control-flow)
6. [Built-in Instructions](#built-in-instructions)
7. [Memory Model](#memory-model)
8. [Error Handling](#error-handling)
9. [File Format](#file-format)
10. [Standard Library](#standard-library)

## Language Overview

Chrysalis is:
- Stack-based: operations manipulate a data stack
- Postfix notation: operators follow their operands
- Statically typed: type checking at compile time
- Module-based: code organized in importable modules
- Cryptocurrency-focused: built-in crypto primitives

Example:
```chrysalis
# Calculate hash of block header
:hash_block_header
    # Stack: [block]
    DUP
    GET index
    SWAP
    GET prev_hash
    SWAP
    GET timestamp
    CONCAT
    HASH SHA256
    RETURN
```

## Core Concepts

### 1. Stack Manipulation

The stack is the primary data structure:
```chrysalis
PUSH 5      # Stack: [5]
PUSH 3      # Stack: [5, 3]
ADD         # Stack: [8]
```

### 2. Words (Functions)

Functions are defined using colons:
```chrysalis
:square
    DUP     # Duplicate top value
    MUL     # Multiply top two values
    RETURN
```

### 3. Comments

```chrysalis
# Single line comment

#[
    Multi-line
    comment
]#
```

## Data Types

### Basic Types

```chrysalis
bool    # true or false
u8      # 8-bit unsigned integer
u16     # 16-bit unsigned integer
u32     # 32-bit unsigned integer
u64     # 64-bit unsigned integer
i32     # 32-bit signed integer
i64     # 64-bit signed integer
f32     # 32-bit float
f64     # 64-bit float
str     # UTF-8 string
bytes   # Byte array
array   # Dynamic array
```

### Custom Types

```chrysalis
STRUCT Block
    u32     index
    bytes32 prev_hash
    u64     timestamp
    bytes   qr_nonce
    bytes   fractal_sigil
END

ENUM TxType
    REGULAR
    COINBASE
    CONTRACT
END
```

## Stack Operations

### Basic Operations

```chrysalis
PUSH    # Push value onto stack
POP     # Remove top value
DUP     # Duplicate top value
SWAP    # Swap top two values
ROT     # Rotate top three values
OVER    # Copy second item to top
DROP    # Remove top value
```

### Stack Manipulation Examples

```chrysalis
PUSH 1      # Stack: [1]
PUSH 2      # Stack: [1, 2]
PUSH 3      # Stack: [1, 2, 3]
ROT         # Stack: [2, 3, 1]
SWAP        # Stack: [2, 1, 3]
DUP         # Stack: [2, 1, 3, 3]
```

## Control Flow

### Conditionals

```chrysalis
IF condition
    # code if true
ELSE
    # code if false
END_IF

IF_NOT condition
    # code if false
END_IF
```

### Loops

```chrysalis
WHILE condition
    # loop body
END_WHILE

LOOP n
    # repeated n times
END_LOOP

FOREACH item
    # process each item
END_FOREACH
```

### Error Handling

```chrysalis
TRY
    # code that might fail
CATCH
    # error handling
END_TRY
```

## Built-in Instructions

### Cryptographic Operations

```chrysalis
HASH SHA256      # Calculate SHA256 hash
HASH RIPEMD160   # Calculate RIPEMD160 hash
SIGN             # Sign data with private key
VERIFY           # Verify signature
```

### QR Code Operations

```chrysalis
QRGEN           # Generate QR code
QR_VALIDATE     # Validate QR properties
QR_DECODE       # Decode QR code
```

### Fractal Operations

```chrysalis
FRACTAL_GEN     # Generate fractal pattern
FRACTAL_VERIFY  # Verify fractal properties
FRACTAL_DRAW    # Draw fractal to terminal
```

### Terminal Operations

```chrysalis
PRINT           # Print to terminal
CLEAR           # Clear terminal screen
DRAW            # Draw to terminal
GET_CHAR        # Get character input
```

### Network Operations

```chrysalis
SOCKET_LISTEN   # Listen for connections
SOCKET_ACCEPT   # Accept connection
SOCKET_SEND     # Send data
SOCKET_RECV     # Receive data
```

## Memory Model

### Storage Types

```chrysalis
CONST           # Immutable constant
STORE           # Persistent storage
MEMORY          # Temporary memory
```

### Memory Operations

```chrysalis
ALLOC           # Allocate memory
FREE            # Free memory
MEMCPY          # Copy memory
MEMSET          # Set memory
```

## Error Handling

### Error Types

```chrysalis
ERROR_NONE      # No error
ERROR_MEMORY    # Memory error
ERROR_STACK     # Stack error
ERROR_TYPE      # Type error
ERROR_IO        # I/O error
ERROR_NETWORK   # Network error
```

### Error Handling Example

```chrysalis
:safe_operation
    TRY
        # Potentially failing operation
        CALL risky_function
        IF_ERR
            LOG_ERROR "Operation failed"
            RETURN_ERR
        END_IF
    CATCH
        # Handle error
        LOG_ERROR "Exception caught"
        RETURN 1
    END_TRY
    RETURN 0
```

## File Format

### File Structure

```chrysalis
# Module imports
IMPORT "other_module.cry"

# Constants
CONST VERSION 1

# Type definitions
STRUCT MyType
    # fields
END

# Global storage
:storage
    u32 counter
    str status
END

# Function definitions
:function_name
    # function body
END
```

## Standard Library

### Standard Modules

```chrysalis
std::math       # Mathematical operations
std::crypto     # Cryptographic functions
std::network    # Network operations
std::terminal   # Terminal I/O
std::memory     # Memory management
```

### Example Usage

```chrysalis
IMPORT "std::math"

:calculate_sqrt
    PUSH 16
    SQRT    # From std::math
    RETURN
```

## Best Practices

1. **Stack Management**
   - Document stack effects in comments
   - Clean up stack before returning
   - Use meaningful variable names

2. **Error Handling**
   - Always check for errors
   - Provide meaningful error messages
   - Clean up resources in error cases

3. **Code Organization**
   - Group related functions
   - Use meaningful names
   - Document complex operations

4. **Performance**
   - Minimize memory allocations
   - Use appropriate data structures
   - Profile critical sections

## Implementation Notes

1. **Compilation**
   ```bash
   chrysalis build source.cry
   ```

2. **Execution**
   ```bash
   chrysalis run binary
   ```

3. **Debugging**
   ```bash
   chrysalis debug source.cry
   ```

## Language Extensions

Chrysalis can be extended through:
1. Custom instructions
2. Native function bindings
3. Module system
4. Preprocessor directives

## Security Considerations

1. **Memory Safety**
   - Bounds checking
   - Type safety
   - Resource cleanup

2. **Cryptographic Operations**
   - Secure random numbers
   - Key management
   - Side-channel protection

3. **Input Validation**
   - Stack depth checks
   - Type verification
   - Resource limits
