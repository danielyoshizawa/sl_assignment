cmake_minimum_required(VERSION 3.3)

set(SOURCE custom.cpp manager.cpp)

add_library(custom STATIC ${SOURCE})

target_include_directories(custom PUBLIC ${CMAKE_SOURCE_DIR}/src/custom)

target_link_libraries(custom cereal)