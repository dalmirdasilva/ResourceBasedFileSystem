/**
 * srbfs - Simple Resource Based File System
 * 
 * srbfs_init_partition.h
 * 
 * Initializes a srbfs partition
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SRBFS_INIT_PARTITION_H__
#define __SRBFS_INIT_PARTITION_H__ 1

#include "srbfs.h"

typedef enum {
    SRBFS_DISK_4K,
    SRBFS_DISK_8K,
    SRBFS_DISK_32K
} srbfs_disk_size_t;

typedef enum {
    SRBFS_ENV_ARDUINO,
    SRBFS_ENV_VIRTUAL
} srbfs_environment_t;

void srbfs_init_partition(srbfs_t *srbfs, srbfs_disk_size_t size, srbfs_environment_t env);

#endif /* __SRBFS_INIT_PARTITION_H__ */
