//
// Created by Luxiantai on 25-5-27.
//
#include <cstring>
#include <iostream>
#include <string>
#include "meminfo.hpp"

int main(int argc, char *argv[]) {
    // 初始化内存池并检查状态
    if (!init_parser()) {
        return EXIT_FAILURE; // 初始化失败，直接退出
    }

    MemInfo info;
    memset(&info,0,sizeof(MemInfo));
    if (!parse_meminfo(&info)) { // 增加解析结果判断
        return EXIT_FAILURE;
    }

    // 检查命令行参数
    bool human_readable = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--human") {
            human_readable = true;
        } else if (arg == "-k" || arg == "--kilo") {
            human_readable = false;
        } else if (arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  -h, --human      Show human-readable output" << std::endl;
            std::cout << "  -k, --kilo       Show output in kilobytes" << std::endl;
            std::cout << "      --help       Show this help message" << std::endl;
            return 0;
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            return 1;
        }
    }

    // 根据参数选择输出格式
    if (human_readable) {
        print_human_format(&info);
    } else {
        print_kb_format(&info);
    }

    return EXIT_SUCCESS;
}