/**
 * srbfs - Simple Resource Based File System
 * 
 * srbfs_util.h
 * 
 * Util lib for srbfs
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SRBFS_UTIL_H__
#define __SRBFS_UTIL_H__ 1

#include "srbfs.h"

/**
 * Write a resource system table to disc
 * 
 * @param driver
 * @param srbfs
 */
void _srbfs_write_srbfs_to_disc(srbfs_driver_t driver, srbfs_t *srbfs);

/**
 * Read a resource system table from disc
 * 
 * @param driver
 * @param srbfs
 */
void _srbfs_read_srbfs_from_disc(srbfs_driver_t driver, srbfs_t *srbfs);

/**
 * Allocate a free cluster from disc if any
 * 
 * @param srbfs
 * @return 
 */
srbfs_memory_address_t _srbfs_alloc_cluster(srbfs_t *srbfs);

/**
 * Test if the given cluster is free
 * 
 * @param srbfs
 * @param cluster
 * @return 
 */
uint8_t _srbfs_is_free_cluster(srbfs_t *srbfs, srbfs_cluster_t cluster);

/**
 * Format a given cluster
 * 
 * @param srbfs
 * @param cluster
 */
void _srbfs_format_cluster(srbfs_t *srbfs, srbfs_cluster_t cluster);

/**
 * Free a given cluster
 * 
 * @param srbfs
 * @param cluster
 */
void _srbfs_free_cluster(srbfs_t *srbfs, srbfs_cluster_t cluster);

/**
 * Create a chain between two clusters
 * 
 * @param srbfs
 * @param prev_cluster
 * @param next_cluster
 */
void _srbfs_create_cluster_chain(srbfs_t *srbfs, srbfs_cluster_t prev_cluster, srbfs_cluster_t next_cluster);

/**
 * Convert resource code to rd
 * 
 * @param resource
 */
#define _srbfs_resource_code_to_resource_descriptor(resource_code)         (srbfs_resource_descriptor_t)(resource_code)

/**
 * Convert rd to resource code
 * 
 * @param resource
 */
#define _srbfs_resource_descriptor_to_resource_code(resource_descriptor)   (srbfs_resource_code_t)(resource_descriptor)

/**
 * Convert cluster to address
 * 
 * @param resource
 */
#define _srbfs_cluster_to_address(srbfs, cluster)                             (srbfs_memory_address_t)(srbfs->cluster_table_address + (cluster * srbfs->sizeof_cluster))

/**
 * Convert address to cluster
 * 
 * @param resource
 */
#define _srbfs_address_to_cluster(srbfs, address)                             (srbfs_cluster_t)((address - srbfs->cluster_table_address) / srbfs->sizeof_cluster)

/**
 * Convert rd to address
 * 
 * @param resource
 */
#define _srbfs_resource_descriptor_to_address(srbfs, resource_descriptor)     (srbfs_memory_address_t)((resource_descriptor * srbfs->sizeof_resource_descriptor) + srbfs->resource_descriptor_table_address)

/**
 * Convert address to rd
 * 
 * @param resource
 */
#define _srbfs_address_to_resource_descriptor(srbfs, address)                 (srbfs_resource_descriptor_t)((address - srbfs->resource_descriptor_table_address) / srbfs->sizeof_resource_descriptor)

/**
 * Check if the end-of-resource is reached and set or clear the respecitve flag
 * 
 * @param resource
 */
void _srbfs_check_for_eor_reached(srbfs_resource_t *resource);

/**
 * Test the end-of-resource flag
 * 
 * @param resource
 * @return 
 */
uint8_t _srbfs_is_eor_reached(srbfs_resource_t *resource);

/**
 * Check if we are at the end of resource, if yes alloc another cluster and 
 * manage the new pointers
 * 
 * @param srbfs
 * @param resource
 * @return 
 */
uint8_t _srbfs_check_for_availability(srbfs_t *srbfs, srbfs_resource_t *resource);

/**
 * Move the current position ahead 'offset' bytes
 * 
 * @param srbfs
 * @param resource
 * @param offset
 * @return 
 */
