# CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

project(MemoryManager)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(ProcessMemoryMonitor)
add_subdirectory(SystemMemeryManager)
