#include "srbfs_spec_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void resource_dump(srbfs_resource_t *resource) {
    printf("======== resource dump begin ========\n");
    printf("  resource descriptor: %6d %s\n", resource->resource_descriptor, itob(resource->resource_descriptor));
    printf("  first cluster:______ %6d %s\n", resource->first_cluster, itob(resource->first_cluster));
    printf("  current cluster:____ %6d %s\n", resource->current_cluster, itob(resource->current_cluster));
    printf("  cluster offset:_____ %6d %s\n", resource->cluster_offset, itob(resource->cluster_offset));
    printf("  size:_______________ %6d %s\n", resource->size, itob(resource->size));
    printf("  current position:___ %6d %s\n", resource->current_position, itob(resource->current_position));
    printf("  flags:______________ %6d %s\n", resource->flags, itob(resource->flags));
    printf("  errors:_____________ %6d %s\n", srbfs_error(resource), itob(srbfs_error(resource)));
    printf("========= resource dump end =========\n");
}

void format_all() {
    uint16_t i;
    for (i = 0; i < 0x8000; i++) {
        _srbfs_io_write(SRBFS_DRIVER_VIRTUAL, i, 0x00);
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

void srbfs_io_memory_dump(srbfs_t *srbfs) {
    srbfs_memory_address_t memory_address;
    uint16_t count, count2;
    uint8_t data = 0;
    FILE *fp;
    if (!_srbfs_is_driver_monted(srbfs->driver)) {
        printf("srbfs not mounted yet\n");
        return;
    }
    fp = fopen("dump", "w+");
    fprintf(fp, "DRIVER: %x\n", srbfs->driver);
    fprintf(fp, "\n==========================\n");
    fprintf(fp, "\nsrbfs\n");
    fprintf(fp, "-----------------\n");
    fprintf(fp, "memory_size:                       0x%04x %4d %s\n", srbfs->memory_size, srbfs->memory_size, itob(srbfs->memory_size));
    fprintf(fp, "resource_descriptor_table_address: 0x%04x %4d %s\n", srbfs->resource_descriptor_table_address, srbfs->resource_descriptor_table_address, itob(srbfs->resource_descriptor_table_address));
    fprintf(fp, "cluster_table_address:             0x%04x %4d %s\n", srbfs->cluster_table_address, srbfs->cluster_table_address, itob(srbfs->cluster_table_address));
    fprintf(fp, "sizeof_resource_descriptor_table:  0x%04x %4d %s\n", srbfs->sizeof_resource_descriptor_table, srbfs->sizeof_resource_descriptor_table, itob(srbfs->sizeof_resource_descriptor_table));
    fprintf(fp, "sizeof_cluster_table:              0x%04x %4d %s\n", srbfs->sizeof_cluster_table, srbfs->sizeof_cluster_table, itob(srbfs->sizeof_resource_descriptor_table));
    fprintf(fp, "sizeof_resource_descriptor:        0x%04x %4d %s\n", srbfs->sizeof_resource_descriptor, srbfs->sizeof_resource_descriptor, itob(srbfs->sizeof_resource_descriptor));
    fprintf(fp, "sizeof_cluster:                    0x%04x %4d %s\n", srbfs->sizeof_cluster, srbfs->sizeof_cluster, itob(srbfs->sizeof_cluster));
    fprintf(fp, "resource_descriptor_count:         0x%04x %4d %s\n", srbfs->resource_descriptor_count, srbfs->resource_descriptor_count, itob(srbfs->resource_descriptor_count));
    fprintf(fp, "cluster_count:                     0x%04x %4d %s\n", srbfs->cluster_count, srbfs->cluster_count, itob(srbfs->cluster_count));
    fprintf(fp, "sizeof_cluster_data:               0x%04x %4d %s\n", srbfs->sizeof_cluster_data, srbfs->sizeof_cluster_data, itob(srbfs->sizeof_cluster_data));
    fprintf(fp, "sizeof_cluster_control:            0x%04x %4d %s\n", srbfs->sizeof_cluster_control, srbfs->sizeof_cluster_control, itob(srbfs->sizeof_cluster_control));
    fprintf(fp, "free_clusters:                     0x%04x %4d %s\n", srbfs->free_clusters, srbfs->free_clusters, itob(srbfs->free_clusters));
    fprintf(fp, "flags:                             0x%04x %4d %s\n", srbfs->flags, srbfs->flags, itob(srbfs->flags));
    fprintf(fp, "\n==========================\n");
    fprintf(fp, "\nResource table\n");
    fprintf(fp, "-----------------\n");
    count = 0;
    for (memory_address = srbfs->resource_descriptor_table_address; memory_address < (srbfs->resource_descriptor_table_address + srbfs->sizeof_resource_descriptor_table); memory_address++) {
        if ((count % srbfs->sizeof_resource_descriptor) == 0) {
            fprintf(fp, "\n%02x: ", (count) ? count / srbfs->sizeof_resource_descriptor : 0);
        }
        fprintf(fp, "%02x ", _srbfs_io_read(srbfs->driver, memory_address));
        count++;
    }
    fprintf(fp, "\n==========================\n");
    fprintf(fp, "\nCluster table\n");
    fprintf(fp, "-----------------\n");
    fprintf(fp, "\n    |nn |pp |");
    for (count = 0; count < srbfs->sizeof_cluster_data; count++) {
        fprintf(fp, "dd ");
    }
    fprintf(fp, "\n    ---------");
    for (count = 0; count < srbfs->sizeof_cluster_data; count++) {
        fprintf(fp, "---");
    }
    count = 0;

    for (memory_address = srbfs->cluster_table_address; memory_address < (srbfs->cluster_table_address + srbfs->sizeof_cluster_table); memory_address++) {
        if ((count % srbfs->sizeof_cluster) == 0) {
            fprintf(fp, "\n%02x: |", (count) ? count / srbfs->sizeof_cluster : 0);
            count2 = 0;
        }
        if (count2 == 1 || count2 == 2) {
            fprintf(fp, "|");
        }
        fprintf(fp, "%02x ", (data = _srbfs_io_read(srbfs->driver, memory_address)));
        fflush(fp);
        count++;
        count2++;
    }
    fclose(fp);
}
