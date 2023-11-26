cmake_minimum_required(VERSION 3.18)

add_library(testUtils srcs/Cleaner.cpp)

target_include_directories(testUtils PUBLIC includes)