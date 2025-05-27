//
// Created by Luxiantai on 25-5-27.
//
#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

// 初始化内存池
void init_memory_pool(void);

// 从内存池分配内存
void* pool_alloc(size_t size);

// 释放内存回内存池
void pool_free(void *ptr);

#endif // MEMORY_POOL_H
