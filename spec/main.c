#define VIRTUAL_ENVIROMENT 1

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../srofs_io.h"
#include "../srofs_util.h"
#include "../srofs.h"
#include "../srofs_init_partition.h"

void resource_dump(srofs_resource_t *resource);

#include "srofs_spec.h"

void format_all() {
    uint16_t i;
    for (i = 0; i < 0x7fff; i++) {
        _srofs_io_write(SROFS_DRIVER_VIRTUAL, i, 0x00);
    }
}

char* itob(int i) {
    int bits;
    int j, k;
    uint16_t mi = 0;
    mi |= i;
    static char buff[sizeof(mi) * 8 + 1];
    bits = sizeof(mi) * 8;
    for (j = bits - 1, k = 0; j >= 0; j--, k++) {
        buff[k] = ((mi >> j) & 0x01) + '0';
    }
    buff[bits] = '\0';
    return buff;
}

void _srofs_io_memory_dump(srofs_t *srofs) {
    srofs_memory_address_t memory_address;
    uint16_t count, count2;
    uint8_t data = 0;
    FILE *fp;
    if (!_srofs_is_driver_monted(srofs->driver)) {
        printf("srofs not mounted yet\n");
        return;
    }
    fp = fopen("dump", "w+");
    fprintf(fp, "DRIVER: %x\n", srofs->driver);
    fprintf(fp, "\n==========================\n");
    fprintf(fp, "\nsrofs\n");
    fprintf(fp, "-----------------\n");
    fprintf(fp, "memory_size:                       0x%04x %4d %s\n", srofs->memory_size, srofs->memory_size, itob(srofs->memory_size));
    fprintf(fp, "resource_descriptor_table_address: 0x%04x %4d %s\n", srofs->resource_descriptor_table_address, srofs->resource_descriptor_table_address, itob(srofs->resource_descriptor_table_address));
    fprintf(fp, "cluster_table_address:             0x%04x %4d %s\n", srofs->cluster_table_address, srofs->cluster_table_address, itob(srofs->cluster_table_address));
    fprintf(fp, "sizeof_resource_descriptor_table:  0x%04x %4d %s\n", srofs->sizeof_resource_descriptor_table, srofs->sizeof_resource_descriptor_table, itob(srofs->sizeof_resource_descriptor_table));
    fprintf(fp, "sizeof_cluster_table:              0x%04x %4d %s\n", srofs->sizeof_cluster_table, srofs->sizeof_cluster_table, itob(srofs->sizeof_resource_descriptor_table));
    fprintf(fp, "sizeof_resource_descriptor:        0x%04x %4d %s\n", srofs->sizeof_resource_descriptor, srofs->sizeof_resource_descriptor, itob(srofs->sizeof_resource_descriptor));
    fprintf(fp, "sizeof_cluster:                    0x%04x %4d %s\n", srofs->sizeof_cluster, srofs->sizeof_cluster, itob(srofs->sizeof_cluster));
    fprintf(fp, "resource_descriptor_count:         0x%04x %4d %s\n", srofs->resource_descriptor_count, srofs->resource_descriptor_count, itob(srofs->resource_descriptor_count));
    fprintf(fp, "cluster_count:                     0x%04x %4d %s\n", srofs->cluster_count, srofs->cluster_count, itob(srofs->cluster_count));
    fprintf(fp, "sizeof_cluster_data:               0x%04x %4d %s\n", srofs->sizeof_cluster_data, srofs->sizeof_cluster_data, itob(srofs->sizeof_cluster_data));
    fprintf(fp, "sizeof_cluster_control:            0x%04x %4d %s\n", srofs->sizeof_cluster_control, srofs->sizeof_cluster_control, itob(srofs->sizeof_cluster_control));
    fprintf(fp, "free_clusters:                     0x%04x %4d %s\n", srofs->free_clusters, srofs->free_clusters, itob(srofs->free_clusters));
    fprintf(fp, "flags:                             0x%04x %4d %s\n", srofs->flags, srofs->flags, itob(srofs->flags));
    fprintf(fp, "\n==========================\n");
    fprintf(fp, "\nResource table\n");
    fprintf(fp, "-----------------\n");
    count = 0;
    for (memory_address = srofs->resource_descriptor_table_address; memory_address < (srofs->resource_descriptor_table_address + srofs->sizeof_resource_descriptor_table); memory_address++) {
        if ((count % srofs->sizeof_resource_descriptor) == 0) {
            fprintf(fp, "\n%02x: ", (count) ? count / srofs->sizeof_resource_descriptor : 0);
        }
        fprintf(fp, "%02x ", _srofs_io_read(srofs->driver, memory_address));
        count++;
    }
    fprintf(fp, "\n==========================\n");
    fprintf(fp, "\nCluster table\n");
    fprintf(fp, "-----------------\n");
    fprintf(fp, "\n    |nn |pp |");
    for (count = 0; count < srofs->sizeof_cluster_data; count++) {
        fprintf(fp, "dd ");
    }
    fprintf(fp, "\n    ---------");
    for (count = 0; count < srofs->sizeof_cluster_data; count++) {
        fprintf(fp, "---");
    }
    count = 0;

    for (memory_address = srofs->cluster_table_address; memory_address < (srofs->cluster_table_address + srofs->sizeof_cluster_table); memory_address++) {
        if ((count % srofs->sizeof_cluster) == 0) {
            fprintf(fp, "\n%02x: |", (count) ? count / srofs->sizeof_cluster : 0);
            count2 = 0;
        }
        if (count2 == 1 || count2 == 2) {
            fprintf(fp, "|");
        }
        fprintf(fp, "%02x ", (data = _srofs_io_read(srofs->driver, memory_address)));
        fflush(fp);
        count++;
        count2++;
    }
    fclose(fp);
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

    srofs_mount(SROFS_DRIVER_VIRTUAL, &srofs, SROFS_MOUNT_OPTION_NORMAL);
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
