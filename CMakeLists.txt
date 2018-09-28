cmake_minimum_required(VERSION 3.3)

project(StreamLab)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_library(cereal INTERFACE)
target_include_directories(cereal INTERFACE "${CMAKE_SOURCE_DIR}/include")

add_subdirectory(src)