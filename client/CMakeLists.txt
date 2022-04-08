add_executable(client src/client.c src/settings.c src/file_sender.c)
target_include_directories(client PRIVATE include)
