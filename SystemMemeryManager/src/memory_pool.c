#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define POOL_SIZE 4096      // 内存池总大小

// 内存块头部结构（包含元数据）
typedef struct MemoryBlock {
    struct MemoryBlock *next;
    struct MemoryBlock *prev; // 双向链表支持
    size_t size;              // 当前块大小（包含头部）
    int is_free;              // 是否空闲
} MemoryBlock;

// 内存池结构
typedef struct {
    MemoryBlock *head;        // 内存块链表头
    char pool[POOL_SIZE];     // 实际内存池
} MemoryPool;

// 全局内存池
static MemoryPool memory_pool;

// 初始化内存池
void init_memory_pool() {
    // 初始化第一个块（覆盖整个内存池）
    MemoryBlock *first_block = (MemoryBlock*)memory_pool.pool;
    first_block->next = NULL;
    first_block->prev = NULL;
    first_block->size = POOL_SIZE;
    first_block->is_free = 1;

    memory_pool.head = first_block;
}

// 从内存池分配内存
void* pool_alloc(size_t size) {
    // 计算所需总大小（包含头部）
    size_t total_size = size + sizeof(MemoryBlock);

    // 查找第一个足够大的空闲块
    MemoryBlock *current = memory_pool.head;
    while (current != NULL) {
        if (current->is_free && current->size >= total_size) {
            // 找到合适的块

            // 检查是否需要分割（剩余空间足够容纳新块）
            if (current->size >= total_size + sizeof(MemoryBlock) + 16) {
                // 创建新块
                MemoryBlock *new_block = (MemoryBlock*)((char*)current + total_size);
                new_block->size = current->size - total_size;
                new_block->is_free = 1;
                new_block->prev = current;
                new_block->next = current->next;

                if (current->next != NULL) {
                    current->next->prev = new_block;
                }

                current->next = new_block;
                current->size = total_size;
            }

            // 标记为已使用
            current->is_free = 0;

            // 返回数据区域指针（跳过头部）
            return (void*)((char*)current + sizeof(MemoryBlock));
        }

        current = current->next;
    }

    // 没有足够空间，使用标准分配
    printf("Out of memory in pool, malloc size %zu\n", size);
    return malloc(size);
}

// 合并相邻的空闲块
static void merge_adjacent_free_blocks() {
    MemoryBlock *current = memory_pool.head;

    while (current != NULL && current->next != NULL) {
        if (current->is_free && current->next->is_free) {
            // 合并当前块和下一个块
            current->size += current->next->size;
            current->next = current->next->next;

            if (current->next != NULL) {
                current->next->prev = current;
            }

            // 继续检查合并后的块是否可以继续合并
            continue;
        }

        current = current->next;
    }
}

// 释放内存回内存池
void pool_free(void *ptr) {
    if (ptr == NULL) return;

    // 计算块头指针
    MemoryBlock *block = (MemoryBlock*)((char*)ptr - sizeof(MemoryBlock));

    // 检查指针是否在内存池范围内
    char *pool_start = (char*)&memory_pool.pool[0];
    char *pool_end = pool_start + POOL_SIZE;

    if ((char*)block >= pool_start && (char*)block < pool_end) {
        // 标记为空闲
        block->is_free = 1;

        // 尝试合并相邻的空闲块
        merge_adjacent_free_blocks();
    } else {
        // 不是内存池分配的内存，使用标准释放
        free(ptr);
    }
}