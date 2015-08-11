/**
 * rbfs - Simple Resource Based File System
 * 
 * rbfs_util.c
 * 
 * Util lib for rbfs
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __RBFS_UTIL_C__
#define __RBFS_UTIL_C__ 1

#include "rbfs_util.h"
#include "rbfs_io.h"

void _rbfs_write_rbfs_to_disk(rbfs_driver_t driver, rbfs_t *rbfs) {
    uint8_t i;
    uint8_t *p;
    rbfs_memory_address_t address = RBFS_FIRST_ADDRESS_OF_MEMORY;
    p = (uint8_t *) rbfs;
    for (i = 0; i < sizeof (rbfs_t); i++) {
        _rbfs_io_write(driver, address++, *(p++));
    }
}

void _rbfs_read_rbfs_from_disk(rbfs_driver_t driver, rbfs_t *rbfs) {
    uint8_t i;
    uint8_t *p;
    rbfs_memory_address_t address = RBFS_FIRST_ADDRESS_OF_MEMORY;
    p = (uint8_t *) rbfs;
    for (i = 0; i < sizeof (rbfs_t); i++) {
        *(p++) = _rbfs_io_read(driver, address++);
    }
}

rbfs_memory_address_t _rbfs_alloc_cluster(rbfs_t *rbfs) {
    rbfs_memory_address_t address;
    uint8_t i;
    address = rbfs->cluster_table_address;
    for (i = 0; i < rbfs->cluster_count; i++) {
        if (_rbfs_is_free_cluster(rbfs, (rbfs_cluster_t) i)) {
            _rbfs_decrease_free_clusters(rbfs, 1);
            return address;
        }
        address += rbfs->sizeof_cluster;
    }
    return RBFS_NULL_CLUSTER_ADDRESS;
}

uint8_t _rbfs_is_free_cluster(rbfs_t *rbfs, rbfs_cluster_t cluster) {
    return (cluster == _rbfs_prev_cluster_by_cluster(rbfs, cluster)) \
        && (cluster == _rbfs_next_cluster_by_cluster(rbfs, cluster));
}

void _rbfs_format_cluster(rbfs_t *rbfs, rbfs_cluster_t cluster) {
    rbfs_memory_address_t address;
    address = _rbfs_cluster_to_address(rbfs, cluster);
    _rbfs_io_write(rbfs->driver, CLUSTER_ADDRESS_TO_NEXT(address), cluster);
    _rbfs_io_write(rbfs->driver, CLUSTER_ADDRESS_TO_PREV(address), cluster);
}

void _rbfs_free_cluster(rbfs_t *rbfs, rbfs_cluster_t cluster) {
    _rbfs_format_cluster(rbfs, cluster);
    _rbfs_increase_free_clusters(rbfs, 1);
}

void _rbfs_create_cluster_chain(rbfs_t *rbfs, rbfs_cluster_t prev_cluster, rbfs_cluster_t next_cluster) {
    rbfs_memory_address_t address;
    if (prev_cluster != RBFS_INEXISTENT_CLUSTER) {
        address = _rbfs_cluster_to_address(rbfs, prev_cluster);
        _rbfs_io_write(rbfs->driver, CLUSTER_ADDRESS_TO_NEXT(address), (uint8_t) next_cluster);
    }
    if (next_cluster != RBFS_INEXISTENT_CLUSTER) {
        address = _rbfs_cluster_to_address(rbfs, next_cluster);
        _rbfs_io_write(rbfs->driver, CLUSTER_ADDRESS_TO_PREV(address), (uint8_t) prev_cluster);
    }
}

void _rbfs_check_for_eor_reached(rbfs_resource_t *resource) {
    if (resource->current_position >= resource->size) {
        resource->flags |= RBFS_RESOURCE_FLAG_BIT_EOR_REACHED;
    } else {
        resource->flags &= ~RBFS_RESOURCE_FLAG_BIT_EOR_REACHED;
    }
}

uint8_t _rbfs_is_eor_reached(rbfs_resource_t *resource) {
    return resource->flags & RBFS_RESOURCE_FLAG_BIT_EOR_REACHED;
}

uint8_t _rbfs_check_for_availability(rbfs_t *rbfs, rbfs_resource_t *resource) {
    rbfs_memory_address_t address;
    rbfs_cluster_t cluster;
    _rbfs_check_for_eor_reached(resource);
    if (resource->cluster_offset >= rbfs->sizeof_cluster) {
        if (rbfs_eor(resource)) {
            address = _rbfs_alloc_cluster(rbfs);
            if (address == RBFS_NULL_CLUSTER_ADDRESS) {
                return 0;
            }
            cluster = _rbfs_address_to_cluster(rbfs, address);
            _rbfs_create_cluster_chain(rbfs, resource->current_cluster, cluster);
            resource->current_cluster = cluster;
        } else {
            resource->current_cluster = _rbfs_next_cluster_by_cluster(rbfs, resource->current_cluster);
        }
        resource->cluster_offset = rbfs->sizeof_cluster_control;
    }
    return 1;
}

uint8_t _rbfs_move_current_position_ahead(rbfs_t *rbfs, rbfs_resource_t *resource, rbfs_seek_int_t offset) {
    uint8_t until_the_end;
    uint8_t how_many_clustes_ahead;
    uint8_t i;
    resource->current_position += offset;
    until_the_end = (rbfs->sizeof_cluster - resource->cluster_offset);
    if (offset <= until_the_end) {
        resource->cluster_offset += offset;
        return 1;
    }
    offset -= until_the_end;
    how_many_clustes_ahead = (offset / rbfs->sizeof_cluster_data) + 1;
    resource->cluster_offset = (offset % rbfs->sizeof_cluster_data) + rbfs->sizeof_cluster_control;
    for (i = 0; i < how_many_clustes_ahead; i++) {
        resource->current_cluster = _rbfs_next_cluster_by_cluster(rbfs, resource->current_cluster);
    }
    return 1;
}

uint8_t _rbfs_move_current_position_back(rbfs_t *rbfs, rbfs_resource_t *resource, rbfs_seek_int_t offset) {
    uint8_t until_the_begin;
    uint8_t how_many_clustes_back;
    uint8_t i;
    resource->current_position -= offset;
    until_the_begin = (resource->cluster_offset - rbfs->sizeof_cluster_control);
    if (offset <= until_the_begin) {
        resource->cluster_offset -= offset;
        return 1;
    }
    offset -= until_the_begin;
    how_many_clustes_back = (offset / rbfs->sizeof_cluster_data);
    if ((offset % rbfs->sizeof_cluster_data) != 0) {
        how_many_clustes_back++;
    }
    resource->cluster_offset = rbfs->sizeof_cluster - (offset % rbfs->sizeof_cluster_data);
    for (i = 0; i < how_many_clustes_back; i++) {
        resource->current_cluster = _rbfs_prev_cluster_by_cluster(rbfs, resource->current_cluster);
    }
    return 1;
}

void _rbfs_format_resorce_descriptor(rbfs_t *rbfs, rbfs_resource_descriptor_t resource_descriptor) {
    int i;
    rbfs_memory_address_t address;
    address = _rbfs_resource_descriptor_to_address(rbfs, resource_descriptor);
    for (i = 0; i < rbfs->sizeof_resource_descriptor; i++) {
        _rbfs_io_write(rbfs->driver, address + i, 0x00);
    }
}

uint8_t _rbfs_is_driver_monted(rbfs_driver_t driver) {
    return rbfs_global_flags.driver_mouted & (1 << driver);
}

void _rbfs_set_driver_monted(rbfs_driver_t driver, uint8_t is) {
    if (is) {
        rbfs_global_flags.driver_mouted |= (1 << driver);
    } else {
        rbfs_global_flags.driver_mouted &= ~(1 << driver);
    }
}

void _rbfs_free_resource_descriptors(rbfs_t *rbfs) {
    uint8_t i;
    for (i = 0; i < rbfs->resource_descriptor_count; i++) {
        _rbfs_free_resource_descriptor(rbfs, i);
    }
}

void _rbfs_free_resource_descriptor(rbfs_t *rbfs, rbfs_resource_descriptor_t resource_descriptor) {
    rbfs_memory_address_t address;
    uint8_t flags;
    address = _rbfs_resource_descriptor_to_address(rbfs, resource_descriptor);
    flags = _rbfs_io_read(rbfs->driver, RD_ADDRESS_TO_FLAG(address));
    flags &= ~(RBFS_RESOURCE_FLAG_BIT_OPENED | RBFS_RESOURCE_FLAG_BIT_READ_ONLY);
    _rbfs_io_write(rbfs->driver, RD_ADDRESS_TO_FLAG(address), flags);
}

void _rbfs_format_resource_clusters(rbfs_t *rbfs, rbfs_resource_t *resource) {
    uint8_t freed_clusters;
    freed_clusters = _rbfs_format_clusterbfs_chain(rbfs, resource->first_cluster);
    _rbfs_increase_free_clusters(rbfs, freed_clusters);
}

uint8_t _rbfs_format_clusterbfs_chain(rbfs_t *rbfs, rbfs_cluster_t cluster) {
    rbfs_cluster_t next_cluster;
    uint8_t formated_clusters = 0;
    do {
        next_cluster = _rbfs_next_cluster_by_cluster(rbfs, cluster);
        _rbfs_format_cluster(rbfs, cluster);
        formated_clusters++;
        if (next_cluster == RBFS_INEXISTENT_CLUSTER || next_cluster == cluster) {
            break;
        }
        cluster = next_cluster;
    } while (1);
    return formated_clusters;
}

uint8_t _rbfs_has_invalid_attributes(rbfs_t *rbfs) {
    if (rbfs->sizeof_resource_descriptor_table != (rbfs->sizeof_resource_descriptor * rbfs->resource_descriptor_count)) {
        // TODO: Use macros or constants here.
        return 1;
    }
    if (rbfs->sizeof_cluster_table != (rbfs->sizeof_cluster * rbfs->cluster_count)) {
        return 2;
    }
    if (rbfs->sizeof_cluster != (rbfs->sizeof_cluster_control + rbfs->sizeof_cluster_data)) {
        return 3;
    }
    if (rbfs->memory_size != rbfs->sizeof_cluster_table + rbfs->cluster_table_address) {
        return 4;
    }
    return 0;
}

#endif // __RBFS_UTIL_C__
