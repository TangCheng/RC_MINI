set(headers ${CMAKE_SOURCE_DIR}/include)

add_executable(receiver
    receiver.c
    rf_comm.c)
target_link_libraries(receiver ${CMAKE_BINARY_DIR}/src/libs/common.lib)
target_include_directories(receiver PUBLIC "${headers}")
