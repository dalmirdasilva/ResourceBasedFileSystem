/**
 * rbfs - Simple Resource Based File System
 * 
 * rbfs_init_partition.h
 * 
 * Initializes a rbfs partition
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __RBFS_MAKE_PARTITION_H__
#define __RBFS_MAKE_PARTITION_H__ 1

#include "rbfs.h"

typedef enum {
    RBFS_DISK_4K,
    RBFS_DISK_8K,
    RBFS_DISK_32K
} rbfs_disk_size_t;

typedef enum {
    RBFS_ENV_ARDUINO,
    RBFS_ENV_VIRTUAL
} rbfs_environment_t;

void rbfs_make_partition(rbfs_t *rbfs, rbfs_disk_size_t size, rbfs_environment_t env, rbfs_driver_t driver);

#endif // __RBFS_MAKE_PARTITION_H__
