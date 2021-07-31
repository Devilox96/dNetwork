cmake_minimum_required(VERSION 3.19)
project(dNetwork)

set(CMAKE_CXX_STANDARD 20)

add_compile_options("-fcoroutines")

add_executable(dNetwork main.cpp dServer.cpp dServer.h dClientData.cpp dClientData.h)