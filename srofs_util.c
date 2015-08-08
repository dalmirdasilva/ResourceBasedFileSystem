/**
 * srofs - Simple Resource Oriented File System
 * 
 * srofs_util.c
 * 
 * Util lib for srofs
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SROFS_UTIL_C__
#define __SROFS_UTIL_C__ 1

#include "srofs_util.h"

void _srofs_write_srofs_to_disc(srofs_driver_t driver, srofs_t *srofs) {
    uint8_t i;
    uint8_t *p;
    srofs_memory_address_t address = SROFS_FIRST_ADDRESS_OF_MEMORY;
    p = (uint8_t *) srofs;
    for (i = 0; i < sizeof (srofs_t); i++) {
        _srofs_io_write(driver, address++, *(p++));
    }
}

void _srofs_read_srofs_from_disc(srofs_driver_t driver, srofs_t *srofs) {
    uint8_t i;
    uint8_t *p;
    srofs_memory_address_t address = SROFS_FIRST_ADDRESS_OF_MEMORY;
    p = (uint8_t *) srofs;
    for (i = 0; i < sizeof (srofs_t); i++) {
        *(p++) = _srofs_io_read(driver, address++);
    }
}

srofs_memory_address_t _srofs_alloc_cluster(srofs_t *srofs) {
    srofs_memory_address_t address;
    uint8_t i;
    address = srofs->cluster_table_address;
    for (i = 0; i < srofs->cluster_count; i++) {
        if (_srofs_is_free_cluster(srofs, (srofs_cluster_t) i)) {
            _srofs_decrease_free_clusters(srofs, 1);
            return address;
        }
        address += srofs->sizeof_cluster;
    }
    return SROFS_NULL_CLUSTER_ADDRESS;
}

uint8_t _srofs_is_free_cluster(srofs_t *srofs, srofs_cluster_t cluster) {
    return (cluster == _srofs_prev_cluster_by_cluster(srofs, cluster)) \
        && (cluster == _srofs_next_cluster_by_cluster(srofs, cluster));
}

void _srofs_format_cluster(srofs_t *srofs, srofs_cluster_t cluster) {
    srofs_memory_address_t address;
    address = _srofs_cluster_to_address(srofs, cluster);
    _srofs_io_write(srofs->driver, CLUSTER_ADDRESS_TO_NEXT(address), cluster);
    _srofs_io_write(srofs->driver, CLUSTER_ADDRESS_TO_PREV(address), cluster);
}

void _srofs_free_cluster(srofs_t *srofs, srofs_cluster_t cluster) {
    _srofs_format_cluster(srofs, cluster);
    _srofs_increase_free_clusters(srofs, 1);
}

void _srofs_create_cluster_chain(srofs_t *srofs, srofs_cluster_t prev_cluster, srofs_cluster_t next_cluster) {
    srofs_memory_address_t address;
    if (prev_cluster != SROFS_INEXISTENT_CLUSTER) {
        address = _srofs_cluster_to_address(srofs, prev_cluster);
        _srofs_io_write(srofs->driver, CLUSTER_ADDRESS_TO_NEXT(address), (uint8_t) next_cluster);
    }
    if (next_cluster != SROFS_INEXISTENT_CLUSTER) {
        address = _srofs_cluster_to_address(srofs, next_cluster);
        _srofs_io_write(srofs->driver, CLUSTER_ADDRESS_TO_PREV(address), (uint8_t) prev_cluster);
    }
}

void _srofs_check_for_eor_reached(srofs_resource_t *resource) {
    if (resource->current_position >= resource->size) {
        resource->flags |= SROFS_RESOURCE_FLAG_BIT_EOR_REACHED;
    } else {
        resource->flags &= ~SROFS_RESOURCE_FLAG_BIT_EOR_REACHED;
    }
}

uint8_t _srofs_is_eor_reached(srofs_resource_t *resource) {
    return resource->flags & SROFS_RESOURCE_FLAG_BIT_EOR_REACHED;
}

uint8_t _srofs_check_for_availability(srofs_t *srofs, srofs_resource_t *resource) {
    srofs_memory_address_t address;
    srofs_cluster_t cluster;
    _srofs_check_for_eor_reached(resource);
    if (resource->cluster_offset >= srofs->sizeof_cluster) {
        if (srofs_eor(resource)) {
            address = _srofs_alloc_cluster(srofs);
            if (address == SROFS_NULL_CLUSTER_ADDRESS) {
                return 0;
            }
            cluster = _srofs_address_to_cluster(srofs, address);
            _srofs_create_cluster_chain(srofs, resource->current_cluster, cluster);
            resource->current_cluster = cluster;
        } else {
            resource->current_cluster = _srofs_next_cluster_by_cluster(srofs, resource->current_cluster);
        }
        resource->cluster_offset = srofs->sizeof_cluster_control;
    }
    return 1;
}

uint8_t _srofs_move_current_position_ahead(srofs_t *srofs, srofs_resource_t *resource, srofs_seek_int_t offset) {
    uint8_t until_the_end;
    uint8_t how_many_clustes_ahead;
    uint8_t i;
    resource->current_position += offset;
    until_the_end = (srofs->sizeof_cluster - resource->cluster_offset);
    if (offset <= until_the_end) {
        resource->cluster_offset += offset;
        return 1;
    }
    offset -= until_the_end;
    how_many_clustes_ahead = (offset / srofs->sizeof_cluster_data) + 1;
    resource->cluster_offset = (offset % srofs->sizeof_cluster_data) + srofs->sizeof_cluster_control;
    for (i = 0; i < how_many_clustes_ahead; i++) {
        resource->current_cluster = _srofs_next_cluster_by_cluster(srofs, resource->current_cluster);
    }
    return 1;
}

uint8_t _srofs_move_current_position_back(srofs_t *srofs, srofs_resource_t *resource, srofs_seek_int_t offset) {
    uint8_t until_the_begin;
    uint8_t how_many_clustes_back;
    uint8_t i;
    resource->current_position -= offset;
    until_the_begin = (resource->cluster_offset - srofs->sizeof_cluster_control);
    if (offset <= until_the_begin) {
        resource->cluster_offset -= offset;
        return 1;
    }
    offset -= until_the_begin;
    how_many_clustes_back = (offset / srofs->sizeof_cluster_data);
    if ((offset % srofs->sizeof_cluster_data) != 0) {
        how_many_clustes_back++;
    }
    resource->cluster_offset = srofs->sizeof_cluster - (offset % srofs->sizeof_cluster_data);
    for (i = 0; i < how_many_clustes_back; i++) {
        resource->current_cluster = _srofs_prev_cluster_by_cluster(srofs, resource->current_cluster);
    }
    return 1;
}

void _srofs_format_resorce_descriptor(srofs_t *srofs, srofs_resource_descriptor_t resource_descriptor) {
    int i;
    srofs_memory_address_t address;
    address = _srofs_resource_descriptor_to_address(srofs, resource_descriptor);
    for (i = 0; i < srofs->sizeof_resource_descriptor; i++) {
        _srofs_io_write(srofs->driver, address + i, 0x00);
    }
}

uint8_t _srofs_is_driver_monted(srofs_driver_t driver) {
    return srofs_global_flags.driver_mouted & (1 << driver);
}

void _srofs_set_driver_monted(srofs_driver_t driver, uint8_t is) {
    if (is) {
        srofs_global_flags.driver_mouted |= (1 << driver);
    } else {
        srofs_global_flags.driver_mouted &= ~(1 << driver);
    }
}

void _srofs_free_resource_descriptors(srofs_t *srofs) {
    uint8_t i;
    for (i = 0; i < srofs->resource_descriptor_count; i++) {
        _srofs_free_resource_descriptor(srofs, i);
    }
}

void _srofs_free_resource_descriptor(srofs_t *srofs, srofs_resource_descriptor_t resource_descriptor) {
    srofs_memory_address_t address;
    uint8_t flags;
    address = _srofs_resource_descriptor_to_address(srofs, resource_descriptor);
    flags = _srofs_io_read(srofs->driver, RD_ADDRESS_TO_FLAG(address));
    flags &= ~(SROFS_RESOURCE_FLAG_BIT_OPENED | SROFS_RESOURCE_FLAG_BIT_READ_ONLY);
    _srofs_io_write(srofs->driver, RD_ADDRESS_TO_FLAG(address), flags);
}

void _srofs_format_resource_clusters(srofs_t *srofs, srofs_resource_t *resource) {
    uint8_t freed_clusters;
    freed_clusters = _srofs_format_clustesrofs_chain(srofs, resource->first_cluster);
    _srofs_increase_free_clusters(srofs, freed_clusters);
}

uint8_t _srofs_format_clustesrofs_chain(srofs_t *srofs, srofs_cluster_t cluster) {
    srofs_cluster_t next_cluster;
    uint8_t formated_clusters = 0;
    do {
        next_cluster = _srofs_next_cluster_by_cluster(srofs, cluster);
        _srofs_format_cluster(srofs, cluster);
        formated_clusters++;
        if (next_cluster == SROFS_INEXISTENT_CLUSTER || next_cluster == cluster) {
            break;
        }
        cluster = next_cluster;
    } while (1);
    return formated_clusters;
}

uint8_t _srofs_has_invalid_attributes(srofs_t *srofs) {
    if (srofs->sizeof_resource_descriptor_table != (srofs->sizeof_resource_descriptor * srofs->resource_descriptor_count)) {
        // TODO: Use macros or constants here.
        return 1;
    }
    if (srofs->sizeof_cluster_table != (srofs->sizeof_cluster * srofs->cluster_count)) {
        return 2;
    }
    if (srofs->sizeof_cluster != (srofs->sizeof_cluster_control + srofs->sizeof_cluster_data)) {
        return 3;
    }
    if (srofs->memory_size != srofs->sizeof_cluster_table + srofs->cluster_table_address) {
        return 4;
    }
    return 0;
}

#endif // __SROFS_UTIL_C__
