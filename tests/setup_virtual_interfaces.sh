#!/bin/bash
# Script to create virtual network interfaces for testing
# This allows testing with custom network configurations including IPv6

set -e

# Function to print usage
usage() {
    echo "Usage: $0 [setup|teardown|export]"
    echo "  setup    - Create virtual network interfaces"
    echo "  teardown - Remove virtual network interfaces"
    echo "  export   - Export interface configuration to a file"
    exit 1
}

# Function to create virtual interfaces
setup_interfaces() {
    echo "Setting up virtual network interfaces..."
    
    # Create a dummy interface for testing IPv4 and IPv6
    if ! ip link show dummy0 &> /dev/null; then
        echo "Creating dummy0 interface..."
        sudo ip link add dummy0 type dummy
        sudo ip link set dummy0 up
        
        # Add IPv4 addresses
        sudo ip addr add 192.168.100.1/24 dev dummy0
        sudo ip addr add 192.168.100.2/24 dev dummy0
        
        # Add IPv6 addresses
        sudo ip -6 addr add 2001:db8::1/64 dev dummy0
        sudo ip -6 addr add 2001:db8::2/64 dev dummy0
        sudo ip -6 addr add fd00::1/64 dev dummy0
    else
        echo "dummy0 interface already exists"
    fi
    
    # Create a second dummy interface for additional testing
    if ! ip link show dummy1 &> /dev/null; then
        echo "Creating dummy1 interface..."
        sudo ip link add dummy1 type dummy
        sudo ip link set dummy1 up
        
        # Add IPv4 address
        sudo ip addr add 10.0.1.1/24 dev dummy1
        
        # Add IPv6 address (link-local will be auto-generated)
        sudo ip -6 addr add fe80::abcd:1/64 dev dummy1
    else
        echo "dummy1 interface already exists"
    fi
    
    echo "Virtual interfaces setup complete!"
    echo ""
    echo "Created interfaces:"
    ip addr show dummy0 2>/dev/null || true
    ip addr show dummy1 2>/dev/null || true
}

# Function to remove virtual interfaces
teardown_interfaces() {
    echo "Tearing down virtual network interfaces..."
    
    if ip link show dummy0 &> /dev/null; then
        echo "Removing dummy0 interface..."
        sudo ip link delete dummy0
    fi
    
    if ip link show dummy1 &> /dev/null; then
        echo "Removing dummy1 interface..."
        sudo ip link delete dummy1
    fi
    
    echo "Teardown complete!"
}

# Function to export current interfaces to a config file
export_interfaces() {
    OUTPUT_FILE="${1:-expected_interfaces.txt}"
    echo "Exporting interface configuration to $OUTPUT_FILE..."
    
    # Clear or create the output file
    > "$OUTPUT_FILE"
    
    # Add a comment header
    echo "# Expected network interfaces" >> "$OUTPUT_FILE"
    echo "# Format: interface_name,ipv4_addr1;ipv4_addr2,...,ipv6_addr1;ipv6_addr2,..." >> "$OUTPUT_FILE"
    echo "" >> "$OUTPUT_FILE"
    
    # Parse interface information from ip command
    # Get all interfaces
    INTERFACES=$(ip -o link show | awk -F': ' '{print $2}' | grep -v '^lo$' | sort -u)
    
    for iface in $INTERFACES; do
        # Get IPv4 addresses
        IPV4_ADDRS=$(ip -4 -o addr show dev "$iface" 2>/dev/null | awk '{print $4}' | cut -d'/' -f1 | tr '\n' ';' | sed 's/;$//')
        
        # Get IPv6 addresses (excluding link-local starting with fe80 unless it's the only one)
        IPV6_ADDRS=$(ip -6 -o addr show dev "$iface" scope global 2>/dev/null | awk '{print $4}' | cut -d'/' -f1 | tr '\n' ';' | sed 's/;$//')
        
        # If no global IPv6, include link-local
        if [ -z "$IPV6_ADDRS" ]; then
            IPV6_ADDRS=$(ip -6 -o addr show dev "$iface" scope link 2>/dev/null | awk '{print $4}' | cut -d'/' -f1 | tr '\n' ';' | sed 's/;$//')
        fi
        
        # Only add interfaces with at least one IP address
        if [ -n "$IPV4_ADDRS" ] || [ -n "$IPV6_ADDRS" ]; then
            echo "$iface,$IPV4_ADDRS,$IPV6_ADDRS" >> "$OUTPUT_FILE"
        fi
    done
    
    # Always add loopback
    echo "lo,127.0.0.1,::1" >> "$OUTPUT_FILE"
    
    echo "Configuration exported to $OUTPUT_FILE"
    cat "$OUTPUT_FILE"
}

# Main script logic
case "${1:-}" in
    setup)
        setup_interfaces
        ;;
    teardown)
        teardown_interfaces
        ;;
    export)
        export_interfaces "${2:-expected_interfaces.txt}"
        ;;
    *)
        usage
        ;;
esac
