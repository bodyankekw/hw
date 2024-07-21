cmake_minimum_required(VERSION 3.10)
project(hw)

add_executable(file_manager file_manager.c)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(file_manager ${CURSES_LIBRARIES})