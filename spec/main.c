#define RBFS_SPEC_DRIVER RBFS_DRIVER_VIRTUAL

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../rbfs_io.h"
#include "../rbfs_util.h"
#include "../rbfs.h"
#include "../rbfs_make_partition.h"

#include "rbfs_spec.h"
#include "rbfs_spec_helper.h"


FILE *rbfs_fp;

uint8_t _rbfs_io_read(rbfs_driver_t driver, rbfs_memory_address_t address);
void _rbfs_io_write(rbfs_driver_t driver, rbfs_memory_address_t address, uint8_t data);

void init_rbfs_io();
void finish_rbfs_io();

int main() {
    rbfs_t rbfs;
    init_rbfs_io();
    rbfs_make_partition(&rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    format_all();
    rbfs_format(&rbfs);
    format_spec(&rbfs);
    mount_spec(&rbfs);
    umount_spec(&rbfs);
    alloc_resource_spec(&rbfs);
    try_to_alloc_resources_that_is_possible_spec(&rbfs);
    open_resource_spec(&rbfs);
    write_resource_spec(&rbfs);
    rewind_resource_spec(&rbfs);
    read_resource_spec(&rbfs);
    close_resource_spec(&rbfs);
    try_read_when_end_of_resource_is_reached_spec(&rbfs);
    try_read_when_resource_is_closed_spec(&rbfs);
    seek_resource_spec(&rbfs);
    random_read_resource_spec(&rbfs);
    random_read_with_seek_resource_spec(&rbfs);
    random_read_with_seek_opening_resource_spec(&rbfs);
    size_resource_spec(&rbfs);
    tell_resource_spec(&rbfs);
    tell_with_seek_resource_spec(&rbfs);
    total_space_resource_spec(&rbfs);
    allocating_multi_format_spec(&rbfs);
    read_only_mounting_spec(&rbfs);
    read_only_opening_spec(&rbfs);

    rbfs_mount(RBFS_DRIVER_VIRTUAL, &rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_io_memory_dump(&rbfs);
    finish_rbfs_io();
    return 0;
}

void init_rbfs_io() {
    if ((rbfs_fp = fopen("img.hd", "rb+")) == NULL) {
        printf("Error reading img.hd");
        exit(1);
    }
}

void finish_rbfs_io() {
    fclose(rbfs_fp);
}

uint8_t _rbfs_io_read(rbfs_driver_t driver, rbfs_memory_address_t address) {
    unsigned char data;
    fseek(rbfs_fp, address, 0);
    fread(&data, sizeof(data), 1, rbfs_fp);
    return data;
}

void _rbfs_io_write(rbfs_driver_t driver, rbfs_memory_address_t address, uint8_t data) {
    fseek(rbfs_fp, address, 0);
    fwrite(&data, sizeof(data), 1, rbfs_fp);
    fflush(rbfs_fp);
}
