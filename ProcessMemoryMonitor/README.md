<!-- TOC -->
* [Process Memory Monitor](#process-memory-monitor)
* [功能特点](#功能特点)
* [目录结构](#目录结构)
* [编译方法](#编译方法)
    * [使用 CMake (推荐)](#使用-cmake-推荐)
    * [使用 Makefile](#使用-makefile)
* [使用方法](#使用方法)
    * [直接运行可执行文件](#直接运行可执行文件)
    * [使用静态库](#使用静态库)
    * [使用动态库](#使用动态库)
    * [集成到其他项目](#集成到其他项目)
* [依赖项](#依赖项)
<!-- TOC -->

# Process Memory Monitor
这是一个用于监控 Linux 系统进程内存使用情况的工具，能够获取所有进程的 PSS、RSS 和 VSS 信息，并统计总内存占用量。工具提供了 C++ 源代码、静态库和动态库，方便集成到其他项目中。

# 功能特点
- 获取系统中所有进程的 PSS (Proportional Set Size)、RSS (Resident Set Size) 和 VSS (Virtual Set Size) 内存使用情况
- 按内存使用量排序显示进程
- 统计系统总内存使用情况
- 支持持续监控或指定次数监控
- 提供静态库和动态库，方便集成到其他项目

# 目录结构
```plaintext
process-memory-monitor/
├── include/                      # 头文件目录
│   └── process_memory_monitor.h  # 库的公共头文件
├── src/                          # 源代码目录
│   ├── main.cpp                  # 主程序入口
│   └── process_memory_monitor.cpp # 库的实现文件
├── CMakeLists.txt                # CMake构建文件
├── Makefile                      # Makefile构建文件
└── README.md                     # 项目说明文档
```

# 编译方法
## 使用 CMake (推荐)
```bash
# 创建构建目录
mkdir build
cd build

# 运行CMake生成Makefile
cmake ..

# 编译项目
make

# 可选：安装到系统
make install
```

## 使用 Makefile
```bash
# 编译所有目标
make

# 清理构建文件
make clean
```

# 使用方法
## 直接运行可执行文件
```bash
# 持续监控（按Ctrl+C终止）
./bin/process_memory_monitor

# 指定监控次数
./bin/process_memory_monitor -t 5
```

## 使用静态库
```bash
# 编译测试程序（使用静态库）
make test-static

# 运行
./bin/test-static
```

## 使用动态库
```bash
# 编译测试程序（使用动态库）
make test-shared

# 运行（需设置库路径）
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
./bin/test-shared
```

## 集成到其他项目
如果你想在自己的项目中使用这个内存监控库，可以这样做：
```cpp
#include "process_memory_monitor.h"

int main() {
ProcessMemoryMonitor monitor;

    // 获取内存数据
    auto processes = monitor.collectMemoryData();
    
    // 其他操作...
    
    return 0;
}
```

编译时链接库：
```bash
g++ your_program.cpp -I/path/to/include -L/path/to/lib -lprocessmemorymonitor -o your_program
```

# 依赖项
Linux 系统
g++ 编译器 (支持 C++11)
CMake (版本 3.10 或更高) 或 Make