set(headers ${CMAKE_SOURCE_DIR}/include)

add_executable(receiver receiver.c)
target_include_directories(receiver PUBLIC "${headers}")
