//
// Created by Luxiantai on 25-5-27.
//
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define POOL_SIZE 4096      // 内存池大小
#define BLOCK_SIZE 64       // 内存块大小
#define BLOCK_COUNT (POOL_SIZE / BLOCK_SIZE)

// 内存块结构
typedef struct MemoryBlock {
    struct MemoryBlock *next;
    char data[BLOCK_SIZE - sizeof(struct MemoryBlock*)];
} MemoryBlock;

// 内存池结构
typedef struct {
    MemoryBlock *free_list;
    char pool[POOL_SIZE];
} MemoryPool;

// 全局内存池
static MemoryPool memory_pool;

// 初始化内存池
void init_memory_pool() {
    memory_pool.free_list = NULL;

    // 将内存池分割成块并链接起来
    for (int i = 0; i < BLOCK_COUNT; i++) {
        MemoryBlock *block = (MemoryBlock*)&memory_pool.pool[i * BLOCK_SIZE];
        block->next = memory_pool.free_list;
        memory_pool.free_list = block;
    }
}

// 从内存池分配内存
void* pool_alloc(size_t size) {
    if (size > BLOCK_SIZE - sizeof(MemoryBlock*)) {
        // 请求的内存大于块大小，使用标准分配
        return malloc(size);
    }

    if (memory_pool.free_list == NULL) {
        // 内存池为空，使用标准分配
        return malloc(size);
    }

    // 从空闲列表获取一个块
    MemoryBlock *block = memory_pool.free_list;
    memory_pool.free_list = block->next;

    return (void*)block->data;
}

// 释放内存回内存池
void pool_free(void *ptr) {
    if (ptr == NULL) return;

    // 检查指针是否在内存池范围内
    char *pool_start = (char*)&memory_pool.pool[0];
    char *pool_end = pool_start + POOL_SIZE;

    if ((char*)ptr >= pool_start && (char*)ptr < pool_end) {
        // 计算块头指针
        MemoryBlock *block = (MemoryBlock*)((char*)ptr - offsetof(MemoryBlock, data));

        // 将块放回空闲列表
        block->next = memory_pool.free_list;
        memory_pool.free_list = block;
    } else {
        // 不是内存池分配的内存，使用标准释放
        free(ptr);
    }
}