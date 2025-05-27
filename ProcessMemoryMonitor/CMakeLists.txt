# 项目名称和版本
project(ProcessMemoryMonitor VERSION 1.0)

# 输出目录设置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# 添加头文件目录
include_directories(include)

# 库源文件
set(LIB_SOURCES
        src/process_memory_monitor.cpp
)

# 主程序源文件
set(MAIN_SOURCES
        src/main.cpp
)

# 生成静态库
add_library(processmemorymonitor_static STATIC ${LIB_SOURCES})
set_target_properties(processmemorymonitor_static PROPERTIES OUTPUT_NAME processmemorymonitor)

# 生成动态库
add_library(processmemorymonitor_shared SHARED ${LIB_SOURCES})
set_target_properties(processmemorymonitor_shared PROPERTIES OUTPUT_NAME processmemorymonitor)

# 直接编译源代码的可执行文件
add_executable(process_memory_monitor ${MAIN_SOURCES} ${LIB_SOURCES})

# 使用静态库的测试程序
add_executable(test-static ${MAIN_SOURCES})
target_link_libraries(test-static PRIVATE ${PROJECT_BINARY_DIR}/lib/libprocessmemorymonitor.a)

# 使用动态库的测试程序（设置rpath）
add_executable(test-shared ${MAIN_SOURCES})
target_link_libraries(test-shared PRIVATE processmemorymonitor_shared)
set_target_properties(test-shared PROPERTIES
        INSTALL_RPATH "\$ORIGIN/../lib"
        BUILD_WITH_INSTALL_RPATH TRUE
)

# 安装规则（可选）
install(TARGETS processmemorymonitor_shared processmemorymonitor_static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(TARGETS process_memory_monitor test-static test-shared
        RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)