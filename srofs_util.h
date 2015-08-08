/**
 * srofs - Simple Resource Oriented File System
 * 
 * srofs_util.h
 * 
 * Util lib for srofs
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SROFS_UTIL_H__
#define __SROFS_UTIL_H__ 1

#include "srofs.h"

/**
 * Write a resource system table to disc
 * 
 * @param driver
 * @param srofs
 */
void _srofs_write_srofs_to_disc(srofs_driver_t driver, srofs_t *srofs);

/**
 * Read a resource system table from disc
 * 
 * @param driver
 * @param srofs
 */
void _srofs_read_srofs_from_disc(srofs_driver_t driver, srofs_t *srofs);

/**
 * Allocate a free cluster from disc if any
 * 
 * @param srofs
 * @return 
 */
srofs_memory_address_t _srofs_alloc_cluster(srofs_t *srofs);

/**
 * Test if the given cluster is free
 * 
 * @param srofs
 * @param cluster
 * @return 
 */
uint8_t _srofs_is_free_cluster(srofs_t *srofs, srofs_cluster_t cluster);

/**
 * Format a given cluster
 * 
 * @param srofs
 * @param cluster
 */
void _srofs_format_cluster(srofs_t *srofs, srofs_cluster_t cluster);

/**
 * Free a given cluster
 * 
 * @param srofs
 * @param cluster
 */
void _srofs_free_cluster(srofs_t *srofs, srofs_cluster_t cluster);

/**
 * Create a chain between two clusters
 * 
 * @param srofs
 * @param prev_cluster
 * @param next_cluster
 */
void _srofs_create_cluster_chain(srofs_t *srofs, srofs_cluster_t prev_cluster, srofs_cluster_t next_cluster);

/**
 * Convert resource code to rd
 * 
 * @param resource
 */
#define _srofs_resource_code_to_resource_descriptor(resource_code)         (srofs_resource_descriptor_t)(resource_code)

/**
 * Convert rd to resource code
 * 
 * @param resource
 */
#define _srofs_resource_descriptor_to_resource_code(resource_descriptor)   (srofs_resource_code_t)(resource_descriptor)

/**
 * Convert cluster to address
 * 
 * @param resource
 */
#define _srofs_cluster_to_address(srofs, cluster)                             (srofs_memory_address_t)(srofs->cluster_table_address + (cluster * srofs->sizeof_cluster))

/**
 * Convert address to cluster
 * 
 * @param resource
 */
#define _srofs_address_to_cluster(srofs, address)                             (srofs_cluster_t)((address - srofs->cluster_table_address) / srofs->sizeof_cluster)

/**
 * Convert rd to address
 * 
 * @param resource
 */
#define _srofs_resource_descriptor_to_address(srofs, resource_descriptor)     (srofs_memory_address_t)((resource_descriptor * srofs->sizeof_resource_descriptor) + srofs->resource_descriptor_table_address)

/**
 * Convert address to rd
 * 
 * @param resource
 */
#define _srofs_address_to_resource_descriptor(srofs, address)                 (srofs_resource_descriptor_t)((address - srofs->resource_descriptor_table_address) / srofs->sizeof_resource_descriptor)

/**
 * Check if the end-of-resource is reached and set or clear the respecitve flag
 * 
 * @param resource
 */
void _srofs_check_for_eor_reached(srofs_resource_t *resource);

/**
 * Test the end-of-resource flag
 * 
 * @param resource
 * @return 
 */
uint8_t _srofs_is_eor_reached(srofs_resource_t *resource);

/**
 * Check if we are at the end of resource, if yes alloc another cluster and 
 * manage the new pointers
 * 
 * @param srofs
 * @param resource
 * @return 
 */
uint8_t _srofs_check_for_availability(srofs_t *srofs, srofs_resource_t *resource);

/**
 * Move the current position ahead 'offset' bytes
 * 
 * @param srofs
 * @param resource
 * @param offset
 * @return 
 */
uint8_t _srofs_move_current_position_ahead(srofs_t *srofs, srofs_resource_t *resource, srofs_seek_int_t offset);

/**
 * Move the current position back 'offset' bytes
 * 
 * @param srofs
 * @param resource
 * @param offset
 * @return 
 */
uint8_t _srofs_move_current_position_back(srofs_t *srofs, srofs_resource_t *resource, srofs_seek_int_t offset);

/**
 * Free a resource description
 * 
 * @param srofs
 * @param resource_descriptor
 */
void _srofs_format_resorce_descriptor(srofs_t *srofs, srofs_resource_descriptor_t resource_descriptor);

/**
 * Test if given driver is mouted
 * 
 * @param driver
 * @return 
 */
uint8_t _srofs_is_driver_monted(srofs_driver_t driver);

/**
 * Set/clear given driver as mouted
 * 
 * @param driver
 * @param is
 */
void _srofs_set_driver_monted(srofs_driver_t driver, uint8_t is);

/**
 * Close all resources
 * 
 * @param srofs
 */
void _srofs_free_resource_descriptors(srofs_t *srofs);

/**
 * Close a single resources
 * 
 * @param srofs
 * @param resource_descriptor
 */
void _srofs_free_resource_descriptor(srofs_t *srofs, srofs_resource_descriptor_t resource_descriptor);

/**
 * Decrease free cluster
 * 
 * @param srofs
 * @param resource
 */
#define _srofs_decrease_free_clusters(srofs, n)   { \
                                                srofs->free_clusters -= n; \
                                                _srofs_write_srofs_to_disc(srofs->driver, srofs); \
                                            }

/**
 * Increase free cluster
 * 
 * @param srofs
 * @param resource
 */
#define _srofs_increase_free_clusters(srofs, n)   { \
                                                srofs->free_clusters += n; \
                                                _srofs_write_srofs_to_disc(srofs->driver, srofs); \
                                            }

/**
 * Free resource cluster
 * 
 * @param srofs
 * @param resource
 */
void _srofs_format_resource_clusters(srofs_t *srofs, srofs_resource_t *resource);

/**
 * Format a chain of clusters
 * 
 * @param srofs
 * @param cluster
 * @return 
 */
uint8_t _srofs_format_clustesrofs_chain(srofs_t *srofs, srofs_cluster_t cluster);

/**
 * Get the previous cluster by a cluster
 * 
 * @param srofs
 * @return 
 */
#define _srofs_prev_cluster_by_cluster(srofs, cluster)                _srofs_prev_cluster_by_cluster_address(srofs, _srofs_cluster_to_address(srofs, cluster))

/**
 * Get the next cluster by a cluster
 * 
 * @param srofs
 * @return 
 */
#define _srofs_next_cluster_by_cluster(srofs, cluster)                _srofs_next_cluster_by_cluster_address(srofs, _srofs_cluster_to_address(srofs, cluster))

/**
 * Get the previous cluster by a cluster address
 * 
 * @param srofs
 * @return 
 */
#define _srofs_prev_cluster_by_cluster_address(srofs, address)        (srofs_cluster_t)(_srofs_io_read(srofs->driver, CLUSTER_ADDRESS_TO_PREV(address)))

/**
 * Get the next cluster by a cluster address
 * 
 * @param srofs
 * @return 
 */
#define _srofs_next_cluster_by_cluster_address(srofs, address)        (srofs_cluster_t)(_srofs_io_read(srofs->driver, CLUSTER_ADDRESS_TO_NEXT(address)))

/**
 * Calculates and evaluate the srofs attributes
 * 
 * @param srofs
 * @return 
 */
uint8_t _srofs_has_invalid_attributes(srofs_t *srofs);

#endif // __SROFS_UTIL_H__
