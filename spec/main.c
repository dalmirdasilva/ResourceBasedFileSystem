#define SRBFS_SPEC_DRIVER SRBFS_DRIVER_VIRTUAL

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../srbfs_io.h"
#include "../srbfs_util.h"
#include "../srbfs.h"
#include "../srbfs_init_partition.h"

#include "srbfs_spec.h"

FILE *srbfs_fp;
uint8_t (*extern_srbfs_read)(uint8_t driver, uint16_t address);
void (*extern_srbfs_write)(uint8_t driver, uint16_t address, uint8_t data_to_write);

uint8_t srbfs_read_imp(uint8_t driver, uint16_t address);
void srbfs_write_imp(uint8_t driver, uint16_t address, uint8_t data_to_write);

void init_srbfs_io();
void finish_srbfs_io();

int main() {

    init_srbfs_io();

    srbfs_t srbfs;

    printf("%ld", sizeof(srbfs_t));

    format_all();
    srbfs_init_partition(&srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
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
    if ((srbfs_fp = fopen("img.hd", "rb")) == NULL) {
        printf("Error reading img.hd");
        exit(1);
    }
    extern_srbfs_read = srbfs_read_imp;
    extern_srbfs_write = srbfs_write_imp;
}

void finish_srbfs_io() {
    fclose(srbfs_fp);
}

uint8_t srbfs_read_imp(uint8_t driver, uint16_t address) {
    unsigned char read_data;
    fseek(srbfs_fp, address, 0);
    fread(&read_data, sizeof(read_data), 1, srbfs_fp);
    return read_data;
}

void srbfs_write_imp(uint8_t driver, uint16_t address, uint8_t data_to_write) {
    fseek(srbfs_fp, address, 0);
    fwrite(&data_to_write, sizeof(data_to_write), 1, srbfs_fp);
    fflush(srbfs_fp);
}
