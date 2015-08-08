#include <stdio.h>
#include "srofs_spec.h"
#include "../srofs.h"
#include "../srofs_init_partition.h"

void format_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    if (op_r != SROFS_OP_RESULT_SUCCESS) {
        srofs_spec_printf("(F) format spec failed. error: %d\n", op_r);
    } else {
        srofs_spec_printf("(*) format spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    }
}

void mount_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    if (op_r != SROFS_OP_RESULT_SUCCESS) {
        srofs_spec_printf("(F) mount spec failed. error: %d\n", op_r);
    } else {
        srofs_spec_printf("(*) mount spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    }
    srofs_umount(srofs);
}

void umount_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    op_r = srofs_umount(srofs);
    if (op_r != SROFS_OP_RESULT_SUCCESS) {
        srofs_spec_printf("(F) unmount spec failed. error: %d\n", op_r);
    } else {
        srofs_spec_printf("(*) unmount spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    }
}

void alloc_resource_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    if (srofs_resource_code == SROFS_NULL_RESOURCE_CODE) {
        srofs_spec_printf("(F) alloc_resource spec failed. error: %d\n", op_r);
    } else {
        srofs_spec_printf("(*) alloc_resource spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    }
    srofs_umount(srofs);
}

void try_to_alloc_resources_that_is_possible_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code[2];
    uint8_t i = 0;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    for (; i < srofs->resource_descriptor_count; i++) {
        srofs_resource_code[0] = srofs_alloc(srofs);
    }
    srofs_resource_code[1] = srofs_alloc(srofs);
    if (srofs_resource_code[0] == (srofs->resource_descriptor_count - 1) && srofs_resource_code[1] == SROFS_NULL_RESOURCE_CODE) {
        srofs_spec_printf("(*) try_to_alloc_resources_that_is_possible spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    } else {
        srofs_spec_printf("(F) try_to_alloc_resources_that_is_possible spec failed. error: %x\n", srofs_resource_code[0]);
        while (1);
    }
    srofs_umount(srofs);
}

void open_resource_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    if (op_r != SROFS_OP_RESULT_SUCCESS) {
        srofs_spec_printf("(F) open_resource spec failed. error: %d\n", op_r);
    } else {
        srofs_spec_printf("(*) open_resource spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    }
    srofs_umount(srofs);
}

void write_resource_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = srofs_write(srofs, &resource, 0xaa);
    if (op_r != SROFS_OP_RESULT_SUCCESS) {
        srofs_spec_printf("(F) write_resource spec failed. error: %d\n", op_r);
    } else {
        srofs_spec_printf("(*) write_resource spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    }
    srofs_umount(srofs);
}

void rewind_resource_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = srofs_write(srofs, &resource, 0xAA);
    op_r = srofs_rewind(srofs, &resource);
    if (op_r != SROFS_OP_RESULT_SUCCESS) {
        srofs_spec_printf("(F) rewind_resource spec failed. error: %d\n", op_r);
    } else {
        srofs_spec_printf("(*) rewind_resource spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    }
    srofs_umount(srofs);
}

void read_resource_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    unsigned char c[2];
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = srofs_write(srofs, &resource, 0x41);
    op_r = srofs_write(srofs, &resource, 0xA1);
    op_r = srofs_rewind(srofs, &resource);
    c[0] = srofs_read(srofs, &resource);
    c[1] = srofs_read(srofs, &resource);
    if (c[0] != 0x41 || c[1] != 0xA1) {
        srofs_spec_printf("(F) read_resource spec failed. error: %x\n", c[0]);
    } else {
        srofs_spec_printf("(*) read_resource spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    }
    srofs_umount(srofs);
}

void close_resource_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    unsigned char c[2];
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = srofs_close(srofs, &resource);
    if (op_r != SROFS_OP_RESULT_SUCCESS) {
        srofs_spec_printf("(F) close_resource spec failed. error: %x\n", op_r);
    } else {
        srofs_spec_printf("(*) close_resource spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    }
    srofs_umount(srofs);
}

void try_read_when_end_of_resource_is_reached_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = srofs_write(srofs, &resource, 0x41);
    op_r = srofs_rewind(srofs, &resource);
    srofs_read(srofs, &resource);
    srofs_read(srofs, &resource);
    if (op_r == 0 && (srofs_eor(&resource))) {
        srofs_spec_printf("(*) try_read_when_end_of_resource_is_reached spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    } else {
        srofs_spec_printf("(F) try_read_when_end_of_resource_is_reached spec failed. error: %x\n", op_r);
    }
    srofs_umount(srofs);
}

void try_read_when_resource_is_closed_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = srofs_close(srofs, &resource);
    srofs_read(srofs, &resource);
    if (op_r == 0 && (resource.flags | SROFS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ)) {
        srofs_spec_printf("(*) try_read_when_resource_is_closed spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    } else {
        srofs_spec_printf("(F) try_read_when_resource_is_closed spec failed. error: %x\n", op_r);
    }
    srofs_umount(srofs);
}

void seek_resource_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    uint8_t i = 0;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 50; i++) {
        op_r = srofs_write(srofs, &resource, (i + 0x65));
    }
    op_r = srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_BEGIN, 20);
    if (op_r == SROFS_OP_RESULT_SUCCESS) {
        srofs_spec_printf("(*) seek_resource spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    } else {
        srofs_spec_printf("(F) seek_resource spec failed. error: %x\n", op_r);
    }
    srofs_umount(srofs);
}

void random_read_resource_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    uint8_t i = 0;
    unsigned char c[5], first_write_char = 0x65;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 255; i++) {
        op_r = srofs_write(srofs, &resource, (i + first_write_char));
    }
    op_r = srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_BEGIN, 0x20);
    c[0] = srofs_read(srofs, &resource);
    op_r = srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_CURRENT, 0x10);
    c[1] = srofs_read(srofs, &resource);
    op_r = srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_BEGIN, 0x48);
    c[2] = srofs_read(srofs, &resource);
    op_r = srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_CURRENT, 0x20);
    c[3] = srofs_read(srofs, &resource);
    op_r = srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_BEGIN, 0x0);
    c[4] = srofs_read(srofs, &resource);
    if (c[0] == first_write_char + 0x20 &&
            c[1] == first_write_char + 0x31 && // TODO: should break
            c[2] == first_write_char + 0x48 &&
            c[3] == first_write_char + 0x69 &&
            c[4] == first_write_char + 0x0) {
        srofs_spec_printf("(*) random_read_resource spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    } else {
        srofs_spec_printf("(F) random_read_resource spec failed. error: %x\n", op_r);
    }
    srofs_umount(srofs);
}

void random_read_with_seek_resource_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    uint8_t i = 0;
    unsigned char c[255];
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (i = 0; i < 255; i++) {
        op_r = srofs_write(srofs, &resource, i);
    }
    srofs_rewind(srofs, &resource);

    for (i = 0; i < 255; i++) {
        srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_BEGIN, i);
        c[i] = srofs_read(srofs, &resource);
        srofs_read(srofs, &resource);
        srofs_read(srofs, &resource);
        srofs_read(srofs, &resource);
    }

    op_r = srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_BEGIN, 199);
    if ((i = srofs_read(srofs, &resource)) != 199) {
        srofs_spec_printf("(F) random_read_with_seek_resource_spec spec failed. != 199\n", 0);
    }

    for (i = 0; i < 255; i++) {
        if (i != c[i]) {
            srofs_spec_printf("(F) random_read_with_seek_resource_spec spec failed. error: %x\n", i);
        }
    }

    srofs_spec_printf("(*) random_read_with_seek_resource_spec spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    srofs_umount(srofs);
}

