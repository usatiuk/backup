cmake_minimum_required(VERSION 3.18)

add_executable(
        FullTest
        srcs/FullTest.cpp
)

target_link_libraries(
        FullTest PRIVATE
        GTest::gtest_main commands utils testUtils repo
)

gtest_discover_tests(FullTest)

