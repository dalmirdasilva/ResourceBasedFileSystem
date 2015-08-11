/**
 * rbfs - Simple Resource Based File System
 * 
 * rs.h
 * 
 * An file system header definition based on the idea of resources
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __RBFS_H__
#define __RBFS_H__ 1

#include <stdint.h>

#define RBFS_NULL_RESOURCE_CODE                                            0xff
#define RBFS_NULL_CLUSTER                                                  0xff

#define RBFS_NULL_RESORCE_DESCRIPTOR_ADDRESS                               0xff
#define RBFS_NULL_CLUSTER_ADDRESS                                          0x00

#define RBFS_FIRST_ADDRESS_OF_MEMORY                                       0x00

#define RBFS_SIZEOF_RESOURCE_SIZE                                          0x02

#define RBFS_INEXISTENT_CLUSTER                                            0xff

#define CLUSTER_ADDRESS_TO_NEXT(CLUSTER_ADDRESS)                            ((CLUSTER_ADDRESS) + 0)
#define CLUSTER_ADDRESS_TO_PREV(CLUSTER_ADDRESS)                            ((CLUSTER_ADDRESS) + 1)
#define CLUSTER_ADDRESS_TO_DATA(CLUSTER_ADDRESS)                            ((CLUSTER_ADDRESS) + 2)

#define RD_ADDRESS_TO_SIZE_LOW(RD_ADDRESS)                                  ((RD_ADDRESS) + 0)
#define RD_ADDRESS_TO_SIZE_HIGH(RD_ADDRESS)                                 ((RD_ADDRESS) + 1)
#define RD_ADDRESS_TO_FIRST_CLUSTER(RD_ADDRESS)                             ((RD_ADDRESS) + 2)
#define RD_ADDRESS_TO_FLAG(RD_ADDRESS)                                      ((RD_ADDRESS) + 3)

typedef uint8_t rbfs_resource_descriptor_t;
typedef uint8_t rbfs_cluster_t;
typedef uint16_t rbfs_resource_size_t;
typedef uint16_t rbfs_memory_address_t;
typedef uint8_t rbfs_resource_code_t;
typedef uint16_t rbfs_seek_int_t;

// Drivers

typedef enum {
    RBFS_DRIVER_VIRTUAL = 0,
    RBFS_DRIVER_SELF_EEPROM = 1,
    RBFS_DRIVER_MULTI_EXTERNAL_EEPROM = 2,
    RBFS_DRIVER_EXTERNAL_EEPROM = 3,
    RBFS_DRIVER_ARDUINO_EEPROM = 4
} rbfs_driver_t;

// Resource fag bit values

typedef enum {
    RBFS_RESOURCE_FLAG_BIT_OPENED = 1,
    RBFS_RESOURCE_FLAG_BIT_READ_ONLY = 2,
    RBFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ = 4,
    RBFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_WRITE = 8,
    RBFS_RESOURCE_FLAG_BIT_ALLOCATED = 16,
    RBFS_RESOURCE_FLAG_BIT_EOR_REACHED = 32
} rbfs_resource_flag_bits_t;

// Options to open a resource

typedef enum {
    RBFS_OPEN_RESOURCE_OPTION_NORMAL = 0,
    RBFS_OPEN_RESOURCE_OPTION_READ_ONLY = 1
} rbfs_open_resource_options_t;

// Options to mount a resource

typedef enum {
    RBFS_MOUNT_OPTION_NORMAL = 0,
    RBFS_MOUNT_OPTION_READ_ONLY = 1
} rbfs_mount_options_t;

// Rs fag bit values

typedef enum {
    RBFS_FLAG_BIT_DRIVER_MOUNTED = 1,
    RBFS_FLAG_BIT_READ_ONLY = 2
} rbfs_flag_bits_t;

// Operation result

typedef enum {
    RBFS_OP_RESULT_SUCCESS = 0,
    RBFS_OP_RESULT_ERROR_RESOURCE_OPENED = 1,
    RBFS_OP_RESULT_ERROR_RESOURCE_CLOSED = 2,
    RBFS_OP_RESULT_ERROR_RESOURCE_READ_ONLY = 3,
    RBFS_OP_RESULT_ERROR_NO_SPACE_AVAILABLE = 4,
    RBFS_OP_RESULT_ERROR_DRIVER_BUSY = 5,
    RBFS_OP_RESULT_ERROR_SEEK_OUT_OF_BOUND = 6,
    RBFS_OP_RESULT_ERROR_RESOURCE_DOES_NOT_ALLOCATED = 7,
    RBFS_OP_RESULT_ERROR_DRIVER_NOT_MOUNTED = 8
} rbfs_op_result_t;

// Seek position reference

typedef enum {
    RBFS_SEEK_ORIGIN_BEGIN = 0,
    RBFS_SEEK_ORIGIN_CURRENT = 1
} rbfs_seek_origin_t;

typedef struct {
    uint8_t flags;
} rbfs_stat_t;

// Resource system

typedef struct {
    rbfs_driver_t driver;
    uint16_t memory_size;
    rbfs_memory_address_t resource_descriptor_table_address;
    rbfs_memory_address_t cluster_table_address;
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
} rbfs_t;

// Resource

typedef struct {
    rbfs_resource_descriptor_t resource_descriptor;
    rbfs_cluster_t first_cluster;
    rbfs_cluster_t current_cluster;
    uint8_t cluster_offset;
    uint16_t size;
    uint16_t current_position;
    uint8_t flags;
} rbfs_resource_t;

typedef struct {
    uint8_t driver_mouted;
} rbfs_global_flags_t;

extern rbfs_global_flags_t rbfs_global_flags;

// Format a device
rbfs_op_result_t rbfs_format(rbfs_t *rs);

// Register a work area
rbfs_op_result_t rbfs_mount(rbfs_driver_t driver, rbfs_t *rs, rbfs_mount_options_t options);

// Unregister a work area
rbfs_op_result_t rbfs_umount(rbfs_t *rs);

// Open/Create a resource (you must give a empty resource)
rbfs_op_result_t rbfs_open(rbfs_t *rs, rbfs_resource_code_t resource_code, rbfs_resource_t *resource, rbfs_open_resource_options_t options);

// Close a resource
rbfs_op_result_t rbfs_close(rbfs_t *rs, rbfs_resource_t *resource);

// Read a byte from resource
uint8_t rbfs_read(rbfs_t *rs, rbfs_resource_t *resource);

// Write a byte from resource
rbfs_op_result_t rbfs_write(rbfs_t *rs, rbfs_resource_t *resource, uint8_t data_to_write);

// Move read/write pointer, (Expand resource size not implemented yet)
rbfs_op_result_t rbfs_seek(rbfs_t *rs, rbfs_resource_t *resource, rbfs_seek_origin_t origin, rbfs_seek_int_t offset);

// Truncate resource size
rbfs_op_result_t rbfs_truncate(rbfs_t *rs, rbfs_resource_t *resource);

// Flush cached data
void rbfs_sync(rbfs_t *rs, rbfs_resource_t *resource);

// Get descriptor status
void rbfs_stat(rbfs_t *rs, rbfs_resource_t *resource, rbfs_stat_t *stat);

// Rewind the position of a resource pointer
rbfs_op_result_t rbfs_rewind(rbfs_t *rs, rbfs_resource_t *resource);

// Create/Allocate a new resource if available
rbfs_resource_code_t rbfs_alloc(rbfs_t *rs);

// Make a resource free to be allocated for another one
uint8_t rbfs_release(rbfs_t *rs, rbfs_resource_t *resource);

// Get size of a resource
rbfs_resource_size_t rbfs_size(rbfs_resource_t *resource);

// Get the current read/write pointer
rbfs_resource_size_t rbfs_tell(rbfs_resource_t *resource);

// Test for end-of-resource on a resource
uint8_t rbfs_eor(rbfs_resource_t *resource);

// Test for an error on a resource
uint8_t rbfs_error(rbfs_resource_t *resource);

// Return the current available space in the partition
rbfs_resource_size_t rbfs_available_space(rbfs_t *rs);

// Return the total space in the partition
rbfs_resource_size_t rbfs_total_space(rbfs_t *rs);

#endif // __RBFS_H__
