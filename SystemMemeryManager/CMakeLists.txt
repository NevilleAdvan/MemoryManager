project(SystemMemoryManager)

# 添加头文件目录
include_directories(include)

# 添加源代码
file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(SystemMemoryManager ${SOURCES})
