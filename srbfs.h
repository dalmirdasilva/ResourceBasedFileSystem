/**
 * srbfs - Simple Resource Based File System
 * 
 * rs.h
 * 
 * An file system header definition based on the idea of resources
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SRBFS_H__
#define __SRBFS_H__ 1

#include <stdint.h>

#define SRBFS_NULL_RESOURCE_CODE                                            0xff
#define SRBFS_NULL_CLUSTER                                                  0xff

#define SRBFS_NULL_RESORCE_DESCRIPTOR_ADDRESS                               0xff
#define SRBFS_NULL_CLUSTER_ADDRESS                                          0x00

#define SRBFS_FIRST_ADDRESS_OF_MEMORY                                       0x00

#define SRBFS_SIZEOF_RESOURCE_SIZE                                          0x02

#define SRBFS_INEXISTENT_CLUSTER                                            0xff

#define CLUSTER_ADDRESS_TO_NEXT(CLUSTER_ADDRESS)                            ((CLUSTER_ADDRESS) + 0)
#define CLUSTER_ADDRESS_TO_PREV(CLUSTER_ADDRESS)                            ((CLUSTER_ADDRESS) + 1)
#define CLUSTER_ADDRESS_TO_DATA(CLUSTER_ADDRESS)                            ((CLUSTER_ADDRESS) + 2)

#define RD_ADDRESS_TO_SIZE_LOW(RD_ADDRESS)                                  ((RD_ADDRESS) + 0)
#define RD_ADDRESS_TO_SIZE_HIGH(RD_ADDRESS)                                 ((RD_ADDRESS) + 1)
#define RD_ADDRESS_TO_FIRST_CLUSTER(RD_ADDRESS)                             ((RD_ADDRESS) + 2)
#define RD_ADDRESS_TO_FLAG(RD_ADDRESS)                                      ((RD_ADDRESS) + 3)

typedef uint8_t srbfs_resource_descriptor_t;
typedef uint8_t srbfs_cluster_t;
typedef uint16_t srbfs_resource_size_t;
typedef uint16_t srbfs_memory_address_t;
typedef uint8_t srbfs_resource_code_t;
typedef uint16_t srbfs_seek_int_t;

// Drivers

typedef enum {
    SRBFS_DRIVER_VIRTUAL = 0,
    SRBFS_DRIVER_SELF_EEPROM = 1,
    SRBFS_DRIVER_MULTI_EXTERNAL_EEPROM = 2,
    SRBFS_DRIVER_EXTERNAL_EEPROM = 3,
    SRBFS_DRIVER_ARDUINO_EEPROM = 4
} srbfs_driver_t;

// Resource fag bit values

typedef enum {
    SRBFS_RESOURCE_FLAG_BIT_OPENED = 1,
    SRBFS_RESOURCE_FLAG_BIT_READ_ONLY = 2,
    SRBFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ = 4,
    SRBFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_WRITE = 8,
    SRBFS_RESOURCE_FLAG_BIT_ALLOCATED = 16,
    SRBFS_RESOURCE_FLAG_BIT_EOR_REACHED = 32
} srbfs_resource_flag_bits_t;

// Options to open a resource

typedef enum {
    SRBFS_OPEN_RESOURCE_OPTION_NORMAL = 0,
    SRBFS_OPEN_RESOURCE_OPTION_READ_ONLY = 1
} srbfs_open_resource_options_t;

// Options to mount a resource

typedef enum {
    SRBFS_MOUNT_OPTION_NORMAL = 0,
    SRBFS_MOUNT_OPTION_READ_ONLY = 1
} srbfs_mount_options_t;

// Rs fag bit values

typedef enum {
    SRBFS_FLAG_BIT_DRIVER_MOUNTED = 1,
    SRBFS_FLAG_BIT_READ_ONLY = 2
} srbfs_flag_bits_t;

// Operation result

typedef enum {
    SRBFS_OP_RESULT_SUCCESS = 0,
    SRBFS_OP_RESULT_ERROR_RESOURCE_OPENED = 1,
    SRBFS_OP_RESULT_ERROR_RESOURCE_CLOSED = 2,
    SRBFS_OP_RESULT_ERROR_RESOURCE_READ_ONLY = 3,
    SRBFS_OP_RESULT_ERROR_NO_SPACE_AVAILABLE = 4,
    SRBFS_OP_RESULT_ERROR_DRIVER_BUSY = 5,
    SRBFS_OP_RESULT_ERROR_SEEK_OUT_OF_BOUND = 6,
    SRBFS_OP_RESULT_ERROR_RESOURCE_DOES_NOT_ALLOCATED = 7,
    SRBFS_OP_RESULT_ERROR_DRIVER_NOT_MOUNTED = 8
} srbfs_op_result_t;

// Seek position reference

typedef enum {
    SRBFS_SEEK_ORIGIN_BEGIN = 0,
    SRBFS_SEEK_ORIGIN_CURRENT = 1
} srbfs_seek_origin_t;

typedef struct {
    uint8_t flags;
} srbfs_stat_t;

// Resource system

typedef struct {
    srbfs_driver_t driver;
    uint16_t memory_size;
    srbfs_memory_address_t resource_descriptor_table_address;
    srbfs_memory_address_t cluster_table_address;
    uint16_t sizeof_resource_descriptor_table;
    uint16_t sizeof_cluster_table;
    uint8_t sizeof_resource_descriptor;
    uint8_t sizeof_cluster;
    uint8_t resource_descriptor_count;
    uint8_t cluster_count;
    uint8_t sizeof_cluster_data;
    uint8_t sizeof_cluster_control;
    uint8_t free_clusters;
    uint8_t flags;
} srbfs_t;

// Resource

typedef struct {
    srbfs_resource_descriptor_t resource_descriptor;
    srbfs_cluster_t first_cluster;
    srbfs_cluster_t current_cluster;
    uint8_t cluster_offset;
    uint16_t size;
    uint16_t current_position;
    uint8_t flags;
} srbfs_resource_t;

typedef struct {
    uint8_t driver_mouted;
} srbfs_global_flags_t;

extern srbfs_global_flags_t srbfs_global_flags;

// Format a device
srbfs_op_result_t srbfs_format(srbfs_t *rs);

// Register a work area
srbfs_op_result_t srbfs_mount(srbfs_driver_t driver, srbfs_t *rs, srbfs_mount_options_t options);

// Unregister a work area
srbfs_op_result_t srbfs_umount(srbfs_t *rs);

// Open/Create a resource (you must give a empty resource)
srbfs_op_result_t srbfs_open(srbfs_t *rs, srbfs_resource_code_t resource_code, srbfs_resource_t *resource, srbfs_open_resource_options_t options);

// Close a resource
srbfs_op_result_t srbfs_close(srbfs_t *rs, srbfs_resource_t *resource);

// Read a byte from resource
uint8_t srbfs_read(srbfs_t *rs, srbfs_resource_t *resource);

// Write a byte from resource
srbfs_op_result_t srbfs_write(srbfs_t *rs, srbfs_resource_t *resource, uint8_t data_to_write);

// Move read/write pointer, (Expand resource size not implemented yet)
srbfs_op_result_t srbfs_seek(srbfs_t *rs, srbfs_resource_t *resource, srbfs_seek_origin_t origin, srbfs_seek_int_t offset);

// Truncate resource size
srbfs_op_result_t srbfs_truncate(srbfs_t *rs, srbfs_resource_t *resource);

// Flush cached data
void srbfs_sync(srbfs_t *rs, srbfs_resource_t *resource);

// Get descriptor status
void srbfs_stat(srbfs_t *rs, srbfs_resource_t *resource, srbfs_stat_t *stat);

// Rewind the position of a resource pointer
srbfs_op_result_t srbfs_rewind(srbfs_t *rs, srbfs_resource_t *resource);

// Create/Allocate a new resource if available
srbfs_resource_code_t srbfs_alloc(srbfs_t *rs);

// Make a resource free to be allocated for another one
uint8_t srbfs_release(srbfs_t *rs, srbfs_resource_t *resource);

// Get size of a resource
srbfs_resource_size_t srbfs_size(srbfs_resource_t *resource);

// Get the current read/write pointer
srbfs_resource_size_t srbfs_tell(srbfs_resource_t *resource);

// Test for end-of-resource on a resource
uint8_t srbfs_eor(srbfs_resource_t *resource);

// Test for an error on a resource
uint8_t srbfs_error(srbfs_resource_t *resource);

// Return the current available space in the partition
srbfs_resource_size_t srbfs_available_space(srbfs_t *rs);

// Return the total space in the partition
srbfs_resource_size_t srbfs_total_space(srbfs_t *rs);

#endif // __SRBFS_H__
