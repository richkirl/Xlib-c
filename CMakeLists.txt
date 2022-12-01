cmake_minimum_required(VERSION 3.24)
project(untitled1 C)
find_package(X11)
set(CMAKE_C_STANDARD 23)

add_executable(untitled1 ${X11_LIBRARY} main.c lib.h lib.c main1.c)
target_link_libraries(untitled1 X11 m)