set(headers ${CMAKE_SOURCE_DIR}/include)

add_executable(controller
    controller.c
    rf_comm.c
    control_signal_process.c)
target_link_libraries(controller ${CMAKE_BINARY_DIR}/src/libs/common.lib)
target_include_directories(controller PUBLIC "${headers}")
