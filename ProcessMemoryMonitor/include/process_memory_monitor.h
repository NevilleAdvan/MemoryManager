#ifndef PROCESS_MEMORY_MONITOR_H
#define PROCESS_MEMORY_MONITOR_H

#include <string>
#include <vector>
#include <chrono>
#include <ctime>

namespace MemoryMonitor {

// 定义进程内存信息结构体
struct ProcessMemoryInfo {
    std::string name;
    double pss;  // 实际使用的物理内存，以MB为单位
    double rss;  // 驻留集大小，以MB为单位
    double vss;  // 虚拟内存大小，以MB为单位
};

// 内存监控器类
class ProcessMemoryMonitor {
public:
    ProcessMemoryMonitor();
    ~ProcessMemoryMonitor();

    // 收集所有进程的内存信息
    std::vector<ProcessMemoryInfo> collectMemoryData();
    
    // 输出内存信息
    void printMemoryData(const std::vector<ProcessMemoryInfo>& processes);
    
    // 获取系统总内存信息
    void getSystemMemoryInfo();
    
    // 按PSS排序进程
    static void sortProcessesByPSS(std::vector<ProcessMemoryInfo>& processes);
    
    // 获取当前时间字符串
    static std::string getCurrentTime();

private:
    // 从文件中提取指定行信息
    std::string extractLine(const std::string& filePath, const std::string& keyword);
    
    // 从行中提取数值
    long extractValue(const std::string& line);
    
    // 计算PSS
    long calculatePSS(const std::string& smapsPath);
};

} // namespace MemoryMonitor

#endif // PROCESS_MEMORY_MONITOR_H