#include <stdio.h>
#include "srbfs_spec.h"
#include "../srbfs.h"
#include "../srbfs_init_partition.h"

#define srbfs_spec_printf printf

void format_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    if (op_r != SRBFS_OP_RESULT_SUCCESS) {
        srbfs_spec_printf("(F) format spec failed. error: %d\n", op_r);
    } else {
        srbfs_spec_printf("(*) format spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    }
}

void mount_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    if (op_r != SRBFS_OP_RESULT_SUCCESS) {
        srbfs_spec_printf("(F) mount spec failed. error: %d\n", op_r);
    } else {
        srbfs_spec_printf("(*) mount spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    }
    srbfs_umount(srbfs);
}

void umount_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    op_r = srbfs_umount(srbfs);
    if (op_r != SRBFS_OP_RESULT_SUCCESS) {
        srbfs_spec_printf("(F) unmount spec failed. error: %d\n", op_r);
    } else {
        srbfs_spec_printf("(*) unmount spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    }
}

void alloc_resource_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    if (srbfs_resource_code == SRBFS_NULL_RESOURCE_CODE) {
        srbfs_spec_printf("(F) alloc_resource spec failed. error: %d\n", op_r);
    } else {
        srbfs_spec_printf("(*) alloc_resource spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    }
    srbfs_umount(srbfs);
}

void try_to_alloc_resources_that_is_possible_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code[2];
    uint8_t i = 0;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    for (; i < srbfs->resource_descriptor_count; i++) {
        srbfs_resource_code[0] = srbfs_alloc(srbfs);
    }
    srbfs_resource_code[1] = srbfs_alloc(srbfs);
    if (srbfs_resource_code[0] == (srbfs->resource_descriptor_count - 1) && srbfs_resource_code[1] == SRBFS_NULL_RESOURCE_CODE) {
        srbfs_spec_printf("(*) try_to_alloc_resources_that_is_possible spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    } else {
        srbfs_spec_printf("(F) try_to_alloc_resources_that_is_possible spec failed. error: %x\n", srbfs_resource_code[0]);
    }
    srbfs_umount(srbfs);
}

void open_resource_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    if (op_r != SRBFS_OP_RESULT_SUCCESS) {
        srbfs_spec_printf("(F) open_resource spec failed. error: %d\n", op_r);
    } else {
        srbfs_spec_printf("(*) open_resource spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    }
    srbfs_umount(srbfs);
}

void write_resource_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = srbfs_write(srbfs, &resource, 0xaa);
    if (op_r != SRBFS_OP_RESULT_SUCCESS) {
        srbfs_spec_printf("(F) write_resource spec failed. error: %d\n", op_r);
    } else {
        srbfs_spec_printf("(*) write_resource spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    }
    srbfs_umount(srbfs);
}

void rewind_resource_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = srbfs_write(srbfs, &resource, 0xAA);
    op_r = srbfs_rewind(srbfs, &resource);
    if (op_r != SRBFS_OP_RESULT_SUCCESS) {
        srbfs_spec_printf("(F) rewind_resource spec failed. error: %d\n", op_r);
    } else {
        srbfs_spec_printf("(*) rewind_resource spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    }
    srbfs_umount(srbfs);
}

void read_resource_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    unsigned char c[2];
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = srbfs_write(srbfs, &resource, 0x41);
    op_r = srbfs_write(srbfs, &resource, 0xA1);
    op_r = srbfs_rewind(srbfs, &resource);
    c[0] = srbfs_read(srbfs, &resource);
    c[1] = srbfs_read(srbfs, &resource);
    if (c[0] != 0x41 || c[1] != 0xA1) {
        srbfs_spec_printf("(F) read_resource spec failed. error: %x\n", c[0]);
    } else {
        srbfs_spec_printf("(*) read_resource spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    }
    srbfs_umount(srbfs);
}

void close_resource_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    unsigned char c[2];
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = srbfs_close(srbfs, &resource);
    if (op_r != SRBFS_OP_RESULT_SUCCESS) {
        srbfs_spec_printf("(F) close_resource spec failed. error: %x\n", op_r);
    } else {
        srbfs_spec_printf("(*) close_resource spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    }
    srbfs_umount(srbfs);
}

void try_read_when_end_of_resource_is_reached_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = srbfs_write(srbfs, &resource, 0x41);
    op_r = srbfs_rewind(srbfs, &resource);
    srbfs_read(srbfs, &resource);
    srbfs_read(srbfs, &resource);
    if (op_r == 0 && (srbfs_eor(&resource))) {
        srbfs_spec_printf("(*) try_read_when_end_of_resource_is_reached spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    } else {
        srbfs_spec_printf("(F) try_read_when_end_of_resource_is_reached spec failed. error: %x\n", op_r);
    }
    srbfs_umount(srbfs);
}

void try_read_when_resource_is_closed_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = srbfs_close(srbfs, &resource);
    srbfs_read(srbfs, &resource);
    if (op_r == 0 && (resource.flags | SRBFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ)) {
        srbfs_spec_printf("(*) try_read_when_resource_is_closed spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    } else {
        srbfs_spec_printf("(F) try_read_when_resource_is_closed spec failed. error: %x\n", op_r);
    }
    srbfs_umount(srbfs);
}

void seek_resource_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    uint8_t i = 0;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 50; i++) {
        op_r = srbfs_write(srbfs, &resource, (i + 0x65));
    }
    op_r = srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_BEGIN, 20);
    if (op_r == SRBFS_OP_RESULT_SUCCESS) {
        srbfs_spec_printf("(*) seek_resource spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    } else {
        srbfs_spec_printf("(F) seek_resource spec failed. error: %x\n", op_r);
    }
    srbfs_umount(srbfs);
}

void random_read_resource_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    uint8_t i = 0;
    unsigned char c[5], first_write_char = 0x65;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    printf("op_r: %d\n", op_r);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    printf("op_r: %d\n", op_r);

    srbfs_resource_code = srbfs_alloc(srbfs);
    printf("srbfs_resource_code: %d\n", srbfs_resource_code);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    printf("op_r: %d\n", op_r);
    for (; i < 255; i++) {
        op_r = srbfs_write(srbfs, &resource, (i + first_write_char));
    }
    op_r = srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_BEGIN, 0x20);
    c[0] = srbfs_read(srbfs, &resource);
    op_r = srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_CURRENT, 0x10);
    c[1] = srbfs_read(srbfs, &resource);
    op_r = srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_BEGIN, 0x48);
    c[2] = srbfs_read(srbfs, &resource);
    op_r = srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_CURRENT, 0x20);
    c[3] = srbfs_read(srbfs, &resource);
    op_r = srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_BEGIN, 0x0);
    c[4] = srbfs_read(srbfs, &resource);
    for (i = 0; i < 5; i++) {
        printf("c[%d]: %d\n", i, c[i]);
    }

    if (c[0] == first_write_char + 0x20 && c[1] == first_write_char + 0x31 && // TODO: should break
            c[2] == first_write_char + 0x48 && c[3] == first_write_char + 0x69 && c[4] == first_write_char + 0x0) {
        srbfs_spec_printf("(*) random_read_resource spec passed, SRBFS_OP_RESULT_SUCCESS: %d.\n", SRBFS_OP_RESULT_SUCCESS);
    } else {
        srbfs_spec_printf("(F) random_read_resource spec failed. error: %x\n", op_r);
    }
    srbfs_umount(srbfs);
}

void random_read_with_seek_resource_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    uint8_t i = 0;
    unsigned char c[255];
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (i = 0; i < 255; i++) {
        op_r = srbfs_write(srbfs, &resource, i);
    }
    srbfs_rewind(srbfs, &resource);
    for (i = 0; i < 255; i++) {
        srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_BEGIN, i);
        c[i] = srbfs_read(srbfs, &resource);
        srbfs_read(srbfs, &resource);
        srbfs_read(srbfs, &resource);
        srbfs_read(srbfs, &resource);
    }
    op_r = srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_BEGIN, 199);
    if ((i = srbfs_read(srbfs, &resource)) != 199) {
        srbfs_spec_printf("(F) random_read_with_seek_resource_spec spec failed. %d != 199\n", 0);
    }

    for (i = 0; i < 255; i++) {
        if (i != c[i]) {
            srbfs_spec_printf("(F) random_read_with_seek_resource_spec spec failed. error: %x\n", i);
        }
    }

    srbfs_spec_printf("(*) random_read_with_seek_resource_spec spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    srbfs_umount(srbfs);
}

