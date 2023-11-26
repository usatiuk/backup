cmake_minimum_required(VERSION 3.18)

add_library(utils srcs/BytesFormatter.cpp srcs/Config.cpp srcs/Exception.cpp srcs/Logger.cpp srcs/Progress.cpp srcs/RunningAverage.cpp srcs/RunningDiffAverage.cpp srcs/Signals.cpp srcs/ThreadPool.cpp)

target_include_directories(utils PUBLIC includes)