cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(change_detectors)
add_subdirectory(chunkers)
add_subdirectory(commands)
add_subdirectory(crypto)
add_subdirectory(filters)
add_subdirectory(repo)
add_subdirectory(utils)
add_subdirectory(fuse)

add_executable(backup main.cpp)
target_link_libraries(backup PRIVATE change_detectors chunkers commands crypto filters repo utils)