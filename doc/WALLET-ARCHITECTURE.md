# VentriQ Wallet Architecture

This document describes the technical architecture of VentriQ's wallet system, including key management, transaction handling, and integration with the fractal-based blockchain.

## Overview

The VentriQ wallet system combines traditional cryptocurrency wallet functionality with unique features:
- Fractal-based key derivation
- QR code transaction signing
- Terminal-native interface
- Visual transaction verification

## System Components

```
Wallet System
├── Key Management
│   ├── Key Generation
│   ├── Key Storage
│   └── Key Recovery
├── Transaction Management
│   ├── Creation
│   ├── Signing
│   └── Verification
├── UTXO Tracking
│   ├── UTXO Set
│   ├── Balance Calculation
│   └── UTXO Selection
└── Integration
    ├── Blockchain Interface
    ├── Network Protocol
    └── Terminal UI
```

## Key Management

### Key Generation

```chrysalis
:generate_keypair
    # Generate entropy
    RANDOM 32     # 256 bits of entropy
    
    # Add fractal entropy
    CALL fractal:generate_entropy
    XOR
    
    # Generate private key
    HASH SHA256
    STORE private_key
    
    # Generate public key
    CALL generate_public_key
    STORE public_key
    
    # Generate address
    CALL generate_address
    STORE address
    
    RETURN
```

### Key Storage

Keys are stored encrypted using:
- AES-256 encryption
- Argon2id key derivation
- Fractal-based key stretching

```chrysalis
STRUCT EncryptedWallet
    bytes   salt
    bytes   iv
    bytes   encrypted_private_key
    bytes   public_key
    bytes   address
    bytes   fractal_sigil
END
```

### Key Recovery

Recovery system using:
- 24-word mnemonic phrase
- Fractal pattern verification
- Visual recovery code

## Transaction Management

### Transaction Structure

```chrysalis
STRUCT Transaction
    u32     version
    u32     timestamp
    array   inputs
    array   outputs
    bytes   signature
    bytes   fractal_mark
END

STRUCT TxInput
    bytes32 prev_tx_hash
    u32     output_index
    bytes   signature_script
    bytes   fractal_verify
END

STRUCT TxOutput
    u64     amount
    bytes   pubkey_script
    bytes   fractal_lock
END
```

### Transaction Creation

1. **Input Selection**
   ```chrysalis
   :select_inputs
       # Stack: [amount]
       GET utxo_set
       CALL select_optimal_utxos
       IF_ERR
           RETURN_ERR
       END_IF
       RETURN
   ```

2. **Output Creation**
   ```chrysalis
   :create_outputs
       # Stack: [to_address, amount, change_amount]
       NEW_OUTPUT
           ROT     # amount
           ROT     # to_address
           CALL create_pubkey_script
       END_OUTPUT
       
       # Create change output if needed
       DUP
       PUSH 0
       GT
       IF
           NEW_OUTPUT
               SWAP    # change_amount
               GET address
               CALL create_pubkey_script
           END_OUTPUT
       END_IF
       
       RETURN
   ```

3. **Transaction Signing**
   ```chrysalis
   :sign_transaction
       # Create signature hash
       CALL create_signature_hash
       
       # Sign with private key
       GET private_key
       CALL crypto:sign_ecdsa
       
       # Add fractal signature
       CALL create_fractal_signature
       
       # Combine signatures
       CONCAT
       
       RETURN
   ```

### Transaction Verification

```chrysalis
:verify_transaction
    # Verify basic structure
    CALL verify_structure
    IF_ERR
        RETURN_ERR
    END_IF
    
    # Verify signatures
    CALL verify_signatures
    IF_ERR
        RETURN_ERR
    END_IF
    
    # Verify fractal patterns
    CALL verify_fractal_marks
    IF_ERR
        RETURN_ERR
    END_IF
    
    # Verify amounts
    CALL verify_amounts
    IF_ERR
        RETURN_ERR
    END_IF
    
    RETURN 0
```

## UTXO Management

### UTXO Set Structure

