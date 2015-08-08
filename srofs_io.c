/**
 * srofs - Simple Resource Oriented File System
 * 
 * srofs_io.c
 * 
 * IO lib for srofs
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SROFS_IO_C__
#define __SROFS_IO_C__ 1

#include "srofs.h"
#include "srofs_io.h"

unsigned char _srofs_io_read(srofs_driver_t driver, srofs_memory_address_t address) {
    return external_read(driver, address);
}

void _srofs_io_write(srofs_driver_t driver, srofs_memory_address_t address, unsigned char data_to_write) {
}

#endif // __SROFS_IO_C__
