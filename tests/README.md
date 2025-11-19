# NetIF Test Environment

This directory contains tools and utilities for testing NetIF with custom network configurations, including "fake" network adapters and interfaces.

## Overview

The test environment provides:

1. **Interface validation** - Validate that detected IP addresses match expected interfaces
2. **Virtual interface creation** - Scripts to create dummy network interfaces on Linux for testing
3. **Docker-based testing** - Containerized test environment with custom network configurations
4. **Cross-platform support** - Works on Linux, with plans for Windows and macOS

## Quick Start

### Running Tests Locally (Linux)

1. **Create virtual test interfaces:**

   ```bash
   sudo ./setup_virtual_interfaces.sh setup
   ```

2. **Export current interfaces to a config file:**

   ```bash
   ./setup_virtual_interfaces.sh export expected_interfaces.txt
   ```

3. **Run tests with validation:**

   ```bash
   cd ../build
   export NETIF_EXPECTED_INTERFACES=../tests/expected_interfaces.txt
   ctest --verbose
   ```

4. **Clean up test interfaces:**
   ```bash
   sudo ./setup_virtual_interfaces.sh teardown
   ```

### Running Tests with Docker

Docker provides an isolated environment where we can create virtual network interfaces without affecting the host system.

1. **Build and run tests:**

   ```bash
   docker-compose up netif-test
   ```

2. **Interactive shell for debugging:**

   ```bash
   docker-compose run netif-test-shell
   ```

3. **Build Docker image manually:**
   ```bash
   docker build -t netif-test -f Dockerfile ..
   docker run --privileged netif-test
   ```

## Files

- **`setup_virtual_interfaces.sh`** - Bash script to create/remove virtual network interfaces and export configurations
- **`InterfaceParser.hpp`** - C++ header with utilities to parse and validate interface configurations
- **`GetAddressTests.cpp`** - Updated test cases with optional validation against expected interfaces
- **`Dockerfile`** - Docker image for testing with custom network configurations
- **`docker-compose.yml`** - Docker Compose configuration for easy testing
- **`docker-entrypoint.sh`** - Entry point script for Docker container

## Interface Configuration File Format

The expected interfaces file uses a simple CSV-like format:

```
# Comments start with #
# Format: interface_name,ipv4_addresses,ipv6_addresses
# Multiple addresses are separated by semicolons

lo,127.0.0.1,::1
eth0,192.168.1.10,fe80::1;2001:db8::1
dummy0,192.168.100.1;192.168.100.2,2001:db8::1;2001:db8::2;fd00::1
```

## Test Validation

When the environment variable `NETIF_EXPECTED_INTERFACES` is set to point to an interface configuration file, the tests will:

1. Parse the expected interfaces from the file
2. Compare detected addresses against expected addresses
3. Report any missing expected addresses
4. Fail the test if expected addresses are not found

If the environment variable is not set, tests run in their original mode (just printing addresses).

## Virtual Interfaces Created

The `setup_virtual_interfaces.sh` script creates the following test interfaces:

- **dummy0**
  - IPv4: 192.168.100.1/24, 192.168.100.2/24
  - IPv6: 2001:db8::1/64, 2001:db8::2/64, fd00::1/64

- **dummy1**
  - IPv4: 10.0.1.1/24
  - IPv6: fe80::abcd:1/64

These interfaces provide comprehensive coverage for testing:

- Multiple addresses per interface
- Both IPv4 and IPv6
- Global unicast IPv6 addresses (2001:db8::/32 - documentation prefix)
- Unique local IPv6 addresses (fd00::/8)
- Link-local IPv6 addresses (fe80::/10)

## CI Integration

To integrate this test environment into CI workflows:

1. **GitHub Actions** (Linux):

   ```yaml
   - name: Setup virtual interfaces
     run: |
       cd tests
       sudo ./setup_virtual_interfaces.sh setup
       ./setup_virtual_interfaces.sh export expected_interfaces.txt

   - name: Run tests with validation
     run: |
       cd build
       export NETIF_EXPECTED_INTERFACES=../tests/expected_interfaces.txt
       ctest --verbose

   - name: Cleanup
     if: always()
     run: sudo ./tests/setup_virtual_interfaces.sh teardown
   ```

2. **Using Docker**:
   ```yaml
   - name: Test with Docker
     run: |
       cd tests
       docker-compose up --exit-code-from netif-test netif-test
   ```

## Platform Support

### Linux

- ✅ Fully supported with `ip` command for creating virtual interfaces
- ✅ Docker support for isolated testing

### Windows

- ⚠️ Virtual interface creation needs alternative approach
- 💡 Consider using Windows Loopback Adapter or Hyper-V virtual switches
- 💡 PowerShell scripts could automate interface creation

### macOS

- ⚠️ Virtual interface creation needs alternative approach
- 💡 Consider using `ifconfig` with loopback aliases
- 💡 Docker Desktop for Mac can provide Linux environment

## Future Enhancements

1. **Network namespaces** - Use Linux network namespaces for even more isolation
2. **Mininet integration** - Use Mininet for complex network topologies
3. **Windows support** - Add PowerShell scripts for Windows
4. **macOS support** - Add scripts for macOS interface creation
5. **Automated CI tests** - Full integration with GitHub Actions and Cirrus CI

## Troubleshooting

### "Operation not permitted" when creating interfaces

- Make sure you run the script with `sudo`
- In Docker, ensure the container runs with `--privileged` or has `CAP_NET_ADMIN` capability

### IPv6 addresses not appearing

- Check if IPv6 is enabled on your system: `sysctl net.ipv6.conf.all.disable_ipv6`
- Enable IPv6 if disabled: `sudo sysctl -w net.ipv6.conf.all.disable_ipv6=0`

### Tests fail with missing addresses

- Verify the expected_interfaces.txt file matches your system
- Re-export the configuration: `./setup_virtual_interfaces.sh export`
- Check that virtual interfaces were created successfully: `ip addr show`

## Contributing

When adding new test cases:

1. Consider whether virtual interfaces are needed
2. Update the `setup_virtual_interfaces.sh` if new interface types are required
3. Update this documentation
4. Ensure tests work both with and without expected interface validation
