#ifndef __SROFS_SPEC_H__
#define __SROFS_SPEC_H__ 1

#define srofs_spec_printf printf
#define SROFS_SPEC_DRIVER SROFS_DRIVER_VIRTUAL

#include "../srofs.h"

void format_spec(srofs_t *srofs);

void mount_spec(srofs_t *srofs);

void umount_spec(srofs_t *srofs);

void alloc_resource_spec(srofs_t *srofs);

void try_to_alloc_resources_that_is_possible_spec(srofs_t *srofs);

void open_resource_spec(srofs_t *srofs);

void write_resource_spec(srofs_t *srofs);

void rewind_resource_spec(srofs_t *srofs);

void read_resource_spec(srofs_t *srofs);

void close_resource_spec(srofs_t *srofs);

void try_read_when_end_of_resource_is_reached_spec(srofs_t *srofs);

void try_read_when_resource_is_closed_spec(srofs_t *srofs);

void seek_resource_spec(srofs_t *srofs);

void random_read_resource_spec(srofs_t *srofs);

void random_read_with_seek_resource_spec(srofs_t *srofs);

void random_read_with_seek_opening_resource_spec(srofs_t *srofs);

void size_resource_spec(srofs_t *srofs);

void tell_resource_spec(srofs_t *srofs);

void tell_with_seek_resource_spec(srofs_t *srofs);

void total_space_resource_spec(srofs_t *srofs);

void allocating_multi_format_spec(srofs_t *srofs);

void read_only_mounting_spec(srofs_t *srofs);

void read_only_opening_spec(srofs_t *srofs);

#endif // __SROFS_SPEC_H__
