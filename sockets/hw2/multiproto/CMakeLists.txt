cmake_minimum_required(VERSION 3.10)
project(hw)

add_executable(client_udp client_udp.c)
add_executable(client_tpc client_tcp.c)
add_executable(server server.c)
