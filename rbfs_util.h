/**
 * rbfs - Simple Resource Based File System
 * 
 * rbfs_util.h
 * 
 * Util lib for rbfs
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __RBFS_UTIL_H__
#define __RBFS_UTIL_H__ 1

#include "rbfs.h"

/**
 * Write a resource system table to disk
 * 
 * @param driver
 * @param rbfs
 */
void _rbfs_write_rbfs_to_disk(rbfs_driver_t driver, rbfs_t *rbfs);

/**
 * Read a resource system table from disk
 * 
 * @param driver
 * @param rbfs
 */
void _rbfs_read_rbfs_from_disk(rbfs_driver_t driver, rbfs_t *rbfs);

/**
 * Allocate a free cluster from disk if any
 * 
 * @param rbfs
 * @return 
 */
rbfs_memory_address_t _rbfs_alloc_cluster(rbfs_t *rbfs);

/**
 * Test if the given cluster is free
 * 
 * @param rbfs
 * @param cluster
 * @return 
 */
uint8_t _rbfs_is_free_cluster(rbfs_t *rbfs, rbfs_cluster_t cluster);

/**
 * Format a given cluster
 * 
 * @param rbfs
 * @param cluster
 */
void _rbfs_format_cluster(rbfs_t *rbfs, rbfs_cluster_t cluster);

/**
 * Free a given cluster
 * 
 * @param rbfs
 * @param cluster
 */
void _rbfs_free_cluster(rbfs_t *rbfs, rbfs_cluster_t cluster);

/**
 * Create a chain between two clusters
 * 
 * @param rbfs
 * @param prev_cluster
 * @param next_cluster
 */
void _rbfs_create_cluster_chain(rbfs_t *rbfs, rbfs_cluster_t prev_cluster, rbfs_cluster_t next_cluster);

/**
 * Convert resource code to rd
 * 
 * @param resource
 */
#define _rbfs_resource_code_to_resource_descriptor(resource_code)         (rbfs_resource_descriptor_t)(resource_code)

/**
 * Convert rd to resource code
 * 
 * @param resource
 */
#define _rbfs_resource_descriptor_to_resource_code(resource_descriptor)   (rbfs_resource_code_t)(resource_descriptor)

/**
 * Convert cluster to address
 * 
 * @param resource
 */
#define _rbfs_cluster_to_address(rbfs, cluster)                             (rbfs_memory_address_t)(rbfs->cluster_table_address + (cluster * rbfs->sizeof_cluster))

/**
 * Convert address to cluster
 * 
 * @param resource
 */
#define _rbfs_address_to_cluster(rbfs, address)                             (rbfs_cluster_t)((address - rbfs->cluster_table_address) / rbfs->sizeof_cluster)

/**
 * Convert rd to address
 * 
 * @param resource
 */
#define _rbfs_resource_descriptor_to_address(rbfs, resource_descriptor)     (rbfs_memory_address_t)((resource_descriptor * rbfs->sizeof_resource_descriptor) + rbfs->resource_descriptor_table_address)

/**
 * Convert address to rd
 * 
 * @param resource
 */
#define _rbfs_address_to_resource_descriptor(rbfs, address)                 (rbfs_resource_descriptor_t)((address - rbfs->resource_descriptor_table_address) / rbfs->sizeof_resource_descriptor)

/**
 * Check if the end-of-resource is reached and set or clear the respecitve flag
 * 
 * @param resource
 */
void _rbfs_check_for_eor_reached(rbfs_resource_t *resource);

/**
 * Test the end-of-resource flag
 * 
 * @param resource
 * @return 
 */
uint8_t _rbfs_is_eor_reached(rbfs_resource_t *resource);

/**
 * Check if we are at the end of resource, if yes alloc another cluster and 
 * manage the new pointers
 * 
 * @param rbfs
 * @param resource
 * @return 
 */
uint8_t _rbfs_check_for_availability(rbfs_t *rbfs, rbfs_resource_t *resource);

/**
 * Move the current position ahead 'offset' bytes
 * 
 * @param rbfs
 * @param resource
 * @param offset
 * @return 
 */
