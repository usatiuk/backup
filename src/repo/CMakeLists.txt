cmake_minimum_required(VERSION 3.22)

add_library(repo srcs/FileRepository.cpp srcs/Object.cpp srcs/Repository.cpp srcs/objects/Archive.cpp srcs/objects/Chunk.cpp srcs/objects/File.cpp srcs/objects/FileBuffer.cpp)

target_include_directories(repo PUBLIC includes)

target_link_libraries(repo filters)