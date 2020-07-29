cmake_minimum_required(VERSION 3.15)
project(hw2)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(hw2 main.cpp lock_guard.h mutex.h unique_lock.h)