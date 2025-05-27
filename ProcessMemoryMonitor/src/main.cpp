// src/main.cpp
#include <iostream>
#include "../include/process_memory_monitor.h"

using namespace MemoryMonitor;

int main() {
    std::cout << "进程内存监控工具 - 测试程序" << std::endl;
    std::cout << "=================================" << std::endl;

    ProcessMemoryMonitor monitor;

    // 显示系统总内存信息
    monitor.getSystemMemoryInfo();
    std::cout << std::endl;

    // 收集并显示进程内存信息
    auto processes = monitor.collectMemoryData();
    ProcessMemoryMonitor::sortProcessesByPSS(processes);
    monitor.printMemoryData(processes);

    return 0;
}