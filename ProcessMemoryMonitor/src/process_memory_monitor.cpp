#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <cstring>
#include "../include/process_memory_monitor.h"

using namespace MemoryMonitor;

ProcessMemoryMonitor::ProcessMemoryMonitor() {
    // 初始化
}

ProcessMemoryMonitor::~ProcessMemoryMonitor() {
    // 清理资源
}

std::string ProcessMemoryMonitor::extractLine(const std::string& filePath, const std::string& keyword) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.find(keyword) != std::string::npos) {
                return line;
            }
        }
        file.close();
    }
    return "";
}

long ProcessMemoryMonitor::extractValue(const std::string& line) {
    std::istringstream iss(line);
    std::string key;
    long value = 0;
    std::string unit;
    if (iss >> key >> value >> unit) {
        // 转换为KB单位
        if (unit == "kB") {
            return value;
        }
    }
    return 0;
}

long ProcessMemoryMonitor::calculatePSS(const std::string& smapsPath) {
    std::ifstream file(smapsPath);
    if (file.is_open()) {
        std::string line;
        long pssTotal = 0;
        while (std::getline(file, line)) {
            if (line.find("Pss:") != std::string::npos) {
                pssTotal += extractValue(line);
            }
        }
        file.close();
        return pssTotal;
    }
    return 0;
}

// 收集所有进程的内存信息
std::vector<ProcessMemoryInfo> ProcessMemoryMonitor::collectMemoryData() {
    std::vector<ProcessMemoryInfo> processes;
    DIR* dir = opendir("/proc");
    if (dir != nullptr) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string pidStr = entry->d_name;
            if (std::all_of(pidStr.begin(), pidStr.end(), ::isdigit)) {
                std::string pidPath = "/proc/" + pidStr;
                std::string statusPath = pidPath + "/status";
                std::string smapsPath = pidPath + "/smaps";

                std::string nameLine = extractLine(statusPath, "Name:");
                std::string rssLine = extractLine(statusPath, "VmRSS:");
                std::string vssLine = extractLine(statusPath, "VmSize:");

                // 处理进程名
                std::string name;
                size_t colonPos = nameLine.find(":");
                if (!nameLine.empty() && colonPos != std::string::npos && colonPos + 2 < nameLine.size()) {
                    name = nameLine.substr(colonPos + 2);
                } else {
                    name = "unknown"; // 如果无法获取进程名，使用默认值
                }

                long rss = extractValue(rssLine);
                long pss = calculatePSS(smapsPath);
                long vss = extractValue(vssLine);

                if (pss > 0 || rss > 0 || vss > 0) {
                    ProcessMemoryInfo info;
                    info.name = name;
                    info.pss = static_cast<double>(pss) / 1024;
                    info.rss = static_cast<double>(rss) / 1024;
                    info.vss = static_cast<double>(vss) / 1024;
                    processes.push_back(info);
                }
            }
        }
        closedir(dir);
    }
    return processes;
}

void ProcessMemoryMonitor::printMemoryData(const std::vector<ProcessMemoryInfo>& processes) {
    std::cout << "统计时间: " << getCurrentTime() << std::endl << std::endl;
    std::cout << "PROCESS                         PSS(MB)           RSS(MB)           VSS(MB)" << std::endl;
    std::cout << "===============================================================================" << std::endl;

    double pssTotal = 0;
    double rssTotal = 0;
    double vssTotal = 0;

    for (const auto& process : processes) {
        std::cout.precision(1);
        std::cout << std::fixed;
        std::cout.width(30);
        std::cout << std::left << process.name;
        std::cout.width(15);
        std::cout << process.pss;
        std::cout.width(15);
        std::cout << process.rss;
        std::cout.width(15);
        std::cout << process.vss << std::endl;

        pssTotal += process.pss;
        rssTotal += process.rss;
        vssTotal += process.vss;
    }

    std::cout << "===============================================================================" << std::endl;
    std::cout.width(30);
    std::cout << std::left << "TOTAL:";
    std::cout.width(15);
    std::cout << pssTotal;
    std::cout.width(15);
    std::cout << rssTotal;
    std::cout.width(15);
    std::cout << vssTotal << std::endl;
}

void ProcessMemoryMonitor::getSystemMemoryInfo() {
    // 实现获取系统总内存信息的功能
    std::string meminfoPath = "/proc/meminfo";
    std::string totalLine = extractLine(meminfoPath, "MemTotal:");
    std::string freeLine = extractLine(meminfoPath, "MemFree:");
    std::string availableLine = extractLine(meminfoPath, "MemAvailable:");
    
    long total = extractValue(totalLine);
    long free = extractValue(freeLine);
    long available = extractValue(availableLine);
    
    std::cout << "系统内存信息:" << std::endl;
    std::cout << "总内存: " << total / 1024 << " MB" << std::endl;
    std::cout << "空闲内存: " << free / 1024 << " MB" << std::endl;
    std::cout << "可用内存: " << available / 1024 << " MB" << std::endl;
}

void ProcessMemoryMonitor::sortProcessesByPSS(std::vector<ProcessMemoryInfo>& processes) {
    std::sort(processes.begin(), processes.end(), 
        [](const ProcessMemoryInfo& a, const ProcessMemoryInfo& b) {
            return a.pss > b.pss;
        });
}

std::string ProcessMemoryMonitor::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    char* timeStr = std::ctime(&currentTime);
    // 移除换行符
    if (timeStr != nullptr && strlen(timeStr) > 0) {
        timeStr[strlen(timeStr) - 1] = '\0';
    }
    return std::string(timeStr ? timeStr : "未知时间");
}