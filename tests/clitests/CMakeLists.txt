cmake_minimum_required(VERSION 3.18)


add_executable(
        CLITests
        srcs/CLITestWrapper.cpp
)
add_dependencies(CLITests backup)

target_link_libraries(
        CLITests PRIVATE
        GTest::gtest_main testUtils
)
gtest_discover_tests(CLITests DISCOVERY_TIMEOUT 600)