void random_read_with_seek_opening_resource_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    uint8_t i = 0;
    unsigned char c[255];
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (i = 0; i < 255; i++) {
        op_r = srofs_write(srofs, &resource, i);
    }
    srofs_close(srofs, &resource);

    for (i = 0; i < 255; i++) {
        op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
        srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_BEGIN, i);
        c[i] = srofs_read(srofs, &resource);
        srofs_read(srofs, &resource);
        srofs_read(srofs, &resource);
        srofs_read(srofs, &resource);
        srofs_close(srofs, &resource);
    }
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    op_r = srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_BEGIN, 199);
    if ((i = srofs_read(srofs, &resource)) != 199) {
        srofs_spec_printf("(F) random_read_with_seek_opening_resource_spec spec failed. != 199\n", op_r);
    }
    srofs_close(srofs, &resource);
    for (i = 0; i < 255; i++) {
        if (i != c[i]) {
            srofs_spec_printf("(F) random_read_with_seek_opening_resource_spec spec failed. error: %x\n", c[i]);
        }
    }
    srofs_spec_printf("(*) random_read_with_seek_opening_resource_spec spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    srofs_umount(srofs);
}

void size_resource_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    uint16_t i = 0;
    uint16_t size = 0xf40;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < size; i++) {
        op_r = srofs_write(srofs, &resource, 0x65);
    }
    if (srofs_size(&resource) == 0xf40) {
        srofs_spec_printf("(*) size_resource spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    } else {
        srofs_spec_printf("(F) size_resource spec failed. error: %x\n", size);
    }
    srofs_umount(srofs);
}

void tell_resource_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    uint8_t i = 0;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 50; i++) {
        op_r = srofs_write(srofs, &resource, 0x65);
    }
    if (srofs_tell(&resource) == 50) {
        srofs_spec_printf("(*) tell_resource spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    } else {
        srofs_spec_printf("(F) tell_resource spec failed. error: %x\n", op_r);
    }
    srofs_umount(srofs);
}

