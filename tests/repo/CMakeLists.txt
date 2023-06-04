cmake_minimum_required(VERSION 3.22)

add_executable(
        FileRepositoryTest
        srcs/FileRepositoryTest.cpp
)

target_link_libraries(
        FileRepositoryTest PRIVATE
        GTest::gtest_main commands utils testUtils repo
)

add_executable(
        ChunkTest
        srcs/ChunkTest.cpp
)

target_link_libraries(
        ChunkTest PRIVATE
        GTest::gtest_main commands utils testUtils repo
)

gtest_discover_tests(ChunkTest)
gtest_discover_tests(FileRepositoryTest)
