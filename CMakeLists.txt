project(file-transmitter)

cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

include_directories(PRIVATE shared/include)
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(shared)

link_libraries(logger)

add_subdirectory(client)
add_subdirectory(server)