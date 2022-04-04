project(file-transmitter)

cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 11)

include_directories(PRIVATE shared)
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(client)
add_subdirectory(server)