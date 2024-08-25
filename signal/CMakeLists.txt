cmake_minimum_required(VERSION 3.10)
project(hw)

add_executable(sigusr1 task1.c)
add_executable(sigint task2.c)
add_executable(eventloop task3.c)