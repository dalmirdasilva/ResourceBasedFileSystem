/**
 * rbfs - Simple Resource Based File System
 * 
 * rbfs_io.h
 * 
 * IO lib for rbfs
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __RBFS_IO_H__
#define __RBFS_IO_H__ 1

#include "rbfs.h"

extern uint8_t _rbfs_io_read(rbfs_driver_t driver, rbfs_memory_address_t address);

extern void _rbfs_io_write(rbfs_driver_t driver, rbfs_memory_address_t address, uint8_t data);

#endif // __RBFS_IO_H__
