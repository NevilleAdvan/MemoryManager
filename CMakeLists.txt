cmake_minimum_required(VERSION 3.28)
project(MemoryManager)

set(CMAKE_CXX_STANDARD 20)

# 添加头文件目录
include_directories(include)

# 添加源代码
file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(MemoryManager ${SOURCES})
