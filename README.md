# Cross-Platform Continuous Integration
Template repository with basic configuration files for CI services to run builds on a variety of operating systems, CPU architectures, and libc implementations (Alpine Linux uses musl).

OS              | CPU Arch      | Service       | Badge
--------------- | ------------- | ------------- | ----------
FreeBSD         | x86_64        | Cirrus CI     | [![Build Status](https://api.cirrus-ci.com/github/nightlark/cpci.svg)](https://cirrus-ci.com/github/nightlark/cpci)
Linux (Alpine)  | ARM/AArch64   | Drone Cloud   | [![Build Status](https://cloud.drone.io/api/badges/nightlark/cpci/status.svg)](https://cloud.drone.io/nightlark/cpci)
Linux (Ubuntu)  | x86_64        | Azure DevOps  | [![Build Status](https://dev.azure.com/ryanmast/cpci/_apis/build/status/nightlark.cpci?branchName=master)](https://dev.azure.com/ryanmast/cpci/_build/latest?definitionId=1&branchName=master)
macOS 10.14     | x86_64        | Azure DevOps  | [![Build Status](https://dev.azure.com/ryanmast/cpci/_apis/build/status/nightlark.cpci?branchName=master)](https://dev.azure.com/ryanmast/cpci/_build/latest?definitionId=1&branchName=master)
Windows 10      | x86_64        | Azure DevOps  | [![Build Status](https://dev.azure.com/ryanmast/cpci/_apis/build/status/nightlark.cpci?branchName=master)](https://dev.azure.com/ryanmast/cpci/_build/latest?definitionId=1&branchName=master)
