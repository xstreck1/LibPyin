project(LibPyin)
cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)

include_directories(./source/)

aux_source_directory(./source/vamp SRC_LIST)
aux_source_directory(./source/vamp-sdk SRC_LIST)
aux_source_directory(./source/pyin SRC_LIST)

set(CMAKE_BUILD_TYPE Release)

# Change SHARED to STATIC here
add_library(LibPyin STATIC ${SRC_LIST} ./source/normal_dist_pdf.cpp ./source/libpyincpp.cpp ./source/libpyinc.cpp)
