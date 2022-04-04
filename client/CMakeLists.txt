add_executable(client src/client.c src/settings.c src/file_sender.c include/file_sender.h)
target_include_directories(client PRIVATE include)
