cmake_minimum_required(VERSION 3.3)

set(SOURCE 
		client.cpp
		main.cpp)

add_executable(client ${SOURCE})

target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/src/client)