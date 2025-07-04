// meminfo.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdint>  // 添加这个头文件
#include <cinttypes>
#include "memory_pool.hpp"
#include "meminfo.hpp"

#define MAX_LINE_LENGTH 256
// 内存池初始化（封装内部实现，外部不可见）
static bool parser_initialized = false;

bool init_parser() {
    if (parser_initialized) return true;

    init_memory_pool();
    parser_initialized = true;
    return true;
}
// 解析/proc/meminfo文件
bool parse_meminfo(MemInfo *info) {
    if (!parser_initialized) {
        std::cerr << "Memory pool not initialized" << std::endl;
        return false;
    }

    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) {
        std::perror("Failed to open /proc/meminfo");
        return false;
    }

    char *line = static_cast<char*>(pool_alloc(MAX_LINE_LENGTH));
    if (!line) {
        std::cerr << "Memory allocation failed" << std::endl;
        return false;
    }
    std::memset(info, 0, sizeof(MemInfo)); // 初始化结构体

    while (file.getline(line, MAX_LINE_LENGTH)) {
        char key[64];
        uint64_t value;
        char unit[8] = {0};

        // 修改：使用 PRIu64 宏替代 %llu
        if (std::sscanf(line, "%63[^:]: %" PRIu64 " %7s", key, &value, unit) >= 2) {
            if (std::strcmp(key, "MemTotal") == 0) {
                info->mem_total = value;
            } else if (std::strcmp(key, "MemFree") == 0) {
                info->mem_free = value;
            } else if (std::strcmp(key, "MemAvailable") == 0) {
                info->mem_available = value;
            } else if (std::strcmp(key, "Buffers") == 0) {
                info->buffers = value;
            } else if (std::strcmp(key, "Cached") == 0) {
                info->cached = value;
            } else if (std::strcmp(key, "SwapCached") == 0) {
                info->swap_cached = value;
            } else if (std::strcmp(key, "Active") == 0) {
                info->active = value;
            } else if (std::strcmp(key, "Inactive") == 0) {
                info->inactive = value;
            } else if (std::strcmp(key, "Active(anon)") == 0) {
                info->active_anon = value;
            } else if (std::strcmp(key, "Inactive(anon)") == 0) {
                info->inactive_anon = value;
            } else if (std::strcmp(key, "Active(file)") == 0) {
                info->active_file = value;
            } else if (std::strcmp(key, "Inactive(file)") == 0) {
                info->inactive_file = value;
            } else if (std::strcmp(key, "Unevictable") == 0) {
                info->unevictable = value;
            } else if (std::strcmp(key, "Mlocked") == 0) {
                info->mlocked = value;
            } else if (std::strcmp(key, "SwapTotal") == 0) {
                info->swap_total = value;
            } else if (std::strcmp(key, "SwapFree") == 0) {
                info->swap_free = value;
            } else if (std::strcmp(key, "Dirty") == 0) {
                info->dirty = value;
            } else if (std::strcmp(key, "Writeback") == 0) {
                info->writeback = value;
            } else if (std::strcmp(key, "AnonPages") == 0) {
                info->anon_pages = value;
            } else if (std::strcmp(key, "Mapped") == 0) {
                info->mapped = value;
            } else if (std::strcmp(key, "Shmem") == 0) {
                info->shmem = value;
            } else if (std::strcmp(key, "Slab") == 0) {
                info->slab = value;
            } else if (std::strcmp(key, "SReclaimable") == 0) {
                info->s_reclaimable = value;
            } else if (std::strcmp(key, "SUnreclaim") == 0) {
                info->s_unreclaim = value;
            } else if (std::strcmp(key, "KernelStack") == 0) {
                info->kernel_stack = value;
            } else if (std::strcmp(key, "PageTables") == 0) {
                info->page_tables = value;
            } else if (std::strcmp(key, "NFS_Unstable") == 0) {
                info->nfs_unstable = value;
            } else if (std::strcmp(key, "Bounce") == 0) {
                info->bounce = value;
            } else if (std::strcmp(key, "WritebackTmp") == 0) {
                info->writeback_tmp = value;
            } else if (std::strcmp(key, "CommitLimit") == 0) {
                info->commit_limit = value;
            } else if (std::strcmp(key, "Committed_AS") == 0) {
                info->committed_as = value;
            } else if (std::strcmp(key, "VmallocTotal") == 0) {
                info->vmalloc_total = value;
            } else if (std::strcmp(key, "VmallocUsed") == 0) {
                info->vmalloc_used = value;
            } else if (std::strcmp(key, "VmallocChunk") == 0) {
                info->vmalloc_chunk = value;
            } else if (std::strcmp(key, "Percpu") == 0) {
                info->percpu = value;
            } else if (std::strcmp(key, "HardwareCorrupted") == 0) {
                info->hardware_corrupted = value;
            } else if (std::strcmp(key, "AnonHugePages") == 0) {
                info->anon_huge_pages = value;
            } else if (std::strcmp(key, "ShmemHugePages") == 0) {
                info->shmem_huge_pages = value;
            } else if (std::strcmp(key, "ShmemPmdMapped") == 0) {
                info->shmem_pmdmapped = value;
            } else if (std::strcmp(key, "FileHugePages") == 0) {
                info->file_huge_pages = value;
            } else if (std::strcmp(key, "FilePmdMapped") == 0) {
                info->file_pmdmapped = value;
            } else if (std::strcmp(key, "HugePages_Total") == 0) {
                info->huge_pages_total = value;
            } else if (std::strcmp(key, "HugePages_Free") == 0) {
                info->huge_pages_free = value;
            } else if (std::strcmp(key, "HugePages_Rsvd") == 0) {
                info->huge_pages_rsvd = value;
            } else if (std::strcmp(key, "HugePages_Surp") == 0) {
                info->huge_pages_surp = value;
            } else if (std::strcmp(key, "Hugepagesize") == 0) {
                info->hugepagesize = value;
            } else if (std::strcmp(key, "Hugetlb") == 0) {
                info->hugetlb = value;
            } else if (std::strcmp(key, "DirectMap4k") == 0) {
                info->direct_map4k = value;
            } else if (std::strcmp(key, "DirectMap2M") == 0) {
                info->direct_map2M = value;
            } else if (std::strcmp(key, "DirectMap1G") == 0) {
                info->direct_map1G = value;
            }
        }
    }

    // 释放内存池分配的缓冲区
    pool_free(line);
    file.close();
    return true;
}

