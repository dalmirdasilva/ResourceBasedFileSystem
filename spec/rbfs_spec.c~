#include <stdio.h>
#include "rbfs_spec.h"
#include "../rbfs.h"
#include "../rbfs_make_partition.h"

void format_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    if (op_r != RBFS_OP_RESULT_SUCCESS) {
        RBFS_SPEC_PRINTF("(F) format spec failed. error: %d\n", op_r);
    } else {
        RBFS_SPEC_PRINTF("(*) format spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    }
}

void mount_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    if (op_r != RBFS_OP_RESULT_SUCCESS) {
        RBFS_SPEC_PRINTF("(F) mount spec failed. error: %d\n", op_r);
    } else {
        RBFS_SPEC_PRINTF("(*) mount spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    }
    rbfs_umount(rbfs);
}

void umount_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    op_r = rbfs_umount(rbfs);
    if (op_r != RBFS_OP_RESULT_SUCCESS) {
        RBFS_SPEC_PRINTF("(F) unmount spec failed. error: %d\n", op_r);
    } else {
        RBFS_SPEC_PRINTF("(*) unmount spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    }
}

void alloc_resource_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    if (rbfs_resource_code == RBFS_NULL_RESOURCE_CODE) {
        RBFS_SPEC_PRINTF("(F) alloc_resource spec failed. error: %d\n", op_r);
    } else {
        RBFS_SPEC_PRINTF("(*) alloc_resource spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    }
    rbfs_umount(rbfs);
}

void try_to_alloc_resources_that_is_possible_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code[2];
    uint8_t i = 0;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    for (; i < rbfs->resource_descriptor_count; i++) {
        rbfs_resource_code[0] = rbfs_alloc(rbfs);
    }
    rbfs_resource_code[1] = rbfs_alloc(rbfs);
    if (rbfs_resource_code[0] == (rbfs->resource_descriptor_count - 1) && rbfs_resource_code[1] == RBFS_NULL_RESOURCE_CODE) {
        RBFS_SPEC_PRINTF("(*) try_to_alloc_resources_that_is_possible spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    } else {
        RBFS_SPEC_PRINTF("(F) try_to_alloc_resources_that_is_possible spec failed. error: %x\n", rbfs_resource_code[0]);
    }
    rbfs_umount(rbfs);
}

void open_resource_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    if (op_r != RBFS_OP_RESULT_SUCCESS) {
        RBFS_SPEC_PRINTF("(F) open_resource spec failed. error: %d\n", op_r);
    } else {
        RBFS_SPEC_PRINTF("(*) open_resource spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    }
    rbfs_umount(rbfs);
}

void write_resource_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = rbfs_write(rbfs, &resource, 0xaa);
    if (op_r != RBFS_OP_RESULT_SUCCESS) {
        RBFS_SPEC_PRINTF("(F) write_resource spec failed. error: %d\n", op_r);
    } else {
        RBFS_SPEC_PRINTF("(*) write_resource spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    }
    rbfs_umount(rbfs);
}

void rewind_resource_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = rbfs_write(rbfs, &resource, 0xAA);
    op_r = rbfs_rewind(rbfs, &resource);
    if (op_r != RBFS_OP_RESULT_SUCCESS) {
        RBFS_SPEC_PRINTF("(F) rewind_resource spec failed. error: %d\n", op_r);
    } else {
        RBFS_SPEC_PRINTF("(*) rewind_resource spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    }
    rbfs_umount(rbfs);
}

void read_resource_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    unsigned char c[2];
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = rbfs_write(rbfs, &resource, 0x41);
    op_r = rbfs_write(rbfs, &resource, 0xA1);
    op_r = rbfs_rewind(rbfs, &resource);
    c[0] = rbfs_read(rbfs, &resource);
    c[1] = rbfs_read(rbfs, &resource);
    if (c[0] != 0x41 || c[1] != 0xA1) {
        RBFS_SPEC_PRINTF("(F) read_resource spec failed. error: %x\n", c[0]);
    } else {
        RBFS_SPEC_PRINTF("(*) read_resource spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    }
    rbfs_umount(rbfs);
}

void close_resource_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    unsigned char c[2];
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = rbfs_close(rbfs, &resource);
    if (op_r != RBFS_OP_RESULT_SUCCESS) {
        RBFS_SPEC_PRINTF("(F) close_resource spec failed. error: %x\n", op_r);
    } else {
        RBFS_SPEC_PRINTF("(*) close_resource spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    }
    rbfs_umount(rbfs);
}

void try_read_when_end_of_resource_is_reached_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = rbfs_write(rbfs, &resource, 0x41);
    op_r = rbfs_rewind(rbfs, &resource);
    rbfs_read(rbfs, &resource);
    rbfs_read(rbfs, &resource);
    if (op_r == 0 && (rbfs_eor(&resource))) {
        RBFS_SPEC_PRINTF("(*) try_read_when_end_of_resource_is_reached spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    } else {
        RBFS_SPEC_PRINTF("(F) try_read_when_end_of_resource_is_reached spec failed. error: %x\n", op_r);
    }
    rbfs_umount(rbfs);
}

void try_read_when_resource_is_closed_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = rbfs_close(rbfs, &resource);
    rbfs_read(rbfs, &resource);
    if (op_r == 0 && (resource.flags | RBFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ)) {
        RBFS_SPEC_PRINTF("(*) try_read_when_resource_is_closed spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    } else {
        RBFS_SPEC_PRINTF("(F) try_read_when_resource_is_closed spec failed. error: %x\n", op_r);
    }
    rbfs_umount(rbfs);
}

void seek_resource_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    uint8_t i = 0;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 50; i++) {
        op_r = rbfs_write(rbfs, &resource, (i + 0x65));
    }
    op_r = rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_BEGIN, 20);
    if (op_r == RBFS_OP_RESULT_SUCCESS) {
        RBFS_SPEC_PRINTF("(*) seek_resource spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    } else {
        RBFS_SPEC_PRINTF("(F) seek_resource spec failed. error: %x\n", op_r);
    }
    rbfs_umount(rbfs);
}

void random_read_resource_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    uint8_t i = 0;
    unsigned char c[5], first_write_char = 0x65;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 255; i++) {
        op_r = rbfs_write(rbfs, &resource, (i + first_write_char));
    }
    op_r = rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_BEGIN, 0x20);
    c[0] = rbfs_read(rbfs, &resource);
    op_r = rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_CURRENT, 0x10);
    c[1] = rbfs_read(rbfs, &resource);
    op_r = rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_BEGIN, 0x48);
    c[2] = rbfs_read(rbfs, &resource);
    op_r = rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_CURRENT, 0x20);
    c[3] = rbfs_read(rbfs, &resource);
    op_r = rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_BEGIN, 0x0);
    c[4] = rbfs_read(rbfs, &resource);
    if (c[0] == first_write_char + 0x20 && c[1] == first_write_char + 0x31 && c[2] == first_write_char + 0x48 && c[3] == first_write_char + 0x69 && c[4] == first_write_char + 0x0) {
        RBFS_SPEC_PRINTF("(*) random_read_resource spec passed, RBFS_OP_RESULT_SUCCESS: %d.\n", RBFS_OP_RESULT_SUCCESS);
    } else {
        RBFS_SPEC_PRINTF("(F) random_read_resource spec failed. error: %x\n", op_r);
    }
    rbfs_umount(rbfs);
}

