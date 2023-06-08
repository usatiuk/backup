cmake_minimum_required(VERSION 3.22)

find_package(PkgConfig REQUIRED)

pkg_check_modules(FUSE REQUIRED IMPORTED_TARGET fuse)

add_library(fuse srcs/RepoFS.cpp)

target_include_directories(fuse PUBLIC includes)

target_link_libraries(fuse utils repo PkgConfig::FUSE)
