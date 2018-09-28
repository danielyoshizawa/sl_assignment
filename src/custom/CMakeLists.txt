cmake_minimum_required(VERSION 3.3)

add_library(custom STATIC custom.cpp)

target_include_directories(custom PUBLIC ${CMAKE_SOURCE_DIR}/src/custom)

target_link_libraries(custom cereal)