```chrysalis
STRUCT UTXO
    bytes32 tx_hash
    u32     output_index
    u64     amount
    bytes   pubkey_script
    bytes   fractal_lock
    bool    is_spent
END
```

### UTXO Tracking

1. **Adding UTXOs**
   ```chrysalis
   :add_utxo
       # Stack: [transaction, output_index]
       NEW_UTXO
           GET tx_hash
           ROT     # output_index
           GET_OUTPUT amount
           GET_OUTPUT pubkey_script
           GET_OUTPUT fractal_lock
           PUSH false   # is_spent
       END_UTXO
       
       GET utxo_set
       ARRAY_PUSH
       
       RETURN
   ```

2. **Spending UTXOs**
   ```chrysalis
   :spend_utxo
       # Stack: [tx_hash, output_index]
       GET utxo_set
       CALL find_utxo
       IF_ERR
           RETURN_ERR
       END_IF
       
       # Mark as spent
       PUSH true
       STORE_FIELD is_spent
       
       RETURN
   ```

### Balance Calculation

```chrysalis
:calculate_balance
    PUSH 0   # Initial balance
    
    GET utxo_set
    FOREACH utxo
        GET is_spent
        NOT
        IF
            GET amount
            ADD
        END_IF
    END_FOREACH
    
    RETURN
```

## Integration

### Blockchain Interface

```chrysalis
:process_new_block
    # Stack: [block]
    # Process transactions
    GET transactions
    FOREACH tx
        # Update UTXO set
        CALL process_transaction
        IF_ERR
            LOG_ERROR "Failed to process transaction"
            CONTINUE
        END_IF
    END_FOREACH
    
    # Update wallet state
    CALL update_wallet_state
    
    RETURN
```

### Network Protocol

1. **Transaction Broadcasting**
   ```chrysalis
   :broadcast_transaction
       # Stack: [transaction]
       # Verify before broadcasting
       CALL verify_transaction
       IF_ERR
           RETURN_ERR
       END_IF
       
       # Create network message
       NEW_MESSAGE
           PUSH MSG_TX
           ROT     # transaction
       END_MESSAGE
       
       # Broadcast to peers
       CALL network:broadcast
       
       RETURN
   ```

2. **Transaction Reception**
   ```chrysalis
   :receive_transaction
       # Stack: [message]
       # Extract transaction
       GET payload
       
       # Verify transaction
       CALL verify_transaction
       IF_ERR
           RETURN_ERR
       END_IF
       
       # Update wallet state
       CALL process_transaction
       
       RETURN
   ```

### Terminal UI

```chrysalis
:display_wallet
    # Clear screen
    TERMINAL:clear
    
    # Draw header
    CALL draw_wallet_header
    
    # Show balance
    CALL calculate_balance
    CALL display_balance
    
    # Show recent transactions
    CALL display_transactions
    
    # Show QR code
    GET address
    CALL display_address_qr
    
    # Show fractal pattern
    GET fractal_sigil
    CALL display_fractal
    
    RETURN
```

## Security Considerations

1. **Key Security**
   - Secure key generation
   - Encrypted storage
   - Memory protection
   - Secure key deletion

2. **Transaction Security**
   - Double-spend prevention
   - Signature verification
   - Amount validation
   - Fractal pattern verification

3. **Network Security**
   - Encrypted communication
   - Peer verification
   - Rate limiting
   - DOS protection

## Best Practices

1. **Wallet Management**
   - Regular backups
   - Password strength requirements
   - Activity monitoring
   - Update management

2. **Transaction Handling**
   - Fee estimation
   - UTXO selection optimization
   - Change address management
   - Transaction batching

3. **Error Handling**
   - Graceful degradation
   - User notification
   - Automatic recovery
   - Error logging

## Future Improvements

1. **Enhanced Security**
   - Multi-signature support
   - Hardware wallet integration
   - Advanced encryption schemes
   - Quantum resistance

2. **User Experience**
   - Improved visualization
   - Transaction categorization
   - Automated backups
   - Address book management

3. **Performance**
   - UTXO set optimization
   - Faster signature verification
   - Improved synchronization
   - Better resource management
