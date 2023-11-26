cmake_minimum_required(VERSION 3.18)

add_executable(
        CryptoTests
        srcs/CryptoTests.cpp
)

target_link_libraries(
        CryptoTests PRIVATE
        GTest::gtest_main crypto
)

gtest_discover_tests(CryptoTests)

