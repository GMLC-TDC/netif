/*
Copyright (c) 2019,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable
Energy, LLC. See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

#include "gmlc/netif/NetIF.hpp"
#include "InterfaceParser.hpp"

void printStringVector(std::string header, std::vector<std::string> vec)
{
    std::cout << header << ":\n";
    for (auto e : vec) {
        std::cout << e << "\n";
    }
    std::cout << "\n";
}

TEST_CASE("Get IPv4 Addresses")
{
    auto addrs = gmlc::netif::getInterfaceAddressesV4();
    printStringVector("IPv4 Addresses", addrs);

    // Optional validation if expected interfaces file is provided
    const char* expected_file = std::getenv("NETIF_EXPECTED_INTERFACES");
    if (expected_file != nullptr) {
        auto expected = gmlc::netif::test::parseInterfaceFile(expected_file);
        std::vector<std::string> missing_v4, missing_v6;
        std::vector<std::string> empty_v6;

        bool valid =
            gmlc::netif::test::validateAddresses(addrs, empty_v6, expected, missing_v4, missing_v6);

        if (!missing_v4.empty()) {
            std::cout << "Missing expected IPv4 addresses:\n";
            printStringVector("Missing IPv4", missing_v4);
        }

        REQUIRE(missing_v4.empty());
    }
}

TEST_CASE("Get IPv6 Addresses")
{
    auto addrs = gmlc::netif::getInterfaceAddressesV6();
    printStringVector("IPv6 Addresses", addrs);

    // Optional validation if expected interfaces file is provided
    const char* expected_file = std::getenv("NETIF_EXPECTED_INTERFACES");
    if (expected_file != nullptr) {
        auto expected = gmlc::netif::test::parseInterfaceFile(expected_file);
        std::vector<std::string> missing_v4, missing_v6;
        std::vector<std::string> empty_v4;

        bool valid =
            gmlc::netif::test::validateAddresses(empty_v4, addrs, expected, missing_v4, missing_v6);

        if (!missing_v6.empty()) {
            std::cout << "Missing expected IPv6 addresses:\n";
            printStringVector("Missing IPv6", missing_v6);
        }

        REQUIRE(missing_v6.empty());
    }
}

TEST_CASE("Get All Addresses")
{
    auto addrs = gmlc::netif::getInterfaceAddressesAll();
    printStringVector("All Addresses", addrs);

    // Verify the "All" function returns both IPv4 and IPv6
    auto v4_addrs = gmlc::netif::getInterfaceAddressesV4();
    auto v6_addrs = gmlc::netif::getInterfaceAddressesV6();

    // All IPv4 addresses should be in the combined list
    for (const auto& v4 : v4_addrs) {
        bool found = false;
        for (const auto& addr : addrs) {
            if (addr == v4) {
                found = true;
                break;
            }
        }
        REQUIRE(found);
    }

    // All IPv6 addresses should be in the combined list
    for (const auto& v6 : v6_addrs) {
        bool found = false;
        for (const auto& addr : addrs) {
            if (addr == v6) {
                found = true;
                break;
            }
        }
        REQUIRE(found);
    }
}
