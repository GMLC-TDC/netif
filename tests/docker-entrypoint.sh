#!/bin/bash
# Docker entrypoint script for NetIF testing
# This script sets up virtual interfaces and runs tests

set -e

# Function to setup virtual interfaces
setup_test_interfaces() {
    echo "Setting up test network interfaces..."
    
    # Create dummy interfaces
    ip link add dummy0 type dummy
    ip link set dummy0 up
    ip addr add 192.168.100.1/24 dev dummy0
    ip addr add 192.168.100.2/24 dev dummy0
    ip -6 addr add 2001:db8::1/64 dev dummy0
    ip -6 addr add 2001:db8::2/64 dev dummy0
    ip -6 addr add fd00::1/64 dev dummy0
    
    ip link add dummy1 type dummy
    ip link set dummy1 up
    ip addr add 10.0.1.1/24 dev dummy1
    ip -6 addr add fe80::abcd:1/64 dev dummy1
    
    echo "Test interfaces created successfully"
    echo ""
    echo "Interface configuration:"
    ip addr show
}

# Function to run tests
run_tests() {
    echo ""
    echo "Running NetIF tests..."
    cd /netif/build
    
    # Export expected interfaces
    /netif/tests/setup_virtual_interfaces.sh export /tmp/expected_interfaces.txt
    
    # Run tests with expected interfaces
    export NETIF_EXPECTED_INTERFACES=/tmp/expected_interfaces.txt
    ctest --verbose
}

# Function to run interactive shell
run_shell() {
    exec /bin/bash
}

# Main logic
case "${1:-test}" in
    test)
        setup_test_interfaces
        run_tests
        ;;
    shell)
        setup_test_interfaces
        run_shell
        ;;
    *)
        exec "$@"
        ;;
esac
