/**
 * srofs - Simple Resource Oriented File System
 * 
 * srofs_init_partition.h
 * 
 * Initializes a srofs partition
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SROFS_INIT_PARTITION_H__
#define __SROFS_INIT_PARTITION_H__ 1

#include "srofs.h"

typedef enum {
    SROFS_DISK_4K,
    SROFS_DISK_8K,
    SROFS_DISK_32K
} srofs_disk_size_t;

typedef enum {
    SROFS_ENV_ARDUINO,
    SROFS_ENV_VIRTUAL
} srofs_environment_t;

void srofs_init_partition(srofs_t *srofs, srofs_disk_size_t size, srofs_environment_t env);

#endif /* __SROFS_INIT_PARTITION_H__ */
