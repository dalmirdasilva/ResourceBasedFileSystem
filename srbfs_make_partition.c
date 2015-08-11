/**
 * srbfs - Simple Resource Based File System
 * 
 * srbfs_init_partition.c
 * 
 * Initializes a srbfs partition
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SRBFS_MAKE_PARTITION_C__
#define __SRBFS_MAKE_PARTITION_C__ 1

#include "srbfs_make_partition.h"

void srbfs_make_partition(srbfs_t *srbfs, srbfs_disk_size_t size, srbfs_environment_t env, srbfs_driver_t driver) {
    
    srbfs->driver = driver;
    switch(size) {
        
        case SRBFS_DISK_32K:
            srbfs->memory_size = 0x7f94; //32660;
            srbfs->resource_descriptor_table_address = 0x0020; //32;
            srbfs->cluster_table_address = 0x00a0; //160;
            srbfs->sizeof_resource_descriptor_table = 0x0080; //128;
            srbfs->sizeof_cluster_table = 0x7ef4; //32500;
            srbfs->sizeof_resource_descriptor = 0x04; //4;
            srbfs->sizeof_cluster = 0x82; //130;
            srbfs->resource_descriptor_count = 0x20; //32;
            srbfs->cluster_count = 0xfa; //250;
            srbfs->sizeof_cluster_data = 0x80; //128;
            srbfs->sizeof_cluster_control = 0x02; //2;
            srbfs->free_clusters = 0xfa; //250;
        break;
        
        case SRBFS_DISK_8K:
            srbfs->memory_size = 0x2000; //8192;
            srbfs->resource_descriptor_table_address = 0x0020; //32;
            srbfs->cluster_table_address = 0x00a0; //160;
            srbfs->sizeof_resource_descriptor_table = 0x0080; //128;
            srbfs->sizeof_cluster_table = 0x1f60; //8032;
            srbfs->sizeof_resource_descriptor = 0x04; //4;
            srbfs->sizeof_cluster = 0x20; //32;
            srbfs->resource_descriptor_count = 0x20; //32;
            srbfs->cluster_count = 0xfb; //251;
            srbfs->sizeof_cluster_data = 0x1e; //30;
            srbfs->sizeof_cluster_control = 0x02; //2;
            srbfs->free_clusters = 0xfb; //251;
        break;
        
        default:
            srbfs->memory_size = 0xf46; //3910;
            srbfs->resource_descriptor_table_address = 0x0020; //32;
            srbfs->cluster_table_address = 0x00a0; //160;
            srbfs->sizeof_resource_descriptor_table = 0x0080; //128;
            srbfs->sizeof_cluster_table = 0xea6; //3750;
            srbfs->sizeof_resource_descriptor = 0x04; //4;
            srbfs->sizeof_cluster = 0x0f; //32;
            srbfs->resource_descriptor_count = 0x20; //32;
            srbfs->cluster_count = 0xfa; //250;
            srbfs->sizeof_cluster_data = 0x0d; //13;
            srbfs->sizeof_cluster_control = 0x02; //2;
            srbfs->free_clusters = 0xfa; //250;
        break;
    }
    srbfs->flags = 0x00; //0;
}

#endif // __SRBFS_MAKE_PARTITION_C__
