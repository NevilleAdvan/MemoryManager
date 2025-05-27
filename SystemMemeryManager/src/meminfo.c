// meminfo.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>  // 添加这个头文件
#include "memory_pool.h"
#include "meminfo.h"

#define MAX_LINE_LENGTH 256
// 内存池初始化（封装内部实现，外部不可见）
static bool parser_initialized  = false;

bool init_parser() {
    if (parser_initialized) return true;

    init_memory_pool();
    parser_initialized = true;
    return true;
}
// 解析/proc/meminfo文件
bool parse_meminfo(MemInfo *info) {
    if (!parser_initialized ) {
        fprintf(stderr, "Memory pool not initialized\n");
        return false;
    }

    FILE *fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/meminfo");
        return false;
    }

    char *line = (char*)pool_alloc(MAX_LINE_LENGTH);
    if (!line) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(fp);
        return false;
    }
    memset(info, 0, sizeof(MemInfo)); // 初始化结构体

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        char key[64];
        uint64_t value;
        char unit[8] = {0};

        // 修改：使用 PRIu64 宏替代 %llu
        if (sscanf(line, "%63[^:]: %" PRIu64 " %7s", key, &value, unit) >= 2) {
            if (strcmp(key, "MemTotal") == 0) {
                info->mem_total = value;
            } else if (strcmp(key, "MemFree") == 0) {
                info->mem_free = value;
            } else if (strcmp(key, "MemAvailable") == 0) {
                info->mem_available = value;
            } else if (strcmp(key, "Buffers") == 0) {
                info->buffers = value;
            } else if (strcmp(key, "Cached") == 0) {
                info->cached = value;
            } else if (strcmp(key, "SwapCached") == 0) {
                info->swap_cached = value;
            } else if (strcmp(key, "Active") == 0) {
                info->active = value;
            } else if (strcmp(key, "Inactive") == 0) {
                info->inactive = value;
            } else if (strcmp(key, "Active(anon)") == 0) {
                info->active_anon = value;
            } else if (strcmp(key, "Inactive(anon)") == 0) {
                info->inactive_anon = value;
            } else if (strcmp(key, "Active(file)") == 0) {
                info->active_file = value;
            } else if (strcmp(key, "Inactive(file)") == 0) {
                info->inactive_file = value;
            } else if (strcmp(key, "Unevictable") == 0) {
                info->unevictable = value;
            } else if (strcmp(key, "Mlocked") == 0) {
                info->mlocked = value;
            } else if (strcmp(key, "SwapTotal") == 0) {
                info->swap_total = value;
            } else if (strcmp(key, "SwapFree") == 0) {
                info->swap_free = value;
            } else if (strcmp(key, "Dirty") == 0) {
                info->dirty = value;
            } else if (strcmp(key, "Writeback") == 0) {
                info->writeback = value;
            } else if (strcmp(key, "AnonPages") == 0) {
                info->anon_pages = value;
            } else if (strcmp(key, "Mapped") == 0) {
                info->mapped = value;
            } else if (strcmp(key, "Shmem") == 0) {
                info->shmem = value;
            } else if (strcmp(key, "Slab") == 0) {
                info->slab = value;
            } else if (strcmp(key, "SReclaimable") == 0) {
                info->s_reclaimable = value;
            } else if (strcmp(key, "SUnreclaim") == 0) {
                info->s_unreclaim = value;
            } else if (strcmp(key, "KernelStack") == 0) {
                info->kernel_stack = value;
            } else if (strcmp(key, "PageTables") == 0) {
                info->page_tables = value;
            } else if (strcmp(key, "NFS_Unstable") == 0) {
                info->nfs_unstable = value;
            } else if (strcmp(key, "Bounce") == 0) {
                info->bounce = value;
            } else if (strcmp(key, "WritebackTmp") == 0) {
                info->writeback_tmp = value;
            } else if (strcmp(key, "CommitLimit") == 0) {
                info->commit_limit = value;
            } else if (strcmp(key, "Committed_AS") == 0) {
                info->committed_as = value;
            } else if (strcmp(key, "VmallocTotal") == 0) {
                info->vmalloc_total = value;
            } else if (strcmp(key, "VmallocUsed") == 0) {
                info->vmalloc_used = value;
            } else if (strcmp(key, "VmallocChunk") == 0) {
                info->vmalloc_chunk = value;
            } else if (strcmp(key, "Percpu") == 0) {
                info->percpu = value;
            } else if (strcmp(key, "HardwareCorrupted") == 0) {
                info->hardware_corrupted = value;
            } else if (strcmp(key, "AnonHugePages") == 0) {
                info->anon_huge_pages = value;
            } else if (strcmp(key, "ShmemHugePages") == 0) {
                info->shmem_huge_pages = value;
            } else if (strcmp(key, "ShmemPmdMapped") == 0) {
                info->shmem_pmdmapped = value;
            } else if (strcmp(key, "FileHugePages") == 0) {
                info->file_huge_pages = value;
            } else if (strcmp(key, "FilePmdMapped") == 0) {
                info->file_pmdmapped = value;
            } else if (strcmp(key, "HugePages_Total") == 0) {
                info->huge_pages_total = value;
            } else if (strcmp(key, "HugePages_Free") == 0) {
                info->huge_pages_free = value;
            } else if (strcmp(key, "HugePages_Rsvd") == 0) {
                info->huge_pages_rsvd = value;
            } else if (strcmp(key, "HugePages_Surp") == 0) {
                info->huge_pages_surp = value;
            } else if (strcmp(key, "Hugepagesize") == 0) {
                info->hugepagesize = value;
            } else if (strcmp(key, "Hugetlb") == 0) {
                info->hugetlb = value;
            } else if (strcmp(key, "DirectMap4k") == 0) {
                info->direct_map4k = value;
            } else if (strcmp(key, "DirectMap2M") == 0) {
                info->direct_map2M = value;
            } else if (strcmp(key, "DirectMap1G") == 0) {
                info->direct_map1G = value;
            }
        }
    }

    // 释放内存池分配的缓冲区
    pool_free(line);
    fclose(fp);
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
        snprintf(buf, size, "%5" PRIu64 " %s", kb, units[unit]);
    } else {
        // 大于等于1024KB时显示一位小数
        snprintf(buf, size, "%5.1f %s", value, units[unit]);
    }
}

