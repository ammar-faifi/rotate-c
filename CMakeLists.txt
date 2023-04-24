cmake_minimum_required(VERSION 3.0.0)
project(rotate VERSION 0.1.0)

set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD 17)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(vr ${SOURCES})
set ( CMAKE_C_FLAGS "-Wall -Werror -g -DDEBUG -ggdb3 -pg -fsanitize=address -Wpedantic")
target_link_libraries(vr m)
