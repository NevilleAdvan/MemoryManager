//
// Created by Luxiantai on 25-5-27.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory_pool.h"
#include "meminfo.h"

int main(int argc, char *argv[]) {
    // 初始化内存池
    init_parser();

    MemInfo info = {0};
    parse_meminfo(&info);

    // 检查命令行参数
    int human_readable = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--human") == 0) {
            human_readable = 1;
        } else if (strcmp(argv[i], "-k") == 0 || strcmp(argv[i], "--kilo") == 0) {
            human_readable = 0;
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [options]\n", argv[0]);
            printf("Options:\n");
            printf("  -h, --human      Show human-readable output\n");
            printf("  -k, --kilo       Show output in kilobytes\n");
            printf("      --help       Show this help message\n");
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            return 1;
        }
    }

    // 根据参数选择输出格式
    if (human_readable) {
        print_human_format(&info);
    } else {
        print_kb_format(&info);
    }

    return 0;
}