void random_read_with_seek_resource_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    uint8_t i = 0;
    unsigned char c[255];
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (i = 0; i < 255; i++) {
        op_r = rbfs_write(rbfs, &resource, i);
    }
    rbfs_rewind(rbfs, &resource);
    for (i = 0; i < 255; i++) {
        rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_BEGIN, i);
        c[i] = rbfs_read(rbfs, &resource);
        rbfs_read(rbfs, &resource);
        rbfs_read(rbfs, &resource);
        rbfs_read(rbfs, &resource);
    }
    op_r = rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_BEGIN, 199);
    if ((i = rbfs_read(rbfs, &resource)) != 199) {
        RBFS_SPEC_PRINTF("(F) random_read_with_seek_resource_spec spec failed. %d != 199\n", 0);
    }

    for (i = 0; i < 255; i++) {
        if (i != c[i]) {
            RBFS_SPEC_PRINTF("(F) random_read_with_seek_resource_spec spec failed. error: %x\n", i);
        }
    }

    RBFS_SPEC_PRINTF("(*) random_read_with_seek_resource_spec spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    rbfs_umount(rbfs);
}

void random_read_with_seek_opening_resource_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    uint8_t i = 0;
    unsigned char c[255];
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (i = 0; i < 255; i++) {
        op_r = rbfs_write(rbfs, &resource, i);
    }
    rbfs_close(rbfs, &resource);

    for (i = 0; i < 255; i++) {
        op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
        rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_BEGIN, i);
        c[i] = rbfs_read(rbfs, &resource);
        rbfs_read(rbfs, &resource);
        rbfs_read(rbfs, &resource);
        rbfs_read(rbfs, &resource);
        rbfs_close(rbfs, &resource);
    }
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_BEGIN, 199);
    if ((i = rbfs_read(rbfs, &resource)) != 199) {
        RBFS_SPEC_PRINTF("(F) random_read_with_seek_opening_resource_spec spec failed. %d != 199\n", op_r);
    }
    rbfs_close(rbfs, &resource);
    for (i = 0; i < 255; i++) {
        if (i != c[i]) {
            RBFS_SPEC_PRINTF("(F) random_read_with_seek_opening_resource_spec spec failed. error: %x\n", c[i]);
        }
    }
    RBFS_SPEC_PRINTF("(*) random_read_with_seek_opening_resource_spec spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    rbfs_umount(rbfs);
}

