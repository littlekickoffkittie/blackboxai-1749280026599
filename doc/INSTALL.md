# VentriQ Installation Guide

This guide provides detailed instructions for installing and running a VentriQ node. VentriQ is designed to be terminal-native and requires minimal dependencies.

## Quick Install

The fastest way to get started is using our one-line installer:

```bash
curl -sSL https://ventriq.io/install.sh | bash
```

This command will:
1. Download and verify the installation script
2. Install the Chrysalis interpreter
3. Set up VentriQ directories
4. Create initial configuration
5. Mine the genesis block
6. Start a VentriQ node

## Manual Installation

If you prefer to install manually, follow these steps:

### 1. System Requirements

- Linux-based operating system
- Terminal with UTF-8 support
- Minimum 2GB RAM
- 1GB free disk space
- Network connectivity for peer discovery

### 2. Install Dependencies

```bash
# Install build essentials
sudo apt-get update
sudo apt-get install -y build-essential git

# Install additional requirements
sudo apt-get install -y libssl-dev zlib1g-dev
```

### 3. Install Chrysalis

```bash
# Clone Chrysalis repository
git clone https://github.com/ventriq/chrysalis.git
cd chrysalis

# Build Chrysalis interpreter
make
sudo make install
```

### 4. Install VentriQ

```bash
# Clone VentriQ repository
git clone https://github.com/ventriq/ventriq.git
cd ventriq

# Create necessary directories
mkdir -p ~/.ventriq/{data,wallet,logs}

# Copy configuration
cp config/ventriq.conf ~/.ventriq/

# Build VentriQ
chrysalis build src/*.cry -o ~/.ventriq/bin/ventriq
```

### 5. Initialize Node

```bash
# Generate initial configuration
ventriq init

# Create wallet
ventriq wallet create

# Mine genesis block
ventriq mine genesis
```

## Configuration

### Directory Structure

After installation, VentriQ creates the following directory structure:

```
~/.ventriq/
├── bin/           # Executable files
├── data/          # Blockchain data
│   ├── blocks/    # Block storage
│   └── chain/     # Chain state
├── wallet/        # Wallet files
└── logs/          # Log files
```

### Configuration File

The main configuration file is located at `~/.ventriq/ventriq.conf`:

```ini
# Network configuration
port = 8333
max_peers = 50
min_peers = 3

# Mining configuration
mining_threads = 4
mining_reward = 50
difficulty_adjustment_interval = 2016

# Node configuration
node_name = "VentriQ Node"
log_level = "info"
data_dir = "~/.ventriq/data"
```

## Post-Installation

### 1. Verify Installation

```bash
# Check Chrysalis version
chrysalis --version

# Check VentriQ version
ventriq --version

# Test node connection
ventriq test
```

### 2. Start Node

```bash
# Start VentriQ node
ventriq start

# Start node with mining enabled
ventriq start --mine
```

### 3. Monitor Node

```bash
# View node status
ventriq status

# View log output
ventriq logs

# View mining status
ventriq mine status
```

## Troubleshooting

### Common Issues

1. **Port Already in Use**
   ```bash
   # Check if port 8333 is in use
   netstat -an | grep 8333
   
   # Change port in configuration
   vim ~/.ventriq/ventriq.conf
   ```

2. **Insufficient Permissions**
   ```bash
   # Fix directory permissions
   chmod -R 700 ~/.ventriq
   ```

3. **Terminal Display Issues**
   ```bash
   # Ensure terminal supports UTF-8
   export LANG=en_US.UTF-8
   
   # Verify terminal capabilities
   echo $TERM
   ```

4. **Mining Problems**
   ```bash
   # Check system resources
   top
   
   # Adjust mining threads
   vim ~/.ventriq/ventriq.conf
   ```

### Error Messages

1. **"Failed to initialize Chrysalis"**
   - Verify Chrysalis installation
   - Check system PATH
   - Reinstall Chrysalis if necessary

2. **"Cannot connect to peers"**
   - Check network connectivity
   - Verify port forwarding
   - Check firewall settings

3. **"Invalid QR code generation"**
   - Verify terminal supports required characters
   - Check available system memory
   - Update terminal emulator

### Getting Help

1. Check the log files:
   ```bash
   tail -f ~/.ventriq/logs/ventriq.log
   ```

2. Enable debug logging:
   ```bash
   # Edit configuration
   vim ~/.ventriq/ventriq.conf
   # Set log_level = "debug"
   ```

3. Generate diagnostic report:
   ```bash
   ventriq diagnostic
   ```

## Updating VentriQ

### Manual Update

```bash
# Stop node
ventriq stop

# Update repository
cd ~/ventriq
git pull

# Rebuild
chrysalis build src/*.cry -o ~/.ventriq/bin/ventriq

# Start node
ventriq start
```

### Automatic Update

```bash
# Update using built-in updater
ventriq update
```

## Uninstallation

If you need to remove VentriQ:

```bash
# Stop node
ventriq stop

# Remove VentriQ files
rm -rf ~/.ventriq

# Remove Chrysalis (optional)
sudo make uninstall -C ~/chrysalis
```

## Next Steps

After installation:
1. Read the [USAGE.md](USAGE.md) guide for operational instructions
2. Create a wallet using `ventriq wallet create`
3. Start mining with `ventriq mine start`
4. Join the network with `ventriq start`

For more information:
- See [README.md](README.md) for project overview
- Check [SECURITY-MODEL.md](SECURITY-MODEL.md) for security considerations
- Review [LANGUAGE-SPECS.md](LANGUAGE-SPECS.md) for Chrysalis language details
