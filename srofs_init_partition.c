/**
 * srofs - Simple Resource Oriented File System
 * 
 * srofs_init_partition.c
 * 
 * Initializes a srofs partition
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SROFS_INIT_PARTITION_C__
#define __SROFS_INIT_PARTITION_C__ 1

#include "srofs_init_partition.h"

void srofs_init_partition(srofs_t *srofs, srofs_disk_size_t size, srofs_environment_t env) {
    
    switch(size) {
        
        case SROFS_DISK_32K:
            if (env == SROFS_ENV_VIRTUAL) {
                srofs->driver = SROFS_DRIVER_VIRTUAL;
            } else {
                srofs->driver = SROFS_DRIVER_ARDUINO_EEPROM;
            }
            srofs->memory_size = 0x7f94; //32660;
            srofs->resource_descriptor_table_address = 0x0020; //32;
            srofs->cluster_table_address = 0x00a0; //160;
            srofs->sizeof_resource_descriptor_table = 0x0080; //128;
            srofs->sizeof_cluster_table = 0x7ef4; //32500;
            srofs->sizeof_resource_descriptor = 0x04; //4;
            srofs->sizeof_cluster = 0x82; //130;
            srofs->resource_descriptor_count = 0x20; //32;
            srofs->cluster_count = 0xfa; //250;
            srofs->sizeof_cluster_data = 0x80; //128;
            srofs->sizeof_cluster_control = 0x02; //2;
            srofs->free_clusters = 0xfa; //250;
            srofs->flags = 0x00; //0;
        break;
        
        case SROFS_DISK_8K:
            if (env == SROFS_ENV_VIRTUAL) {
                srofs->driver = SROFS_DRIVER_VIRTUAL;
            } else {
                srofs->driver = SROFS_DRIVER_ARDUINO_EEPROM;
            }
            srofs->memory_size = 0x2000; //8192;
            srofs->resource_descriptor_table_address = 0x0020; //32;
            srofs->cluster_table_address = 0x00a0; //160;
            srofs->sizeof_resource_descriptor_table = 0x0080; //128;
            srofs->sizeof_cluster_table = 0x1f60; //8032;
            srofs->sizeof_resource_descriptor = 0x04; //4;
            srofs->sizeof_cluster = 0x20; //32;
            srofs->resource_descriptor_count = 0x20; //32;
            srofs->cluster_count = 0xfb; //251;
            srofs->sizeof_cluster_data = 0x1e; //30;
            srofs->sizeof_cluster_control = 0x02; //2;
            srofs->free_clusters = 0xfb; //251;
            srofs->flags = 0x00; //0;
        break;
        
        default:
            
            if (env == SROFS_ENV_VIRTUAL) {
                srofs->driver = SROFS_DRIVER_VIRTUAL;
            } else {
                srofs->driver = SROFS_DRIVER_ARDUINO_EEPROM;
            }
            srofs->memory_size = 0xf46; //3910;
            srofs->resource_descriptor_table_address = 0x0020; //32;
            srofs->cluster_table_address = 0x00a0; //160;
            srofs->sizeof_resource_descriptor_table = 0x0080; //128;
            srofs->sizeof_cluster_table = 0xea6; //3750;
            srofs->sizeof_resource_descriptor = 0x04; //4;
            srofs->sizeof_cluster = 0x0f; //32;
            srofs->resource_descriptor_count = 0x20; //32;
            srofs->cluster_count = 0xfa; //250;
            srofs->sizeof_cluster_data = 0x0d; //13;
            srofs->sizeof_cluster_control = 0x02; //2;
            srofs->free_clusters = 0xfa; //250;
            srofs->flags = 0x00; //0;
        break;
    }
}

#endif // __SROFS_INIT_PARTITION_C__
