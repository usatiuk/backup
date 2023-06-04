cmake_minimum_required(VERSION 3.22)

add_library(change_detectors srcs/ChangeDetector.cpp srcs/ChangeDetectorContainer.cpp srcs/ChangeDetectorFactory.cpp srcs/ComparableFile.cpp srcs/ContentsChangeDetector.cpp srcs/EditTimeChangeDetector.cpp srcs/SizeChangeDetector.cpp srcs/TypeChangeDetector.cpp)

target_include_directories(change_detectors PUBLIC includes)

target_link_libraries(change_detectors utils)
