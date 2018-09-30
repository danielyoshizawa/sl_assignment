cmake_minimum_required(VERSION 3.3)

set(
	SOURCE
		server.cpp
		processor.cpp
		commands/printCommand.cpp
		commands/createCustomCommand.cpp
		commands/customsListCommand.cpp
		commands/retrieveCustomCommand.cpp
	)

add_executable(server ${SOURCE} main.cpp)

target_include_directories(server PRIVATE 
										${CMAKE_SOURCE_DIR}/src/server
										${CMAKE_SOURCE_DIR}/src/common
										${CMAKE_SOURCE_DIR}/src/server/commands
						  )

target_link_libraries(server custom)