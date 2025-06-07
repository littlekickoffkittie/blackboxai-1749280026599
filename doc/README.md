# VentriQ Blockchain

VentriQ is a novel, fully headless, terminal-native cryptocurrency built with fractal-based visualization and QR code-based Proof of Work. It's implemented entirely in Chrysalis, a custom stack-based language designed specifically for experimental cryptographic systems.

## Core Features

- **QR Code Proof of Work**: Mining requires generating QR codes that encode valid hashes below a target threshold
- **Fractal Integration**: All terminal output uses Sierpiński triangle-based visualizations
- **Terminal Native**: Runs entirely in the terminal with no GUI dependencies
- **Custom Language**: Built in Chrysalis, a stack-based language inspired by Forth and WASM

## Quick Start

Install and run VentriQ with a single command:

```bash
curl -sSL https://ventriq.io/install.sh | bash
```

This will:
1. Install the Chrysalis interpreter
2. Set up the VentriQ node
3. Mine the genesis block
4. Start the node

## Project Structure

```
VentriQ/
├── src/                # Core Chrysalis source files
│   ├── main.cry       # Entry point and initialization
│   ├── blockchain.cry # Core blockchain logic
│   ├── mining.cry     # QR-based mining implementation
│   ├── fractal.cry    # Fractal generation and rendering
│   ├── terminal.cry   # Terminal UI and display
│   ├── network.cry    # P2P networking
│   ├── wallet.cry     # Wallet and transaction management
│   └── node.cry       # Node coordination
├── doc/               # Documentation
│   ├── README.md
│   ├── WHITEPAPER.md
│   ├── INSTALL.md
│   ├── LICENSE
│   ├── USAGE.md
│   ├── LANGUAGE-SPECS.md
│   ├── WALLET-ARCHITECTURE.md
│   └── SECURITY-MODEL.md
└── ext/               # Extension modules
    ├── wallet_system.cry
    ├── miner_reward_distribution.cry
    ├── peer_discovery.cry
    ├── terminal_fractal_renderer.cry
    ├── qr_pow_validator.cry
    ├── node_heartbeat_system.cry
    ├── gossip_protocol.cry
    └── chrysalis_stdlib_bootstrap.cry
```

## Key Components

### QR Code Proof of Work
- Mining difficulty is determined by QR code visual density and noise suppression
- Each block contains a QR-encoded nonce that must satisfy the current target threshold
- Visual validation provides a unique, human-verifiable consensus mechanism

### Fractal Integration
- Block headers contain unique fractal patterns identifying the block and miner
- Mining progress and network status are visualized using Sierpiński triangles
- Terminal output uses fractal-based patterns for logs and status displays

### Terminal-Native Design
- Pure terminal interface with no GUI dependencies
- ASCII art rendering of QR codes and fractals
- Real-time mining visualization using terminal graphics

### Chrysalis Language
The custom Chrysalis language provides:
- Stack-based execution model
- Built-in cryptographic primitives
- Native support for QR code generation
- Fractal computation instructions
- Terminal drawing capabilities

## Getting Started

1. Install VentriQ using the one-line installer
2. Run `ventriq start` to launch a node
3. Use `ventriq wallet create` to create a new wallet
4. Begin mining with `ventriq mine start`

See [INSTALL.md](INSTALL.md) for detailed installation instructions and [USAGE.md](USAGE.md) for complete command reference.

## Documentation

- [WHITEPAPER.md](WHITEPAPER.md) - Technical design and architecture
- [INSTALL.md](INSTALL.md) - Installation guide
- [USAGE.md](USAGE.md) - Command reference and usage
- [LANGUAGE-SPECS.md](LANGUAGE-SPECS.md) - Chrysalis language specification
- [WALLET-ARCHITECTURE.md](WALLET-ARCHITECTURE.md) - Wallet system design
- [SECURITY-MODEL.md](SECURITY-MODEL.md) - Security analysis and threat model

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) for details.

## Contributing

VentriQ is an experimental project exploring novel approaches to cryptocurrency design. Contributions are welcome! Please read our contributing guidelines before submitting pull requests.

## Security

For security issues, please see [SECURITY-MODEL.md](SECURITY-MODEL.md) and report any vulnerabilities responsibly.
