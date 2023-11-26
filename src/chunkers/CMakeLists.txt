cmake_minimum_required(VERSION 3.18)

add_library(chunkers srcs/Buzhash.cpp srcs/BuzhashChunker.cpp srcs/Chunker.cpp srcs/ChunkerFactory.cpp srcs/ConstChunker.cpp)

target_include_directories(chunkers PUBLIC includes)

target_link_libraries(chunkers crypto)