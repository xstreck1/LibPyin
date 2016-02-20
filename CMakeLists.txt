project(LibPyin)
cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)

add_definitions(-DBUILD_SHARED)

include_directories(.)

aux_source_directory(. SRC_LIST)
aux_source_directory(vamp SRC_LIST)
aux_source_directory(vamp-sdk SRC_LIST)
aux_source_directory(pyin SRC_LIST)


set(CMAKE_BUILD_TYPE Release)

add_library(LibPyin SHARED ${SRC_LIST})
