#ifndef __RBFS_SPEC_H__
#define __RBFS_SPEC_H__ 1

#define RBFS_SPEC_PRINTF printf

#include "../rbfs.h"

void format_spec(rbfs_t *rbfs);

void mount_spec(rbfs_t *rbfs);

void umount_spec(rbfs_t *rbfs);

void alloc_resource_spec(rbfs_t *rbfs);

void try_to_alloc_resources_that_is_possible_spec(rbfs_t *rbfs);

void open_resource_spec(rbfs_t *rbfs);

void write_resource_spec(rbfs_t *rbfs);

void rewind_resource_spec(rbfs_t *rbfs);

void read_resource_spec(rbfs_t *rbfs);

void close_resource_spec(rbfs_t *rbfs);

void try_read_when_end_of_resource_is_reached_spec(rbfs_t *rbfs);

void try_read_when_resource_is_closed_spec(rbfs_t *rbfs);

void seek_resource_spec(rbfs_t *rbfs);

void random_read_resource_spec(rbfs_t *rbfs);

void random_read_with_seek_resource_spec(rbfs_t *rbfs);

void random_read_with_seek_opening_resource_spec(rbfs_t *rbfs);

void size_resource_spec(rbfs_t *rbfs);

void tell_resource_spec(rbfs_t *rbfs);

void tell_with_seek_resource_spec(rbfs_t *rbfs);

void total_space_resource_spec(rbfs_t *rbfs);

void allocating_multi_format_spec(rbfs_t *rbfs);

void read_only_mounting_spec(rbfs_t *rbfs);

void read_only_opening_spec(rbfs_t *rbfs);

#endif // __RBFS_SPEC_H__