void random_read_with_seek_opening_resource_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    uint8_t i = 0;
    unsigned char c[255];
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (i = 0; i < 255; i++) {
        op_r = srbfs_write(srbfs, &resource, i);
    }
    srbfs_close(srbfs, &resource);

    for (i = 0; i < 255; i++) {
        op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
        srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_BEGIN, i);
        c[i] = srbfs_read(srbfs, &resource);
        srbfs_read(srbfs, &resource);
        srbfs_read(srbfs, &resource);
        srbfs_read(srbfs, &resource);
        srbfs_close(srbfs, &resource);
    }
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_BEGIN, 199);
    if ((i = srbfs_read(srbfs, &resource)) != 199) {
        srbfs_spec_printf("(F) random_read_with_seek_opening_resource_spec spec failed. %d != 199\n", op_r);
    }
    srbfs_close(srbfs, &resource);
    for (i = 0; i < 255; i++) {
        if (i != c[i]) {
            srbfs_spec_printf("(F) random_read_with_seek_opening_resource_spec spec failed. error: %x\n", c[i]);
        }
    }
    srbfs_spec_printf("(*) random_read_with_seek_opening_resource_spec spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    srbfs_umount(srbfs);
}

void size_resource_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    uint16_t i = 0;
    uint16_t size = 0xf40;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < size; i++) {
        op_r = srbfs_write(srbfs, &resource, 0x65);
    }
    if (srbfs_size(&resource) == 0xf40) {
        srbfs_spec_printf("(*) size_resource spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    } else {
        srbfs_spec_printf("(F) size_resource spec failed. error: %x\n", size);
    }
    srbfs_umount(srbfs);
}

