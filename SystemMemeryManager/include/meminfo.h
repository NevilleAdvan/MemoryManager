//
// Created by Luxiantai on 25-5-27.
//
#ifndef MEMINFO_H
#define MEMINFO_H

#include <stdbool.h>
#include <stdint.h>

// 内存信息结构体
typedef struct {
    uint64_t mem_total;
    uint64_t mem_free;
    uint64_t mem_available;
    uint64_t buffers;
    uint64_t cached;
    uint64_t swap_cached;
    uint64_t active;
    uint64_t inactive;
    uint64_t active_anon;
    uint64_t inactive_anon;
    uint64_t active_file;
    uint64_t inactive_file;
    uint64_t unevictable;
    uint64_t mlocked;
    uint64_t swap_total;
    uint64_t swap_free;
    uint64_t dirty;
    uint64_t writeback;
    uint64_t anon_pages;
    uint64_t mapped;
    uint64_t shmem;
    uint64_t slab;
    uint64_t s_reclaimable;
    uint64_t s_unreclaim;
    uint64_t kernel_stack;
    uint64_t page_tables;
    uint64_t nfs_unstable;
    uint64_t bounce;
    uint64_t writeback_tmp;
    uint64_t commit_limit;
    uint64_t committed_as;
    uint64_t vmalloc_total;
    uint64_t vmalloc_used;
    uint64_t vmalloc_chunk;
    uint64_t percpu;
    uint64_t hardware_corrupted;
    uint64_t anon_huge_pages;
    uint64_t shmem_huge_pages;
    uint64_t shmem_pmdmapped;
    uint64_t file_huge_pages;
    uint64_t file_pmdmapped;
    uint64_t huge_pages_total;
    uint64_t huge_pages_free;
    uint64_t huge_pages_rsvd;
    uint64_t huge_pages_surp;
    uint64_t hugepagesize;
    uint64_t hugetlb;
    uint64_t direct_map4k;
    uint64_t direct_map2M;
    uint64_t direct_map1G;
} MemInfo;

bool init_parser();
// 解析/proc/meminfo文件
bool parse_meminfo(MemInfo *info);

// 打印内存信息（KB格式）
void print_kb_format(const MemInfo *info);

// 打印内存信息（人类可读格式）
void print_human_format(const MemInfo *info);

#endif // MEMINFO_H