// 转换为人类可读格式
static void format_human_readable(char *buf, size_t size, uint64_t kb) {
    static const char *units[] = {"K", "M", "G", "T"};
    int unit = 0;
    double value = kb;

    while (value >= 1024 && unit < 3) {
        value /= 1024;
        unit++;
    }

    if (unit == 0) {
        // 修改：使用 PRIu64 宏替代 %llu
        std::snprintf(buf, size, "%5" PRIu64 " %s", kb, units[unit]);
    } else {
        // 大于等于1024KB时显示一位小数
        std::snprintf(buf, size, "%5.1f %s", value, units[unit]);
    }
}

// 打印内存信息（KB格式）
void print_kb_format(const MemInfo *info) {
    // 计算内存使用情况
    uint64_t mem_used = info->mem_total - info->mem_free;
    uint64_t swap_used = info->swap_total - info->swap_free;
    uint64_t buff_cache = info->buffers + info->cached + info->s_reclaimable;

    // 打印表头
    std::cout << "              total        used        free      shared  buff/cache   available" << std::endl;

    // 修改：使用 PRIu64 宏替代 %llu
    std::printf("Mem:       %10" PRIu64 " %10" PRIu64 " %10" PRIu64 " %10" PRIu64 " %10" PRIu64 " %10" PRIu64 "\n",
           info->mem_total, mem_used, info->mem_free, info->shmem, buff_cache, info->mem_available);

    // 修改：使用 PRIu64 宏替代 %llu
    std::printf("Swap:      %10" PRIu64 " %10" PRIu64 " %10" PRIu64 "\n",
           info->swap_total, swap_used, info->swap_free);
}

// 打印内存信息（人类可读格式）
void print_human_format(const MemInfo *info) {
    // 计算内存使用情况
    uint64_t mem_used = info->mem_total - info->mem_free;
    uint64_t swap_used = info->swap_total - info->swap_free;
    uint64_t buff_cache = info->buffers + info->cached + info->s_reclaimable;

    // 使用内存池分配的缓冲区
    char *total = static_cast<char*>(pool_alloc(16));
    char *used = static_cast<char*>(pool_alloc(16));
    char *free = static_cast<char*>(pool_alloc(16));
    char *shared = static_cast<char*>(pool_alloc(16));
    char *buff = static_cast<char*>(pool_alloc(16));
    char *avail = static_cast<char*>(pool_alloc(16));

    if (!total || !used || !free || !shared || !buff || !avail) {
        std::cerr << "Memory allocation failed" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // 格式化内存信息
    format_human_readable(total, 16, info->mem_total);
    format_human_readable(used, 16, mem_used);
    format_human_readable(free, 16, info->mem_free);
    format_human_readable(shared, 16, info->shmem);
    format_human_readable(buff, 16, buff_cache);
    format_human_readable(avail, 16, info->mem_available);

    // 打印表头
    std::cout << "              total        used        free      shared  buff/cache   available" << std::endl;

    // 打印内存信息
    std::cout << "Mem:         " << total << "     " << used << "     " << free << "     " << shared << "     " << buff << "     " << avail << std::endl;

    // 格式化交换空间信息
    format_human_readable(total, 16, info->swap_total);
    format_human_readable(used, 16, swap_used);
    format_human_readable(free, 16, info->swap_free);

    // 打印交换空间信息
    std::cout << "Swap:        " << total << "     " << used << "     " << free << std::endl;

    // 释放内存池分配的缓冲区
    pool_free(total);
    pool_free(used);
    pool_free(free);
    pool_free(shared);
    pool_free(buff);
    pool_free(avail);
}