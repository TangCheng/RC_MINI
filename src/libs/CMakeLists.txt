cmake_policy(SET CMP0076 NEW)

set(headers ${CMAKE_SOURCE_DIR}/include)

add_library(common "")
target_include_directories(common PUBLIC "${headers}")

add_subdirectory(drv)
add_subdirectory(sys)
add_subdirectory(ui)
add_subdirectory(utils)
