add_executable(server src/server.c src/settings.c src/file_receiver.c)
target_include_directories(server PRIVATE include)
