#ifndef __SRBFS_SPEC_HELPER_H__
#define __SRBFS_SPEC_HELPER_H__ 1

#include <srbfs.h>

void resource_dump(srbfs_resource_t *resource);
void format_all();
char* itob(int i);
void srbfs_io_memory_dump(srbfs_t *srbfs);

#endif // __SRBFS_SPEC_HELPER_H__
