cmake_minimum_required(VERSION 3.20)
project(hw)
add_compile_options(-fno-stack-protector -no-pie)
add_executable(task_FUNCTIONS task.c)
