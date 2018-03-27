#ifndef _MEMMAP__H
#define _MEMMAP__H

typedef struct memory_mapping {
	const char *name;
	uint32_t size, addr, end_addr, num_children;
	const struct memory_mapping *children;
} mmap_t;

void print_mmap_t(const mmap_t *entry);
void print_all(const mmap_t start[], int count);
int find_entry(const char *name, const mmap_t start[], int count);

#define _MMAP_ENTRY(_n, _a, _s, _c, _e)\
{\
	.name = _n, .size = _s, .addr = _a, .end_addr = _e,\
	.num_children = (sizeof(_c) / sizeof(mmap_t)), .children = _c\
},

#define MMAP_END(_n, _a, _e, _c)\
	_MMAP_ENTRY(_n, _a, 0, _c, _e)

#define MIN_MMAP_END(_n, _a, _e)\
	_MMAP_ENTRY(_n, _a, 0, NULL, _e)

#define MMAP_ENTRY(_n, _a, _s, _c)\
	_MMAP_ENTRY(_n, _a, _s, _c, 0)

#define MIN_MMAP_ENTRY(_name, _addr, _size)\
	MMAP_ENTRY(_name, _addr, _size, NULL)

#endif
