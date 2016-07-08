#ifndef pte_header
#define pte_header

typedef struct PageTableEntry {
	unsigned int present : 1;
	unsigned int modified : 1;
	unsigned int referenced : 1;
	unsigned int pagedout : 1;
	unsigned int frameIndex : 6;
} PageTableEntry;


#endif