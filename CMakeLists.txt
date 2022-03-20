cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME socketReadWrite)
project(PROJECT_NAME)

add_executable(server server.cpp)
add_executable(client client.cpp)