uint8_t _rbfs_move_current_position_ahead(rbfs_t *rbfs, rbfs_resource_t *resource, rbfs_seek_int_t offset);

/**
 * Move the current position back 'offset' bytes
 * 
 * @param rbfs
 * @param resource
 * @param offset
 * @return 
 */
uint8_t _rbfs_move_current_position_back(rbfs_t *rbfs, rbfs_resource_t *resource, rbfs_seek_int_t offset);

/**
 * Free a resource description
 * 
 * @param rbfs
 * @param resource_descriptor
 */
void _rbfs_format_resorce_descriptor(rbfs_t *rbfs, rbfs_resource_descriptor_t resource_descriptor);

/**
 * Test if given driver is mouted
 * 
 * @param driver
 * @return 
 */
uint8_t _rbfs_is_driver_monted(rbfs_driver_t driver);

/**
 * Set/clear given driver as mouted
 * 
 * @param driver
 * @param is
 */
void _rbfs_set_driver_monted(rbfs_driver_t driver, uint8_t is);

/**
 * Close all resources
 * 
 * @param rbfs
 */
void _rbfs_free_resource_descriptors(rbfs_t *rbfs);

/**
 * Close a single resources
 * 
 * @param rbfs
 * @param resource_descriptor
 */
void _rbfs_free_resource_descriptor(rbfs_t *rbfs, rbfs_resource_descriptor_t resource_descriptor);

/**
 * Decrease free cluster
 * 
 * @param rbfs
 * @param resource
 */
#define _rbfs_decrease_free_clusters(rbfs, n)   { \
                                                rbfs->free_clusters -= n; \
                                                _rbfs_write_rbfs_to_disk(rbfs->driver, rbfs); \
                                            }

/**
 * Increase free cluster
 * 
 * @param rbfs
 * @param resource
 */
#define _rbfs_increase_free_clusters(rbfs, n)   { \
                                                rbfs->free_clusters += n; \
                                                _rbfs_write_rbfs_to_disk(rbfs->driver, rbfs); \
                                            }

/**
 * Free resource cluster
 * 
 * @param rbfs
 * @param resource
 */
void _rbfs_format_resource_clusters(rbfs_t *rbfs, rbfs_resource_t *resource);

/**
 * Format a chain of clusters
 * 
 * @param rbfs
 * @param cluster
 * @return 
 */
uint8_t _rbfs_format_clusterbfs_chain(rbfs_t *rbfs, rbfs_cluster_t cluster);

/**
 * Get the previous cluster by a cluster
 * 
 * @param rbfs
 * @return 
 */
#define _rbfs_prev_cluster_by_cluster(rbfs, cluster)                _rbfs_prev_cluster_by_cluster_address(rbfs, _rbfs_cluster_to_address(rbfs, cluster))

/**
 * Get the next cluster by a cluster
 * 
 * @param rbfs
 * @return 
 */
#define _rbfs_next_cluster_by_cluster(rbfs, cluster)                _rbfs_next_cluster_by_cluster_address(rbfs, _rbfs_cluster_to_address(rbfs, cluster))

/**
 * Get the previous cluster by a cluster address
 * 
 * @param rbfs
 * @return 
 */
#define _rbfs_prev_cluster_by_cluster_address(rbfs, address)        (rbfs_cluster_t)(_rbfs_io_read(rbfs->driver, CLUSTER_ADDRESS_TO_PREV(address)))

/**
 * Get the next cluster by a cluster address
 * 
 * @param rbfs
 * @return 
 */
#define _rbfs_next_cluster_by_cluster_address(rbfs, address)        (rbfs_cluster_t)(_rbfs_io_read(rbfs->driver, CLUSTER_ADDRESS_TO_NEXT(address)))

/**
 * Calculates and evaluate the rbfs attributes
 * 
 * @param rbfs
 * @return 
 */
uint8_t _rbfs_has_invalid_attributes(rbfs_t *rbfs);

#endif // __RBFS_UTIL_H__
