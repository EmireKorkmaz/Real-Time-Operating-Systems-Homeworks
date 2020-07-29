cmake_minimum_required(VERSION 3.15)
project(hw1)

set(CMAKE_CXX_STANDARD 17)

add_executable(hw1 main.cpp ITimer.h MyTimer.h)
TARGET_LINK_LIBRARIES( hw1 pthread)