void tell_resource_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    uint8_t i = 0;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 50; i++) {
        op_r = srbfs_write(srbfs, &resource, 0x65);
    }
    if (srbfs_tell(&resource) == 50) {
        srbfs_spec_printf("(*) tell_resource spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    } else {
        srbfs_spec_printf("(F) tell_resource spec failed. error: %x\n", op_r);
    }
    srbfs_umount(srbfs);
}

void tell_with_seek_resource_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    uint8_t i = 0;
    srbfs_resource_size_t s[5];
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 50; i++) {
        op_r = srbfs_write(srbfs, &resource, 0x65);
    }
    op_r = srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_BEGIN, 20);
    s[0] = srbfs_tell(&resource);
    op_r = srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_CURRENT, 10);
    s[1] = srbfs_tell(&resource);
    op_r = srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_BEGIN, 48);
    s[2] = srbfs_tell(&resource);
    op_r = srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_CURRENT, 20);
    s[3] = srbfs_tell(&resource);
    op_r = srbfs_seek(srbfs, &resource, SRBFS_SEEK_ORIGIN_BEGIN, 0);
    s[4] = srbfs_tell(&resource);
    if (s[0] == 20 && s[1] == 30 && s[2] == 48 && s[3] == 17 && s[4] == 0) {
        srbfs_spec_printf("(*) tell_with_seek_resource spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    } else {
        srbfs_spec_printf("(F) tell_with_seek_resource spec failed. error: %d\n", s[3]);
    }
    srbfs_umount(srbfs);
}

