cmake_minimum_required(VERSION 3.3)

set(
	SOURCE
		server.cpp
		manager.cpp
		main.cpp
	)

add_executable(server ${SOURCE})

target_include_directories(server PRIVATE 
										${CMAKE_SOURCE_DIR}/src/server
										${CMAKE_SOURCE_DIR}/src/message
						  )

target_link_libraries(server custom)