cmake_minimum_required(VERSION 3.31)
project(projekt1)

set(CMAKE_CXX_STANDARD 20)
add_executable(projekt1 main.cpp player.cpp player.h)
target_link_libraries(projekt1 readline)