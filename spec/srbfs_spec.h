#ifndef __SRBFS_SPEC_H__
#define __SRBFS_SPEC_H__ 1

#define SRBFS_SPEC_PRINTF printf

#include "../srbfs.h"

void format_spec(srbfs_t *srbfs);

void mount_spec(srbfs_t *srbfs);

void umount_spec(srbfs_t *srbfs);

void alloc_resource_spec(srbfs_t *srbfs);

void try_to_alloc_resources_that_is_possible_spec(srbfs_t *srbfs);

void open_resource_spec(srbfs_t *srbfs);

void write_resource_spec(srbfs_t *srbfs);

void rewind_resource_spec(srbfs_t *srbfs);

void read_resource_spec(srbfs_t *srbfs);

void close_resource_spec(srbfs_t *srbfs);

void try_read_when_end_of_resource_is_reached_spec(srbfs_t *srbfs);

void try_read_when_resource_is_closed_spec(srbfs_t *srbfs);

void seek_resource_spec(srbfs_t *srbfs);

void random_read_resource_spec(srbfs_t *srbfs);

void random_read_with_seek_resource_spec(srbfs_t *srbfs);

void random_read_with_seek_opening_resource_spec(srbfs_t *srbfs);

void size_resource_spec(srbfs_t *srbfs);

void tell_resource_spec(srbfs_t *srbfs);

void tell_with_seek_resource_spec(srbfs_t *srbfs);

void total_space_resource_spec(srbfs_t *srbfs);

void allocating_multi_format_spec(srbfs_t *srbfs);

void read_only_mounting_spec(srbfs_t *srbfs);

void read_only_opening_spec(srbfs_t *srbfs);

#endif // __SRBFS_SPEC_H__
