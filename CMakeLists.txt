cmake_minimum_required(VERSION 3.8)
project(RC_Mini C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_FLAGS "--fsigned-char --stack-auto")

add_subdirectory(src)