void tell_with_seek_resource_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    uint8_t i = 0;
    srofs_resource_size_t s[5];
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < 50; i++) {
        op_r = srofs_write(srofs, &resource, 0x65);
    }
    op_r = srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_BEGIN, 20);
    s[0] = srofs_tell(&resource);
    op_r = srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_CURRENT, 10);
    s[1] = srofs_tell(&resource);
    op_r = srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_BEGIN, 48);
    s[2] = srofs_tell(&resource);
    op_r = srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_CURRENT, 20);
    s[3] = srofs_tell(&resource);
    op_r = srofs_seek(srofs, &resource, SROFS_SEEK_ORIGIN_BEGIN, 0);
    s[4] = srofs_tell(&resource);
    if (s[0] == 20 &&
            s[1] == 30 &&
            s[2] == 48 &&
            s[3] == 17 &&
            s[4] == 0) {
        srofs_spec_printf("(*) tell_with_seek_resource spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    } else {
        srofs_spec_printf("(F) tell_with_seek_resource spec failed. error: %d\n", s[3]);
    }
    srofs_umount(srofs);
}

void total_space_resource_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_code_t srofs_resource_code;
    srofs_resource_t resource;
    srofs_resource_size_t total_space[2];
    uint16_t i = 0;
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    total_space[0] = srofs_available_space(srofs);
    srofs_resource_code = srofs_alloc(srofs);
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    for (; i < srofs->sizeof_cluster_data + 1; i++) {
        op_r = srofs_write(srofs, &resource, 0x65);
    }
    total_space[1] = srofs_available_space(srofs);
    if (total_space[0] - total_space[1] == (srofs->sizeof_cluster_data * 2)) {
        srofs_spec_printf("(*) total_space_resource spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    } else {
        srofs_spec_printf("(F) total_space_resource spec failed. error: %d != 50\n", total_space[0] - total_space[1]);
    }
    srofs_umount(srofs);
}

void allocating_multi_format_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_t resource;
    uint8_t count = 3;
    uint8_t j, i;
    srofs_resource_code_t srofs_resource_code[3];
    uint8_t passed = 1;
    for (j = 0; j < count; j++) {
        srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
        op_r = srofs_format(srofs);
        op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
        srofs_resource_code[j] = srofs_alloc(srofs);
        op_r = srofs_open(srofs, srofs_resource_code[j], &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
        for (i = 0; i < 50; i++) {
            op_r = srofs_write(srofs, &resource, 0x65);
        }
        srofs_close(srofs, &resource);
    }
    for (j = 0; j < count; j++) {
        if (srofs_resource_code[j] != 0) {
            srofs_spec_printf("(F) allocating_multi_format spec failed %x\n", srofs_resource_code[j]);
            passed = 0;
        }
    }
    if (passed) {
        srofs_spec_printf("(*) allocating_multi_format spec passed\n", SROFS_OP_RESULT_SUCCESS);
    }
    srofs_umount(srofs);
}

void read_only_mounting_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_t resource;
    srofs_resource_code_t srofs_resource_code;
    char error_msg[] = "(F) read_only_mounting spec failed. %d\n";
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_READ_ONLY);
    srofs_resource_code = srofs_alloc(srofs);
    if (srofs_resource_code == SROFS_NULL_RESOURCE_CODE) {
        srofs_spec_printf(error_msg, op_r);
    }
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_NORMAL);
    if (op_r != SROFS_OP_RESULT_SUCCESS) {
        srofs_spec_printf(error_msg, op_r);
    }
    op_r = srofs_write(srofs, &resource, 0xaa);
    if (op_r == SROFS_OP_RESULT_ERROR_RESOURCE_READ_ONLY) {
        srofs_spec_printf("(*) read_only_mounting spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    } else {
        srofs_spec_printf(error_msg, op_r);
    }
    srofs_umount(srofs);
}

void read_only_opening_spec(srofs_t *srofs) {
    srofs_op_result_t op_r;
    srofs_resource_t resource;
    srofs_resource_code_t srofs_resource_code;
    char error_msg[] = "(F) read_only_opening spec failed. %d\n";
    srofs_init_partition(srofs, SROFS_DISK_32K, SROFS_ENV_VIRTUAL);
    op_r = srofs_format(srofs);
    op_r = srofs_mount(SROFS_SPEC_DRIVER, srofs, SROFS_MOUNT_OPTION_NORMAL);
    srofs_resource_code = srofs_alloc(srofs);
    if (srofs_resource_code == SROFS_NULL_RESOURCE_CODE) {
        srofs_spec_printf(error_msg, op_r);
    }
    op_r = srofs_open(srofs, srofs_resource_code, &resource, SROFS_OPEN_RESOURCE_OPTION_READ_ONLY);
    if (op_r != SROFS_OP_RESULT_SUCCESS) {
        srofs_spec_printf(error_msg, op_r);
    }
    op_r = srofs_write(srofs, &resource, 0xaa);
    if (op_r == SROFS_OP_RESULT_ERROR_RESOURCE_READ_ONLY) {
        srofs_spec_printf("(*) read_only_opening spec passed.\n", SROFS_OP_RESULT_SUCCESS);
    } else {
        srofs_spec_printf(error_msg, op_r);
    }
    srofs_umount(srofs);
}
