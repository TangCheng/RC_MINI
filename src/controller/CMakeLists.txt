set(headers ${CMAKE_SOURCE_DIR}/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCES)

add_executable(controller ${SOURCES})
target_link_libraries(controller ${CMAKE_BINARY_DIR}/src/libs/common.lib)
target_include_directories(controller PUBLIC "${headers}")
