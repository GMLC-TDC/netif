#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include <iostream>
#include <vector>
#include <string>

#include "gmlc/netif/NetIF.hpp"

void printStringVector(std::string header, std::vector<std::string> vec) {
    std::cout << header << ":\n";
    for (auto e : vec) {
        std::cout << e << "\n";
    }
    std::cout << "\n";
}

TEST_CASE("Get IPv4 Addresses") {
    auto addrs = gmlc::netif::getInterfaceAddressesV4();
    printStringVector("IPv4 Addresses", addrs);
}

TEST_CASE("Get IPv6 Addresses") {
    auto addrs = gmlc::netif::getInterfaceAddressesV6();
    printStringVector("IPv6 Addresses", addrs);
}
