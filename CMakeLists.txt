# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Copyright (c) 2019, Battelle Memorial Institute; Lawrence Livermore
# National Security, LLC; Alliance for Sustainable Energy, LLC.
# See the top-level NOTICE for additional details.
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

cmake_minimum_required(VERSION 3.4)
project("NetIF")

include(GNUInstallDirs)

add_library(NetIF INTERFACE)
add_library(NetIF::NetIF ALIAS NetIF)

target_include_directories(${PROJECT_NAME}
  INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>)
if(${CMAKE_VERSION} VERSION_LESS "3.8")
  set(CMAKE_CXX_STANDARD 14)
else()
  target_compile_features(NetIF INTERFACE cxx_std_14)
endif()

OPTION(ENABLE_NETIF_TESTS "Build tests" OFF)
if(ENABLE_NETIF_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

#install(DIRECTORY ${PROJECT_SOURCE_DIR}/gmlc DESTINATION include/gmlc)
