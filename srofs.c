/**
 * srofs - Simple Resource Oriented File System
 * 
 * srofs.c
 * 
 * A file system implementation based on the idea of resources
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SROFS_C__
#define __SROFS_C__ 1

#include <srofs.h>
#include <srofs_io.h>
#include <srofs_util.h>
#include <stdint.h>

srofs_global_flags_t srofs_global_flags;

srofs_op_result_t srofs_format(srofs_t *srofs) {
    uint8_t i;
    _srofs_write_srofs_to_disc(srofs->driver, srofs);
    for (i = 0; i < srofs->resource_descriptor_count; i++) {
        _srofs_format_resorce_descriptor(srofs, i);
    }
    for (i = 0; i < srofs->cluster_count; i++) {
        _srofs_format_cluster(srofs, i);
    }
    return SROFS_OP_RESULT_SUCCESS;
}

srofs_op_result_t srofs_mount(srofs_driver_t driver, srofs_t *srofs, srofs_mount_options_t options) {
    if (_srofs_is_driver_monted(driver)) {
        return SROFS_OP_RESULT_ERROR_DRIVER_BUSY;
    }
    _srofs_read_srofs_from_disc(driver, srofs);
    _srofs_set_driver_monted(driver, 1);
    if (options & SROFS_MOUNT_OPTION_READ_ONLY) {
        srofs->flags |= SROFS_FLAG_BIT_READ_ONLY;
    }
    srofs->driver = driver;
    _srofs_free_resource_descriptors(srofs);
    return SROFS_OP_RESULT_SUCCESS;
}

srofs_op_result_t srofs_umount(srofs_t *srofs) {
    if (_srofs_is_driver_monted(srofs->driver)) {
        _srofs_set_driver_monted(srofs->driver, 0);
    }
    return SROFS_OP_RESULT_SUCCESS;
}

srofs_op_result_t srofs_open(srofs_t *srofs, srofs_resource_code_t resource_code, srofs_resource_t *resource, srofs_open_resource_options_t options) {
    uint8_t i;
    srofs_memory_address_t address;
    srofs_resource_descriptor_t resource_descriptor;
    uint8_t flags;
    if (!_srofs_is_driver_monted(srofs->driver)) {
        return SROFS_OP_RESULT_ERROR_DRIVER_NOT_MOUNTED;
    }
    resource_descriptor = _srofs_resource_code_to_resource_descriptor(resource_code);
    address = _srofs_resource_descriptor_to_address(srofs, resource_descriptor);
    flags = _srofs_io_read(srofs->driver, RD_ADDRESS_TO_FLAG(address));
    if (!(flags & SROFS_RESOURCE_FLAG_BIT_ALLOCATED)) {
        return SROFS_OP_RESULT_ERROR_RESOURCE_DOES_NOT_ALLOCATED;
    }
    if (flags & SROFS_RESOURCE_FLAG_BIT_OPENED) {
        return SROFS_OP_RESULT_ERROR_RESOURCE_OPENED;
    }
    flags |= SROFS_RESOURCE_FLAG_BIT_OPENED;
    if ((options & SROFS_OPEN_RESOURCE_OPTION_READ_ONLY) || (srofs->flags & SROFS_FLAG_BIT_READ_ONLY)) {
        flags |= SROFS_RESOURCE_FLAG_BIT_READ_ONLY;

    }
    _srofs_io_write(srofs->driver, RD_ADDRESS_TO_FLAG(address), flags);
    resource->resource_descriptor = resource_descriptor;
    resource->first_cluster = _srofs_io_read(srofs->driver, RD_ADDRESS_TO_FIRST_CLUSTER(address));
    resource->current_cluster = resource->first_cluster;
    resource->cluster_offset = srofs->sizeof_cluster_control;
    resource->current_position = 0;
    for (i = 0; i < SROFS_SIZEOF_RESOURCE_SIZE; i++) {
        *((uint8_t *) (&resource->size) + i) = _srofs_io_read(srofs->driver, address + i);
    }
    resource->flags = flags;
    _srofs_check_for_eor_reached(resource);
    return SROFS_OP_RESULT_SUCCESS;
}

srofs_op_result_t srofs_close(srofs_t *srofs, srofs_resource_t *resource) {
    srofs_sync(srofs, resource);
    _srofs_free_resource_descriptor(srofs, resource->resource_descriptor);
    resource->flags = ~SROFS_RESOURCE_FLAG_BIT_OPENED;
    return SROFS_OP_RESULT_SUCCESS;
}

uint8_t srofs_read(srofs_t *srofs, srofs_resource_t *resource) {
    srofs_memory_address_t address;
    uint8_t read_data;
    if (!(resource->flags & SROFS_RESOURCE_FLAG_BIT_OPENED)) {
        resource->flags |= SROFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ;
        return 0;
    }
    if (_srofs_is_eor_reached(resource)) {
        return 0;
    }
    _srofs_check_for_availability(srofs, resource);
    address = _srofs_cluster_to_address(srofs, resource->current_cluster);
    read_data = _srofs_io_read(srofs->driver, address + resource->cluster_offset);
    resource->current_position++;
    resource->cluster_offset++;
    _srofs_check_for_eor_reached(resource);
    return read_data;
}

srofs_op_result_t srofs_write(srofs_t *srofs, srofs_resource_t *resource, uint8_t data_to_write) {
    srofs_memory_address_t address;
    if (!(resource->flags & SROFS_RESOURCE_FLAG_BIT_OPENED)) {
        return SROFS_OP_RESULT_ERROR_RESOURCE_CLOSED;
    }
    if (resource->flags & SROFS_RESOURCE_FLAG_BIT_READ_ONLY) {
        return SROFS_OP_RESULT_ERROR_RESOURCE_READ_ONLY;
    }
    if (!_srofs_check_for_availability(srofs, resource)) {
        return SROFS_OP_RESULT_ERROR_NO_SPACE_AVAILABLE;
    }
    address = _srofs_cluster_to_address(srofs, resource->current_cluster);
    _srofs_io_write(srofs->driver, address + resource->cluster_offset, data_to_write);
    resource->cluster_offset++;
    resource->current_position++;
    if (srofs_eor(resource)) {
        resource->size++;
        srofs_sync(srofs, resource);
    }
    return SROFS_OP_RESULT_SUCCESS;
}

srofs_op_result_t srofs_seek(srofs_t *srofs, srofs_resource_t *resource, srofs_seek_origin_t origin, srofs_seek_int_t offset) {
    int16_t new_position = 0;
    if (resource->size == 0) {
        return SROFS_OP_RESULT_SUCCESS;
    }
    switch (origin) {
        case SROFS_SEEK_ORIGIN_BEGIN:
            new_position = offset;
            break;
        case SROFS_SEEK_ORIGIN_CURRENT:
            new_position = resource->current_position + offset;
            break;
    }
    new_position %= resource->size + 1;
    if (new_position < 0) {
        new_position += resource->size;
    }
    if (new_position == 0) {
        srofs_rewind(srofs, resource);
        return SROFS_OP_RESULT_SUCCESS;
    }
    if (new_position < resource->current_position) {
        if (new_position > (resource->current_position - new_position)) {
            _srofs_move_current_position_back(srofs, resource, (resource->current_position - new_position));
        } else {
            srofs_rewind(srofs, resource);
            _srofs_move_current_position_ahead(srofs, resource, new_position);
        }
    } else {
        _srofs_move_current_position_ahead(srofs, resource, (new_position - resource->current_position));
    }
    _srofs_check_for_eor_reached(resource);
    return SROFS_OP_RESULT_SUCCESS;
}

srofs_op_result_t srofs_truncate(srofs_t *srofs, srofs_resource_t *resource) {
    uint8_t flags;
    srofs_memory_address_t resource_descriptor_address;
    uint8_t freed_clusters = 0;
    resource_descriptor_address = _srofs_resource_descriptor_to_address(srofs, resource->resource_descriptor);
    flags = _srofs_io_read(srofs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address));
    if (!(flags & SROFS_RESOURCE_FLAG_BIT_ALLOCATED)) {
        return SROFS_OP_RESULT_ERROR_RESOURCE_DOES_NOT_ALLOCATED;
    }
    if (resource->size > srofs->sizeof_cluster_data) {
        freed_clusters = _srofs_format_clustesrofs_chain(srofs, _srofs_next_cluster_by_cluster(srofs, resource->first_cluster));
    }
    _srofs_increase_free_clusters(srofs, freed_clusters);
    resource->size = 0x00;
    _srofs_io_write(srofs->driver, RD_ADDRESS_TO_SIZE_LOW(resource_descriptor_address), 0x00);
    _srofs_io_write(srofs->driver, RD_ADDRESS_TO_SIZE_HIGH(resource_descriptor_address), 0x00);
    return SROFS_OP_RESULT_SUCCESS;
}

void srofs_sync(srofs_t *srofs, srofs_resource_t *resource) {
    uint8_t i;
    srofs_memory_address_t address;
    address = _srofs_resource_descriptor_to_address(srofs, resource->resource_descriptor);
    for (i = 0; i < 2; i++) {
        _srofs_io_write(srofs->driver, address + i, *((uint8_t *) (&(resource->size)) + i));
    }
}

void srofs_stat(srofs_t *srofs, srofs_resource_t *resource, srofs_stat_t *stat) { // TODO
    stat->flags = 0xff;
}

srofs_op_result_t srofs_rewind(srofs_t *srofs, srofs_resource_t *resource) {
    resource->current_cluster = resource->first_cluster;
    resource->cluster_offset = srofs->sizeof_cluster_control;
    resource->current_position = 0;
    _srofs_check_for_eor_reached(resource);
    return SROFS_OP_RESULT_SUCCESS;
}

srofs_resource_code_t srofs_alloc(srofs_t *srofs) {
    uint8_t i;
    uint8_t flags;
    srofs_cluster_t first_cluster;
    srofs_memory_address_t resource_descriptor_address, cluster_address;
    if (srofs->free_clusters < 1) {
        return SROFS_NULL_RESOURCE_CODE;
    }
    resource_descriptor_address = srofs->resource_descriptor_table_address;
    for (i = 0; i < srofs->resource_descriptor_count; i++) {
        flags = _srofs_io_read(srofs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address));
        if (!(flags & SROFS_RESOURCE_FLAG_BIT_ALLOCATED)) {
            cluster_address = _srofs_alloc_cluster(srofs);
            if (cluster_address == SROFS_NULL_CLUSTER_ADDRESS) {
                return SROFS_NULL_RESOURCE_CODE;
            }
            flags |= SROFS_RESOURCE_FLAG_BIT_ALLOCATED;
            first_cluster = _srofs_address_to_cluster(srofs, cluster_address);
            _srofs_create_cluster_chain(srofs, first_cluster, SROFS_INEXISTENT_CLUSTER);
            _srofs_io_write(srofs->driver, RD_ADDRESS_TO_FIRST_CLUSTER(resource_descriptor_address), first_cluster);
            _srofs_io_write(srofs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address), flags);
            return _srofs_resource_descriptor_to_resource_code(i);
        }
        resource_descriptor_address += srofs->sizeof_resource_descriptor;
    }
    return SROFS_NULL_RESOURCE_CODE;
}

uint8_t srofs_release(srofs_t *srofs, srofs_resource_t *resource) {
    uint8_t flags;
    srofs_memory_address_t resource_descriptor_address;
    resource_descriptor_address = _srofs_resource_descriptor_to_address(srofs, resource->resource_descriptor);
    flags = _srofs_io_read(srofs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address));
    if (!(flags & SROFS_RESOURCE_FLAG_BIT_ALLOCATED)) {
        return 1;
    }
    _srofs_format_resource_clusters(srofs, resource);
    _srofs_format_resorce_descriptor(srofs, resource->resource_descriptor);
    resource->flags = 0x00;
    return 1;
}

srofs_resource_size_t srofs_size(srofs_resource_t *resource) {
    return resource->size;
}

srofs_resource_size_t srofs_tell(srofs_resource_t *resource) {
    return resource->current_position;
}

uint8_t srofs_eor(srofs_resource_t *resource) {
    return _srofs_is_eor_reached(resource);
}

uint8_t srofs_error(srofs_resource_t *resource) {
    return (resource->flags & SROFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ || resource->flags & SROFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_WRITE);
}

srofs_resource_size_t srofs_available_space(srofs_t *srofs) {
    return srofs->free_clusters * srofs->sizeof_cluster_data;
}

srofs_resource_size_t srofs_total_space(srofs_t *srofs) {
    return srofs->cluster_count * srofs->sizeof_cluster_data;
}

#endif // __SROFS_C__
