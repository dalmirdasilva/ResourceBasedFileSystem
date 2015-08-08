/**
 * srbfs - Simple Resource Based File System
 * 
 * srbfs_io.c
 * 
 * IO lib for srbfs
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SRBFS_IO_C__
#define __SRBFS_IO_C__ 1

#include "srbfs.h"
#include "srbfs_io.h"

unsigned char _srbfs_io_read(srbfs_driver_t driver, srbfs_memory_address_t address) {
    return extern_srbfs_read(driver, address);
}

void _srbfs_io_write(srbfs_driver_t driver, srbfs_memory_address_t address, unsigned char data_to_write) {
    extern_srbfs_write(driver, address, data_to_write);
}

#endif // __SRBFS_IO_C__
