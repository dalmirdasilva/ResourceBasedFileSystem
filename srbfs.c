/**
 * srbfs - Simple Resource Based File System
 * 
 * srbfs.c
 * 
 * A file system implementation based on the idea of resources
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SRBFS_C__
#define __SRBFS_C__ 1

#include <srbfs.h>
#include <srbfs_io.h>
#include <srbfs_util.h>
#include <stdint.h>

srbfs_global_flags_t srbfs_global_flags;

srbfs_op_result_t srbfs_format(srbfs_t *srbfs) {
    uint8_t i;
    _srbfs_write_srbfs_to_disk(srbfs->driver, srbfs);
    for (i = 0; i < srbfs->resource_descriptor_count; i++) {
        _srbfs_format_resorce_descriptor(srbfs, i);
    }
    for (i = 0; i < srbfs->cluster_count; i++) {
        _srbfs_format_cluster(srbfs, i);
    }
    return SRBFS_OP_RESULT_SUCCESS;
}

srbfs_op_result_t srbfs_mount(srbfs_driver_t driver, srbfs_t *srbfs, srbfs_mount_options_t options) {
    if (_srbfs_is_driver_monted(driver)) {
        return SRBFS_OP_RESULT_ERROR_DRIVER_BUSY;
    }
    _srbfs_read_srbfs_from_disk(driver, srbfs);
    _srbfs_set_driver_monted(driver, 1);
    if (options & SRBFS_MOUNT_OPTION_READ_ONLY) {
        srbfs->flags |= SRBFS_FLAG_BIT_READ_ONLY;
    }
    srbfs->driver = driver;
    _srbfs_free_resource_descriptors(srbfs);
    return SRBFS_OP_RESULT_SUCCESS;
}

srbfs_op_result_t srbfs_umount(srbfs_t *srbfs) {
    if (_srbfs_is_driver_monted(srbfs->driver)) {
        _srbfs_set_driver_monted(srbfs->driver, 0);
    }
    return SRBFS_OP_RESULT_SUCCESS;
}

srbfs_op_result_t srbfs_open(srbfs_t *srbfs, srbfs_resource_code_t resource_code, srbfs_resource_t *resource, srbfs_open_resource_options_t options) {
    uint8_t i;
    srbfs_memory_address_t address;
    srbfs_resource_descriptor_t resource_descriptor;
    uint8_t flags;
    if (!_srbfs_is_driver_monted(srbfs->driver)) {
        return SRBFS_OP_RESULT_ERROR_DRIVER_NOT_MOUNTED;
    }
    resource_descriptor = _srbfs_resource_code_to_resource_descriptor(resource_code);
    address = _srbfs_resource_descriptor_to_address(srbfs, resource_descriptor);
    flags = _srbfs_io_read(srbfs->driver, RD_ADDRESS_TO_FLAG(address));
    if (!(flags & SRBFS_RESOURCE_FLAG_BIT_ALLOCATED)) {
        return SRBFS_OP_RESULT_ERROR_RESOURCE_DOES_NOT_ALLOCATED;
    }
    if (flags & SRBFS_RESOURCE_FLAG_BIT_OPENED) {
        return SRBFS_OP_RESULT_ERROR_RESOURCE_OPENED;
    }
    flags |= SRBFS_RESOURCE_FLAG_BIT_OPENED;
    if ((options & SRBFS_OPEN_RESOURCE_OPTION_READ_ONLY) || (srbfs->flags & SRBFS_FLAG_BIT_READ_ONLY)) {
        flags |= SRBFS_RESOURCE_FLAG_BIT_READ_ONLY;

    }
    _srbfs_io_write(srbfs->driver, RD_ADDRESS_TO_FLAG(address), flags);
    resource->resource_descriptor = resource_descriptor;
    resource->first_cluster = _srbfs_io_read(srbfs->driver, RD_ADDRESS_TO_FIRST_CLUSTER(address));
    resource->current_cluster = resource->first_cluster;
    resource->cluster_offset = srbfs->sizeof_cluster_control;
    resource->current_position = 0;
    for (i = 0; i < SRBFS_SIZEOF_RESOURCE_SIZE; i++) {
        *((uint8_t *) (&resource->size) + i) = _srbfs_io_read(srbfs->driver, address + i);
    }
    resource->flags = flags;
    _srbfs_check_for_eor_reached(resource);
    return SRBFS_OP_RESULT_SUCCESS;
}

srbfs_op_result_t srbfs_close(srbfs_t *srbfs, srbfs_resource_t *resource) {
    srbfs_sync(srbfs, resource);
    _srbfs_free_resource_descriptor(srbfs, resource->resource_descriptor);
    resource->flags = ~SRBFS_RESOURCE_FLAG_BIT_OPENED;
    return SRBFS_OP_RESULT_SUCCESS;
}

uint8_t srbfs_read(srbfs_t *srbfs, srbfs_resource_t *resource) {
    srbfs_memory_address_t address;
    uint8_t read_data;
    if (!(resource->flags & SRBFS_RESOURCE_FLAG_BIT_OPENED)) {
        resource->flags |= SRBFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ;
        return 0;
    }
    if (_srbfs_is_eor_reached(resource)) {
        return 0;
    }
    _srbfs_check_for_availability(srbfs, resource);
    address = _srbfs_cluster_to_address(srbfs, resource->current_cluster);
    read_data = _srbfs_io_read(srbfs->driver, address + resource->cluster_offset);
    resource->current_position++;
    resource->cluster_offset++;
    _srbfs_check_for_eor_reached(resource);
    return read_data;
}

srbfs_op_result_t srbfs_write(srbfs_t *srbfs, srbfs_resource_t *resource, uint8_t data_to_write) {
    srbfs_memory_address_t address;
    if (!(resource->flags & SRBFS_RESOURCE_FLAG_BIT_OPENED)) {
        return SRBFS_OP_RESULT_ERROR_RESOURCE_CLOSED;
    }
    if (resource->flags & SRBFS_RESOURCE_FLAG_BIT_READ_ONLY) {
        return SRBFS_OP_RESULT_ERROR_RESOURCE_READ_ONLY;
    }
    if (!_srbfs_check_for_availability(srbfs, resource)) {
        return SRBFS_OP_RESULT_ERROR_NO_SPACE_AVAILABLE;
    }
    address = _srbfs_cluster_to_address(srbfs, resource->current_cluster);
    _srbfs_io_write(srbfs->driver, address + resource->cluster_offset, data_to_write);
    resource->cluster_offset++;
    resource->current_position++;
    if (srbfs_eor(resource)) {
        resource->size++;
        srbfs_sync(srbfs, resource);
    }
    return SRBFS_OP_RESULT_SUCCESS;
}

srbfs_op_result_t srbfs_seek(srbfs_t *srbfs, srbfs_resource_t *resource, srbfs_seek_origin_t origin, srbfs_seek_int_t offset) {
    int16_t new_position = 0;
    if (resource->size == 0) {
        return SRBFS_OP_RESULT_SUCCESS;
    }
    switch (origin) {
        case SRBFS_SEEK_ORIGIN_BEGIN:
            new_position = offset;
            break;
        case SRBFS_SEEK_ORIGIN_CURRENT:
            new_position = resource->current_position + offset;
            break;
    }
    new_position %= resource->size + 1;
    if (new_position < 0) {
        new_position += resource->size;
    }
    if (new_position == 0) {
        srbfs_rewind(srbfs, resource);
        return SRBFS_OP_RESULT_SUCCESS;
    }
    if (new_position < resource->current_position) {
        if (new_position > (resource->current_position - new_position)) {
            _srbfs_move_current_position_back(srbfs, resource, (resource->current_position - new_position));
        } else {
            srbfs_rewind(srbfs, resource);
            _srbfs_move_current_position_ahead(srbfs, resource, new_position);
        }
    } else {
        _srbfs_move_current_position_ahead(srbfs, resource, (new_position - resource->current_position));
    }
    _srbfs_check_for_eor_reached(resource);
    return SRBFS_OP_RESULT_SUCCESS;
}

srbfs_op_result_t srbfs_truncate(srbfs_t *srbfs, srbfs_resource_t *resource) {
    uint8_t flags;
    srbfs_memory_address_t resource_descriptor_address;
    uint8_t freed_clusters = 0;
    resource_descriptor_address = _srbfs_resource_descriptor_to_address(srbfs, resource->resource_descriptor);
    flags = _srbfs_io_read(srbfs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address));
    if (!(flags & SRBFS_RESOURCE_FLAG_BIT_ALLOCATED)) {
        return SRBFS_OP_RESULT_ERROR_RESOURCE_DOES_NOT_ALLOCATED;
    }
    if (resource->size > srbfs->sizeof_cluster_data) {
        freed_clusters = _srbfs_format_clustesrbfs_chain(srbfs, _srbfs_next_cluster_by_cluster(srbfs, resource->first_cluster));
    }
    _srbfs_increase_free_clusters(srbfs, freed_clusters);
    resource->size = 0x00;
    _srbfs_io_write(srbfs->driver, RD_ADDRESS_TO_SIZE_LOW(resource_descriptor_address), 0x00);
    _srbfs_io_write(srbfs->driver, RD_ADDRESS_TO_SIZE_HIGH(resource_descriptor_address), 0x00);
    return SRBFS_OP_RESULT_SUCCESS;
}

void srbfs_sync(srbfs_t *srbfs, srbfs_resource_t *resource) {
    uint8_t i;
    srbfs_memory_address_t address;
    address = _srbfs_resource_descriptor_to_address(srbfs, resource->resource_descriptor);
    for (i = 0; i < 2; i++) {
        _srbfs_io_write(srbfs->driver, address + i, *((uint8_t *) (&(resource->size)) + i));
    }
}

void srbfs_stat(srbfs_t *srbfs, srbfs_resource_t *resource, srbfs_stat_t *stat) { // TODO
    stat->flags = 0xff;
}

srbfs_op_result_t srbfs_rewind(srbfs_t *srbfs, srbfs_resource_t *resource) {
    resource->current_cluster = resource->first_cluster;
    resource->cluster_offset = srbfs->sizeof_cluster_control;
    resource->current_position = 0;
    _srbfs_check_for_eor_reached(resource);
    return SRBFS_OP_RESULT_SUCCESS;
}

srbfs_resource_code_t srbfs_alloc(srbfs_t *srbfs) {
    uint8_t i;
    uint8_t flags;
    srbfs_cluster_t first_cluster;
    srbfs_memory_address_t resource_descriptor_address, cluster_address;
    if (srbfs->free_clusters < 1) {
        return SRBFS_NULL_RESOURCE_CODE;
    }
    resource_descriptor_address = srbfs->resource_descriptor_table_address;
    for (i = 0; i < srbfs->resource_descriptor_count; i++) {
        flags = _srbfs_io_read(srbfs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address));
        if (!(flags & SRBFS_RESOURCE_FLAG_BIT_ALLOCATED)) {
            cluster_address = _srbfs_alloc_cluster(srbfs);
            if (cluster_address == SRBFS_NULL_CLUSTER_ADDRESS) {
                return SRBFS_NULL_RESOURCE_CODE;
            }
            flags |= SRBFS_RESOURCE_FLAG_BIT_ALLOCATED;
            first_cluster = _srbfs_address_to_cluster(srbfs, cluster_address);
            _srbfs_create_cluster_chain(srbfs, first_cluster, SRBFS_INEXISTENT_CLUSTER);
            _srbfs_io_write(srbfs->driver, RD_ADDRESS_TO_FIRST_CLUSTER(resource_descriptor_address), first_cluster);
            _srbfs_io_write(srbfs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address), flags);
            return _srbfs_resource_descriptor_to_resource_code(i);
        }
        resource_descriptor_address += srbfs->sizeof_resource_descriptor;
    }
    return SRBFS_NULL_RESOURCE_CODE;
}

uint8_t srbfs_release(srbfs_t *srbfs, srbfs_resource_t *resource) {
    uint8_t flags;
    srbfs_memory_address_t resource_descriptor_address;
    resource_descriptor_address = _srbfs_resource_descriptor_to_address(srbfs, resource->resource_descriptor);
    flags = _srbfs_io_read(srbfs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address));
    if (!(flags & SRBFS_RESOURCE_FLAG_BIT_ALLOCATED)) {
        return 1;
    }
    _srbfs_format_resource_clusters(srbfs, resource);
    _srbfs_format_resorce_descriptor(srbfs, resource->resource_descriptor);
    resource->flags = 0x00;
    return 1;
}

srbfs_resource_size_t srbfs_size(srbfs_resource_t *resource) {
    return resource->size;
}

srbfs_resource_size_t srbfs_tell(srbfs_resource_t *resource) {
    return resource->current_position;
}

uint8_t srbfs_eor(srbfs_resource_t *resource) {
    return _srbfs_is_eor_reached(resource);
}

uint8_t srbfs_error(srbfs_resource_t *resource) {
    return (resource->flags & SRBFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ || resource->flags & SRBFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_WRITE);
}

srbfs_resource_size_t srbfs_available_space(srbfs_t *srbfs) {
    return srbfs->free_clusters * srbfs->sizeof_cluster_data;
}

srbfs_resource_size_t srbfs_total_space(srbfs_t *srbfs) {
    return srbfs->cluster_count * srbfs->sizeof_cluster_data;
}

#endif // __SRBFS_C__
