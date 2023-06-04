cmake_minimum_required(VERSION 3.22)

add_executable(
        BuzhashTest
        srcs/BuzhashTest.cpp
)
target_link_libraries(
        BuzhashTest PRIVATE
        GTest::gtest_main chunkers
)
gtest_discover_tests(BuzhashTest)