void total_space_resource_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_code_t srbfs_resource_code;
    srbfs_resource_t resource;
    srbfs_resource_size_t total_space[2];
    uint16_t i = 0;
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    total_space[0] = srbfs_available_space(srbfs);
    srbfs_resource_code = srbfs_alloc(srbfs);
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < srbfs->sizeof_cluster_data + 1; i++) {
        op_r = srbfs_write(srbfs, &resource, 0x65);
    }
    total_space[1] = srbfs_available_space(srbfs);
    if (total_space[0] - total_space[1] == (srbfs->sizeof_cluster_data * 2)) {
        srbfs_spec_printf("(*) total_space_resource spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    } else {
        srbfs_spec_printf("(F) total_space_resource spec failed. error: %d != 50\n", total_space[0] - total_space[1]);
    }
    srbfs_umount(srbfs);
}

void allocating_multi_format_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_t resource;
    uint8_t count = 3;
    uint8_t j, i;
    srbfs_resource_code_t srbfs_resource_code[3];
    uint8_t passed = 1;
    for (j = 0; j < count; j++) {
        srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
        op_r = srbfs_format(srbfs);
        op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
        srbfs_resource_code[j] = srbfs_alloc(srbfs);
        op_r = srbfs_open(srbfs, srbfs_resource_code[j], &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
        for (i = 0; i < 50; i++) {
            op_r = srbfs_write(srbfs, &resource, 0x65);
        }
        srbfs_close(srbfs, &resource);
    }
    for (j = 0; j < count; j++) {
        if (srbfs_resource_code[j] != 0) {
            srbfs_spec_printf("(F) allocating_multi_format spec failed %x\n", srbfs_resource_code[j]);
            passed = 0;
        }
    }
    if (passed) {
        srbfs_spec_printf("(*) allocating_multi_format spec passed %d\n", SRBFS_OP_RESULT_SUCCESS);
    }
    srbfs_umount(srbfs);
}

void read_only_mounting_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_t resource;
    srbfs_resource_code_t srbfs_resource_code;
    char error_msg[] = "(F) read_only_mounting spec failed. %d\n";
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_READ_ONLY);
    srbfs_resource_code = srbfs_alloc(srbfs);
    if (srbfs_resource_code == SRBFS_NULL_RESOURCE_CODE) {
        srbfs_spec_printf(error_msg, op_r);
    }
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_NORMAL);
    if (op_r != SRBFS_OP_RESULT_SUCCESS) {
        srbfs_spec_printf(error_msg, op_r);
    }
    op_r = srbfs_write(srbfs, &resource, 0xaa);
    if (op_r == SRBFS_OP_RESULT_ERROR_RESOURCE_READ_ONLY) {
        srbfs_spec_printf("(*) read_only_mounting spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    } else {
        srbfs_spec_printf(error_msg, op_r);
    }
    srbfs_umount(srbfs);
}

void read_only_opening_spec(srbfs_t *srbfs) {
    srbfs_op_result_t op_r;
    srbfs_resource_t resource;
    srbfs_resource_code_t srbfs_resource_code;
    char error_msg[] = "(F) read_only_opening spec failed. %d\n";
    srbfs_init_partition(srbfs, SRBFS_DISK_32K, SRBFS_ENV_VIRTUAL);
    op_r = srbfs_format(srbfs);
    op_r = srbfs_mount(SRBFS_DRIVER_VIRTUAL, srbfs, SRBFS_MOUNT_OPTION_NORMAL);
    srbfs_resource_code = srbfs_alloc(srbfs);
    if (srbfs_resource_code == SRBFS_NULL_RESOURCE_CODE) {
        srbfs_spec_printf(error_msg, op_r);
    }
    op_r = srbfs_open(srbfs, srbfs_resource_code, &resource, SRBFS_OPEN_RESOURCE_OPTION_READ_ONLY);
    if (op_r != SRBFS_OP_RESULT_SUCCESS) {
        srbfs_spec_printf(error_msg, op_r);
    }
    op_r = srbfs_write(srbfs, &resource, 0xaa);
    if (op_r == SRBFS_OP_RESULT_ERROR_RESOURCE_READ_ONLY) {
        srbfs_spec_printf("(*) read_only_opening spec passed %d.\n", SRBFS_OP_RESULT_SUCCESS);
    } else {
        srbfs_spec_printf(error_msg, op_r);
    }
    srbfs_umount(srbfs);
}