void size_resource_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    uint16_t i = 0;
    uint16_t size = 0xf40;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < size; i++) {
        op_r = rbfs_write(rbfs, &resource, 0x65);
    }
    if (rbfs_size(&resource) == 0xf40) {
        RBFS_SPEC_PRINTF("(*) size_resource spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    } else {
        RBFS_SPEC_PRINTF("(F) size_resource spec failed. error: %x\n", size);
    }
    rbfs_umount(rbfs);
}

void tell_resource_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    uint8_t i = 0;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 50; i++) {
        op_r = rbfs_write(rbfs, &resource, 0x65);
    }
    if (rbfs_tell(&resource) == 50) {
        RBFS_SPEC_PRINTF("(*) tell_resource spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    } else {
        RBFS_SPEC_PRINTF("(F) tell_resource spec failed. error: %x\n", op_r);
    }
    rbfs_umount(rbfs);
}

void tell_with_seek_resource_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    uint8_t i = 0;
    rbfs_resource_size_t s[5];
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 50; i++) {
        op_r = rbfs_write(rbfs, &resource, 0x65);
    }
    op_r = rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_BEGIN, 20);
    s[0] = rbfs_tell(&resource);
    op_r = rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_CURRENT, 10);
    s[1] = rbfs_tell(&resource);
    op_r = rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_BEGIN, 48);
    s[2] = rbfs_tell(&resource);
    op_r = rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_CURRENT, 20);
    s[3] = rbfs_tell(&resource);
    op_r = rbfs_seek(rbfs, &resource, RBFS_SEEK_ORIGIN_BEGIN, 0);
    s[4] = rbfs_tell(&resource);
    if (s[0] == 20 && s[1] == 30 && s[2] == 48 && s[3] == 17 && s[4] == 0) {
        RBFS_SPEC_PRINTF("(*) tell_with_seek_resource spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    } else {
        RBFS_SPEC_PRINTF("(F) tell_with_seek_resource spec failed. error: %d\n", s[3]);
    }
    rbfs_umount(rbfs);
}

void total_space_resource_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_code_t rbfs_resource_code;
    rbfs_resource_t resource;
    rbfs_resource_size_t total_space[2];
    uint16_t i = 0;
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    total_space[0] = rbfs_available_space(rbfs);
    rbfs_resource_code = rbfs_alloc(rbfs);
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < rbfs->sizeof_cluster_data + 1; i++) {
        op_r = rbfs_write(rbfs, &resource, 0x65);
    }
    total_space[1] = rbfs_available_space(rbfs);
    if (total_space[0] - total_space[1] == (rbfs->sizeof_cluster_data * 2)) {
        RBFS_SPEC_PRINTF("(*) total_space_resource spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    } else {
        RBFS_SPEC_PRINTF("(F) total_space_resource spec failed. error: %d != 50\n", total_space[0] - total_space[1]);
    }
    rbfs_umount(rbfs);
}