// 打印内存信息（KB格式）
void print_kb_format(const MemInfo *info) {
    // 计算内存使用情况
    uint64_t mem_used = info->mem_total - info->mem_free;
    uint64_t swap_used = info->swap_total - info->swap_free;
    uint64_t buff_cache = info->buffers + info->cached + info->s_reclaimable;

    // 打印表头
    printf("              total        used        free      shared  buff/cache   available\n");

    // 修改：使用 PRIu64 宏替代 %llu
    printf("Mem:       %10" PRIu64 " %10" PRIu64 " %10" PRIu64 " %10" PRIu64 " %10" PRIu64 " %10" PRIu64 "\n",
           info->mem_total, mem_used, info->mem_free, info->shmem, buff_cache, info->mem_available);

    // 修改：使用 PRIu64 宏替代 %llu
    printf("Swap:      %10" PRIu64 " %10" PRIu64 " %10" PRIu64 "\n",
           info->swap_total, swap_used, info->swap_free);
}

// 打印内存信息（人类可读格式）
void print_human_format(const MemInfo *info) {
    // 计算内存使用情况
    uint64_t mem_used = info->mem_total - info->mem_free;
    uint64_t swap_used = info->swap_total - info->swap_free;
    uint64_t buff_cache = info->buffers + info->cached + info->s_reclaimable;

    // 使用内存池分配的缓冲区
    char *total = (char*)pool_alloc(16);
    char *used = (char*)pool_alloc(16);
    char *free = (char*)pool_alloc(16);
    char *shared = (char*)pool_alloc(16);
    char *buff = (char*)pool_alloc(16);
    char *avail = (char*)pool_alloc(16);

    if (!total || !used || !free || !shared || !buff || !avail) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // 格式化内存信息
    format_human_readable(total, 16, info->mem_total);
    format_human_readable(used, 16, mem_used);
    format_human_readable(free, 16, info->mem_free);
    format_human_readable(shared, 16, info->shmem);
    format_human_readable(buff, 16, buff_cache);
    format_human_readable(avail, 16, info->mem_available);

    // 打印表头
    printf("              total        used        free      shared  buff/cache   available\n");

    // 打印内存信息
    printf("Mem:         %s     %s     %s     %s     %s     %s\n",
           total, used, free, shared, buff, avail);

    // 格式化交换空间信息
    format_human_readable(total, 16, info->swap_total);
    format_human_readable(used, 16, swap_used);
    format_human_readable(free, 16, info->swap_free);

    // 打印交换空间信息
    printf("Swap:        %s     %s     %s\n", total, used, free);

    // 释放内存池分配的缓冲区
    pool_free(total);
    pool_free(used);
    pool_free(free);
    pool_free(shared);
    pool_free(buff);
    pool_free(avail);
}