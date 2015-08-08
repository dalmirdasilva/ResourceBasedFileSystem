/**
 * srofs - Simple Resource Oriented File System
 * 
 * srofs_io.h
 * 
 * IO lib for srofs
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SROFS_IO_H__
#define __SROFS_IO_H__ 1

#include "srofs.h"

uint8_t (*external_read)(uint8_t driver, uint16_t address);
void (*external_write)(uint8_t driver, uint16_t address, uint8_t data_to_write);

uint8_t _srofs_io_read(srofs_driver_t driver, srofs_memory_address_t address);

void _srofs_io_write(srofs_driver_t driver, srofs_memory_address_t address, uint8_t data_to_write);

#endif // __SROFS_IO_H__
