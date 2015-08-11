/**
 * rbfs - Simple Resource Based File System
 * 
 * rbfs.c
 * 
 * A file system implementation based on the idea of resources
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __RBFS_C__
#define __RBFS_C__ 1

#include <rbfs.h>
#include <rbfs_io.h>
#include <rbfs_util.h>
#include <stdint.h>

rbfs_global_flags_t rbfs_global_flags;

rbfs_op_result_t rbfs_format(rbfs_t *rbfs) {
    uint8_t i;
    _rbfs_write_rbfs_to_disk(rbfs->driver, rbfs);
    for (i = 0; i < rbfs->resource_descriptor_count; i++) {
        _rbfs_format_resorce_descriptor(rbfs, i);
    }
    for (i = 0; i < rbfs->cluster_count; i++) {
        _rbfs_format_cluster(rbfs, i);
    }
    return RBFS_OP_RESULT_SUCCESS;
}

rbfs_op_result_t rbfs_mount(rbfs_driver_t driver, rbfs_t *rbfs, rbfs_mount_options_t options) {
    if (_rbfs_is_driver_monted(driver)) {
        return RBFS_OP_RESULT_ERROR_DRIVER_BUSY;
    }
    _rbfs_read_rbfs_from_disk(driver, rbfs);
    _rbfs_set_driver_monted(driver, 1);
    if (options & RBFS_MOUNT_OPTION_READ_ONLY) {
        rbfs->flags |= RBFS_FLAG_BIT_READ_ONLY;
    }
    rbfs->driver = driver;
    _rbfs_free_resource_descriptors(rbfs);
    return RBFS_OP_RESULT_SUCCESS;
}

rbfs_op_result_t rbfs_umount(rbfs_t *rbfs) {
    if (_rbfs_is_driver_monted(rbfs->driver)) {
        _rbfs_set_driver_monted(rbfs->driver, 0);
    }
    return RBFS_OP_RESULT_SUCCESS;
}

rbfs_op_result_t rbfs_open(rbfs_t *rbfs, rbfs_resource_code_t resource_code, rbfs_resource_t *resource, rbfs_open_resource_options_t options) {
    uint8_t i;
    rbfs_memory_address_t address;
    rbfs_resource_descriptor_t resource_descriptor;
    uint8_t flags;
    if (!_rbfs_is_driver_monted(rbfs->driver)) {
        return RBFS_OP_RESULT_ERROR_DRIVER_NOT_MOUNTED;
    }
    resource_descriptor = _rbfs_resource_code_to_resource_descriptor(resource_code);
    address = _rbfs_resource_descriptor_to_address(rbfs, resource_descriptor);
    flags = _rbfs_io_read(rbfs->driver, RD_ADDRESS_TO_FLAG(address));
    if (!(flags & RBFS_RESOURCE_FLAG_BIT_ALLOCATED)) {
        return RBFS_OP_RESULT_ERROR_RESOURCE_DOES_NOT_ALLOCATED;
    }
    if (flags & RBFS_RESOURCE_FLAG_BIT_OPENED) {
        return RBFS_OP_RESULT_ERROR_RESOURCE_OPENED;
    }
    flags |= RBFS_RESOURCE_FLAG_BIT_OPENED;
    if ((options & RBFS_OPEN_RESOURCE_OPTION_READ_ONLY) || (rbfs->flags & RBFS_FLAG_BIT_READ_ONLY)) {
        flags |= RBFS_RESOURCE_FLAG_BIT_READ_ONLY;

    }
    _rbfs_io_write(rbfs->driver, RD_ADDRESS_TO_FLAG(address), flags);
    resource->resource_descriptor = resource_descriptor;
    resource->first_cluster = _rbfs_io_read(rbfs->driver, RD_ADDRESS_TO_FIRST_CLUSTER(address));
    resource->current_cluster = resource->first_cluster;
    resource->cluster_offset = rbfs->sizeof_cluster_control;
    resource->current_position = 0;
    for (i = 0; i < RBFS_SIZEOF_RESOURCE_SIZE; i++) {
        *((uint8_t *) (&resource->size) + i) = _rbfs_io_read(rbfs->driver, address + i);
    }
    resource->flags = flags;
    _rbfs_check_for_eor_reached(resource);
    return RBFS_OP_RESULT_SUCCESS;
}

rbfs_op_result_t rbfs_close(rbfs_t *rbfs, rbfs_resource_t *resource) {
    rbfs_sync(rbfs, resource);
    _rbfs_free_resource_descriptor(rbfs, resource->resource_descriptor);
    resource->flags = ~RBFS_RESOURCE_FLAG_BIT_OPENED;
    return RBFS_OP_RESULT_SUCCESS;
}

uint8_t rbfs_read(rbfs_t *rbfs, rbfs_resource_t *resource) {
    rbfs_memory_address_t address;
    uint8_t read_data;
    if (!(resource->flags & RBFS_RESOURCE_FLAG_BIT_OPENED)) {
        resource->flags |= RBFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ;
        return 0;
    }
    if (_rbfs_is_eor_reached(resource)) {
        return 0;
    }
    _rbfs_check_for_availability(rbfs, resource);
    address = _rbfs_cluster_to_address(rbfs, resource->current_cluster);
    read_data = _rbfs_io_read(rbfs->driver, address + resource->cluster_offset);
    resource->current_position++;
    resource->cluster_offset++;
    _rbfs_check_for_eor_reached(resource);
    return read_data;
}

rbfs_op_result_t rbfs_write(rbfs_t *rbfs, rbfs_resource_t *resource, uint8_t data_to_write) {
    rbfs_memory_address_t address;
    if (!(resource->flags & RBFS_RESOURCE_FLAG_BIT_OPENED)) {
        return RBFS_OP_RESULT_ERROR_RESOURCE_CLOSED;
    }
    if (resource->flags & RBFS_RESOURCE_FLAG_BIT_READ_ONLY) {
        return RBFS_OP_RESULT_ERROR_RESOURCE_READ_ONLY;
    }
    if (!_rbfs_check_for_availability(rbfs, resource)) {
        return RBFS_OP_RESULT_ERROR_NO_SPACE_AVAILABLE;
    }
    address = _rbfs_cluster_to_address(rbfs, resource->current_cluster);
    _rbfs_io_write(rbfs->driver, address + resource->cluster_offset, data_to_write);
    resource->cluster_offset++;
    resource->current_position++;
    if (rbfs_eor(resource)) {
        resource->size++;
        rbfs_sync(rbfs, resource);
    }
    return RBFS_OP_RESULT_SUCCESS;
}

rbfs_op_result_t rbfs_seek(rbfs_t *rbfs, rbfs_resource_t *resource, rbfs_seek_origin_t origin, rbfs_seek_int_t offset) {
    int16_t new_position = 0;
    if (resource->size == 0) {
        return RBFS_OP_RESULT_SUCCESS;
    }
    switch (origin) {
        case RBFS_SEEK_ORIGIN_BEGIN:
            new_position = offset;
            break;
        case RBFS_SEEK_ORIGIN_CURRENT:
            new_position = resource->current_position + offset;
            break;
    }
    new_position %= resource->size + 1;
    if (new_position < 0) {
        new_position += resource->size;
    }
    if (new_position == 0) {
        rbfs_rewind(rbfs, resource);
        return RBFS_OP_RESULT_SUCCESS;
    }
    if (new_position < resource->current_position) {
        if (new_position > (resource->current_position - new_position)) {
            _rbfs_move_current_position_back(rbfs, resource, (resource->current_position - new_position));
        } else {
            rbfs_rewind(rbfs, resource);
            _rbfs_move_current_position_ahead(rbfs, resource, new_position);
        }
    } else {
        _rbfs_move_current_position_ahead(rbfs, resource, (new_position - resource->current_position));
    }
    _rbfs_check_for_eor_reached(resource);
    return RBFS_OP_RESULT_SUCCESS;
}

rbfs_op_result_t rbfs_truncate(rbfs_t *rbfs, rbfs_resource_t *resource) {
    uint8_t flags;
    rbfs_memory_address_t resource_descriptor_address;
    uint8_t freed_clusters = 0;
    resource_descriptor_address = _rbfs_resource_descriptor_to_address(rbfs, resource->resource_descriptor);
    flags = _rbfs_io_read(rbfs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address));
    if (!(flags & RBFS_RESOURCE_FLAG_BIT_ALLOCATED)) {
        return RBFS_OP_RESULT_ERROR_RESOURCE_DOES_NOT_ALLOCATED;
    }
    if (resource->size > rbfs->sizeof_cluster_data) {
        freed_clusters = _rbfs_format_clusterbfs_chain(rbfs, _rbfs_next_cluster_by_cluster(rbfs, resource->first_cluster));
    }
    _rbfs_increase_free_clusters(rbfs, freed_clusters);
    resource->size = 0x00;
    _rbfs_io_write(rbfs->driver, RD_ADDRESS_TO_SIZE_LOW(resource_descriptor_address), 0x00);
    _rbfs_io_write(rbfs->driver, RD_ADDRESS_TO_SIZE_HIGH(resource_descriptor_address), 0x00);
    return RBFS_OP_RESULT_SUCCESS;
}

void rbfs_sync(rbfs_t *rbfs, rbfs_resource_t *resource) {
    uint8_t i;
    rbfs_memory_address_t address;
    address = _rbfs_resource_descriptor_to_address(rbfs, resource->resource_descriptor);
    for (i = 0; i < 2; i++) {
        _rbfs_io_write(rbfs->driver, address + i, *((uint8_t *) (&(resource->size)) + i));
    }
}

void rbfs_stat(rbfs_t *rbfs, rbfs_resource_t *resource, rbfs_stat_t *stat) { // TODO
    stat->flags = 0xff;
}

rbfs_op_result_t rbfs_rewind(rbfs_t *rbfs, rbfs_resource_t *resource) {
    resource->current_cluster = resource->first_cluster;
    resource->cluster_offset = rbfs->sizeof_cluster_control;
    resource->current_position = 0;
    _rbfs_check_for_eor_reached(resource);
    return RBFS_OP_RESULT_SUCCESS;
}

rbfs_resource_code_t rbfs_alloc(rbfs_t *rbfs) {
    uint8_t i;
    uint8_t flags;
    rbfs_cluster_t first_cluster;
    rbfs_memory_address_t resource_descriptor_address, cluster_address;
    if (rbfs->free_clusters < 1) {
        return RBFS_NULL_RESOURCE_CODE;
    }
    resource_descriptor_address = rbfs->resource_descriptor_table_address;
    for (i = 0; i < rbfs->resource_descriptor_count; i++) {
        flags = _rbfs_io_read(rbfs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address));
        if (!(flags & RBFS_RESOURCE_FLAG_BIT_ALLOCATED)) {
            cluster_address = _rbfs_alloc_cluster(rbfs);
            if (cluster_address == RBFS_NULL_CLUSTER_ADDRESS) {
                return RBFS_NULL_RESOURCE_CODE;
            }
            flags |= RBFS_RESOURCE_FLAG_BIT_ALLOCATED;
            first_cluster = _rbfs_address_to_cluster(rbfs, cluster_address);
            _rbfs_create_cluster_chain(rbfs, first_cluster, RBFS_INEXISTENT_CLUSTER);
            _rbfs_io_write(rbfs->driver, RD_ADDRESS_TO_FIRST_CLUSTER(resource_descriptor_address), first_cluster);
            _rbfs_io_write(rbfs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address), flags);
            return _rbfs_resource_descriptor_to_resource_code(i);
        }
        resource_descriptor_address += rbfs->sizeof_resource_descriptor;
    }
    return RBFS_NULL_RESOURCE_CODE;
}

uint8_t rbfs_release(rbfs_t *rbfs, rbfs_resource_t *resource) {
    uint8_t flags;
    rbfs_memory_address_t resource_descriptor_address;
    resource_descriptor_address = _rbfs_resource_descriptor_to_address(rbfs, resource->resource_descriptor);
    flags = _rbfs_io_read(rbfs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address));
    if (!(flags & RBFS_RESOURCE_FLAG_BIT_ALLOCATED)) {
        return 1;
    }
    _rbfs_format_resource_clusters(rbfs, resource);
    _rbfs_format_resorce_descriptor(rbfs, resource->resource_descriptor);
    resource->flags = 0x00;
    return 1;
}

rbfs_resource_size_t rbfs_size(rbfs_resource_t *resource) {
    return resource->size;
}

rbfs_resource_size_t rbfs_tell(rbfs_resource_t *resource) {
    return resource->current_position;
}

uint8_t rbfs_eor(rbfs_resource_t *resource) {
    return _rbfs_is_eor_reached(resource);
}

uint8_t rbfs_error(rbfs_resource_t *resource) {
    return (resource->flags & RBFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ || resource->flags & RBFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_WRITE);
}

rbfs_resource_size_t rbfs_available_space(rbfs_t *rbfs) {
    return rbfs->free_clusters * rbfs->sizeof_cluster_data;
}

rbfs_resource_size_t rbfs_total_space(rbfs_t *rbfs) {
    return rbfs->cluster_count * rbfs->sizeof_cluster_data;
}

#endif // __RBFS_C__
