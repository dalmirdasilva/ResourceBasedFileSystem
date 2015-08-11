/**
 * rbfs - Simple Resource Based File System
 * 
 * rbfs_init_partition.c
 * 
 * Initializes a rbfs partition
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __RBFS_MAKE_PARTITION_C__
#define __RBFS_MAKE_PARTITION_C__ 1

#include "rbfs_make_partition.h"

void rbfs_make_partition(rbfs_t *rbfs, rbfs_disk_size_t size, rbfs_environment_t env, rbfs_driver_t driver) {
    
    rbfs->driver = driver;
    switch(size) {
        
        case RBFS_DISK_32K:
            rbfs->memory_size = 0x7f94; //32660;
            rbfs->resource_descriptor_table_address = 0x0020; //32;
            rbfs->cluster_table_address = 0x00a0; //160;
            rbfs->sizeof_resource_descriptor_table = 0x0080; //128;
            rbfs->sizeof_cluster_table = 0x7ef4; //32500;
            rbfs->sizeof_resource_descriptor = 0x04; //4;
            rbfs->sizeof_cluster = 0x82; //130;
            rbfs->resource_descriptor_count = 0x20; //32;
            rbfs->cluster_count = 0xfa; //250;
            rbfs->sizeof_cluster_data = 0x80; //128;
            rbfs->sizeof_cluster_control = 0x02; //2;
            rbfs->free_clusters = 0xfa; //250;
        break;
        
        case RBFS_DISK_8K:
            rbfs->memory_size = 0x2000; //8192;
            rbfs->resource_descriptor_table_address = 0x0020; //32;
            rbfs->cluster_table_address = 0x00a0; //160;
            rbfs->sizeof_resource_descriptor_table = 0x0080; //128;
            rbfs->sizeof_cluster_table = 0x1f60; //8032;
            rbfs->sizeof_resource_descriptor = 0x04; //4;
            rbfs->sizeof_cluster = 0x20; //32;
            rbfs->resource_descriptor_count = 0x20; //32;
            rbfs->cluster_count = 0xfb; //251;
            rbfs->sizeof_cluster_data = 0x1e; //30;
            rbfs->sizeof_cluster_control = 0x02; //2;
            rbfs->free_clusters = 0xfb; //251;
        break;
        
        default:
            rbfs->memory_size = 0xf46; //3910;
            rbfs->resource_descriptor_table_address = 0x0020; //32;
            rbfs->cluster_table_address = 0x00a0; //160;
            rbfs->sizeof_resource_descriptor_table = 0x0080; //128;
            rbfs->sizeof_cluster_table = 0xea6; //3750;
            rbfs->sizeof_resource_descriptor = 0x04; //4;
            rbfs->sizeof_cluster = 0x0f; //32;
            rbfs->resource_descriptor_count = 0x20; //32;
            rbfs->cluster_count = 0xfa; //250;
            rbfs->sizeof_cluster_data = 0x0d; //13;
            rbfs->sizeof_cluster_control = 0x02; //2;
            rbfs->free_clusters = 0xfa; //250;
        break;
    }
    rbfs->flags = 0x00; //0;
}

#endif // __RBFS_MAKE_PARTITION_C__
