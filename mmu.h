#include "pagetableentry.h"

#ifndef nullptr
#define nullptr __null
#endif

class mmu {
	protected:
		vector<PageTableEntry> * pageTable; //maps virtual page to a frame
		vector<unsigned int> * frameToPage; //inverse page table mapping frame to a virtual page
		vector<unsigned int> * frameTable; 
		int numFrames;
};