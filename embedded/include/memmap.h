#ifndef _MEMMAP__H
#define _MEMMAP__H

typedef struct memory_mapping {
	const char *name;
	uint32_t size, addr, num_children;
	const struct memory_mapping *children;
} mmap_t;

void print_mmap_t(const mmap_t *entry);
void print_all(const mmap_t start[], int count);
int find_entry(const char *name, const mmap_t start[], int count);

#define MMAP_ENTRY(_n, _a, _s, _c)\
{\
	.name = _n, .size = _s, .addr = _a,\
	.num_children = (sizeof(_c) / sizeof(mmap_t)), .children = _c\
},

#define MIN_MMAP_ENTRY(_name, _addr, _size)\
	MMAP_ENTRY(_name, _addr, _size, NULL)

#endif
