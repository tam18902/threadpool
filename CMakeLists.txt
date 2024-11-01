cmake_minimum_required(VERSION 3.10)

# Set the project name
project(ServerClientProject)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/client)
include_directories(${PROJECT_SOURCE_DIR}/server)

# Add the server executable
add_executable(server
    server/main.cc
    server/server.cc
    server/server.h
    server/threadpool.cc
    server/threadpool.h
)

# Add the client executable
add_executable(client
    client/main.cc
    client/client.cc
    client/client.h
)

# Set permissions for the executables if needed
if(UNIX)
    add_custom_command(TARGET server POST_BUILD COMMAND chmod +x $<TARGET_FILE:server>)
    add_custom_command(TARGET client POST_BUILD COMMAND chmod +x $<TARGET_FILE:client>)
endif()

# Link libraries if necessary (e.g., pthread for Linux)
target_link_libraries(server pthread)
target_link_libraries(client pthread)