void allocating_multi_format_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_t resource;
    uint8_t count = 3;
    uint8_t j, i;
    rbfs_resource_code_t rbfs_resource_code[3];
    uint8_t passed = 1;
    for (j = 0; j < count; j++) {
        rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
        op_r = rbfs_format(rbfs);
        op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
        rbfs_resource_code[j] = rbfs_alloc(rbfs);
        op_r = rbfs_open(rbfs, rbfs_resource_code[j], &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
        for (i = 0; i < 50; i++) {
            op_r = rbfs_write(rbfs, &resource, 0x65);
        }
        rbfs_close(rbfs, &resource);
    }
    for (j = 0; j < count; j++) {
        if (rbfs_resource_code[j] != 0) {
            RBFS_SPEC_PRINTF("(F) allocating_multi_format spec failed %x\n", rbfs_resource_code[j]);
            passed = 0;
        }
    }
    if (passed) {
        RBFS_SPEC_PRINTF("(*) allocating_multi_format spec passed %d\n", RBFS_OP_RESULT_SUCCESS);
    }
    rbfs_umount(rbfs);
}

void read_only_mounting_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_t resource;
    rbfs_resource_code_t rbfs_resource_code;
    char error_msg[] = "(F) read_only_mounting spec failed. %d\n";
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_READ_ONLY);
    rbfs_resource_code = rbfs_alloc(rbfs);
    if (rbfs_resource_code == RBFS_NULL_RESOURCE_CODE) {
        RBFS_SPEC_PRINTF(error_msg, op_r);
    }
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_NORMAL);
    if (op_r != RBFS_OP_RESULT_SUCCESS) {
        RBFS_SPEC_PRINTF(error_msg, op_r);
    }
    op_r = rbfs_write(rbfs, &resource, 0xaa);
    if (op_r == RBFS_OP_RESULT_ERROR_RESOURCE_READ_ONLY) {
        RBFS_SPEC_PRINTF("(*) read_only_mounting spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    } else {
        RBFS_SPEC_PRINTF(error_msg, op_r);
    }
    rbfs_umount(rbfs);
}

void read_only_opening_spec(rbfs_t *rbfs) {
    rbfs_op_result_t op_r;
    rbfs_resource_t resource;
    rbfs_resource_code_t rbfs_resource_code;
    char error_msg[] = "(F) read_only_opening spec failed. %d\n";
    rbfs_make_partition(rbfs, RBFS_DISK_32K, RBFS_ENV_VIRTUAL, RBFS_DRIVER_VIRTUAL);
    op_r = rbfs_format(rbfs);
    op_r = rbfs_mount(RBFS_DRIVER_VIRTUAL, rbfs, RBFS_MOUNT_OPTION_NORMAL);
    rbfs_resource_code = rbfs_alloc(rbfs);
    if (rbfs_resource_code == RBFS_NULL_RESOURCE_CODE) {
        RBFS_SPEC_PRINTF(error_msg, op_r);
    }
    op_r = rbfs_open(rbfs, rbfs_resource_code, &resource, RBFS_OPEN_RESOURCE_OPTION_READ_ONLY);
    if (op_r != RBFS_OP_RESULT_SUCCESS) {
        RBFS_SPEC_PRINTF(error_msg, op_r);
    }
    op_r = rbfs_write(rbfs, &resource, 0xaa);
    if (op_r == RBFS_OP_RESULT_ERROR_RESOURCE_READ_ONLY) {
        RBFS_SPEC_PRINTF("(*) read_only_opening spec passed %d.\n", RBFS_OP_RESULT_SUCCESS);
    } else {
        RBFS_SPEC_PRINTF(error_msg, op_r);
    }
    rbfs_umount(rbfs);
}
