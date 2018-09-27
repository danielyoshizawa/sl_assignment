cmake_minimum_required(VERSION 3.3)

set(
	SOURCE
		server.cpp
		custom.cpp
		main.cpp
	)

add_executable(server ${SOURCE})

target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/server )