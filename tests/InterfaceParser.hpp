/*
Copyright (c) 2019,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable
Energy, LLC. See the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace gmlc {
namespace netif {
    namespace test {

        /**
         * Struct representing a network interface with its addresses
         */
        struct InterfaceInfo {
            std::string name;
            std::vector<std::string> ipv4_addresses;
            std::vector<std::string> ipv6_addresses;
        };

        /**
         * Parse a standard interface file format to get expected interfaces
         * File format (one entry per line):
         *   interface_name,ipv4_addr1;ipv4_addr2,...,ipv6_addr1;ipv6_addr2,...
         * Example:
         *   lo,127.0.0.1,::1
         *   eth0,192.168.1.10,fe80::1
         *
         * @param filename path to the interface definition file
         * @return vector of InterfaceInfo structs
         */
        inline std::vector<InterfaceInfo> parseInterfaceFile(const std::string& filename)
        {
            std::vector<InterfaceInfo> interfaces;
            std::ifstream file(filename);

            if (!file.is_open()) {
                return interfaces;
            }

            std::string line;
            while (std::getline(file, line)) {
                // Skip empty lines and comments
                if (line.empty() || line[0] == '#') {
                    continue;
                }

                InterfaceInfo iface;
                std::stringstream ss(line);
                std::string token;
                int field = 0;

                while (std::getline(ss, token, ',')) {
                    if (field == 0) {
                        // Interface name
                        iface.name = token;
                    } else if (field == 1) {
                        // IPv4 addresses (semicolon separated)
                        std::stringstream addr_ss(token);
                        std::string addr;
                        while (std::getline(addr_ss, addr, ';')) {
                            if (!addr.empty()) {
                                iface.ipv4_addresses.push_back(addr);
                            }
                        }
                    } else if (field == 2) {
                        // IPv6 addresses (semicolon separated)
                        std::stringstream addr_ss(token);
                        std::string addr;
                        while (std::getline(addr_ss, addr, ';')) {
                            if (!addr.empty()) {
                                iface.ipv6_addresses.push_back(addr);
                            }
                        }
                    }
                    field++;
                }

                if (!iface.name.empty()) {
                    interfaces.push_back(iface);
                }
            }

            return interfaces;
        }

        /**
         * Check if a list of addresses contains all expected addresses
         * @param actual the actual addresses found
         * @param expected the expected addresses
         * @return true if all expected addresses are found in actual
         */
        inline bool containsAllAddresses(const std::vector<std::string>& actual,
                                         const std::vector<std::string>& expected)
        {
            for (const auto& exp_addr : expected) {
                bool found = false;
                for (const auto& act_addr : actual) {
                    if (act_addr == exp_addr) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    return false;
                }
            }
            return true;
        }

        /**
         * Validate that detected addresses match expected interfaces
         * @param detected_v4 detected IPv4 addresses
         * @param detected_v6 detected IPv6 addresses
         * @param expected expected interfaces from config file
         * @param[out] missing_v4 IPv4 addresses that were expected but not found
         * @param[out] missing_v6 IPv6 addresses that were expected but not found
         * @return true if all expected addresses were found
         */
        inline bool validateAddresses(const std::vector<std::string>& detected_v4,
                                      const std::vector<std::string>& detected_v6,
                                      const std::vector<InterfaceInfo>& expected,
                                      std::vector<std::string>& missing_v4,
                                      std::vector<std::string>& missing_v6)
        {
            missing_v4.clear();
            missing_v6.clear();

            // Collect all expected addresses
            std::vector<std::string> expected_v4;
            std::vector<std::string> expected_v6;

            for (const auto& iface : expected) {
                expected_v4.insert(expected_v4.end(),
                                   iface.ipv4_addresses.begin(),
                                   iface.ipv4_addresses.end());
                expected_v6.insert(expected_v6.end(),
                                   iface.ipv6_addresses.begin(),
                                   iface.ipv6_addresses.end());
            }

            // Check for missing IPv4 addresses
            for (const auto& exp_addr : expected_v4) {
                bool found = false;
                for (const auto& det_addr : detected_v4) {
                    if (det_addr == exp_addr) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    missing_v4.push_back(exp_addr);
                }
            }

            // Check for missing IPv6 addresses
            for (const auto& exp_addr : expected_v6) {
                bool found = false;
                for (const auto& det_addr : detected_v6) {
                    if (det_addr == exp_addr) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    missing_v6.push_back(exp_addr);
                }
            }

            return missing_v4.empty() && missing_v6.empty();
        }

    }  // namespace test
}  // namespace netif
}  // namespace gmlc
