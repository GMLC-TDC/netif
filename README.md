# NetIF

Get addresses associated with network interfaces on a system without using name lookups. Header-only, requires C++14.

## Usage

Add the header file to the include path for your project. The main functions for getting interface addresses all return a vector of strings (`std::vector<std::string>`) with the typical human-readable representation of IP addresses (e.g. `127.0.0.1` for IPv4 and `2001:0db8:85a3:0000:0000:8a2e:0370:7334` for IPv6).

```cpp
#include "gmlc/netif/NetIF.hpp"

auto addrs_ipv4 = gmlc::netif::getInterfaceAddressesV4(); // Get all IPv4 addresses
auto addrs_ipv6 = gmlc::netif::getInterfaceAddressesV6(); // Get all IPv6 addresses
auto addrs_all = gmlc::netif::getInterfaceAddressesAll(); // Get all addresses (IPv4 and IPv6)
```

## CI Test Status

| OS             | CPU Arch    | Status                                                                                                                                          |
| -------------- | ----------- | ----------------------------------------------------------------------------------------------------------------------------------------------- |
| FreeBSD        | x86_64      | [![Build Status](http://api.cirrus-ci.com/github/GMLC-TDC/netif.svg)](https://cirrus-ci.com/github/GMLC-TDC/netif)                              |
| Linux (Alpine) | ARM/AArch64 | [![Build Status](https://cloud.drone.io/api/badges/GMLC-TDC/netif/status.svg)](https://cloud.drone.io/GMLC-TDC/netif)                           |
| Linux (Ubuntu) | x86_64      | [![CI](https://github.com/GMLC-TDC/netif/actions/workflows/build.yml/badge.svg)](https://github.com/GMLC-TDC/netif/actions/workflows/build.yml) |
| macOS          | x86_64      | [![CI](https://github.com/GMLC-TDC/netif/actions/workflows/build.yml/badge.svg)](https://github.com/GMLC-TDC/netif/actions/workflows/build.yml) |
| Windows 10     | x86_64      | [![CI](https://github.com/GMLC-TDC/netif/actions/workflows/build.yml/badge.svg)](https://github.com/GMLC-TDC/netif/actions/workflows/build.yml) |

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change. See [Contributing](CONTRIBUTING) for more details and [Contributors](CONTRIBUTORS) for a list of the current and past contributors to this project.

Please make sure to update tests as appropriate.

## Release

NetIF is distributed under the terms of the BSD-3 clause license. All new
contributions must be made under this license. See [LICENSE](LICENSE) for more detail.

SPDX-License-Identifier: BSD-3-Clause

Portions of the code written by LLNL with release number LLNL-CODE-739319
