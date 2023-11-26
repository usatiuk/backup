cmake_minimum_required(VERSION 3.18)


find_package(ZLIB REQUIRED)

add_library(filters srcs/CheckFilter.cpp srcs/Filter.cpp srcs/FilterAES.cpp srcs/FilterContainer.cpp srcs/FilterShift.cpp srcs/FilterShiftSecret.cpp srcs/FilterZlib.cpp srcs/FilterFactory.cpp)

target_include_directories(filters PUBLIC includes)

target_link_libraries(filters ZLIB::ZLIB utils crypto)
