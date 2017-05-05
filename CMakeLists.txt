cmake_minimum_required(VERSION 3.6)
project(Chess)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp move_engine.cpp func.h func.cpp move_engine.h board_constants.h typedefs.h)
add_executable(Chess ${SOURCE_FILES})