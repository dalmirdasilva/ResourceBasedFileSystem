/**
 * srbfs - Simple Resource Based File System
 * 
 * srbfs_util.c
 * 
 * Util lib for srbfs
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SRBFS_UTIL_C__
#define __SRBFS_UTIL_C__ 1

#include "srbfs_util.h"
#include "srbfs_io.h"

void _srbfs_write_srbfs_to_disk(srbfs_driver_t driver, srbfs_t *srbfs) {
    uint8_t i;
    uint8_t *p;
    srbfs_memory_address_t address = SRBFS_FIRST_ADDRESS_OF_MEMORY;
    p = (uint8_t *) srbfs;
    for (i = 0; i < sizeof (srbfs_t); i++) {
        _srbfs_io_write(driver, address++, *(p++));
    }
}

void _srbfs_read_srbfs_from_disk(srbfs_driver_t driver, srbfs_t *srbfs) {
    uint8_t i;
    uint8_t *p;
    srbfs_memory_address_t address = SRBFS_FIRST_ADDRESS_OF_MEMORY;
    p = (uint8_t *) srbfs;
    for (i = 0; i < sizeof (srbfs_t); i++) {
        *(p++) = _srbfs_io_read(driver, address++);
    }
}

srbfs_memory_address_t _srbfs_alloc_cluster(srbfs_t *srbfs) {
    srbfs_memory_address_t address;
    uint8_t i;
    address = srbfs->cluster_table_address;
    for (i = 0; i < srbfs->cluster_count; i++) {
        if (_srbfs_is_free_cluster(srbfs, (srbfs_cluster_t) i)) {
            _srbfs_decrease_free_clusters(srbfs, 1);
            return address;
        }
        address += srbfs->sizeof_cluster;
    }
    return SRBFS_NULL_CLUSTER_ADDRESS;
}

uint8_t _srbfs_is_free_cluster(srbfs_t *srbfs, srbfs_cluster_t cluster) {
    return (cluster == _srbfs_prev_cluster_by_cluster(srbfs, cluster)) \
        && (cluster == _srbfs_next_cluster_by_cluster(srbfs, cluster));
}

void _srbfs_format_cluster(srbfs_t *srbfs, srbfs_cluster_t cluster) {
    srbfs_memory_address_t address;
    address = _srbfs_cluster_to_address(srbfs, cluster);
    _srbfs_io_write(srbfs->driver, CLUSTER_ADDRESS_TO_NEXT(address), cluster);
    _srbfs_io_write(srbfs->driver, CLUSTER_ADDRESS_TO_PREV(address), cluster);
}

void _srbfs_free_cluster(srbfs_t *srbfs, srbfs_cluster_t cluster) {
    _srbfs_format_cluster(srbfs, cluster);
    _srbfs_increase_free_clusters(srbfs, 1);
}

void _srbfs_create_cluster_chain(srbfs_t *srbfs, srbfs_cluster_t prev_cluster, srbfs_cluster_t next_cluster) {
    srbfs_memory_address_t address;
    if (prev_cluster != SRBFS_INEXISTENT_CLUSTER) {
        address = _srbfs_cluster_to_address(srbfs, prev_cluster);
        _srbfs_io_write(srbfs->driver, CLUSTER_ADDRESS_TO_NEXT(address), (uint8_t) next_cluster);
    }
    if (next_cluster != SRBFS_INEXISTENT_CLUSTER) {
        address = _srbfs_cluster_to_address(srbfs, next_cluster);
        _srbfs_io_write(srbfs->driver, CLUSTER_ADDRESS_TO_PREV(address), (uint8_t) prev_cluster);
    }
}

void _srbfs_check_for_eor_reached(srbfs_resource_t *resource) {
    if (resource->current_position >= resource->size) {
        resource->flags |= SRBFS_RESOURCE_FLAG_BIT_EOR_REACHED;
    } else {
        resource->flags &= ~SRBFS_RESOURCE_FLAG_BIT_EOR_REACHED;
    }
}

uint8_t _srbfs_is_eor_reached(srbfs_resource_t *resource) {
    return resource->flags & SRBFS_RESOURCE_FLAG_BIT_EOR_REACHED;
}

uint8_t _srbfs_check_for_availability(srbfs_t *srbfs, srbfs_resource_t *resource) {
    srbfs_memory_address_t address;
    srbfs_cluster_t cluster;
    _srbfs_check_for_eor_reached(resource);
    if (resource->cluster_offset >= srbfs->sizeof_cluster) {
        if (srbfs_eor(resource)) {
            address = _srbfs_alloc_cluster(srbfs);
            if (address == SRBFS_NULL_CLUSTER_ADDRESS) {
                return 0;
            }
            cluster = _srbfs_address_to_cluster(srbfs, address);
            _srbfs_create_cluster_chain(srbfs, resource->current_cluster, cluster);
            resource->current_cluster = cluster;
        } else {
            resource->current_cluster = _srbfs_next_cluster_by_cluster(srbfs, resource->current_cluster);
        }
        resource->cluster_offset = srbfs->sizeof_cluster_control;
    }
    return 1;
}

uint8_t _srbfs_move_current_position_ahead(srbfs_t *srbfs, srbfs_resource_t *resource, srbfs_seek_int_t offset) {
    uint8_t until_the_end;
    uint8_t how_many_clustes_ahead;
    uint8_t i;
    resource->current_position += offset;
    until_the_end = (srbfs->sizeof_cluster - resource->cluster_offset);
    if (offset <= until_the_end) {
        resource->cluster_offset += offset;
        return 1;
    }
    offset -= until_the_end;
    how_many_clustes_ahead = (offset / srbfs->sizeof_cluster_data) + 1;
    resource->cluster_offset = (offset % srbfs->sizeof_cluster_data) + srbfs->sizeof_cluster_control;
    for (i = 0; i < how_many_clustes_ahead; i++) {
        resource->current_cluster = _srbfs_next_cluster_by_cluster(srbfs, resource->current_cluster);
    }
    return 1;
}

uint8_t _srbfs_move_current_position_back(srbfs_t *srbfs, srbfs_resource_t *resource, srbfs_seek_int_t offset) {
    uint8_t until_the_begin;
    uint8_t how_many_clustes_back;
    uint8_t i;
    resource->current_position -= offset;
    until_the_begin = (resource->cluster_offset - srbfs->sizeof_cluster_control);
    if (offset <= until_the_begin) {
        resource->cluster_offset -= offset;
        return 1;
    }
    offset -= until_the_begin;
    how_many_clustes_back = (offset / srbfs->sizeof_cluster_data);
    if ((offset % srbfs->sizeof_cluster_data) != 0) {
        how_many_clustes_back++;
    }
    resource->cluster_offset = srbfs->sizeof_cluster - (offset % srbfs->sizeof_cluster_data);
    for (i = 0; i < how_many_clustes_back; i++) {
        resource->current_cluster = _srbfs_prev_cluster_by_cluster(srbfs, resource->current_cluster);
    }
    return 1;
}

void _srbfs_format_resorce_descriptor(srbfs_t *srbfs, srbfs_resource_descriptor_t resource_descriptor) {
    int i;
    srbfs_memory_address_t address;
    address = _srbfs_resource_descriptor_to_address(srbfs, resource_descriptor);
    for (i = 0; i < srbfs->sizeof_resource_descriptor; i++) {
        _srbfs_io_write(srbfs->driver, address + i, 0x00);
    }
}

uint8_t _srbfs_is_driver_monted(srbfs_driver_t driver) {
    return srbfs_global_flags.driver_mouted & (1 << driver);
}

void _srbfs_set_driver_monted(srbfs_driver_t driver, uint8_t is) {
    if (is) {
        srbfs_global_flags.driver_mouted |= (1 << driver);
    } else {
        srbfs_global_flags.driver_mouted &= ~(1 << driver);
    }
}

void _srbfs_free_resource_descriptors(srbfs_t *srbfs) {
    uint8_t i;
    for (i = 0; i < srbfs->resource_descriptor_count; i++) {
        _srbfs_free_resource_descriptor(srbfs, i);
    }
}

void _srbfs_free_resource_descriptor(srbfs_t *srbfs, srbfs_resource_descriptor_t resource_descriptor) {
    srbfs_memory_address_t address;
    uint8_t flags;
    address = _srbfs_resource_descriptor_to_address(srbfs, resource_descriptor);
    flags = _srbfs_io_read(srbfs->driver, RD_ADDRESS_TO_FLAG(address));
    flags &= ~(SRBFS_RESOURCE_FLAG_BIT_OPENED | SRBFS_RESOURCE_FLAG_BIT_READ_ONLY);
    _srbfs_io_write(srbfs->driver, RD_ADDRESS_TO_FLAG(address), flags);
}

void _srbfs_format_resource_clusters(srbfs_t *srbfs, srbfs_resource_t *resource) {
    uint8_t freed_clusters;
    freed_clusters = _srbfs_format_clustesrbfs_chain(srbfs, resource->first_cluster);
    _srbfs_increase_free_clusters(srbfs, freed_clusters);
}

uint8_t _srbfs_format_clustesrbfs_chain(srbfs_t *srbfs, srbfs_cluster_t cluster) {
    srbfs_cluster_t next_cluster;
    uint8_t formated_clusters = 0;
    do {
        next_cluster = _srbfs_next_cluster_by_cluster(srbfs, cluster);
        _srbfs_format_cluster(srbfs, cluster);
        formated_clusters++;
        if (next_cluster == SRBFS_INEXISTENT_CLUSTER || next_cluster == cluster) {
            break;
        }
        cluster = next_cluster;
    } while (1);
    return formated_clusters;
}

uint8_t _srbfs_has_invalid_attributes(srbfs_t *srbfs) {
    if (srbfs->sizeof_resource_descriptor_table != (srbfs->sizeof_resource_descriptor * srbfs->resource_descriptor_count)) {
        // TODO: Use macros or constants here.
        return 1;
    }
    if (srbfs->sizeof_cluster_table != (srbfs->sizeof_cluster * srbfs->cluster_count)) {
        return 2;
    }
    if (srbfs->sizeof_cluster != (srbfs->sizeof_cluster_control + srbfs->sizeof_cluster_data)) {
        return 3;
    }
    if (srbfs->memory_size != srbfs->sizeof_cluster_table + srbfs->cluster_table_address) {
        return 4;
    }
    return 0;
}

#endif // __SRBFS_UTIL_C__
