cmake_minimum_required(VERSION 3.10)
project(hw)

add_executable(task1 task1.c)
add_executable(task2 task2.c)
add_executable(task3 task3.c)

target_link_libraries(task1 pthread)
target_link_libraries(task2 pthread)
target_link_libraries(task3 pthread)
