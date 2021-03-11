cmake_minimum_required(VERSION 3.17)
project(ctrl2fn C)

set(CMAKE_C_STANDARD 11)

add_executable(ctrl2fn ctrl2fn.c)
target_compile_options(ctrl2fn PRIVATE -Wall -Wextra -Ofast)

install(TARGETS ctrl2fn RUNTIME DESTINATION bin)
