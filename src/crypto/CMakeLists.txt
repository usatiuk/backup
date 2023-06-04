cmake_minimum_required(VERSION 3.22)

find_package(OpenSSL REQUIRED)

add_library(crypto srcs/AES.cpp srcs/CRC32.cpp srcs/MD5.cpp)

target_include_directories(crypto PUBLIC includes)

target_link_libraries(crypto OpenSSL::SSL utils)
