cmake_minimum_required(VERSION 3.10)
project(hw)
add_subdirectory(lib)
add_executable(calculator main.c)
target_link_libraries(calculator calc)