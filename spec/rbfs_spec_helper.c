#include "rbfs_spec_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void resource_dump(rbfs_resource_t *resource) {
    printf("======== resource dump begin ========\n");
    printf("  resource descriptor: %6d %s\n", resource->resource_descriptor, itob(resource->resource_descriptor));
    printf("  first cluster:______ %6d %s\n", resource->first_cluster, itob(resource->first_cluster));
    printf("  current cluster:____ %6d %s\n", resource->current_cluster, itob(resource->current_cluster));
    printf("  cluster offset:_____ %6d %s\n", resource->cluster_offset, itob(resource->cluster_offset));
    printf("  size:_______________ %6d %s\n", resource->size, itob(resource->size));
    printf("  current position:___ %6d %s\n", resource->current_position, itob(resource->current_position));
    printf("  flags:______________ %6d %s\n", resource->flags, itob(resource->flags));
    printf("  errors:_____________ %6d %s\n", rbfs_error(resource), itob(rbfs_error(resource)));
    printf("========= resource dump end =========\n");
}

void format_all() {
    uint16_t i;
    for (i = 0; i < 0x8000; i++) {
        _rbfs_io_write(RBFS_DRIVER_VIRTUAL, i, 0x00);
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

void rbfs_io_memory_dump(rbfs_t *rbfs) {
    rbfs_memory_address_t memory_address;
    uint16_t count, count2;
    uint8_t data = 0;
    FILE *fp;
    if (!_rbfs_is_driver_monted(rbfs->driver)) {
        printf("rbfs not mounted yet\n");
        return;
    }
    fp = fopen("dump", "w+");
    fprintf(fp, "DRIVER: %x\n", rbfs->driver);
    fprintf(fp, "\n==========================\n");
    fprintf(fp, "\nrbfs\n");
    fprintf(fp, "-----------------\n");
    fprintf(fp, "memory_size:                       0x%04x %4d %s\n", rbfs->memory_size, rbfs->memory_size, itob(rbfs->memory_size));
    fprintf(fp, "resource_descriptor_table_address: 0x%04x %4d %s\n", rbfs->resource_descriptor_table_address, rbfs->resource_descriptor_table_address, itob(rbfs->resource_descriptor_table_address));
    fprintf(fp, "cluster_table_address:             0x%04x %4d %s\n", rbfs->cluster_table_address, rbfs->cluster_table_address, itob(rbfs->cluster_table_address));
    fprintf(fp, "sizeof_resource_descriptor_table:  0x%04x %4d %s\n", rbfs->sizeof_resource_descriptor_table, rbfs->sizeof_resource_descriptor_table, itob(rbfs->sizeof_resource_descriptor_table));
    fprintf(fp, "sizeof_cluster_table:              0x%04x %4d %s\n", rbfs->sizeof_cluster_table, rbfs->sizeof_cluster_table, itob(rbfs->sizeof_resource_descriptor_table));
    fprintf(fp, "sizeof_resource_descriptor:        0x%04x %4d %s\n", rbfs->sizeof_resource_descriptor, rbfs->sizeof_resource_descriptor, itob(rbfs->sizeof_resource_descriptor));
    fprintf(fp, "sizeof_cluster:                    0x%04x %4d %s\n", rbfs->sizeof_cluster, rbfs->sizeof_cluster, itob(rbfs->sizeof_cluster));
    fprintf(fp, "resource_descriptor_count:         0x%04x %4d %s\n", rbfs->resource_descriptor_count, rbfs->resource_descriptor_count, itob(rbfs->resource_descriptor_count));
    fprintf(fp, "cluster_count:                     0x%04x %4d %s\n", rbfs->cluster_count, rbfs->cluster_count, itob(rbfs->cluster_count));
    fprintf(fp, "sizeof_cluster_data:               0x%04x %4d %s\n", rbfs->sizeof_cluster_data, rbfs->sizeof_cluster_data, itob(rbfs->sizeof_cluster_data));
    fprintf(fp, "sizeof_cluster_control:            0x%04x %4d %s\n", rbfs->sizeof_cluster_control, rbfs->sizeof_cluster_control, itob(rbfs->sizeof_cluster_control));
    fprintf(fp, "free_clusters:                     0x%04x %4d %s\n", rbfs->free_clusters, rbfs->free_clusters, itob(rbfs->free_clusters));
    fprintf(fp, "flags:                             0x%04x %4d %s\n", rbfs->flags, rbfs->flags, itob(rbfs->flags));
    fprintf(fp, "\n==========================\n");
    fprintf(fp, "\nResource table\n");
    fprintf(fp, "-----------------\n");
    count = 0;
    for (memory_address = rbfs->resource_descriptor_table_address; memory_address < (rbfs->resource_descriptor_table_address + rbfs->sizeof_resource_descriptor_table); memory_address++) {
        if ((count % rbfs->sizeof_resource_descriptor) == 0) {
            fprintf(fp, "\n%02x: ", (count) ? count / rbfs->sizeof_resource_descriptor : 0);
        }
        fprintf(fp, "%02x ", _rbfs_io_read(rbfs->driver, memory_address));
        count++;
    }
    fprintf(fp, "\n==========================\n");
    fprintf(fp, "\nCluster table\n");
    fprintf(fp, "-----------------\n");
    fprintf(fp, "\n    |nn |pp |");
    for (count = 0; count < rbfs->sizeof_cluster_data; count++) {
        fprintf(fp, "dd ");
    }
    fprintf(fp, "\n    ---------");
    for (count = 0; count < rbfs->sizeof_cluster_data; count++) {
        fprintf(fp, "---");
    }
    count = 0;

    for (memory_address = rbfs->cluster_table_address; memory_address < (rbfs->cluster_table_address + rbfs->sizeof_cluster_table); memory_address++) {
        if ((count % rbfs->sizeof_cluster) == 0) {
            fprintf(fp, "\n%02x: |", (count) ? count / rbfs->sizeof_cluster : 0);
            count2 = 0;
        }
        if (count2 == 1 || count2 == 2) {
            fprintf(fp, "|");
        }
        fprintf(fp, "%02x ", (data = _rbfs_io_read(rbfs->driver, memory_address)));
        fflush(fp);
        count++;
        count2++;
    }
    fclose(fp);
}
