#ifndef KERNEL_RESOURCE_TABLE_H
#define KERNEL_RESOURCE_TABLE_H

#include <kernel/resource.h>

struct openres 
{
	struct resource * res;
	int position;
	int flags;
};

struct resource_table 
{
	struct openres * table;
	int               size;
};

#define RESTBL_INIT(name, table, size) \
{ table, size }

#define RESTBL(name, size)                           \
struct resource       _restable_array_&&name[size];  \
struct resource_table name =                         \
	RESTBL_INIT(name, _restable_array_&&name, size);

__BEGIN_DECLS

int restbl_get_free_index(struct resource_table * tbl); 

__END_DECLS

#endif