uint8_t _srbfs_move_current_position_ahead(srbfs_t *srbfs, srbfs_resource_t *resource, srbfs_seek_int_t offset);

/**
 * Move the current position back 'offset' bytes
 * 
 * @param srbfs
 * @param resource
 * @param offset
 * @return 
 */
uint8_t _srbfs_move_current_position_back(srbfs_t *srbfs, srbfs_resource_t *resource, srbfs_seek_int_t offset);

/**
 * Free a resource description
 * 
 * @param srbfs
 * @param resource_descriptor
 */
void _srbfs_format_resorce_descriptor(srbfs_t *srbfs, srbfs_resource_descriptor_t resource_descriptor);

/**
 * Test if given driver is mouted
 * 
 * @param driver
 * @return 
 */
uint8_t _srbfs_is_driver_monted(srbfs_driver_t driver);

/**
 * Set/clear given driver as mouted
 * 
 * @param driver
 * @param is
 */
void _srbfs_set_driver_monted(srbfs_driver_t driver, uint8_t is);

/**
 * Close all resources
 * 
 * @param srbfs
 */
void _srbfs_free_resource_descriptors(srbfs_t *srbfs);

/**
 * Close a single resources
 * 
 * @param srbfs
 * @param resource_descriptor
 */
void _srbfs_free_resource_descriptor(srbfs_t *srbfs, srbfs_resource_descriptor_t resource_descriptor);

/**
 * Decrease free cluster
 * 
 * @param srbfs
 * @param resource
 */
#define _srbfs_decrease_free_clusters(srbfs, n)   { \
                                                srbfs->free_clusters -= n; \
                                                _srbfs_write_srbfs_to_disc(srbfs->driver, srbfs); \
                                            }

/**
 * Increase free cluster
 * 
 * @param srbfs
 * @param resource
 */
#define _srbfs_increase_free_clusters(srbfs, n)   { \
                                                srbfs->free_clusters += n; \
                                                _srbfs_write_srbfs_to_disc(srbfs->driver, srbfs); \
                                            }

/**
 * Free resource cluster
 * 
 * @param srbfs
 * @param resource
 */
void _srbfs_format_resource_clusters(srbfs_t *srbfs, srbfs_resource_t *resource);

/**
 * Format a chain of clusters
 * 
 * @param srbfs
 * @param cluster
 * @return 
 */
uint8_t _srbfs_format_clustesrbfs_chain(srbfs_t *srbfs, srbfs_cluster_t cluster);

/**
 * Get the previous cluster by a cluster
 * 
 * @param srbfs
 * @return 
 */
#define _srbfs_prev_cluster_by_cluster(srbfs, cluster)                _srbfs_prev_cluster_by_cluster_address(srbfs, _srbfs_cluster_to_address(srbfs, cluster))

/**
 * Get the next cluster by a cluster
 * 
 * @param srbfs
 * @return 
 */
#define _srbfs_next_cluster_by_cluster(srbfs, cluster)                _srbfs_next_cluster_by_cluster_address(srbfs, _srbfs_cluster_to_address(srbfs, cluster))

/**
 * Get the previous cluster by a cluster address
 * 
 * @param srbfs
 * @return 
 */
#define _srbfs_prev_cluster_by_cluster_address(srbfs, address)        (srbfs_cluster_t)(_srbfs_io_read(srbfs->driver, CLUSTER_ADDRESS_TO_PREV(address)))

/**
 * Get the next cluster by a cluster address
 * 
 * @param srbfs
 * @return 
 */
#define _srbfs_next_cluster_by_cluster_address(srbfs, address)        (srbfs_cluster_t)(_srbfs_io_read(srbfs->driver, CLUSTER_ADDRESS_TO_NEXT(address)))

/**
 * Calculates and evaluate the srbfs attributes
 * 
 * @param srbfs
 * @return 
 */
uint8_t _srbfs_has_invalid_attributes(srbfs_t *srbfs);

#endif // __SRBFS_UTIL_H__
