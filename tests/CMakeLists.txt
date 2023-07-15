cmake_minimum_required(VERSION 3.22)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif ()

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_subdirectory(utils)

add_subdirectory(chunkers)
add_subdirectory(clitests)
add_subdirectory(crypto)
add_subdirectory(fulltests)
add_subdirectory(repo)