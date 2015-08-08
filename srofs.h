/**
 * srofs - Simple Resource Oriented File System
 * 
 * rs.h
 * 
 * An file system header definition based on the idea of resources
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SROFS_H__
#define __SROFS_H__ 1

#include <stdint.h>

#define SROFS_NULL_RESOURCE_CODE                                            0xff
#define SROFS_NULL_CLUSTER                                                  0xff

#define SROFS_NULL_RESORCE_DESCRIPTOR_ADDRESS                               0xff
#define SROFS_NULL_CLUSTER_ADDRESS                                          0x00

#define SROFS_FIRST_ADDRESS_OF_MEMORY                                       0x00

#define SROFS_SIZEOF_RESOURCE_SIZE                                          0x02

#define SROFS_INEXISTENT_CLUSTER                                            0xff

#define CLUSTER_ADDRESS_TO_NEXT(CLUSTER_ADDRESS)                            ((CLUSTER_ADDRESS) + 0)
#define CLUSTER_ADDRESS_TO_PREV(CLUSTER_ADDRESS)                            ((CLUSTER_ADDRESS) + 1)
#define CLUSTER_ADDRESS_TO_DATA(CLUSTER_ADDRESS)                            ((CLUSTER_ADDRESS) + 2)

#define RD_ADDRESS_TO_SIZE_LOW(RD_ADDRESS)                                  ((RD_ADDRESS) + 0)
#define RD_ADDRESS_TO_SIZE_HIGH(RD_ADDRESS)                                 ((RD_ADDRESS) + 1)
#define RD_ADDRESS_TO_FIRST_CLUSTER(RD_ADDRESS)                             ((RD_ADDRESS) + 2)
#define RD_ADDRESS_TO_FLAG(RD_ADDRESS)                                      ((RD_ADDRESS) + 3)

typedef uint8_t srofs_resource_descriptor_t;
typedef uint8_t srofs_cluster_t;
typedef uint16_t srofs_resource_size_t;
typedef uint16_t srofs_memory_address_t;
typedef uint8_t srofs_resource_code_t;
typedef uint16_t srofs_seek_int_t;

// Drivers

typedef enum {
    SROFS_DRIVER_VIRTUAL = 0,
    SROFS_DRIVER_SELF_EEPROM = 1,
    SROFS_DRIVER_MULTI_EXTERNAL_EEPROM = 2,
    SROFS_DRIVER_EXTERNAL_EEPROM = 3,
    SROFS_DRIVER_ARDUINO_EEPROM = 4
} srofs_driver_t;

// Resource fag bit values

typedef enum {
    SROFS_RESOURCE_FLAG_BIT_OPENED = 1,
    SROFS_RESOURCE_FLAG_BIT_READ_ONLY = 2,
    SROFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ = 4,
    SROFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_WRITE = 8,
    SROFS_RESOURCE_FLAG_BIT_ALLOCATED = 16,
    SROFS_RESOURCE_FLAG_BIT_EOR_REACHED = 32
} srofs_resource_flag_bits_t;

// Options to open a resource

typedef enum {
    SROFS_OPEN_RESOURCE_OPTION_NORMAL = 0,
    SROFS_OPEN_RESOURCE_OPTION_READ_ONLY = 1
} srofs_open_resource_options_t;

// Options to mount a resource

typedef enum {
    SROFS_MOUNT_OPTION_NORMAL = 0,
    SROFS_MOUNT_OPTION_READ_ONLY = 1
} srofs_mount_options_t;

// Rs fag bit values

typedef enum {
    SROFS_FLAG_BIT_DRIVER_MOUNTED = 1,
    SROFS_FLAG_BIT_READ_ONLY = 2
} srofs_flag_bits_t;

// Operation result

typedef enum {
    SROFS_OP_RESULT_SUCCESS = 0,
    SROFS_OP_RESULT_ERROR_RESOURCE_OPENED = 1,
    SROFS_OP_RESULT_ERROR_RESOURCE_CLOSED = 2,
    SROFS_OP_RESULT_ERROR_RESOURCE_READ_ONLY = 3,
    SROFS_OP_RESULT_ERROR_NO_SPACE_AVAILABLE = 4,
    SROFS_OP_RESULT_ERROR_DRIVER_BUSY = 5,
    SROFS_OP_RESULT_ERROR_SEEK_OUT_OF_BOUND = 6,
    SROFS_OP_RESULT_ERROR_RESOURCE_DOES_NOT_ALLOCATED = 7,
    SROFS_OP_RESULT_ERROR_DRIVER_NOT_MOUNTED = 8
} srofs_op_result_t;

// Seek position reference

typedef enum {
    SROFS_SEEK_ORIGIN_BEGIN = 0,
    SROFS_SEEK_ORIGIN_CURRENT = 1
} srofs_seek_origin_t;

typedef struct {
    uint8_t flags;
} srofs_stat_t;

// Resource system

typedef struct {
    srofs_driver_t driver;
    uint16_t memory_size;
    srofs_memory_address_t resource_descriptor_table_address;
    srofs_memory_address_t cluster_table_address;
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
} srofs_t;

// Resource

typedef struct {
    srofs_resource_descriptor_t resource_descriptor;
    srofs_cluster_t first_cluster;
    srofs_cluster_t current_cluster;
    uint8_t cluster_offset;
    uint16_t size;
    uint16_t current_position;
    uint8_t flags;
} srofs_resource_t;

typedef struct {
    uint8_t driver_mouted;
} srofs_global_flags_t;

extern srofs_global_flags_t srofs_global_flags;

// Format a device
srofs_op_result_t srofs_format(srofs_t *rs);

// Register a work area
srofs_op_result_t srofs_mount(srofs_driver_t driver, srofs_t *rs, srofs_mount_options_t options);

// Unregister a work area
srofs_op_result_t srofs_umount(srofs_t *rs);

// Open/Create a resource (you must give a empty resource)
srofs_op_result_t srofs_open(srofs_t *rs, srofs_resource_code_t resource_code, srofs_resource_t *resource, srofs_open_resource_options_t options);

// Close a resource
srofs_op_result_t srofs_close(srofs_t *rs, srofs_resource_t *resource);

// Read a byte from resource
uint8_t srofs_read(srofs_t *rs, srofs_resource_t *resource);

// Write a byte from resource
srofs_op_result_t srofs_write(srofs_t *rs, srofs_resource_t *resource, uint8_t data_to_write);

// Move read/write pointer, (Expand resource size not implemented yet)
srofs_op_result_t srofs_seek(srofs_t *rs, srofs_resource_t *resource, srofs_seek_origin_t origin, srofs_seek_int_t offset);

// Truncate resource size
srofs_op_result_t srofs_truncate(srofs_t *rs, srofs_resource_t *resource);

// Flush cached data
void srofs_sync(srofs_t *rs, srofs_resource_t *resource);

// Get descriptor status
void srofs_stat(srofs_t *rs, srofs_resource_t *resource, srofs_stat_t *stat);

// Rewind the position of a resource pointer
srofs_op_result_t srofs_rewind(srofs_t *rs, srofs_resource_t *resource);

// Create/Allocate a new resource if available
srofs_resource_code_t srofs_alloc(srofs_t *rs);

// Make a resource free to be allocated for another one
uint8_t srofs_release(srofs_t *rs, srofs_resource_t *resource);

// Get size of a resource
srofs_resource_size_t srofs_size(srofs_resource_t *resource);

// Get the current read/write pointer
srofs_resource_size_t srofs_tell(srofs_resource_t *resource);

// Test for end-of-resource on a resource
uint8_t srofs_eor(srofs_resource_t *resource);

// Test for an error on a resource
uint8_t srofs_error(srofs_resource_t *resource);

// Return the current available space in the partition
srofs_resource_size_t srofs_available_space(srofs_t *rs);

// Return the total space in the partition
srofs_resource_size_t srofs_total_space(srofs_t *rs);

#endif // __SROFS_H__
