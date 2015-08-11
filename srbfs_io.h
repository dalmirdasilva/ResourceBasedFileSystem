/**
 * srbfs - Simple Resource Based File System
 * 
 * srbfs_io.h
 * 
 * IO lib for srbfs
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SRBFS_IO_H__
#define __SRBFS_IO_H__ 1

#include "srbfs.h"

extern uint8_t _srbfs_io_read(srbfs_driver_t driver, srbfs_memory_address_t address);

extern void _srbfs_io_write(srbfs_driver_t driver, srbfs_memory_address_t address, uint8_t data);

#endif // __SRBFS_IO_H__
