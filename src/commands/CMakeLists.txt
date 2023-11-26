cmake_minimum_required(VERSION 3.18)

add_library(commands srcs/CommandDiff.cpp srcs/CommandList.cpp srcs/CommandListFiles.cpp srcs/CommandRestore.cpp srcs/CommandRun.cpp srcs/CommandsCommon.cpp srcs/Diff.cpp srcs/CommandMount.cpp)

target_include_directories(commands PUBLIC includes)

target_link_libraries(commands crypto repo chunkers utils change_detectors fuse)
