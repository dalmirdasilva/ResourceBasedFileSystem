#define VIRTUAL_ENVIROMENT 1

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "srofs_io.c"
#include "srofs_util.c"
#include "srofs.c"
#include "srofs_init_partition.c"

void resource_dump(srofs_resource_t *resource);

#include "srofs_spec.h"

void format_all() {
    uint16_t i;
    for (i = 0; i < 0x7fff; i++) {
        _srofs_io_write(SROFS_DRIVER_VIRTUAL, i, 0x00);
    }
}

int main() {
    srofs_t srofs;

    format_all();
    srofs_init_partition(&srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    srofs_format(&srofs);
    format_spec(&srofs);
    mount_spec(&srofs);
    umount_spec(&srofs);
    alloc_resource_spec(&srofs);
    try_to_alloc_resources_that_is_possible_spec(&srofs);
    open_resource_spec(&srofs);
    write_resource_spec(&srofs);
    rewind_resource_spec(&srofs);
    read_resource_spec(&srofs);
    close_resource_spec(&srofs);
    try_read_when_end_of_resource_is_reached_spec(&srofs);
    try_read_when_resource_is_closed_spec(&srofs);
    seek_resource_spec(&srofs);
    random_read_resource_spec(&srofs);
    random_read_with_seek_resource_spec(&srofs);
    random_read_with_seek_opening_resource_spec(&srofs);
    size_resource_spec(&srofs);
    tell_resource_spec(&srofs);
    tell_with_seek_resource_spec(&srofs);
    total_space_resource_spec(&srofs);
    allocating_multi_format_spec(&srofs);
    read_only_mounting_spec(&srofs);
    read_only_opening_spec(&srofs);

    srofs_mount(SROFS_SPEC_DRIVER, &srofs, SROFS_MOUNT_OPTION_NORMAL);
    _srofs_io_memory_dump(&srofs);
    return 0;
}

void resource_dump(srofs_resource_t *resource) {
    printf("======== resource dump begin ========\n");
    printf("  resource descriptor: %6d %s\n", resource->resource_descriptor, itob(resource->resource_descriptor));
    printf("  first cluster:______ %6d %s\n", resource->first_cluster, itob(resource->first_cluster));
    printf("  current cluster:____ %6d %s\n", resource->current_cluster, itob(resource->current_cluster));
    printf("  cluster offset:_____ %6d %s\n", resource->cluster_offset, itob(resource->cluster_offset));
    printf("  size:_______________ %6d %s\n", resource->size, itob(resource->size));
    printf("  current position:___ %6d %s\n", resource->current_position, itob(resource->current_position));
    printf("  flags:______________ %6d %s\n", resource->flags, itob(resource->flags));
    printf("  errors:_____________ %6d %s\n", srofs_error(resource), itob(srofs_error(resource)));
    printf("========= resource dump end =========\n");
}
