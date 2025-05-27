# 项目名称和版本
project(SystemMemoryManager VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 输出目录设置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# 添加头文件目录
include_directories(include)

# 库源文件（排除main.cpp）
set(LIB_SOURCES
        src/meminfo.cpp
        src/memory_pool.cpp
)

# 主程序源文件
set(MAIN_SOURCES
        src/main.cpp
)

# 生成静态库
add_library(systemmemorymanager_static STATIC ${LIB_SOURCES})
set_target_properties(systemmemorymanager_static PROPERTIES
        OUTPUT_NAME systemmemorymanager
        POSITION_INDEPENDENT_CODE ON  # 确保静态库可用于动态链接
)

# 生成动态库
add_library(systemmemorymanager_shared SHARED ${LIB_SOURCES})
set_target_properties(systemmemorymanager_shared PROPERTIES
        OUTPUT_NAME systemmemorymanager
        VERSION 1.0.0
        SOVERSION 1
)

# 直接编译源代码的可执行文件
add_executable(systemmemorymanager ${MAIN_SOURCES} ${LIB_SOURCES})

# 使用静态库的测试程序
add_executable(systemmemorymanagertest-static ${MAIN_SOURCES})
target_link_libraries(systemmemorymanagertest-static PRIVATE systemmemorymanager_static)

# 使用动态库的测试程序（设置rpath）
add_executable(systemmemorymanagertest-shared ${MAIN_SOURCES})
target_link_libraries(systemmemorymanagertest-shared PRIVATE systemmemorymanager_shared)
set_target_properties(systemmemorymanagertest-shared PROPERTIES
        INSTALL_RPATH "\$ORIGIN/../lib"  # 运行时搜索lib目录
        BUILD_WITH_INSTALL_RPATH TRUE
)

# 自定义安装路径（项目根目录下的lib和bin）
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

# 安装规则
install(TARGETS systemmemorymanager_shared systemmemorymanager_static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(TARGETS systemmemorymanager systemmemorymanagertest-static systemmemorymanagertest-shared
        RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)