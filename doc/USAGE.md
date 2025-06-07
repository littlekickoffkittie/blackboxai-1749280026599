# VentriQ Usage Guide

This guide provides comprehensive documentation for using VentriQ, including all available commands, configuration options, and best practices.

## Command Line Interface

VentriQ provides a terminal-native interface with the following main commands:

### Basic Commands

```bash
ventriq start              # Start VentriQ node
ventriq stop               # Stop VentriQ node
ventriq status            # Show node status
ventriq version           # Show version information
ventriq help              # Show help message
```

### Node Management

```bash
# Start node with options
ventriq start [options]
  --mine                  # Start with mining enabled
  --port <port>          # Specify port (default: 8333)
  --peers <max>          # Maximum number of peers
  --log-level <level>    # Set log level (debug|info|warn|error)

# View node information
ventriq info             # Show detailed node information
ventriq peers            # List connected peers
ventriq metrics          # Show performance metrics
```

### Mining Operations

```bash
ventriq mine start       # Start mining
ventriq mine stop        # Stop mining
ventriq mine status      # Show mining status
ventriq mine info        # Show mining statistics

# Mining configuration
ventriq mine config [options]
  --threads <n>          # Set number of mining threads
  --reward <address>     # Set reward address
```

### Wallet Management

```bash
# Wallet commands
ventriq wallet create    # Create new wallet
ventriq wallet list      # List wallets
ventriq wallet backup    # Backup wallet
ventriq wallet restore   # Restore wallet from backup

# Transaction operations
ventriq tx send <to> <amount>    # Send transaction
ventriq tx list                  # List transactions
ventriq tx info <txid>          # Show transaction details
```

### Blockchain Operations

```bash
ventriq chain info       # Show blockchain information
ventriq chain verify     # Verify blockchain integrity
ventriq chain export     # Export blockchain data
ventriq block <height>   # Show block information
```

## Terminal Interface

### Main Screen Layout

```
╔════════════════════════════════════════╗
║            VentriQ Node                ║
╚════════════════════════════════════════╝

Mining Status:
Hash Rate: 1234 H/s
Target: 0000fffff...

[QR Code Visualization]
░░░▓▓▓░░░
░▓░░░░░▓░
░░░▓▓▓░░░

[Fractal Pattern]
   ▲
  ▲ ▲
 ▲   ▲
▲ ▲ ▲ ▲

Network Status: 12 peers connected
Chain Height: 1234
```

### Interactive Commands

While the node is running, you can use these keyboard shortcuts:

```
m       Toggle mining
i       Show detailed information
c       Clear screen
h       Show help
q       Quit
```

## Configuration

### Configuration File

Located at `~/.ventriq/ventriq.conf`:

```ini
# Network Configuration
port = 8333
max_peers = 50
min_peers = 3
bootstrap_nodes = [
    "node1.ventriq.io:8333",
    "node2.ventriq.io:8333"
]

# Mining Configuration
mining_threads = 4
mining_reward_address = "vq1..."
qr_min_density = 0.3
qr_max_noise = 0.2

# Node Configuration
node_name = "MyVentriQNode"
log_level = "info"
data_dir = "~/.ventriq/data"
```

### Environment Variables

```bash
VENTRIQ_HOME          # VentriQ home directory
VENTRIQ_NETWORK       # Network (mainnet|testnet)
VENTRIQ_LOG_LEVEL    # Log level
VENTRIQ_NODE_NAME    # Node name
```

## Visual Elements

### QR Code Mining

The mining process displays QR codes representing potential solutions:

```
Candidate QR Code:
╔════════════════╗
║ ▓▓░░▓▓░░▓▓░░  ║
║ ░░▓▓░░▓▓░░▓▓  ║
║ ▓▓░░▓▓░░▓▓░░  ║
╚════════════════╝

Density: 0.45
Noise: 0.15
```

### Fractal Patterns

Block headers are visualized using Sierpiński triangles:

```
Block #1234 Fractal Pattern:
      ▲
     ▲ ▲
    ▲   ▲
   ▲ ▲ ▲ ▲
  ▲       ▲
 ▲ ▲     ▲ ▲
▲   ▲   ▲   ▲
```

## Monitoring and Maintenance

### Log Files

```bash
~/.ventriq/logs/
├── ventriq.log        # Main log file
├── mining.log         # Mining activity
├── network.log        # Network operations
└── error.log          # Error messages
```

### Diagnostic Commands

```bash
ventriq diagnostic           # Generate diagnostic report
ventriq verify-installation  # Verify installation
ventriq test-network        # Test network connectivity
ventriq benchmark           # Run performance benchmark
```

### Maintenance Commands

```bash
ventriq backup              # Backup node data
ventriq restore <backup>    # Restore from backup
ventriq clean              # Clean temporary files
ventriq reset              # Reset node state
```

## Advanced Usage

### Custom Mining Strategies

```bash
# Configure mining parameters
ventriq mine config custom
  --qr-density <value>     # Set QR code density threshold
  --noise-tolerance <value> # Set noise tolerance
  --fractal-depth <depth>  # Set fractal pattern depth
```

### Network Analysis

```bash
ventriq analyze peers      # Analyze peer performance
ventriq analyze chain      # Analyze blockchain state
ventriq analyze blocks     # Analyze block propagation
```

### Development Tools

```bash
ventriq dev console        # Start development console
ventriq dev test          # Run test suite
ventriq dev debug         # Enable debug mode
```

## Best Practices

1. **Node Operation**
   - Maintain stable network connection
   - Regular backups of wallet and configuration
   - Monitor system resources

2. **Mining**
   - Adjust thread count based on CPU cores
   - Monitor temperature and resource usage
   - Regular verification of mining rewards

3. **Security**
   - Regular password changes
   - Backup wallet keys securely
   - Keep software updated

4. **Troubleshooting**
   - Check logs for errors
   - Verify network connectivity
   - Monitor peer connections

## Error Messages

Common error messages and their solutions:

1. **"Failed to connect to peers"**
   - Check network connection
   - Verify port forwarding
   - Check firewall settings

2. **"Invalid QR code generation"**
   - Verify terminal supports required characters
   - Check system resources
   - Update terminal emulator

3. **"Mining failed to start"**
   - Check system resources
   - Verify wallet configuration
   - Check mining permissions

## Additional Resources

- [WHITEPAPER.md](WHITEPAPER.md) - Technical details
- [SECURITY-MODEL.md](SECURITY-MODEL.md) - Security information
- [LANGUAGE-SPECS.md](LANGUAGE-SPECS.md) - Chrysalis language reference
- [WALLET-ARCHITECTURE.md](WALLET-ARCHITECTURE.md) - Wallet system details
