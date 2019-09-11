add_executable(GetAddressTests GetAddressTests.cpp)
target_link_libraries(GetAddressTests NetIF::NetIF)

add_test(GetAddressTests GetAddressTests)
