#define SRBFS_SPEC_DRIVER SRBFS_DRIVER_VIRTUAL

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../srbfs_io.h"
#include "../srbfs_util.h"
#include "../srbfs.h"
#include "../srbfs_make_partition.h"

#include "srbfs_spec.h"
#include "srbfs_spec_helper.h"


FILE *srbfs_fp;

uint8_t _srbfs_io_read(srbfs_driver_t driver, srbfs_memory_address_t address);
void _srbfs_io_write(srbfs_driver_t driver, srbfs_memory_address_t address, uint8_t data);

void init_srbfs_io();
void finish_srbfs_io();

int main() {
    srbfs_t srbfs;
    init_srbfs_io();
    srbfs_make_partition(&srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL, SRBFS_DRIVER_VIRTUAL);
    format_all();
    srbfs_format(&srbfs);
    format_spec(&srbfs);
    mount_spec(&srbfs);
    umount_spec(&srbfs);
    alloc_resource_spec(&srbfs);
    try_to_alloc_resources_that_is_possible_spec(&srbfs);
    open_resource_spec(&srbfs);
    write_resource_spec(&srbfs);
    rewind_resource_spec(&srbfs);
    read_resource_spec(&srbfs);
    close_resource_spec(&srbfs);
    try_read_when_end_of_resource_is_reached_spec(&srbfs);
    try_read_when_resource_is_closed_spec(&srbfs);
    seek_resource_spec(&srbfs);
    random_read_resource_spec(&srbfs);
    random_read_with_seek_resource_spec(&srbfs);
    random_read_with_seek_opening_resource_spec(&srbfs);
    size_resource_spec(&srbfs);
    tell_resource_spec(&srbfs);
    tell_with_seek_resource_spec(&srbfs);
    total_space_resource_spec(&srbfs);
    allocating_multi_format_spec(&srbfs);
    read_only_mounting_spec(&srbfs);
    read_only_opening_spec(&srbfs);

    srbfs_mount(SRBFS_DRIVER_VIRTUAL, &srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_io_memory_dump(&srbfs);
    finish_srbfs_io();
    return 0;
}

void init_srbfs_io() {
    if ((srbfs_fp = fopen("img.hd", "rb+")) == NULL) {
        printf("Error reading img.hd");
        exit(1);
    }
}

void finish_srbfs_io() {
    fclose(srbfs_fp);
}

uint8_t _srbfs_io_read(srbfs_driver_t driver, srbfs_memory_address_t address) {
    unsigned char data;
    fseek(srbfs_fp, address, 0);
    fread(&data, sizeof(data), 1, srbfs_fp);
    return data;
}

void _srbfs_io_write(srbfs_driver_t driver, srbfs_memory_address_t address, uint8_t data) {
    fseek(srbfs_fp, address, 0);
    fwrite(&data, sizeof(data), 1, srbfs_fp);
    fflush(srbfs_fp);
}
